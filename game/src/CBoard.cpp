#include <CBoard.h>

#include <Utils.h>
#include <set>
#include <vector>
#include <CCandy.h>
#include <CMoveAnimation.h>

namespace CBoardInternal {
	std::map<TileType, const std::string> TextureFiles =
	{
		{TileType::RED, "../assets/Red.png"},
		{TileType::GREEN, "../assets/Green.png"},
		{TileType::BLUE, "../assets/Blue.png"},
		{TileType::YELLOW, "../assets/Yellow.png"},
		{TileType::PURPLE, "../assets/Purple.png"},
		{TileType::EMPTY, "../assets/Empty.png"}
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
	TriggerSwapAnimations(tileCoords_1, tileCoords_2);
	mBoardState.Swap(tileCoords_1, tileCoords_2);
	// check for matches at each tile
	if (mMatcher.IsMatchInTile(tileCoords_1))
	{
		mMatcher.DoMatchInTile(tileCoords_1);
	}
	else if (mMatcher.IsMatchInTile(tileCoords_2))
	{
		mMatcher.DoMatchInTile(tileCoords_2);
	}
	else
	{
		// if not match, undo swap
		TriggerSwapAnimations(tileCoords_1, tileCoords_2);
		mBoardState.Swap(tileCoords_2, tileCoords_1);
	}
}

void CBoard::TriggerSwapAnimations(SBoardCoords tile_1, SBoardCoords tile_2)
{
	SDL_Point point_1 = GetBoardTilePos(tile_1);
	SDL_Point point_2 = GetBoardTilePos(tile_2);
	mAnimationQueue.AddAnimation(std::make_unique<CMoveAnimation>(point_1, point_2, .4f, mBoardState.GetCandy(tile_1)));
	mAnimationQueue.AddAnimation(std::make_unique<CMoveAnimation>(point_2, point_1, .4f, mBoardState.GetCandy(tile_2)));
}

void CBoard::Update(float delta_time)
{
	if (mAnimationQueue.AllAnimationsCompleted()) {
		DoPendingMatches();
		// if there's a pending swap, do it
		if (mSwappedTileCoords_1.row >= 0 && mSwappedTileCoords_2.row >= 0)
		{
			DoSwap(mSwappedTileCoords_1, mSwappedTileCoords_2);
			mSwappedTileCoords_1 = mSwappedTileCoords_2 = {-1, -1};
		}
		RefillBoard();
	}
	else
	{
		PlayAllPendingAnimations(delta_time);
	}
}

void CBoard::RefillBoard()
{
	std::vector<CCandy*> fallingCandies = mBoardState.Refill();
	for (CCandy* candy : fallingCandies)
	{
		SDL_Point end = candy->GetPos();
		SDL_Point start = {end.x, end.y - TILE_SIZE};
		mAnimationQueue.AddAnimation(std::make_unique<CMoveAnimation>(start, end, .5f, candy));
	}
}

void CBoard::PlayAllPendingAnimations(float delta_time)
{
	CAnimation& anim = mAnimationQueue.GetNextAnimation();
	anim.Update(delta_time);
	if (anim.IsCompleted())
	{
		mAnimationQueue.CurrentAnimationComplete();
	}
}

void CBoard::DoPendingMatches()
{
	SBoardCoords coords;
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		coords.row = row;
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			coords.col = col;
			if (mBoardState.GetTile(coords) == TileType::EMPTY)
			{
				continue;
			}
			if (mMatcher.IsMatchInTile(coords))
			{
				mMatcher.DoMatchInTile(coords);
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
	return {-1, -1};
}

SDL_Point CBoard::GetBoardTilePos(SBoardCoords coords) const
{
	SDL_Point point;
	point.x = ORIGIN_X + coords.col * TILE_SIZE;
	point.y = ORIGIN_Y + coords.row * TILE_SIZE;
	return point;
}

void CBoard::Render(SDL_Renderer* renderer)
{
	SDL_Rect rect = {0, 0, 0, 0};

	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			CCandy* candy = mBoardState.GetCandy({row, col});
			rect.x = candy->GetX();
			rect.y = candy->GetY();
			TileType tile = candy->GetType();
			SDL_QueryTexture(mTextures.find(tile)->second, nullptr, nullptr, &rect.w, &rect.h);
			SDL_RenderCopy(renderer, mTextures.find(tile)->second, nullptr, &rect);
		}
	}
}
