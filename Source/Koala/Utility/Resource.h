#ifndef KOALA__UTILITY__RESOURCE
#define KOALA__UTILITY__RESOURCE

#include <string>
#include <map>

namespace Koala::Utility {

enum class Text;
enum class Icon;

class Resource
{
public:
	static void Initialize(bool loadIcons=false);

	static const std::string& GetText(Text text);
	static void* GetIcon(Icon icon);

	static const std::string& LogFilePath()
	{
		static const std::string path = "Data/user.log";
		return path;
	}

private:
	Resource();
	~Resource() noexcept;
	Resource(const Resource& other) = delete;
	Resource(Resource&& other) = delete;
	Resource& operator=(const Resource& other) = delete;
	Resource& operator=(Resource&& other) = delete;

	static Resource& Instance()
	{
		static Resource instance;
		return instance;
	}

private:
	std::map<Text, std::string> m_TextMap;
	std::map<Icon, void*> m_IconMap;

};

enum class Text
{
	Empty,

	ControlPanel,
	Details,
	Palette,
	Program,

	Functions,
	Variables,

	Search,
	Arithmetic,
	System,

	Add,
	Subtract,
	Multiply,
	Divide,
	SquareRoot,
	Power,
	Modulo,
	Print,
	ReadNumber,
	ReadString,

	Result,
};

enum class Icon
{
	Empty,

	New,
	Open,
	Save,
	Settings,
	Compile,
	Run
};

} // namespace Koala::Utility

#endif // KOALA__UTILITY__RESOURCE
