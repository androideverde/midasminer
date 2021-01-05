#include <CSwapper.h>

#include <CMoveAnimation.h>
#include <CParallelAnimation.h>
#include <CTile.h>

CSwapper::CSwapper(CBoardState& state, CAnimationSystem& animationQueue, CMatcher& matcher)
	: mState(state)
	, mAnimationQueue(animationQueue)
	, mMatcher(matcher)
{
}

bool CSwapper::DoSwap(SBoardCoords tileCoords_1, SBoardCoords tileCoords_2)
{
	// do swap
	TriggerSwapInAnimations(tileCoords_1, tileCoords_2);
	mState.Swap(tileCoords_1, tileCoords_2);
	if (!(mMatcher.IsMatchInTile(tileCoords_1) || mMatcher.IsMatchInTile(tileCoords_2)))
	{
		// if not match, undo swap
		TriggerSwapOutAnimations(tileCoords_1, tileCoords_2);
		mState.Swap(tileCoords_2, tileCoords_1);
	}
}

void CSwapper::TriggerSwapInAnimations(SBoardCoords tile_1, SBoardCoords tile_2)
{
	CCandy* candy_1 = mState.GetTile(tile_1).GetCandy();
	CCandy* candy_2 = mState.GetTile(tile_2).GetCandy();
	std::vector<std::unique_ptr<CAnimation>> parallelAnims;
	parallelAnims.emplace_back(std::make_unique<CMoveAnimation>(candy_1->GetPos(), candy_2->GetPos(), .2f, candy_1));
	parallelAnims.emplace_back(std::make_unique<CMoveAnimation>(candy_2->GetPos(), candy_1->GetPos(), .2f, candy_2));
	mAnimationQueue.AddAnimation(std::make_unique<CParallelAnimation>(.2f, std::move(parallelAnims)));
}

void CSwapper::TriggerSwapOutAnimations(SBoardCoords tile_1, SBoardCoords tile_2)
{
	CCandy* candy_1 = mState.GetTile(tile_2).GetCandy();
	CCandy* candy_2 = mState.GetTile(tile_1).GetCandy();
	std::vector<std::unique_ptr<CAnimation>> parallelAnims;
	parallelAnims.emplace_back(std::make_unique<CMoveAnimation>(candy_1->GetPos(), candy_2->GetPos(), .2f, candy_2));
	parallelAnims.emplace_back(std::make_unique<CMoveAnimation>(candy_2->GetPos(), candy_1->GetPos(), .2f, candy_1));
	mAnimationQueue.AddAnimation(std::make_unique<CParallelAnimation>(.2f, std::move(parallelAnims)));
}
