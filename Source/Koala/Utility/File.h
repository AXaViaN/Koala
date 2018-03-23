#ifndef KOALA__UTILITY__FILE
#define KOALA__UTILITY__FILE

#include <string>

namespace Koala::Utility {

class File
{
public:
	File(const std::string& path);

	void Write(const std::string& data);
	void WriteLine(const std::string& data);

	std::string Read(size_t size);
	std::string ReadLine();

	void MoveHead(int delta);
	void MoveHeadToFront();
	void MoveHeadToEnd();

	void Delete();
	void DeleteContents();

	bool IsValid() const
	{
		return (m_Handle != nullptr);
	}

public:
	~File() noexcept;
	File(const File& other) = delete;
	File(File&& other) noexcept;
	File& operator=(const File& other) = delete;
	File& operator=(File&& other) noexcept;

private:
	void* m_Handle = nullptr;
	std::string m_Path;

};

} // namespace Koala::Utility

#endif // KOALA__UTILITY__FILE
