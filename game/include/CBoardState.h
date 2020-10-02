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
	TileType GetTile(int pos) const { return mBoardState[pos]; }
	std::vector<int> GetRowNeighboursSameAsTile(int pos) const;
	std::vector<int> GetColNeighboursSameAsTile(int pos) const;
private:
	std::vector<TileType> mBoardState;
};
