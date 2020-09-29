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

void CBoard::Render(SDL_Renderer* renderer)
{
	SDL_Rect rect = {0, 0, 0, 0};
	int cellSize = 43;

	for (int row = 0; row < 8; row++)
	{
		int x = 330;
		int y = 100 + row * cellSize;
		int offset = row * 8;
		for (int i = 0; i < 8; i++)
		{
			TileType tile = mBoardState.GetTile(i + offset);
			rect.x = x;
			rect.y = y;
			SDL_QueryTexture(mTextures.find(tile)->second, nullptr, nullptr, &rect.w, &rect.h);
			SDL_RenderCopy(renderer, mTextures.find(tile)->second, nullptr, &rect);
			x += cellSize;
		}
	}
}
