#pragma once

#include <ETileType.h>
#include <SBoardCoords.h>
#include <CCandyGenerator.h>
#include <CCandy.h>
#include <SDL.h>
#include <vector>
#include <string>
#include <set>
#include <map>

class CBoardState
{
public:
	CBoardState(int size, int tile_size, std::unique_ptr<const CCandyGenerator> candyGenerator);
	void SetupBoard(const std::vector<int>& boardDefinition);
	void SetTile(SBoardCoords coords, TileType type);
	TileType GetTile(SBoardCoords coords) const;
	CCandy* GetCandy(SBoardCoords coords);
	int CountRowNeighboursSameAsTile(SBoardCoords coords) const;
	int CountColNeighboursSameAsTile(SBoardCoords coords) const;
	std::set<SBoardCoords> GetNeighboursSameAsTile(SBoardCoords coords) const;
	void Refill();
	std::string GetTileName(TileType tile) const;
private:
	std::vector<std::unique_ptr<CCandy>> mCandies;
	const int BOARD_SIZE;
	const int TILE_SIZE;
	std::unique_ptr<const CCandyGenerator> mCandyGenerator;
	enum class EDirection {LEFT, RIGHT, UP, DOWN};
	SBoardCoords GetCoordsFromIndex(int index) const;
	int GetIndexFromCoords(SBoardCoords coords) const;
	std::vector<int> GetNeighbours(SBoardCoords coords, EDirection direction) const;
	std::vector<int> GetRowNeighboursSameAsTile(SBoardCoords coords) const;
	std::vector<int> GetColNeighboursSameAsTile(SBoardCoords coords) const;
	void ShiftColumnDown(SBoardCoords coords);
	void AddNewCandy(SBoardCoords coords);
	SDL_Point ResetCandyPos(int index);
};
