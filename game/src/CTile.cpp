#include <CTile.h>

CTile::CTile()
	: mCoords({-1000, -1000})
	, mCandy(nullptr)
{
}

CTile::CTile(SBoardCoords coords)
	: mCoords(coords)
	, mCandy(nullptr)
{
}

CTile::CTile(SBoardCoords coords, CCandy* candy)
	: mCoords(coords)
	, mCandy(candy)
{
}
