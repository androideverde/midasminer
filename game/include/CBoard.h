#pragma once

#include <CBoardState.h>
#include <ETileType.h>
#include <SBoardCoords.h>
#include <SDL.h>
#include <set>
#include <map>

class CBoard
{
public:
	CBoard();
	~CBoard();
	void LoadResources(SDL_Renderer* renderer);
	void Update(float delta_time);
	void Render(SDL_Renderer* renderer);
	SBoardCoords GetBoardTileCoords(int x, int y) const;
	void OnClick(SBoardCoords coords);
	void OnDrag(SBoardCoords startCoords, SBoardCoords endCoords);
	bool DoSwap(SBoardCoords tileCoords_1, SBoardCoords tileCoords_2);
	bool IsMatchInTile(SBoardCoords coords) const;
	void DoMatchInTile(SBoardCoords coords);
	void DoPendingMatches();
private:
	std::map<TileType, SDL_Texture*> mTextures;
	const int TILE_SIZE = 43; // each board tile is 43x43 pixels
	const int ORIGIN_X = 330; // board top left is at (330, 100) of the BackGround.jpg image
	const int ORIGIN_Y = 100;
	const int BOARD_SIZE = 8; // board is 8x8 tiles
	CBoardState mBoardState;
	SBoardCoords mSwappedTileCoords_1;
	SBoardCoords mSwappedTileCoords_2;
};
