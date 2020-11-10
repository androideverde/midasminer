#pragma once

#include <CAnimation.h>

class CDestroyAnimation
	: public CAnimation
{
public:
	CDestroyAnimation(float duration, CCandy* candy);
	void Update(float delta_time) override;
private:
	CCandy* mCandy;
};
