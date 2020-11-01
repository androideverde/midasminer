#pragma once

#include <SDL.h>
#include <ETileType.h>

class CCandy
{
public:
	CCandy(TileType tileType);
	CCandy(TileType tileType, SDL_Point pos);
	TileType GetType() const { return mTileType; }
	int GetX() const { return mPoint.x; }
	int GetY() const { return mPoint.y; }
	SDL_Point GetPos() const { return mPoint; }
	void SetType(TileType tileType);
	void SetPos(int x, int y);
	void SetPos(SDL_Point point);
private:
	SDL_Point mPoint;
	TileType mTileType;
};
