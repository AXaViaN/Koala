#ifndef KOALA__EDITOR__MAIN
#define KOALA__EDITOR__MAIN

#include <Koala/Editor/Service/MessageMember.h>
#include <Koala/Editor/Tool/Window.h>

namespace Koala::Editor {

class Main final : private Service::MessageMember
{
public:
	Main();

	void Run();

	virtual void OnMessage(Service::MessageType type, void* data) override;
	virtual void OnInput(Service::InputMessageType type, const Service::InputMessageData& data) override;

public: // Rule of five
	~Main() noexcept;
	Main(const Main& other) = delete;
	Main(Main&& other) = delete;
	Main& operator=(const Main& other) = delete;
	Main& operator=(Main&& other) = delete;

private:
	Tool::Window m_MainWindow;

	bool m_CanRun = false;

};

} // namespace Koala::Editor

#endif // KOALA__EDITOR__MAIN
