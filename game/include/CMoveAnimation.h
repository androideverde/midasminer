#pragma once

#include <CAnimation.h>

class CMoveAnimation
	: public CAnimation
{
public:
	CMoveAnimation(SDL_Point start, SDL_Point end, float duration, CCandy* candy);
	void Update(float delta_time) override;
	SDL_Point GetStart() const { return mPositionStart; }
	SDL_Point GetEnd() const { return mPositionEnd; }
private:
	SDL_Point mPositionStart;
	SDL_Point mPositionEnd;
	CCandy* mCandy;
};
