#include <Koala/Editor/Main.h>
#include <Koala/Editor/CodeBoard.h>
#include <Koala/Editor/ToolBar.h>
#include <Koala/Editor/ControlPanel.h>
#include <Koala/Editor/Details.h>
#include <Koala/Editor/Palette.h>
#include <Koala/Editor/Gfx/Renderer.h>
#include <Koala/Editor/Tool/Input.h>
#include <Koala/Editor/Tool/PlatformManager.h>
#include <Koala/Utility/Resource.h>

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

	Utility::Resource::Initialize(true);

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
		{
			std::string title = Utility::Resource::GetText(Utility::Text::KoalaEditor) + " - ";
			if(m_ProjectName.size() == 0)
			{
				title += Utility::Resource::GetText(Utility::Text::NewProject);
			}
			else
			{
				title += m_ProjectName;
			}

			m_MainWindow.RenameTitle(title);
		}

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
	switch(type)
	{
		case Service::MessageType::NewProject:
		{
			m_ProjectName.clear();
			break;
		}
		case Service::MessageType::SaveProject:
		case Service::MessageType::LoadProject:
		{
			auto saveData = static_cast<Service::SaveProjectData*>(data);

			m_ProjectName = saveData->ProjectName;
			break;
		}
	}
}
void Main::OnInput(Service::InputMessageType type, const Service::InputMessageData& data)
{ }

Main::~Main() noexcept
{
	Gfx::Renderer::Terminate();

	// Explicit call to destroy before platform termination.
	m_MainWindow.Destroy();

	Tool::PlatformManager::Terminate();
}

} // namespace Koala::Editor
