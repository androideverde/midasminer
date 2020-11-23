#pragma once

#include <SBoardCoords.h>
#include <CBoardState.h>
#include <CAnimationSystem.h>
#include <CMatcher.h>

class CSwapper
{
public:
	CSwapper(CBoardState& state, CAnimationSystem& animationQueue, CMatcher& matcher);
	bool DoSwap(SBoardCoords tileCoords_1, SBoardCoords tileCoords_2);
	void TriggerSwapInAnimations(SBoardCoords tile_1, SBoardCoords tile_2);
	void TriggerSwapOutAnimations(SBoardCoords tile_1, SBoardCoords tile_2);
private:
	CBoardState& mState;
	CAnimationSystem& mAnimationQueue;
	CMatcher& mMatcher;
};

