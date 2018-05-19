#ifndef KOALA__UTILITY__FILE
#define KOALA__UTILITY__FILE

#include <string>

namespace Koala::Utility {

enum class FileType
{
	Binary,
	Text
};

class File
{
public:
	File(const std::string& path, FileType fileType=FileType::Binary, bool create=true);

	void Write(const std::string& data);
	void WriteLine(const std::string& data);

	std::string Read(size_t size);
	std::string ReadLine();

	template<typename T> void WriteBinaryNumber(const T& value)
	{
		WriteBinaryNumber(&value, sizeof(T));
	}
	template<typename T> T ReadBinaryNumber()
	{
		T value = static_cast<T>(-1);
		ReadBinaryNumber(&value, sizeof(T));
		return value;
	}

	void MoveHead(int delta);
	void MoveHeadToFront();
	void MoveHeadToEnd();

	void Delete();
	void DeleteContents();

	bool IsValid() const
	{
		return (m_Handle != nullptr);
	}
	bool IsEOF() const
	{
		return m_IsEOF;
	}

public:
	~File() noexcept;
	File(const File& other) = delete;
	File(File&& other) noexcept;
	File& operator=(const File& other) = delete;
	File& operator=(File&& other) noexcept;

private:
	void WriteBinaryNumber(const void* value, size_t size);
	void ReadBinaryNumber(void* value, size_t size);

private:
	void* m_Handle = nullptr;
	std::string m_Path;
	FileType m_FileType;
	bool m_IsEOF = true;

};

} // namespace Koala::Utility

#endif // KOALA__UTILITY__FILE
