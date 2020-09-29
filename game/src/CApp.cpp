#include <CApp.h>

#include <chrono>
#include <thread>

#include <SDL_image.h>
#include <Utils.h>

CApp::CApp()
	: mRunning(true)
	, mWindow(nullptr)
	, mRenderer(nullptr)
	, mBackground(nullptr)
	, mBoard()
	, mClicked(false)
	, mMouseX(0)
	, mMouseY(0)
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
	using clock = std::chrono::system_clock;
	auto previousTime = clock::now();
	
	// game loop
	while (mRunning)
	{
		// adjust time step for this frame
		auto startTime = clock::now();
		int elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(startTime - previousTime).count();
		previousTime = startTime;
		elapsed_ms = std::min(200, elapsed_ms);
		
		// process inputs
		while (SDL_PollEvent(&event))
		{
			OnEvent(&event);
		}
		
		OnLoop(elapsed_ms /1000.f);
		
		OnRender();
		
		// sleep for the rest of the frame
		auto endTime = clock::now();
		int frameTime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
		int sleep_ms = MS_PER_UPDATE - frameTime_ms;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
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
	mBoard.LoadResources(mRenderer);
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
	if (mClicked)
	{
		mMouseX = x;
		mMouseY = y;
	}
}

void CApp::OnLButtonDown(int x, int y)
{
	mClicked = true;
	mMouseX = x;
	mMouseY = y;
}

void CApp::OnRButtonDown(int x, int y)
{
}

void CApp::OnMButtonDown(int x, int y)
{
}

void CApp::OnButtonUp(int x, int y)
{
	mClicked = false;
	mMouseX = x;
	mMouseY = y;
}

void CApp::OnLoop(float delta_time)
{
	mBoard.Update(delta_time, mClicked, mMouseX, mMouseY);
}

void CApp::OnRender()
{
	SDL_RenderClear(mRenderer);
	SDL_RenderCopy(mRenderer, mBackground, nullptr, nullptr);
	mBoard.Render(mRenderer);
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
