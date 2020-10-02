#include <CBoardState.h>

namespace CBoardStateInternal {
	std::vector<int> SampleBoard = {
		1, 2, 3, 4, 5, 1, 2, 3,
		4, 5, 1, 2, 3, 4, 5, 1,
		2, 3, 1, 5, 1, 2, 3, 4,
		5, 1, 2, 3, 4, 5, 1, 2,
		3, 4, 5, 1, 2, 3, 4, 5,
		1, 2, 3, 4, 5, 1, 1, 3,
		4, 5, 1, 2, 3, 4, 5, 1,
		2, 3, 4, 5, 1, 2, 3, 4
	};
}

CBoardState::CBoardState(int size)
	: BOARD_SIZE(size)
{
	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
	{
		mBoardState.push_back(TileType::EMPTY);
	}
}

void CBoardState::SetupBoard()
{
	int index = 0;
	for (int value : CBoardStateInternal::SampleBoard)
	{
		mBoardState[index] = static_cast<TileType>(value);
		index++;
	}
}

void CBoardState::SetTile(int pos, TileType type)
{
	mBoardState[pos] = type;
}

std::vector<int> CBoardState::GetRowNeighboursSameAsTile(int pos) const
{
	std::vector<int> result;
	TileType tile = mBoardState[pos];
	int col = pos % BOARD_SIZE;
	// LEFT
	if (col > 0)
	{
		for (int index = pos - 1; index > pos - col; index--)
		{
			TileType neighbour = mBoardState[index];
			if (neighbour == tile)
			{
				result.push_back(index);
			}
			else
			{
				break;
			}
		}
	}
	// RIGHT
	if (col < BOARD_SIZE - 1)
	{
		for (int index = pos + 1; index < pos + BOARD_SIZE - col; index++)
		{
			TileType neighbour = mBoardState[index];
			if (neighbour == tile)
			{
				result.push_back(index);
			}
			else
			{
				break;
			}
		}
	}
	return result;
}

std::vector<int> CBoardState::GetColNeighboursSameAsTile(int pos) const
{
	std::vector<int> result;
	TileType tile = mBoardState[pos];
	int row = pos / BOARD_SIZE;
	int col = pos % BOARD_SIZE;
	// TOP
	if (row > 0)
	{
		for (int index = pos - BOARD_SIZE; index > pos - row * BOARD_SIZE; index -= BOARD_SIZE)
		{
			TileType neighbour = mBoardState[index];
			if (neighbour == tile)
			{
				result.push_back(index);
			}
			else
			{
				break;
			}
		}
	}
	// DOWN
	if (row < BOARD_SIZE - 1)
	{
		for (int index = pos + BOARD_SIZE; index < pos + (BOARD_SIZE - row) * BOARD_SIZE; index += BOARD_SIZE)
		{
			TileType neighbour = mBoardState[index];
			if (neighbour == tile)
			{
				result.push_back(index);
			}
			else
			{
				break;
			}
		}
	}
	return result;
}
