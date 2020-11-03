#include <CAnimation.h>

CAnimation::CAnimation(AnimationType type, SDL_Point start, SDL_Point end, float duration, CCandy* candy)
	: mAnimationType(type)
	, mPositionStart(start)
	, mPositionEnd(end)
	, mDuration(duration)
	, mElapsed(0.f)
	, mItem(candy)
	, mCompleted(false)
{
}

void CAnimation::Update(float delta_time)
{
	float timeStep;
	int moveStepX;
	int moveStepY;
	SDL_Point pos;
	timeStep = delta_time / (mDuration - mElapsed);
	switch (mAnimationType) {
		case AnimationType::MOVE:
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
			break;
		case AnimationType::DESTROY:
			mElapsed += delta_time;
			if (mElapsed >= mDuration)
			{
				mCompleted = true;
			}
			break;
	}
}
