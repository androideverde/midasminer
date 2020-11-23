#include <CBoard.h>

#include <Utils.h>
#include <set>
#include <vector>
#include <CMoveAnimation.h>
#include <CParallelAnimation.h>

namespace CBoardInternal {
	std::map<CandyType, const std::string> TextureFiles =
	{
		{CandyType::RED, "../assets/Red.png"},
		{CandyType::GREEN, "../assets/Green.png"},
		{CandyType::BLUE, "../assets/Blue.png"},
		{CandyType::YELLOW, "../assets/Yellow.png"},
		{CandyType::PURPLE, "../assets/Purple.png"},
		{CandyType::EMPTY, "../assets/Empty.png"}
	};
}

void CBoard::LoadResources(SDL_Renderer* renderer)
{
	assert(mTextures.empty());
	for (const auto& it : CBoardInternal::TextureFiles)
	{
		mTextures.insert({it.first, Utils::LoadImage(it.second, renderer)});
	}
}

CBoard::CBoard()
	: mBoardState(BOARD_SIZE, TILE_SIZE, ORIGIN_X, ORIGIN_Y, std::make_unique<const CCandyGenerator>())
	, mAnimationQueue()
	, mMatcher(mBoardState, mAnimationQueue)
	, mSwappedTileCoords_1()
	, mSwappedTileCoords_2()
{
}

CBoard::~CBoard()
{
	for (const auto& it : mTextures)
	{
		SDL_DestroyTexture(it.second);
	}
}

void CBoard::OnClick(SBoardCoords coords)
{
	printf("Clicked tile: (%d,%d)\n", coords.row, coords.col);
}

void CBoard::OnDrag(SBoardCoords startCoords, SBoardCoords endCoords)
{
	mSwappedTileCoords_1 = startCoords;
	mSwappedTileCoords_2 = endCoords;
	printf("Drag completed: from (%d,%d) to (%d,%d)\n", startCoords.row, startCoords.col, endCoords.row, endCoords.col);
}

bool CBoard::DoSwap(SBoardCoords tileCoords_1, SBoardCoords tileCoords_2)
{
	// do swap
	TriggerSwapInAnimations(tileCoords_1, tileCoords_2);
	mBoardState.Swap(tileCoords_1, tileCoords_2);
	if (!(mMatcher.IsMatchInTile(tileCoords_1) || mMatcher.IsMatchInTile(tileCoords_2)))
	{
		// if not match, undo swap
		TriggerSwapOutAnimations(tileCoords_1, tileCoords_2);
		mBoardState.Swap(tileCoords_2, tileCoords_1);
	}
}

void CBoard::TriggerSwapInAnimations(SBoardCoords tile_1, SBoardCoords tile_2)
{
	CCandy* candy_1 = mBoardState.GetTile(tile_1).GetCandy();
	CCandy* candy_2 = mBoardState.GetTile(tile_2).GetCandy();
	std::vector<std::unique_ptr<CAnimation>> parallelAnims;
	parallelAnims.emplace_back(std::make_unique<CMoveAnimation>(candy_1->GetPos(), candy_2->GetPos(), .4f, candy_1));
	parallelAnims.emplace_back(std::make_unique<CMoveAnimation>(candy_2->GetPos(), candy_1->GetPos(), .4f, candy_2));
	mAnimationQueue.AddAnimation(std::make_unique<CParallelAnimation>(.4f, std::move(parallelAnims)));
}

void CBoard::TriggerSwapOutAnimations(SBoardCoords tile_1, SBoardCoords tile_2)
{
	CCandy* candy_1 = mBoardState.GetTile(tile_2).GetCandy();
	CCandy* candy_2 = mBoardState.GetTile(tile_1).GetCandy();
	std::vector<std::unique_ptr<CAnimation>> parallelAnims;
	parallelAnims.emplace_back(std::make_unique<CMoveAnimation>(candy_1->GetPos(), candy_2->GetPos(), .4f, candy_2));
	parallelAnims.emplace_back(std::make_unique<CMoveAnimation>(candy_2->GetPos(), candy_1->GetPos(), .4f, candy_1));
	mAnimationQueue.AddAnimation(std::make_unique<CParallelAnimation>(.4f, std::move(parallelAnims)));
}

void CBoard::TriggerFallAnimation(SBoardCoords origin, SBoardCoords destination)
{
	CCandy* candy = mBoardState.GetTile(origin).GetCandy();
	mAnimationQueue.AddAnimation(std::make_unique<CMoveAnimation>(candy->GetPos(), GetBoardTilePos(destination), .3f, candy));
}

void CBoard::Update(float delta_time)
{
	// switch
	//   case animationOngoing:
	//        animation.update()
	//   case matchOnBoard:
	//        doMatch()
	//   case refillPending:
	//        doRefill()
	//   case swapPending:
	//        doSwap()
	mAnimationQueue.Update(delta_time);
	if (mAnimationQueue.AllAnimationsCompleted()) {
		if (IsRefillPending())
		{
			RefillBoard();
		}
		else if (IsMatchPending())
		{
			mMatcher.DoMatch();
		}
		else if (IsSwapPending())
		{
			DoSwap(mSwappedTileCoords_1, mSwappedTileCoords_2);
			mSwappedTileCoords_1 = mSwappedTileCoords_2 = {-100, -100};
		}
	}
}

bool CBoard::IsMatchPending() const
{
	SBoardCoords coords;
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		coords.row = row;
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			coords.col = col;
			if (mMatcher.IsMatchInTile(coords))
			{
				printf("Match found in (%d, %d)!\n", coords.row, coords.col);
				return true;
			}
		}
	}
	return false;
}

bool CBoard::IsRefillPending() const
{
	SBoardCoords coords;
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		coords.row = row;
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			coords.col = col;
			if (mBoardState.GetTile(coords).GetCandy() == nullptr)
			{
				printf("We need to refill! Empty found in (%d, %d)!\n", coords.row, coords.col);
				return true;
			}
		}
	}
	return false;
}

bool CBoard::IsSwapPending() const
{
	return mSwappedTileCoords_1.row >= 0 && mSwappedTileCoords_2.row >= 0;
}

void CBoard::RefillBoard()
{
	std::vector<int> emptiesInCol;
	emptiesInCol.clear();
	SBoardCoords coords;
	
	// find empty tiles in board
	for (int col = 0; col < BOARD_SIZE; col++)
	{
		coords.col = col;
		emptiesInCol.push_back(0);
		for (int rowIndex = 0; rowIndex < BOARD_SIZE; rowIndex++)
		{
			coords.row = BOARD_SIZE - 1 - rowIndex;
			if (mBoardState.GetTile(coords).GetCandy() == nullptr)
			{
				printf("Empty found in (%d, %d)\n", coords.row, coords.col);
				emptiesInCol[col]++;
				SBoardCoords above;
				// find first non-empty tile going up in column
				for (int i = 1; i < coords.row + 1; i++)
				{
					if (mBoardState.GetTile({coords.row - i, col}).GetCandy() != nullptr)
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
					mBoardState.Swap(coords, above);
					emptiesInCol[col]--;
				}
			}
		}
	}
	// create new candies to fill empty tiles
	for (int col = 0; col < BOARD_SIZE; col++)
	{
		int empties = emptiesInCol[col];
		for (int i = 1; i <= empties; i++)
		{
			SBoardCoords coords = {0, col};
			SBoardCoords destination = {empties - i, col};
			mBoardState.AddNewCandy(coords);
			TriggerFallAnimation(coords, destination);
			mBoardState.Swap(coords, destination);
		}
	}
}

std::set<SBoardCoords> CBoard::DoMatch()
{
	SBoardCoords coords;
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		coords.row = row;
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			coords.col = col;
			if (mBoardState.GetTile(coords).GetCandy() == nullptr)
			{
				continue;
			}
			if (mMatcher.IsMatchInTile(coords))
			{
				return mMatcher.DoMatchInTile(coords);
			}
		}
	}
}

SBoardCoords CBoard::GetBoardTileCoords(int x, int y) const
{
	if (!(x < ORIGIN_X || x > ORIGIN_X + BOARD_SIZE * TILE_SIZE ||
		  y < ORIGIN_Y || y > ORIGIN_Y + BOARD_SIZE * TILE_SIZE))
	{
		SBoardCoords tileCoords;
		tileCoords.row = (y - ORIGIN_Y) / TILE_SIZE;
		tileCoords.col = (x - ORIGIN_X) / TILE_SIZE;
		return tileCoords;
	}
	return {-100, -100};
}

SDL_Point CBoard::GetBoardTilePos(SBoardCoords coords) const
{
	SDL_Point pos;
	pos.x = ORIGIN_X + coords.col * TILE_SIZE;
	pos.y = ORIGIN_Y + coords.row * TILE_SIZE;
	return pos;
}

void CBoard::Render(SDL_Renderer* renderer) const
{
	SDL_Rect rect = {0, 0, 0, 0};

	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			CCandy* candy = mBoardState.GetTile({row, col}).GetCandy();
			if (candy != nullptr)
			{
				rect.x = candy->GetX();
				rect.y = candy->GetY();
				CandyType tile = candy->GetType();
				SDL_QueryTexture(mTextures.find(tile)->second, nullptr, nullptr, &rect.w, &rect.h);
				SDL_RenderCopy(renderer, mTextures.find(tile)->second, nullptr, &rect);
			}
		}
	}
}
