#include <CAnimation.h>

CAnimation::CAnimation(AnimationType type, SDL_Point start, SDL_Point end, float duration, CCandy* candy)
	: mAnimationType(type)
	, mPositionStart(start)
	, mPositionEnd(end)
	, mDuration(duration)
	, mElapsed(0.f)
	, mItem(candy)
{
}
