#pragma once

#include <SBoardCoords.h>
#include <CBoardState.h>
#include <CAnimationSystem.h>

class CMatcher
{
public:
	CMatcher(CBoardState& state, CAnimationSystem& animationQueue);
	bool IsMatchInTile(SBoardCoords coords);
	void DoMatchInTile(SBoardCoords coords);
private:
	CBoardState& mState;
	CAnimationSystem& mAnimationQueue;
};
