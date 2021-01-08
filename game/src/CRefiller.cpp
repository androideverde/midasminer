#include <CRefiller.h>

#include <CMoveAnimation.h>
#include <CParallelAnimation.h>
#include <CTile.h>

CRefiller::CRefiller(CBoardState& state, CAnimationSystem& animationQueue)
	: mState(state)
	, mAnimationQueue(animationQueue)
{
}

bool CRefiller::IsRefillPending() const
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
				printf("We need to refill! Empty found in (%d, %d)!\n", coords.row, coords.col);
				return true;
			}
		}
	}
	return false;
}

void CRefiller::RefillBoard()
{
	std::vector<int> emptiesInCol;
	emptiesInCol.clear();
	SBoardCoords coords;
	
	// find empty tiles in board
	for (int col = 0; col < mState.GetBoardSize(); col++)
	{
		coords.col = col;
		emptiesInCol.push_back(0);
		for (int rowIndex = 0; rowIndex < mState.GetBoardSize(); rowIndex++)
		{
			coords.row = mState.GetBoardSize() - 1 - rowIndex;
			if (mState.GetTile(coords).GetCandy() == nullptr)
			{
				printf("Empty found in (%d, %d)\n", coords.row, coords.col);
				emptiesInCol[col]++;
				SBoardCoords above;
				// find first non-empty tile going up in column
				for (int i = 1; i < coords.row + 1; i++)
				{
					if (mState.GetTile({coords.row - i, col}).GetCandy() != nullptr)
					{
						above = {coords.row - i, col};
						printf("Will fill (%d, %d) with (%d, %d)\n", coords.row, coords.col, above.row, above.col);
						break;
					}
				}
				// above is the first non-empty tile or (-100,-100) if there's nothing above
				if (above.row != -100 && above.col != -100)
				{
					TriggerFallAnimation(above, coords);
					mState.Swap(above, coords);
					emptiesInCol[col]--;
				}
			}
		}
	}
	// create new candies to fill empty tiles
	for (int col = 0; col < mState.GetBoardSize(); col++)
	{
		int empties = emptiesInCol[col];
		for (int i = 1; i <= empties; i++)
		{
			SBoardCoords coords = {0, col};
			SBoardCoords destination = {empties - i, col};
			mState.AddNewCandy(coords);
			TriggerFallAnimation(coords, destination);
			mState.Swap(coords, destination);
		}
	}
}

void CRefiller::TriggerFallAnimation(SBoardCoords origin, SBoardCoords destination)
{
	CCandy* candy = mState.GetTile(origin).GetCandy();
	mAnimationQueue.AddAnimation(std::make_unique<CMoveAnimation>(candy->GetPos(), mState.GetTilePos(destination), 700.f, candy));
}
