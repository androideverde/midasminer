#include <CMatcher.h>

#include <CDestroyAnimation.h>

CMatcher::CMatcher(CBoardState& state, CAnimationSystem& animationQueue)
	: mState(state)
	, mAnimationQueue(animationQueue)
{
}

void CMatcher::DoMatchInTile(SBoardCoords coords)
{
	std::set<SBoardCoords> matches = mState.GetNeighboursSameAsTile(coords);
	for (SBoardCoords tileCoords : matches)
	{
		CCandy* candy = mState.GetCandy(tileCoords);
		candy->SetType(TileType::EMPTY);
		mAnimationQueue.AddAnimation(std::make_unique<CDestroyAnimation>(.2f, candy));
	}
}

bool CMatcher::IsMatchInTile(SBoardCoords coords)
{
	if (mState.CountRowNeighboursSameAsTile(coords) >= 3)
	{
		printf("row match of %s in (%d, %d)!\n", mState.GetTileName(mState.GetTile(coords)).c_str(), coords.row, coords.col);
		return true;
	}
	if (mState.CountColNeighboursSameAsTile(coords) >= 3)
	{
		printf("col match of %s in (%d, %d)!\n", mState.GetTileName(mState.GetTile(coords)).c_str(), coords.row, coords.col);
		return true;
	}
	return false;
}
