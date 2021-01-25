#pragma once

#include <CAnimation.h>

#include <SPixelCoords.h>

class CMoveAnimation
	: public CAnimation
{
public:
	CMoveAnimation(SPixelCoords start, SPixelCoords end, float speed, CCandy* candy);
	void Update(float delta_time) override;
	SPixelCoords GetStart() const { return mPositionStart; }
	SPixelCoords GetEnd() const { return mPositionEnd; }
private:
	SPixelCoords mPositionStart;
	SPixelCoords mPositionEnd;
	CCandy* mCandy;
	float UpdatePos(float delta_time, float oldPos, float speed) const;
	bool IsMoveComplete(float oldPos, float newPos, float endPos) const;
};
