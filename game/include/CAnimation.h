#pragma once

#include <SDL.h>
#include <CCandy.h>

class CAnimation
{
public:
	CAnimation(SDL_Point start, SDL_Point end, float duration, CCandy* candy);
	virtual ~CAnimation();
	SDL_Point GetStart() const { return mPositionStart; }
	SDL_Point GetEnd() const { return mPositionEnd; }
	float GetDuration() const { return mDuration; }
	float GetElapsed() const { return mElapsed; }
	void SetElapsed(float value) { mElapsed = value; }
	CCandy* GetCandy() { return mItem; }
	virtual void Update(float delta_time) = 0;
	bool IsCompleted() const { return mCompleted; }
protected:
	SDL_Point mPositionStart;
	SDL_Point mPositionEnd;
	float mDuration;
	float mElapsed;
	CCandy* mItem;
	bool mCompleted;
};
