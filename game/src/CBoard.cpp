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
	mFont = TTF_OpenFont("../assets/Impact.ttf", 28);
}

CBoard::CBoard()
	: mBoardState(BOARD_SIZE, TILE_SIZE, ORIGIN_X, ORIGIN_Y, std::make_unique<const CCandyGenerator>())
	, mFont(nullptr)
	, mAnimationQueue()
	, mMatcher(mBoardState, mAnimationQueue)
	, mSwapper(mBoardState, mAnimationQueue, mMatcher)
	, mRefiller(mBoardState, mAnimationQueue)
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
		if (mRefiller.IsRefillPending())
		{
			mRefiller.RefillBoard();
		}
		else if (mMatcher.IsMatchPending())
		{
			mMatcher.DoMatch();
		}
		else if (IsSwapPending())
		{
			mSwapper.DoSwap(mSwappedTileCoords_1, mSwappedTileCoords_2);
			mSwappedTileCoords_1 = mSwappedTileCoords_2 = {-100, -100};
		}
	}
}

bool CBoard::IsSwapPending() const
{
	return mSwappedTileCoords_1.row >= 0 && mSwappedTileCoords_2.row >= 0;
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

void CBoard::Render(SDL_Renderer* renderer) const
{
	RenderTime(renderer, 0);
	RenderScore(renderer, 0);
	
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

void CBoard::RenderTime(SDL_Renderer* renderer, int secondsLeft) const
{
	char secondsLeft_str[4];
	sprintf(secondsLeft_str, "0:%02d", secondsLeft);
	SDL_Rect textPosition = {80, 430, 75, 30};
	SDL_RenderCopy(renderer, Utils::LoadText(secondsLeft_str, mFont, renderer), nullptr, &textPosition);
}

void CBoard::RenderScore(SDL_Renderer* renderer, int score) const
{
	char score_str[5];
	sprintf(score_str, "%05d", score);
	SDL_Rect textPosition = {80, 468, 75, 30};
	SDL_RenderCopy(renderer, Utils::LoadText(score_str, mFont, renderer), nullptr, &textPosition);
}
