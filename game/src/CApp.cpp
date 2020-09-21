#include <CApp.h>

#include <chrono>
#include <algorithm>
#include <thread>

#include <SDL_image.h>
#include <Utils.h>

CApp::CApp()
	: mRunning(true)
	, mWindow(nullptr)
	, mRenderer(nullptr)
	, mBackground(nullptr)
{
}

// main entry point
int CApp::OnExecute()
{
	// initialise
	if (!OnInit())
	{
		return -1;
	}
	SDL_Event event;
	
	// game loop
	auto previousTime = std::chrono::system_clock::now();
	int lag_ms = 0;
	while (mRunning)
	{
		// adjust time step
		auto currentTime = std::chrono::system_clock::now();
		int elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count();
		previousTime = currentTime;
		elapsed_ms = std::max(200, elapsed_ms);
		lag_ms += elapsed_ms;
		
		// process inputs
		while (SDL_PollEvent(&event))
		{
			OnEvent(&event);
		}
		
		while (lag_ms >= MS_PER_UPDATE)
		{
			OnLoop(MS_PER_UPDATE / 1000.f);
			lag_ms -= MS_PER_UPDATE;
		}
		OnRender();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	
	// destroy everything
	OnCleanup();
	
	return 0;
}

bool CApp::OnInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}
	mWindow = SDL_CreateWindow("SDL Tutorial", 0, 0, 755, 600, SDL_WINDOW_SHOWN);
	if (mWindow == nullptr)
	{
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == nullptr)
	{
		return false;
	}
	if (!LoadResources())
	{
		return false;
	}
	return true;
}

bool CApp::LoadResources()
{
	mBackground = Utils::LoadImage("../assets/BackGround.jpg", mRenderer);
	if (mBackground == nullptr)
	{
		return false;
	}
	return true;
}

void CApp::OnEvent(SDL_Event* event)
{
	CEvent::OnEvent(event);
}

void CApp::OnExit()
{
	mRunning = false;
}

void CApp::OnMouseMove(int x, int y, int delta_x, int delta_y, bool l_button, bool r_button, bool m_button)
{
}

void CApp::OnLButtonDown(int x, int y)
{
}

void CApp::OnRButtonDown(int x, int y)
{
}

void CApp::OnMButtonDown(int x, int y)
{
}

void CApp::OnLoop(float delta_time)
{
}

void CApp::OnRender()
{
	SDL_RenderClear(mRenderer);
	SDL_RenderCopy(mRenderer, mBackground, nullptr, nullptr);
	SDL_RenderPresent(mRenderer);
}

void CApp::OnCleanup()
{
	SDL_DestroyRenderer(mRenderer);
	mRenderer = nullptr;
	SDL_DestroyTexture(mBackground);
	mBackground = nullptr;
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;
	SDL_Quit();
}
