#include <Koala/Editor/Main.h>
#include <Koala/Editor/Gfx/Renderer.h>
#include <Koala/Editor/Tool/Input.h>
#include <Koala/Editor/Tool/PlatformManager.h>

namespace Koala::Editor {

Main::Main()
{
	// Init graphics/media libraries
	bool success = Tool::PlatformManager::Initialize();
	if(success == false)
	{
		return;
	}

	m_MainWindow.Create("Koala Editor");
	if(m_MainWindow.IsValid() == false)
	{
		std::printf("Window creation error\n");
		std::getchar();
		return;
	}

	m_TestWindow.Create("test", 800, 600);

	m_CanRun = true;
}

void Main::Run()
{
	if(m_CanRun == false)
	{
		return;
	}

	// Main program loop
	while(m_MainWindow.IsValid())
	{
		Tool::Input::Update();

		if(m_TestWindow.IsValid())
		{
			m_TestWindow.Activate();
			Gfx::Renderer::ClearViewport(Gfx::Color(0.8f, 0.2f, 0.4f));
			m_TestWindow.Update();
		}

		m_MainWindow.Activate();
		Gfx::Renderer::ClearViewport(Gfx::Color(0.2f, 0.4f, 0.8f));
		m_MainWindow.Update();
	}

	SendMessage(Service::MessageType::ProgramExit, nullptr);
}

void Main::OnMessage(Service::MessageType type, void* data)
{

}
void Main::OnInput(Service::InputMessageType type, const Service::InputMessageData& data)
{
	if(data.Window != m_MainWindow)
	{
		return;
	}

	switch(type)
	{
		case Koala::Editor::Service::InputMessageType::KeyPress:
		{
			if(data.Key == Tool::KeyType::Space)
			{
				m_TestWindow.Destroy();
			}
			else if(data.Key == Tool::KeyType::Escape)
			{
				m_MainWindow.Destroy();
			}
			break;
		}
	}
}

Main::~Main() noexcept
{
	// Explicit call to destroy before platform termination.
	m_MainWindow.Destroy();
	m_TestWindow.Destroy();

	Tool::PlatformManager::Terminate();
}

} // namespace Koala::Editor
