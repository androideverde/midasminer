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
	: mBoardState()
	, mMatchCell1(-1)
	, mMatchCell2(-1)
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

void CBoard::OnClick(int pos)
{
	printf("Clicked cell: %d\n", pos);
}

void CBoard::OnDrag(int start, int end)
{
	printf("Drag completed: from %d to %d\n", start, end);
	mMatchCell1 = start;
	mMatchCell2 = end;
}

bool CBoard::DoSwap(int cell1, int cell2)
{
	// do swap
	TileType oldCell1 = mBoardState.GetTile(cell1);
	TileType oldCell2 = mBoardState.GetTile(cell2);
	mBoardState.SetTile(cell1, oldCell2);
	mBoardState.SetTile(cell2, oldCell1);
	// check for matches at pos cell1
	if (CheckForMatchesAtPos(cell1) || CheckForMatchesAtPos(cell2))
	{
		printf("match found!\n");
	}
	else
	{
		// if not match, undo swap
		mBoardState.SetTile(cell1, oldCell1);
		mBoardState.SetTile(cell2, oldCell2);
	}
}

bool CBoard::CheckForMatchesAtPos(int pos) const
{
	std::vector<int> matchInRow = mBoardState.GetRowNeighboursSameAsTile(pos);
	if (matchInRow.size() >= 2)
	{
		printf("found match in row!\n");
		return true;
	}
	std::vector<int> matchInCol = mBoardState.GetColNeighboursSameAsTile(pos);
	if (matchInCol.size() >= 2)
	{
		printf("found match in col!\n");
		return true;
	}
	return false;
}

void CBoard::Update(float delta_time)
{
	// if there's a pending swap, do it
	if (mMatchCell1 >= 0 && mMatchCell2 >= 0)
	{
		DoSwap(mMatchCell1, mMatchCell2);
		mMatchCell1 = mMatchCell2 = -1;
	}
}

int CBoard::GetBoardPos(int x, int y) const
{
	if (!(x < ORIGIN_X || x > ORIGIN_X + 8 * CELL_SIZE ||
		  y < ORIGIN_Y || y > ORIGIN_Y + 8 * CELL_SIZE))
	{
		int row = (y - ORIGIN_Y) / CELL_SIZE;
		int col = (x - ORIGIN_X) / CELL_SIZE;
		return col + row * 8;
	}
	return -1;
}

void CBoard::Render(SDL_Renderer* renderer)
{
	SDL_Rect rect = {0, 0, 0, 0};

	for (int row = 0; row < 8; row++)
	{
		int x = ORIGIN_X;
		int y = ORIGIN_Y + row * CELL_SIZE;
		int offset = row * 8;
		for (int i = 0; i < 8; i++)
		{
			TileType tile = mBoardState.GetTile(i + offset);
			rect.x = x;
			rect.y = y;
			SDL_QueryTexture(mTextures.find(tile)->second, nullptr, nullptr, &rect.w, &rect.h);
			SDL_RenderCopy(renderer, mTextures.find(tile)->second, nullptr, &rect);
			x += CELL_SIZE;
		}
	}
}
