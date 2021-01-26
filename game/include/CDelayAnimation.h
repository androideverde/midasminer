#pragma once

#include <CAnimation.h>

class CDelayAnimation
	: public CAnimation
{
public:
	CDelayAnimation(float speed, float duration);
	void Update(float delta_time) override;
private:
	float mDuration;
	float mElapsed;
};
