#include <CBoard.h>

#include <Utils.h>
#include <set>

#include <vector>
#include <CCandy.h>

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
	: mBoardState(BOARD_SIZE, TILE_SIZE, std::make_unique<const CCandyGenerator>())
	, mMatcher(mBoardState)
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
	TileType oldTile_1 = mBoardState.GetTile(tileCoords_1);
	TileType oldTile_2 = mBoardState.GetTile(tileCoords_2);
	mBoardState.SetTile(tileCoords_1, oldTile_2);
	mBoardState.SetTile(tileCoords_2, oldTile_1);
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
		mBoardState.SetTile(tileCoords_1, oldTile_1);
		mBoardState.SetTile(tileCoords_2, oldTile_2);
	}
}

void CBoard::Update(float delta_time)
{
	DoPendingMatches();
	// if there's a pending swap, do it
	if (mSwappedTileCoords_1.row >= 0 && mSwappedTileCoords_2.row >= 0)
	{
		DoSwap(mSwappedTileCoords_1, mSwappedTileCoords_2);
		mSwappedTileCoords_1 = mSwappedTileCoords_2 = {-1, -1};
	}
	mBoardState.Refill();
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

void CBoard::Render(SDL_Renderer* renderer)
{
	SDL_Rect rect = {0, 0, 0, 0};

	for (int row = 0; row < BOARD_SIZE; row++)
	{
		int x = ORIGIN_X;
		int y = ORIGIN_Y + row * TILE_SIZE;
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			TileType tile = mBoardState.GetTile({row, col});
			rect.x = x;
			rect.y = y;
			SDL_QueryTexture(mTextures.find(tile)->second, nullptr, nullptr, &rect.w, &rect.h);
			SDL_RenderCopy(renderer, mTextures.find(tile)->second, nullptr, &rect);
			x += TILE_SIZE;
		}
	}
}
