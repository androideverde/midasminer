#include <CEvent.h>

CEvent::CEvent()
{
}

CEvent::~CEvent()
{
}

void CEvent::OnEvent(SDL_Event *event)
{
	switch (event->type) {
		case SDL_QUIT:
			OnExit();
			break;
		case SDL_MOUSEMOTION:
			OnMouseMove(event->motion.x, event->motion.y,
						event->motion.xrel, event->motion.yrel,
						(event->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0,
						(event->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0,
						(event->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event->button.button) {
				case SDL_BUTTON_LEFT:
					OnLButtonDown(event->button.x, event->button.y);
					break;
				case SDL_BUTTON_RIGHT:
					OnRButtonDown(event->button.x, event->button.y);
					break;
				case SDL_BUTTON_MIDDLE:
					OnMButtonDown(event->button.x, event->button.y);
					break;
			}
			break;
		default:
			break;
	}
}
