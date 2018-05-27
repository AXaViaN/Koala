#ifndef KOALA__UTILITY__SERIALIZATION
#define KOALA__UTILITY__SERIALIZATION

#include <Koala/Utility/Core/Node.h>
#include <string>
#include <vector>

namespace Koala::Utility {

static const std::string KoalaBinaryMark = "KOA";
static const std::string KoalaProjectMark = "KPJ";

class Serialization
{
public:
	struct Vector2;
	struct SceneNode;
	struct Function;
	struct Data;

public:
	static void SaveProject(const std::string& path, const Data& data);
	static Data LoadProject(const std::string& path);

};

// Mocking the Editor data types
struct Serialization::Vector2
{
	float X;
	float Y;
};
struct Serialization::SceneNode
{
	Core::Node Node = Core::Node(0);
	Vector2 Position;
};
struct Serialization::Function
{
	std::vector<SceneNode> SceneNodes;
	size_t CoreNodeCount = 1;

	Vector2 DragOffset;
};
struct Serialization::Data
{
	std::string ProjectName;

	std::vector<Function> Functions;
	size_t SelectedFunction;

	bool IsDataValid = false;
};

} // namespace Koala::Utility

#endif // KOALA__UTILITY__SERIALIZATION
