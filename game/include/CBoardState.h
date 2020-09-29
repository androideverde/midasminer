#pragma once

#include <ETileType.h>
#include <SDL.h>
#include <vector>
#include <map>

class CBoardState
{
public:
	CBoardState();
	void SetupBoard();
	void SetTile(int pos, TileType type);
	const TileType GetTile(int pos) const { return mBoardState[pos]; }
private:
	std::vector<TileType> mBoardState;
};
