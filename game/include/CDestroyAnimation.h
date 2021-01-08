#pragma once

#include <CAnimation.h>

class CDestroyAnimation
	: public CAnimation
{
public:
	CDestroyAnimation(float speed, CCandy* candy);
	void Update(float delta_time) override;
private:
	CCandy* mCandy;
	int mCount = 0;
};
