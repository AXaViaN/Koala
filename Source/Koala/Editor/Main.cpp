#include <Koala/Editor/Main.h>
#include <Koala/Editor/CodeBoard.h>
#include <Koala/Editor/ToolBar.h>
#include <Koala/Editor/ControlPanel.h>
#include <Koala/Editor/Details.h>
#include <Koala/Editor/Palette.h>
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

	Gfx::Renderer::Initialize();

	m_MainWindow.Create("Koala Editor");
	if(m_MainWindow.IsValid() == false)
	{
		auto data = GenerateLogMessageData();
		data.Message = "Window creation error";
		SendMessage(Service::MessageType::LogError, &data);
		return;
	}

	m_CanRun = true;
}

void Main::Run()
{
	if(m_CanRun == false)
	{
		return;
	}

	CodeBoard codeBoard(m_MainWindow);
	ToolBar toolBar(m_MainWindow);
	ControlPanel controlPanel(m_MainWindow);
	Details details(m_MainWindow);
	Palette palette(m_MainWindow);
	Gfx::Panel* panels[] = {
		&codeBoard,
		&toolBar,
		&controlPanel,
		&details,
		&palette
	};

	// Main program loop
	while(m_MainWindow.IsValid())
	{
		m_MainWindow.Activate();

		Gfx::Renderer::ClearViewport(Gfx::Color(0.2f, 0.4f, 0.8f));
		for( auto& panel : panels )
		{
			panel->Update();
		}
	#if 0
		Gfx::Renderer::DrawGuiDemo();
	#endif
		Gfx::Renderer::Update();

		m_MainWindow.Update();

		Tool::Input::Update();
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
			if(data.Key == Tool::KeyType::Escape)
			{
				m_MainWindow.Destroy();
			}
			else if(data.Key == Tool::KeyType::S)
			{
				auto data = GenerateLogMessageData();
				data.Message = "Test";

				SendMessage(Service::MessageType::LogInfo, &data);
			}
			else if(data.Key == Tool::KeyType::D)
			{
				auto data = GenerateLogMessageData();
				data.Message = "Test2";

				SendMessage(Service::MessageType::LogError, &data);
			}

			break;
		}
	}
}

Main::~Main() noexcept
{
	Gfx::Renderer::Terminate();

	// Explicit call to destroy before platform termination.
	m_MainWindow.Destroy();

	Tool::PlatformManager::Terminate();
}

} // namespace Koala::Editor
