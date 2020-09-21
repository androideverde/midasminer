#pragma once

#include <SDL.h>

class CEvent
{
public:
	CEvent();
	virtual ~CEvent();
	virtual void OnEvent(SDL_Event* event);
	virtual void OnExit() = 0;
	virtual void OnLButtonDown(int x, int y) = 0;
	virtual void OnRButtonDown(int x, int y) = 0;
	virtual void OnMButtonDown(int x, int y) = 0;
	virtual void OnMouseMove(int x, int y, int delta_x, int delta_y, bool l_button, bool r_button, bool m_button) = 0;
};
