#include <CDestroyAnimation.h>

CDestroyAnimation::CDestroyAnimation(float duration, CCandy* candy)
	: CAnimation({0, 0}, {0, 0}, duration, candy)
{
}

CDestroyAnimation::~CDestroyAnimation()
{
}

void CDestroyAnimation::Update(float delta_time)
{
	mElapsed += delta_time;
	if (mElapsed >= mDuration)
	{
		mCompleted = true;
	}
}
