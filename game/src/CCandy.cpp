#include <CCandy.h>

CCandy::CCandy(TileType tileType)
	: mTileType(tileType)
	, mPoint({0, 0})
{
}

CCandy::CCandy(TileType tileType, SDL_Point pos)
	: mTileType(tileType)
	, mPoint(pos)
{
}

void CCandy::SetType(TileType tileType)
{
	mTileType = tileType;
}

void CCandy::SetPos(int x, int y)
{
	mPoint.x = x;
	mPoint.y = y;
}

void CCandy::SetPos(SDL_Point point)
{
	mPoint = point;
}
