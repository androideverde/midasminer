#pragma once

#include <SDL.h>
#include <CCandy.h>

class CAnimation
{
public:
	CAnimation() = default;
	CAnimation(float speed);
	virtual ~CAnimation() = default;
	virtual void Update(float delta_time) = 0;
	bool IsCompleted() const { return mCompleted; }
protected:
	float mSpeed = 0.f;
	bool mCompleted = false;
};
