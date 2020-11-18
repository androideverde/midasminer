#pragma once

#include <SBoardCoords.h>
#include <CCandyGenerator.h>
#include <CCandy.h>
#include <CTile.h>
#include <SDL.h>
#include <vector>
#include <string>
#include <set>
#include <map>

class CBoardState
{
public:
	CBoardState(int size, int tileSize, int originX, int originY, std::unique_ptr<const CCandyGenerator> candyGenerator);
	void SetupBoard(const std::vector<int>& boardDefinition);
	CandyType GetCandyType(SBoardCoords coords) const;
	CCandy* GetCandy(SBoardCoords coords) const;
	CTile& GetTile(SBoardCoords coords) { return mTiles[GetIndexFromCoords(coords)]; }
	int CountRowNeighboursSameAsTile(SBoardCoords coords) const;
	int CountColNeighboursSameAsTile(SBoardCoords coords) const;
	std::set<SBoardCoords> GetNeighboursSameAsTile(SBoardCoords coords) const;
	std::vector<CCandy*> Refill();
	void Swap(SBoardCoords tileCoords_1, SBoardCoords tileCoords_2);
	std::string GetTileName(CandyType tile) const;
	std::map<CCandy*, int> GetFallingCandies() const;
	std::map<std::unique_ptr<CCandy>, int> GenerateNewCandies() const;
private:
	std::vector<std::unique_ptr<CCandy>> mCandies;
	std::vector<CTile> mTiles;
	const int BOARD_SIZE;
	const int TILE_SIZE;
	const SDL_Point BOARD_ORIGIN;
	std::unique_ptr<const CCandyGenerator> mCandyGenerator;
	enum class EDirection {LEFT, RIGHT, UP, DOWN};
	SBoardCoords GetCoordsFromIndex(int index) const;
	int GetIndexFromCoords(SBoardCoords coords) const;
	std::vector<int> GetNeighbours(SBoardCoords coords, EDirection direction) const;
	std::vector<int> GetRowNeighboursSameAsTile(SBoardCoords coords) const;
	std::vector<int> GetColNeighboursSameAsTile(SBoardCoords coords) const;
	std::vector<SBoardCoords> ShiftColumnDown(SBoardCoords coords);
	void AddNewCandy(SBoardCoords coords);
	SDL_Point ResetCandyPos(int index) const;
};
