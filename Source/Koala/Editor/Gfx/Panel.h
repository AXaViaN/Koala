#ifndef KOALA__EDITOR__GFX__PANEL
#define KOALA__EDITOR__GFX__PANEL

#include <Koala/Editor/Service/MessageMember.h>
#include <Koala/Editor/Gfx/Vector2.h>
#include <Koala/Editor/Tool/Window.h>

namespace Koala::Editor::Gfx {

class Panel : protected Service::MessageMember
{
public:
	Panel(const Tool::Window& window, Vector2 startPoint, Vector2 endPoint);

	void Update();

public: // Rule of five
	virtual ~Panel() noexcept = default;
	Panel(const Panel& other) = delete;
	Panel(Panel&& other) = delete;
	Panel& operator=(const Panel& other) = delete;
	Panel& operator=(Panel&& other) = delete;

protected:
	virtual void OnGui() = 0;

	virtual void OnMessage(Service::MessageType type, void* data) override
	{ }
	virtual void OnInput(Service::InputMessageType type, const Service::InputMessageData& data) override
	{ }

	void DisableScrollBar();

	static const Gfx::Vector2& CodeBoardStartPosition()
	{
		static const Gfx::Vector2 startPosition(0.15f, 0.05f);
		return startPosition;
	}
	static const Gfx::Vector2& CodeBoardEndPosition()
	{
		static const Gfx::Vector2 endPosition(0.85f, 1.0f);
		return endPosition;
	}

private:
	const Tool::Window& m_Window;

	Vector2 m_StartPoint;
	Vector2 m_EndPoint;

	unsigned int m_AdditionalFlags = 0;

};

} // namespace Koala::Editor::Gfx

#endif // KOALA__EDITOR__GFX__PANEL
