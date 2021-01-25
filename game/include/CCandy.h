#pragma once

#include <SPixelCoords.h>
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
	CCandy(CandyType tileType, SPixelCoords pos);
	CandyType GetType() const { return mTileType; }
	std::string GetName() const;
	float GetX() const { return mPoint.x; }
	float GetY() const { return mPoint.y; }
	SPixelCoords GetPos() const { return mPoint; }
	void SetType(CandyType tileType);
	void SetPos(float x, float y);
	void SetPos(SPixelCoords point);
private:
	SPixelCoords mPoint;
	CandyType mTileType;
};
