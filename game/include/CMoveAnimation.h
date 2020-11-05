#pragma once

#include <CAnimation.h>

class CMoveAnimation
	: public CAnimation
{
public:
	CMoveAnimation(SDL_Point start, SDL_Point end, float duration, CCandy* candy);
	~CMoveAnimation();
	void Update(float delta_time);
};
