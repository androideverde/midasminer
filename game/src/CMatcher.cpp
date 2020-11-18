#include <CMatcher.h>

#include <CDestroyAnimation.h>
#include <CParallelAnimation.h>
#include <CTile.h>

CMatcher::CMatcher(CBoardState& state, CAnimationSystem& animationQueue)
	: mState(state)
	, mAnimationQueue(animationQueue)
{
}

void CMatcher::DoMatchInTile(SBoardCoords coords)
{
	std::vector<std::unique_ptr<CAnimation>> parallelAnims;
	std::set<SBoardCoords> matches = mState.GetNeighboursSameAsTile(coords);
	for (SBoardCoords tileCoords : matches)
	{
		CTile& tile = mState.GetTile(tileCoords);
		CCandy* candy = mState.GetCandy(tileCoords);
		printf("destroyed candy: (%d, %d) %s\n", tileCoords.row, tileCoords.col, mState.GetTileName(candy->GetType()).c_str());
		tile.SetCandy(nullptr);
//		candy->SetType(CandyType::EMPTY);
		parallelAnims.emplace_back(std::make_unique<CDestroyAnimation>(.2f, candy));
	}
	mAnimationQueue.AddAnimation(std::make_unique<CParallelAnimation>(.2f, std::move(parallelAnims)));

}

bool CMatcher::IsMatchInTile(SBoardCoords coords)
{
	if (mState.CountRowNeighboursSameAsTile(coords) >= 3)
	{
		printf("row match of %s in (%d, %d)!\n", mState.GetTileName(mState.GetCandyType(coords)).c_str(), coords.row, coords.col);
		return true;
	}
	if (mState.CountColNeighboursSameAsTile(coords) >= 3)
	{
		printf("col match of %s in (%d, %d)!\n", mState.GetTileName(mState.GetCandyType(coords)).c_str(), coords.row, coords.col);
		return true;
	}
	return false;
}
