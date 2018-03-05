#ifndef KOALA__EDITOR__MAIN
#define KOALA__EDITOR__MAIN

#include <Koala/Editor/Tool/Window.h>

namespace Koala::Editor {

class Main final
{
public:
	Main();

	void Run();

public: // Rule of five
	~Main() noexcept;
	Main(const Main& other) = delete;
	Main(Main&& other) = delete;
	Main& operator=(const Main& other) = delete;
	Main& operator=(Main&& other) = delete;

private:
	Tool::Window m_MainWindow;
	Tool::Window m_TestWindow;

	bool m_CanRun = false;

};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__MAIN
