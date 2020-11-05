#include <CMoveAnimation.h>

#include <cassert>

CMoveAnimation::CMoveAnimation(SDL_Point start, SDL_Point end, float duration, CCandy* candy)
	: CAnimation(start, end, duration, candy)
{
}

CMoveAnimation::~CMoveAnimation()
{
}

void CMoveAnimation::Update(float delta_time)
{
	float timeStep;
	int moveStepX;
	int moveStepY;
	SDL_Point pos;
	assert(mElapsed < mDuration);
	timeStep = delta_time / (mDuration - mElapsed);
	pos = mPositionStart;
	moveStepX = (mPositionEnd.x - pos.x) * timeStep;
	moveStepY = (mPositionEnd.y - pos.y) * timeStep;
	pos.x += moveStepX;
	pos.y += moveStepY;
	mItem->SetPos(pos);
	mElapsed += delta_time;
	if (mElapsed >= mDuration)
	{
		mCompleted = true;
		mItem->SetPos(mPositionEnd);
	}
}
