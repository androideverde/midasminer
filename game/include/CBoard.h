#pragma once

#include <CBoardState.h>
#include <CMatcher.h>
#include <CSwapper.h>
#include <CRefiller.h>
#include <SBoardCoords.h>
#include <CAnimationSystem.h>
#include <CCandy.h>
#include <SDL.h>
#include <set>
#include <map>
#include <vector>

class CBoard
{
public:
	CBoard();
	~CBoard();
	void LoadResources(SDL_Renderer* renderer);
	void Update(float delta_time);
	void Render(SDL_Renderer* renderer) const;
	SBoardCoords GetBoardTileCoords(int x, int y) const;
	void OnClick(SBoardCoords coords);
	void OnDrag(SBoardCoords startCoords, SBoardCoords endCoords);
	bool IsSwapPending() const;

private:
	std::map<CandyType, SDL_Texture*> mTextures;
	const int TILE_SIZE = 43; // each board tile is 43x43 pixels
	const int ORIGIN_X = 330; // board top left is at (330, 100) of the BackGround.jpg image
	const int ORIGIN_Y = 100;
	const int BOARD_SIZE = 8; // board is 8x8 tiles
	CBoardState mBoardState;
	CMatcher mMatcher;
	CSwapper mSwapper;
	CRefiller mRefiller;
	SBoardCoords mSwappedTileCoords_1;
	SBoardCoords mSwappedTileCoords_2;
	CAnimationSystem mAnimationQueue;
};
