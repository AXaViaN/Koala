#include <Koala/Utility/File.h>
#include <Koala/Utility/Platform.h>
#include <algorithm>
#include <cstdio>

#if OS_WINDOWS
#include <Windows.h>
#endif

namespace Koala::Utility {

// Platform specific folder functions
static bool DoesFolderExists(const std::string& path);
static void CreateFolder(const std::string& path);

File::File(const std::string& path) : 
	m_Path(path)
{
	// Default mode for file I/O
	std::string mode = "ab+";

	// Fix path slashes to use forward slashes
	std::replace(m_Path.begin(), m_Path.end(), '\\', '/');

	// Check if the path is a folder
	if(*(m_Path.rbegin()) == '/')
	{
		std::printf("Cannot open %s, it is a folder\n", m_Path.c_str());
		return;
	}

	m_Handle = std::fopen(m_Path.c_str(), mode.c_str());
	if(m_Handle == nullptr)
	{
		// Create the folders on the path
		CreateFolder(m_Path.substr(0, m_Path.find_last_of('/')));

		// Retry opening
		m_Handle = std::fopen(m_Path.c_str(), mode.c_str());
		if(m_Handle == nullptr)
		{
			std::printf("Cannot open %s\n", m_Path.c_str());
			return;
		}
	}
}

void File::Write(const std::string& data)
{
	if(m_Handle == nullptr)
	{
		return;
	}

	std::fwrite(data.data(), sizeof(char), data.size(), static_cast<std::FILE*>(m_Handle));
}
void File::WriteLine(const std::string& data)
{
	Write(data + "\n");
}

std::string File::Read(size_t size)
{
	if(m_Handle == nullptr)
	{
		return "";
	}

	std::string data(size+1u, '\0');
	std::fread(data.data(), sizeof(char), size, static_cast<std::FILE*>(m_Handle));

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
		if(c == '\n' || 
		   std::feof(static_cast<std::FILE*>(m_Handle)))
			break;

		data += c;
	}

	return data;
}

void File::MoveHead(int delta)
{
	std::fseek(static_cast<std::FILE*>(m_Handle), delta, SEEK_CUR);
}
void File::MoveHeadToFront()
{
	std::fseek(static_cast<std::FILE*>(m_Handle), 0, SEEK_SET);
}
void File::MoveHeadToEnd()
{
	std::fseek(static_cast<std::FILE*>(m_Handle), 0, SEEK_END);
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
}
void File::DeleteContents()
{
	if(m_Handle == nullptr)
	{
		return;
	}

	// Delete by re-opening with the writing mode
	std::fclose(static_cast<std::FILE*>(m_Handle));
	m_Handle = std::fopen(m_Path.c_str(), "wb+");
}

File::~File() noexcept
{
	if(m_Handle)
	{
		std::fclose(static_cast<std::FILE*>(m_Handle));
		m_Handle = nullptr;
		m_Path = "";
	}
}
File::File(File&& other) noexcept : 
	m_Handle(other.m_Handle),
	m_Path(other.m_Path)
{
	other.m_Handle = nullptr;
	other.m_Path = "";
}
File& File::operator=(File&& other) noexcept
{
	if(this == &other)
	{
		return *this;
	}

	m_Handle = other.m_Handle;
	m_Path = other.m_Path;
	other.m_Handle = nullptr;
	other.m_Path = "";

	return *this;
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
