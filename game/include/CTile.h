#pragma once

#include <SBoardCoords.h>
#include <CCandy.h>

class CTile
{
public:
	CTile();
	CTile(SBoardCoords coords);
	CTile(SBoardCoords coords, CCandy* candy);
	SBoardCoords GetCoords() const { return mCoords; }
	int GetRow() const { return mCoords.row; }
	int GetCol() const { return mCoords.col; }
	CCandy* GetCandy() const { return mCandy; }
	void SetCandy(CCandy* candy) { mCandy = candy; }
private:
	SBoardCoords mCoords;
	CCandy* mCandy;
};
