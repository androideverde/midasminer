#include <CMoveAnimation.h>

#include <cassert>

CMoveAnimation::CMoveAnimation(SDL_Point start, SDL_Point end, float speed, CCandy* candy)
	: mPositionStart(start)
	, mPositionEnd(end)
	, mCandy(candy)
	, CAnimation(speed)
{
}

void CMoveAnimation::Update(float delta_time)
{
	SDL_Point oldPos = mCandy->GetPos();
	SDL_Point newPos;
	float deltaX = mPositionEnd.x - mPositionStart.x;
	float deltaY = mPositionEnd.y - mPositionStart.y;
	float speed;
	if (deltaX == 0)
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
		printf("move anim done! (%d, %d) -> (%d, %d) %d\n", mPositionStart.x, mPositionStart.y, mPositionEnd.x, mPositionEnd.y, mCandy->GetType());
	}
	else
	{
		mCandy->SetPos(newPos);
	}
}

int CMoveAnimation::UpdatePos(float delta_time, int oldPos, float speed) const
{
	float result = oldPos + speed * delta_time;
	return result;
}

bool CMoveAnimation::IsMoveComplete(int oldPos, int newPos, int endPos) const
{
	if ((oldPos - endPos) * (newPos - endPos) <= 0)
	{
		return true;
	}
	return false;
}
