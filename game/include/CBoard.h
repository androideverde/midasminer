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
	void Update(float delta_time);
	void Render(SDL_Renderer* renderer);
	const int GetBoardPos(int x, int y) const;
	void OnClick(int pos);
	void OnDrag(int start, int end);
private:
	CBoardState mBoardState;
	std::map<TileType, SDL_Texture*> mTextures;
	const int CELL_SIZE = 43; // each board square is 43x43 pixels
	const int ORIGIN_X = 330; // board top left is at (330, 100) of the BackGround.jpg image
	const int ORIGIN_Y = 100;
};
