#pragma once

#include <SBoardCoords.h>
#include <CBoardState.h>
#include <CAnimationSystem.h>

class CMatcher
{
public:
	CMatcher(CBoardState& state, CAnimationSystem& animationQueue);
	bool IsMatchInTile(SBoardCoords coords) const;
	std::set<SBoardCoords> GetMatchGroup(SBoardCoords coords);
	std::set<SBoardCoords> DoMatchInTile(SBoardCoords coords);
	std::set<SBoardCoords> DoMatch();
	bool IsMatchPending() const;
private:
	CBoardState& mState;
	CAnimationSystem& mAnimationQueue;
	std::set<SBoardCoords> GetMatchesRecursively(std::set<SBoardCoords> initial);
};
