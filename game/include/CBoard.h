#pragma once

#include <CBoardState.h>
#include <ETileType.h>
#include <SDL.h>
#include <map>

class CBoard
{
public:
	CBoard();
	~CBoard();
	void LoadResources(SDL_Renderer* renderer);
	void Update();
	void Render(SDL_Renderer* renderer);
private:
	CBoardState mBoardState;
	std::map<TileType, SDL_Texture*> mTextures;
};
