#include <CDelayAnimation.h>

CDelayAnimation::CDelayAnimation(float speed, float duration)
	: mDuration(duration)
	, mElapsed(0.f)
	, CAnimation(speed)
{
}

void CDelayAnimation::Update(float delta_time)
{
	mElapsed += delta_time;
	if (mElapsed >= mDuration)
	{
		mCompleted = true;
	}
}
