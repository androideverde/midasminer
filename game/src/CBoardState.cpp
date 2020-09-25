#include <CBoardState.h>

CBoardState::CBoardState()
{
	for (int i = 0; i < 64; i++)
	{
		mBoardState.push_back(TileType::EMPTY);
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
					mBoardState[i + offset] = TileType::PURPLE;
				}
			}
		}
	}
}
