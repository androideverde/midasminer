#pragma once

#include <ETileType.h>
#include <SBoardCoords.h>
#include <SDL.h>
#include <vector>
#include <set>
#include <map>

class CBoardState
{
public:
	CBoardState(int size);
	void SetupBoard();
	void SetTile(SBoardCoords coords, TileType type);
	TileType GetTile(SBoardCoords coords) const;
	int CountRowNeighboursSameAsTile(SBoardCoords coords) const;
	int CountColNeighboursSameAsTile(SBoardCoords coords) const;
	std::set<SBoardCoords> GetNeighboursSameAsTile(SBoardCoords coords) const;
	void Refill();
private:
	std::vector<TileType> mBoardState;
	const int BOARD_SIZE;
	enum class EDirection {LEFT, RIGHT, UP, DOWN};
	SBoardCoords GetCoordsFromIndex(int index) const;
	int GetIndexFromCoords(SBoardCoords coords) const;
	std::vector<int> GetNeighbours(SBoardCoords coords, EDirection direction) const;
	std::vector<int> GetRowNeighboursSameAsTile(SBoardCoords coords) const;
	std::vector<int> GetColNeighboursSameAsTile(SBoardCoords coords) const;
	void ShiftColumnDown(SBoardCoords coords);
	void AddNewCandy(SBoardCoords coords);
};
