#include <CMoveAnimation.h>

#include <cassert>
#include <cmath>
#include <limits>

CMoveAnimation::CMoveAnimation(SPixelCoords start, SPixelCoords end, float speed, CCandy* candy)
	: mPositionStart(start)
	, mPositionEnd(end)
	, mCandy(candy)
	, CAnimation(speed)
{
}

void CMoveAnimation::Update(float delta_time)
{
	SPixelCoords oldPos = mCandy->GetPos();
	SPixelCoords newPos;
	float deltaX = mPositionEnd.x - mPositionStart.x;
	float deltaY = mPositionEnd.y - mPositionStart.y;
	float speed;
	float epsilon = std::numeric_limits<float>::epsilon();
	if (std::abs(deltaX) < epsilon)
	{
		newPos.x = oldPos.x;
		speed = deltaY > 0 ? mSpeed : -mSpeed;
		newPos.y = UpdatePos(delta_time, oldPos.y, speed);
		mCompleted = IsMoveComplete(oldPos.y, newPos.y, mPositionEnd.y);
	}
	if (deltaY == 0)
	{
		newPos.y = oldPos.y;
		speed = deltaX > 0 ? mSpeed : -mSpeed;
		newPos.x = UpdatePos(delta_time, oldPos.x, speed);
		mCompleted = IsMoveComplete(oldPos.x, newPos.x, mPositionEnd.x);
	}
	if (mCompleted)
	{
		mCandy->SetPos(mPositionEnd);
		printf("move anim done! (%f, %f) -> (%f, %f) %d\n", mPositionStart.x, mPositionStart.y, mPositionEnd.x, mPositionEnd.y, mCandy->GetType());
	}
	else
	{
		mCandy->SetPos(newPos);
	}
}

float CMoveAnimation::UpdatePos(float delta_time, float oldPos, float speed) const
{
	return oldPos + speed * delta_time;
}

bool CMoveAnimation::IsMoveComplete(float oldPos, float newPos, float endPos) const
{
	if ((oldPos - endPos) * (newPos - endPos) <= 0.f)
	{
		return true;
	}
	return false;
}
