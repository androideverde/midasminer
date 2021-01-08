#include <CDestroyAnimation.h>

CDestroyAnimation::CDestroyAnimation(float speed, CCandy* candy)
	: mCandy(candy)
	, CAnimation(speed)
{
}

void CDestroyAnimation::Update(float delta_time)
{
	mCount++;
	if (mCount == mSpeed)
	{
		mCompleted = true;
	}
}
