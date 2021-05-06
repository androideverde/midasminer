#pragma once

#include <SDL.h>

#include <functional>

class CApp;

class CInputHandler
{
public:
	CInputHandler(std::function<void()> exitCallback);
	void OnEvent(SDL_Event* event);
	void OnExit();
	void OnLButtonDown(int x, int y);
	void OnRButtonDown(int x, int y);
	void OnMButtonDown(int x, int y);
	void OnButtonUp(int x, int y);
	void OnMouseMove(int x, int y, int delta_x, int delta_y, bool l_button, bool r_button, bool m_button);
private:
	std::function<void()> mExitCallback;
};
