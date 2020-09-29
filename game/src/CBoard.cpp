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
	auto it = CBoardInternal::TextureFiles.begin();
	while (it != CBoardInternal::TextureFiles.end())
	{
		mTextures.insert({it->first, Utils::LoadImage(it->second, renderer)});
		it++;
	}
}

CBoard::CBoard()
	: mBoardState()
{
	mBoardState.SetupBoard();
}

CBoard::~CBoard()
{
	auto it = mTextures.begin();
	while (it != mTextures.end())
	{
		SDL_DestroyTexture(it->second);
		it++;
	}
}

void CBoard::Update(float delta_time, bool clicked, int mouseX, int mouseY)
{
	if (clicked)
	{
		int pos = GetBoardPos(mouseX, mouseY);
		printf("Cell clicked: %d\n", pos);
	}
}

const int CBoard::GetBoardPos(int x, int y) const
{
	int row = (y - ORIGIN_Y) / CELL_SIZE;
	int col = (x - ORIGIN_X) / CELL_SIZE;
	return col + row * 8;
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
