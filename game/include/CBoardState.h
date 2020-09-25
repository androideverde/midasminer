#pragma once

#include <ETileType.h>
#include <SDL.h>
#include <vector>
#include <map>

class CBoardState
{
public:
	CBoardState();
	~CBoardState();
	void SetupBoard();
	void LoadResources(SDL_Renderer* renderer);
	void SetTile(int pos, TileType type);
	void RemoveTile(int pos);
	const TileType GetTile(int pos) const;
	const std::vector<TileType> GetRow(int row) const;
	const std::vector<TileType> GetColumn(int col) const;
	void Update();
	void Render(SDL_Renderer* renderer);
private:
	std::vector<TileType> mBoardState;
	std::map<TileType, SDL_Texture*> mTextures;
};
