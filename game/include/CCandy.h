#pragma once

#include <SDL.h>
#include <string>

enum class CandyType {
	EMPTY,
	BLUE,
	GREEN,
	RED,
	YELLOW,
	PURPLE
};

class CCandy
{
public:
	CCandy(CandyType tileType);
	CCandy(CandyType tileType, SDL_Point pos);
	CandyType GetType() const { return mTileType; }
	std::string GetName() const;
	int GetX() const { return mPoint.x; }
	int GetY() const { return mPoint.y; }
	SDL_Point GetPos() const { return mPoint; }
	void SetType(CandyType tileType);
	void SetPos(int x, int y);
	void SetPos(SDL_Point point);
private:
	SDL_Point mPoint;
	CandyType mTileType;
};
