#pragma once

#include <SBoardCoords.h>
#include <CBoardState.h>

class CMatcher {
public:
	CMatcher(CBoardState& state);
	bool IsMatchInTile(SBoardCoords coords);
	void DoMatchInTile(SBoardCoords coords);
private:
	CBoardState& mState;
};
