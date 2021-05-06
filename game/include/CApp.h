#pragma once

#include <SDL.h>
#include <CEvent.h>
#include <SBoardCoords.h>
#include <CBoard.h>
#include <CScoringSystem.h>
#include <CInputHandler.h>
#include <string>

class CApp
{
public:
	CApp();
	int OnExecute();
	bool OnInit();
	void OnEvent(SDL_Event* event);
	void OnExit();
	void OnLoop(float delta_time);
	void OnRender();
	void OnCleanup();
private:
	bool mRunning;
	SBoardCoords mTileClickedCoords;
	bool mDragging;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Texture* mBackground;
	const int MS_PER_UPDATE = 16; // run at 60 FPS
	CScoringSystem mScoringSystem;
	CBoard mBoard;
	CInputHandler mInputHandler;

	bool LoadResources();
};
