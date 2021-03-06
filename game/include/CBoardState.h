#pragma once

#include <SBoardCoords.h>
#include <SPixelCoords.h>
#include <CCandyGenerator.h>
#include <CCandy.h>
#include <CTile.h>
#include <vector>
#include <string>
#include <set>
#include <map>

class CBoardState
{
public:
	CBoardState(int size, int tileSize, int originX, int originY, std::unique_ptr<const CCandyGenerator> candyGenerator);
	void SetupBoard(const std::vector<int>& boardDefinition);
	const CTile& GetTile(SBoardCoords coords) const { return mTiles[GetIndexFromCoords(coords)]; }
	CTile& GetTile(SBoardCoords coords) { return mTiles[GetIndexFromCoords(coords)]; }
	int CountRowNeighboursSameAsTile(SBoardCoords coords) const;
	int CountColNeighboursSameAsTile(SBoardCoords coords) const;
	std::set<SBoardCoords> GetMatchedNeighboursSameAsTile(SBoardCoords coords);
	std::set<SBoardCoords> KeepCollectingNeighbours(const std::set<SBoardCoords>& list, const std::string& orientation, std::set<SBoardCoords>& result);
	void Swap(SBoardCoords tileCoords_1, SBoardCoords tileCoords_2);
	int GetBoardSize() const { return BOARD_SIZE; }
	int GetTileSize() const { return TILE_SIZE; }
	SPixelCoords GetTilePos(int index) const;
	SPixelCoords GetTilePos(SBoardCoords coords) const;
	void AddNewCandy(SBoardCoords coords);
private:
	std::vector<std::unique_ptr<CCandy>> mCandies;
	std::vector<CTile> mTiles;
	const int BOARD_SIZE;
	const int TILE_SIZE;
	const SPixelCoords BOARD_ORIGIN;
	std::unique_ptr<const CCandyGenerator> mCandyGenerator;
	enum class EDirection {LEFT, RIGHT, UP, DOWN};
	SBoardCoords GetCoordsFromIndex(int index) const;
	int GetIndexFromCoords(SBoardCoords coords) const;
	std::vector<int> GetNeighbours(SBoardCoords coords, EDirection direction) const;
	std::vector<int> GetRowNeighboursSameAsTile(SBoardCoords coords) const;
	std::vector<int> GetColNeighboursSameAsTile(SBoardCoords coords) const;
};
