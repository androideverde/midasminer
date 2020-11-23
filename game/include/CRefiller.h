#pragma once

#include <SBoardCoords.h>
#include <CBoardState.h>
#include <CAnimationSystem.h>

class CRefiller
{
public:
	CRefiller(CBoardState& state, CAnimationSystem& animationQueue);
	bool IsRefillPending() const;
	void RefillBoard();
	void TriggerFallAnimation(SBoardCoords origin, SBoardCoords destination);
private:
	CBoardState& mState;
	CAnimationSystem& mAnimationQueue;
};

