#include <CAnimation.h>

CAnimation::CAnimation(float duration)
	: mDuration(duration)
	, mElapsed(0.f)
	, mCompleted(false)
{
}

CAnimation::~CAnimation()
{
}
