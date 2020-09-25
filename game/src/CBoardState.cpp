#include <CBoardState.h>

#include <Utils.h>

namespace CBoardStateInternal {
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

CBoardState::CBoardState()
{
	for (int i = 0; i < 64; i++)
	{
		mBoardState.push_back(TileType::EMPTY);
	}
}

void CBoardState::LoadResources(SDL_Renderer* renderer)
{
	auto it = CBoardStateInternal::TextureFiles.begin();
	while (it != CBoardStateInternal::TextureFiles.end())
	{
		mTextures.insert({it->first, Utils::LoadImage(it->second, renderer)});
		it++;
	}
}

CBoardState::~CBoardState()
{
	auto it = mTextures.begin();
	while (it != mTextures.end())
	{
		SDL_DestroyTexture(it->second);
		it++;
	}
}

void CBoardState::SetupBoard()
{
	for (int row = 1; row < 9; row++)
	{
		int offset = (row - 1) * 8;
		if (row % 2 == 0)
		{
			for (int i = 0; i < 8; i++)
			{
				if (i % 2 == 0)
				{
					mBoardState[i + offset] = TileType::RED;
				}
				else
				{
					mBoardState[i + offset] = TileType::GREEN;
				}
			}
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				if (i % 2 == 0)
				{
					mBoardState[i + offset] = TileType::YELLOW;
				}
				else
				{
					mBoardState[i + offset] = TileType::BLUE;
				}
			}
		}
	}
}

void CBoardState::Render(SDL_Renderer* renderer)
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
			TileType tile = mBoardState[i + offset];
			rect.x = x;
			rect.y = y;
			SDL_QueryTexture(mTextures.find(tile)->second, nullptr, nullptr, &rect.w, &rect.h);
			SDL_RenderCopy(renderer, mTextures.find(tile)->second, nullptr, &rect);
			x += cellSize;
		}
	}

}
