#include <CBoard.h>

#include <Utils.h>

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
	: mBoardState(BOARD_SIZE)
	, mSwappedTile_1(-1)
	, mSwappedTile_2(-1)
{
	mBoardState.SetupBoard();
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
	printf("Clicked tile: %d, %d\n", coords.row, coords.col);
}

void CBoard::OnDrag(SBoardCoords startCoords, SBoardCoords endCoords)
{
	int start = mBoardState.GetIndexFromCoords(startCoords);
	int end = mBoardState.GetIndexFromCoords(endCoords);
	printf("Drag completed: from %d to %d\n", start, end);
	mSwappedTile_1 = start;
	mSwappedTile_2 = end;
}

bool CBoard::DoSwap(int tile_1, int tile_2)
{
	// do swap
	TileType oldTile_1 = mBoardState.GetTile(tile_1);
	TileType oldTile_2 = mBoardState.GetTile(tile_2);
	mBoardState.SetTile(tile_1, oldTile_2);
	mBoardState.SetTile(tile_2, oldTile_1);
	// check for matches at pos cell1
	if (CheckForMatchesAtPos(tile_1) || CheckForMatchesAtPos(tile_2))
	{
		printf("match found!\n");
	}
	else
	{
		// if not match, undo swap
		mBoardState.SetTile(tile_1, oldTile_1);
		mBoardState.SetTile(tile_2, oldTile_2);
	}
}

bool CBoard::CheckForMatchesAtPos(int pos) const
{
	std::vector<int> matchInRow = mBoardState.GetRowNeighboursSameAsTile(pos);
	if (matchInRow.size() >= 3)
	{
		printf("found match in row!\n");
		return true;
	}
	std::vector<int> matchInCol = mBoardState.GetColNeighboursSameAsTile(pos);
	if (matchInCol.size() >= 3)
	{
		printf("found match in col!\n");
		return true;
	}
	return false;
}

void CBoard::Update(float delta_time)
{
	// if there's a pending swap, do it
	if (mSwappedTile_1 >= 0 && mSwappedTile_2 >= 0)
	{
		DoSwap(mSwappedTile_1, mSwappedTile_2);
		mSwappedTile_1 = mSwappedTile_2 = -1;
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
		int offset = row * BOARD_SIZE;
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			TileType tile = mBoardState.GetTile(i + offset);
			rect.x = x;
			rect.y = y;
			SDL_QueryTexture(mTextures.find(tile)->second, nullptr, nullptr, &rect.w, &rect.h);
			SDL_RenderCopy(renderer, mTextures.find(tile)->second, nullptr, &rect);
			x += TILE_SIZE;
		}
	}
}
