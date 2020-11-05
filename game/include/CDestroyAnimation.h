#pragma once

#include <CAnimation.h>

class CDestroyAnimation
	: public CAnimation
{
public:
	CDestroyAnimation(float duration, CCandy* candy);
	~CDestroyAnimation();
	void Update(float delta_time);
};
