#pragma once

#include <SDL.h>
#include <CCandy.h>

enum class AnimationType
{
	MOVE,
	DESTROY
};

class CAnimation
{
public:
	CAnimation(AnimationType type, SDL_Point start, SDL_Point end, float duration, CCandy* candy);
	AnimationType GetType() const { return mAnimationType; }
	SDL_Point GetStart() const { return mPositionStart; }
	SDL_Point GetEnd() const { return mPositionEnd; }
	float GetDuration() const { return mDuration; }
	float GetElapsed() const { return mElapsed; }
	void SetElapsed(float value) { mElapsed = value; }
	CCandy* GetCandy() { return mItem; }
private:
	AnimationType mAnimationType;
	SDL_Point mPositionStart;
	SDL_Point mPositionEnd;
	float mDuration;
	float mElapsed;
	CCandy* mItem;
};
