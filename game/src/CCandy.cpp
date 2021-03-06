#include <CCandy.h>

CCandy::CCandy(CandyType tileType)
	: mTileType(tileType)
	, mPoint({0, 0})
{
}

CCandy::CCandy(CandyType tileType, SPixelCoords pos)
	: mTileType(tileType)
	, mPoint(pos)
{
}

void CCandy::SetType(CandyType tileType)
{
	mTileType = tileType;
}

void CCandy::SetPos(float x, float y)
{
	mPoint.x = x;
	mPoint.y = y;
}

void CCandy::SetPos(SPixelCoords point)
{
	mPoint = point;
}

std::string CCandy::GetName() const
{
	switch (mTileType) {
		case CandyType::BLUE:
			return "BLUE";
			break;
		case CandyType::GREEN:
			return "GREEN";
			break;
		case CandyType::RED:
			return "RED";
			break;
		case CandyType::YELLOW:
			return "YELLOW";
			break;
		case CandyType::PURPLE:
			return "PURPLE";
			break;
		case CandyType::EMPTY:
			return "EMPTY";
			break;
	}
}
