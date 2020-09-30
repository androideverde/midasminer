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
}

void CBoard::Update(float delta_time)
{
}

const int CBoard::GetBoardPos(int x, int y) const
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
