#pragma once

#include <ETileType.h>
#include <SBoardCoords.h>
#include <SDL.h>
#include <vector>
#include <map>

class CBoardState
{
public:
	CBoardState(int size);
	void SetupBoard();
	void SetTile(int pos, TileType type);
	TileType GetTile(int pos) const { return mBoardState[pos]; }
	std::vector<int> GetRowNeighboursSameAsTile(int pos) const;
	std::vector<int> GetColNeighboursSameAsTile(int pos) const;
	int GetIndexFromCoords(SBoardCoords coords) const;
private:
	std::vector<TileType> mBoardState;
	const int BOARD_SIZE;
	enum class EDirection {LEFT, RIGHT, UP, DOWN};
	SBoardCoords GetCoordsFromIndex(int index) const;
	std::vector<int> GetNeighbours(int pos, EDirection direction) const;
};
