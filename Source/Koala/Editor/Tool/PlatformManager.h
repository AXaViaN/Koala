#ifndef KOALA__EDITOR__TOOL__PLATFORM_MANAGER
#define KOALA__EDITOR__TOOL__PLATFORM_MANAGER

namespace Koala::Editor::Tool {

class PlatformManager
{
public:
	static bool Initialize();
	static bool InitializeRenderContext();

	static void Terminate();

};

} // namespace Koala::Editor::Tool

#endif // KOALA__EDITOR__TOOL__PLATFORM_MANAGER
