#include <CMatcher.h>

#include <CDestroyAnimation.h>
#include <CParallelAnimation.h>
#include <CTile.h>

CMatcher::CMatcher(CBoardState& state, CAnimationSystem& animationQueue, CScoringSystem& scorer)
	: mState(state)
	, mAnimationQueue(animationQueue)
	, mScorer(scorer)
{
}

std::set<SBoardCoords> CMatcher::DoMatchInTile(SBoardCoords coords)
{
	std::vector<std::unique_ptr<CAnimation>> parallelAnims;
	std::set<SBoardCoords> matches = GetMatchGroup(coords);
	for (SBoardCoords tileCoords : matches)
	{
		CTile& tile = mState.GetTile(tileCoords);
		CCandy* candy = tile.GetCandy();
		printf("destroyed candy: (%d, %d) %s\n", tileCoords.row, tileCoords.col, candy->GetName().c_str());
		tile.SetCandy(nullptr);
		parallelAnims.emplace_back(std::make_unique<CDestroyAnimation>(10.f, candy));
	}
	mAnimationQueue.AddAnimation(std::make_unique<CParallelAnimation>(std::move(parallelAnims)));
	mScorer.AddToScore(matches.size());
	return matches;
}

std::set<SBoardCoords> CMatcher::GetMatchGroup(SBoardCoords coords)
{
	std::set<SBoardCoords> result;
	std::set<SBoardCoords> initial = mState.GetMatchedNeighboursSameAsTile(coords);
	result.insert(initial.begin(), initial.end());
	std::set<SBoardCoords> recursive;
	recursive = GetMatchesRecursively(initial);
	result.insert(recursive.begin(), recursive.end());
	return result;
}

std::set<SBoardCoords> CMatcher::GetMatchesRecursively(std::set<SBoardCoords> initial)
{
	std::set<SBoardCoords> result;
	bool goDeeper = false;
	for (SBoardCoords coord: initial)
	{
		std::set<SBoardCoords> partial = mState.GetMatchedNeighboursSameAsTile(coord);
		if (partial.size() >= initial.size() + 2)
		{
			result.insert(partial.begin(), partial.end());
			goDeeper = true;
		}
	}
	if (goDeeper)
	{
		initial = result;
		result = GetMatchesRecursively(initial);
	}
	return result;
}

bool CMatcher::IsMatchInTile(SBoardCoords coords) const
{
	if (mState.CountRowNeighboursSameAsTile(coords) >= 3)
	{
		printf("row match of %s in (%d, %d)!\n", mState.GetTile(coords).GetCandy()->GetName().c_str(), coords.row, coords.col);
		return true;
	}
	if (mState.CountColNeighboursSameAsTile(coords) >= 3)
	{
		printf("col match of %s in (%d, %d)!\n", mState.GetTile(coords).GetCandy()->GetName().c_str(), coords.row, coords.col);
		return true;
	}
	return false;
}

std::set<SBoardCoords> CMatcher::DoMatch()
{
	SBoardCoords coords;
	for (int row = 0; row < mState.GetBoardSize(); row++)
	{
		coords.row = row;
		for (int col = 0; col < mState.GetBoardSize(); col++)
		{
			coords.col = col;
			if (mState.GetTile(coords).GetCandy() == nullptr)
			{
				continue;
			}
			if (IsMatchInTile(coords))
			{
				return DoMatchInTile(coords);
			}
		}
	}
}

bool CMatcher::IsMatchPending() const
{
	SBoardCoords coords;
	for (int row = 0; row < mState.GetBoardSize(); row++)
	{
		coords.row = row;
		for (int col = 0; col < mState.GetBoardSize(); col++)
		{
			coords.col = col;
			if (IsMatchInTile(coords))
			{
				printf("Match found in (%d, %d)!\n", coords.row, coords.col);
				return true;
			}
		}
	}
	return false;
}
