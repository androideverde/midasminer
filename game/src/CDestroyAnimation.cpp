#include <CDestroyAnimation.h>

CDestroyAnimation::CDestroyAnimation(float duration, CCandy* candy)
	: mCandy(candy)
	, CAnimation(duration)
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
