#include <CAnimation.h>

CAnimation::CAnimation(SDL_Point start, SDL_Point end, float duration, CCandy* candy)
	: mPositionStart(start)
	, mPositionEnd(end)
	, mDuration(duration)
	, mElapsed(0.f)
	, mItem(candy)
	, mCompleted(false)
{
}

CAnimation::~CAnimation()
{
}
