#pragma once

#include <SDL.h>
#include <CCandy.h>

class CAnimation
{
public:
	CAnimation(float duration);
	virtual ~CAnimation();
	virtual void Update(float delta_time) = 0;
	float GetDuration() const { return mDuration; }
	bool IsCompleted() const { return mCompleted; }
protected:
	float mDuration;
	float mElapsed;
	bool mCompleted;
	float GetElapsed() const { return mElapsed; }
	void SetElapsed(float value) { mElapsed = value; }
};
