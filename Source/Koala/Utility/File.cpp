#include <Koala/Utility/File.h>
#include <Koala/Utility/Logger.h>
#include <Koala/Utility/Platform.h>
#include <algorithm>
#include <cstdio>

#if OS_WINDOWS
#include <Windows.h>
#endif

namespace Koala::Utility {

// Helpers
static void FixEndianness(void* value, size_t size);
// Platform specific folder functions
static bool DoesFolderExists(const std::string& path);
static void CreateFolder(const std::string& path);

File::File(const std::string& path, FileType fileType, bool create) : 
	m_Path(path),
	m_FileType(fileType)
{
	// Default mode for file I/O
	std::string mode = "ab+";
	if(m_FileType == FileType::Text)
	{
		mode = "a+";
	}
	if(create == false)
	{
		mode[0] = 'r';
	}

	// Fix path slashes to use forward slashes
	std::replace(m_Path.begin(), m_Path.end(), '\\', '/');

	// Check if the path is a folder
	if(*(m_Path.rbegin()) == '/')
	{
		LogError("Cannot open " + m_Path + ", it is a folder");
		return;
	}

	m_Handle = std::fopen(m_Path.c_str(), mode.c_str());
	if(m_Handle == nullptr)
	{
		// Create the folders on the path
		size_t folderPathIdx = m_Path.find_last_of('/');
		if(folderPathIdx != m_Path.npos)
		{
			CreateFolder(m_Path.substr(0, folderPathIdx));
		}

		// Retry opening
		m_Handle = std::fopen(m_Path.c_str(), mode.c_str());
		if(m_Handle == nullptr)
		{
			LogError("Cannot open " + m_Path);
			return;
		}
	}

	// Move head for constancy
	if(create == false)
	{
		MoveHeadToEnd();
	}
}

void File::Write(const std::string& data)
{
	if(m_Handle == nullptr)
	{
		return;
	}

	if(m_FileType == FileType::Binary)
	{
		std::fwrite(data.data(), sizeof(char), data.size(), static_cast<std::FILE*>(m_Handle));
	}
	else
	{
		std::fprintf(static_cast<std::FILE*>(m_Handle), "%s", data.c_str());
	}
}
void File::WriteLine(const std::string& data)
{
	if(m_FileType == FileType::Binary)
	{
		Write(data + "\n");
	}
	else
	{
		Write(data + "\r\n");
	}
}

std::string File::Read(size_t size)
{
	if(m_Handle == nullptr)
	{
		return "";
	}

	std::string data(size, '\0');
	std::fread(data.data(), sizeof(char), size, static_cast<std::FILE*>(m_Handle));
	if(std::feof(static_cast<std::FILE*>(m_Handle)))
	{
		m_IsEOF = true;
	}

	return data;
}
std::string File::ReadLine()
{
	if(m_Handle == nullptr)
	{
		return "";
	}

	std::string data;
	while(true)
	{
		char c;
		std::fread(&c, sizeof(char), 1u, static_cast<std::FILE*>(m_Handle));
		if(std::feof(static_cast<std::FILE*>(m_Handle)))
		{
			m_IsEOF = true;
			break;
		}
		if(c == '\n')
		{
			break;
		}
		else if(c == '\r')
		{
			continue;
		}

		data += c;
	}

	return data;
}

void File::MoveHead(int delta)
{
	std::fseek(static_cast<std::FILE*>(m_Handle), delta, SEEK_CUR);

	if(std::feof(static_cast<std::FILE*>(m_Handle)))
	{
		m_IsEOF = true;
	}
	else
	{
		m_IsEOF = false;
	}
}
void File::MoveHeadToFront()
{
	std::fseek(static_cast<std::FILE*>(m_Handle), 0, SEEK_SET);
	m_IsEOF = false;
}
void File::MoveHeadToEnd()
{
	std::fseek(static_cast<std::FILE*>(m_Handle), 0, SEEK_END);
	m_IsEOF = true;
}

void File::Delete()
{
	if(m_Handle == nullptr)
	{
		return;
	}

	std::fclose(static_cast<std::FILE*>(m_Handle));
	m_Handle = nullptr;

	std::remove(m_Path.c_str());
	m_Path = "";
	m_IsEOF = true;
}
void File::DeleteContents()
{
	if(m_Handle == nullptr)
	{
		return;
	}

	// Delete by re-opening with the writing mode
	std::fclose(static_cast<std::FILE*>(m_Handle));
	if(m_FileType == FileType::Binary)
	{
		m_Handle = std::fopen(m_Path.c_str(), "wb+");
	}
	else
	{
		m_Handle = std::fopen(m_Path.c_str(), "w+");
	}

	m_IsEOF = true;
}

File::~File() noexcept
{
	if(m_Handle)
	{
		std::fclose(static_cast<std::FILE*>(m_Handle));
		m_Handle = nullptr;
		m_Path = "";
		m_IsEOF = true;
	}
}
File::File(File&& other) noexcept : 
	m_Handle(other.m_Handle),
	m_Path(other.m_Path),
	m_FileType(other.m_FileType),
	m_IsEOF(other.m_IsEOF)
{
	other.m_Handle = nullptr;
	other.m_Path = "";
	other.m_IsEOF = true;
}
File& File::operator=(File&& other) noexcept
{
	if(this == &other)
	{
		return *this;
	}

	m_Handle = other.m_Handle;
	m_Path = other.m_Path;
	m_FileType = other.m_FileType;
	m_IsEOF = other.m_IsEOF;
	other.m_Handle = nullptr;
	other.m_Path = "";
	other.m_IsEOF = true;

	return *this;
}

void File::WriteBinaryNumber(const void* value, size_t size)
{
	if(m_Handle == nullptr)
	{
		return;
	}

	// Fix endian
	void* fixedValue = alloca(size);
	for( size_t i=0 ; i<size ; ++i )
	{
		static_cast<char*>(fixedValue)[i] = static_cast<const char*>(value)[i];
	}
	FixEndianness(fixedValue, size);

	std::fwrite(fixedValue, size, 1u, static_cast<std::FILE*>(m_Handle));
}
void File::ReadBinaryNumber(void* value, size_t size)
{
	if(m_Handle == nullptr)
	{
		return;
	}

	std::fread(value, size, 1u, static_cast<std::FILE*>(m_Handle));
	if(std::feof(static_cast<std::FILE*>(m_Handle)))
	{
		m_IsEOF = true;
	}

	FixEndianness(value, size);
}

static void FixEndianness(void* value, size_t size)
{
	// Machine byte order flag. Check with a lambda function and save on static const
	static const bool IsLittleEndianMachine = ([]() -> bool 
	{
		int x = 1;

		// If the machine is little-endian, the least important bit
		// will be on lower address. Little-endian will set the x=1
		// as 01000000, big-endian 00000001. So casting to char will
		// result in '1' for little-endian and '0' for big-endian.
		if(*((char*)&x) == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	})();

	if(IsLittleEndianMachine)
	{
		return;
	}

	size_t i = 0;
	size_t j = size - 1u;
	while(i < j)
	{
		std::swap(static_cast<char*>(value)[i], static_cast<char*>(value)[j]);

		++i;
		--j;
	}
}

static bool DoesFolderExists(const std::string& path)
{
	bool result = false;

	// Check if folder exists
#if OS_WINDOWS
	DWORD dwAttrib = GetFileAttributes(path.c_str());

	result = dwAttrib != INVALID_FILE_ATTRIBUTES && 
			 (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
#else
#error No implementation on this OS
#endif

	return result;
}
static void CreateFolder(const std::string& path)
{
	std::string folderPath = path;
	// Remove the last slash
	if(*(folderPath.rbegin()) == '/')
	{
		folderPath.erase(folderPath.end()-1);
	}

	if(DoesFolderExists(folderPath) == false)
	{
		size_t lastSlash = folderPath.find_last_of('/');
		if(lastSlash != folderPath.npos)
		{
			// Recursively check if the folder exists. After this method,
			// we know that the folders before the slash are existing.
			CreateFolder(folderPath.substr(0, lastSlash));
		}
		
		// Create the folder
	#if OS_WINDOWS
		CreateDirectory(folderPath.c_str(), nullptr);
	#else
	#error No implementation on this OS
	#endif
	}
}

} // namespace Koala::Utility
