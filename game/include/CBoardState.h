#pragma once

#include <ETileType.h>
#include <SDL.h>
#include <vector>

class CBoardState
{
public:
	CBoardState();
	void SetupBoard();
	void SetTile(int pos, TileType type);
	void RemoveTile(int pos);
	const TileType GetTile(int pos) const;
	const std::vector<TileType> GetRow(int row) const;
	const std::vector<TileType> GetColumn(int col) const;
private:
	std::vector<TileType> mBoardState;
};
