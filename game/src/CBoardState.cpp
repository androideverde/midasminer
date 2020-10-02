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

CBoardState::CBoardState()
{
	for (int i = 0; i < 64; i++)
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
	int col = pos % 8;
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
	if (col < 7)
	{
		for (int index = pos + 1; index < pos + 8 - col; index++)
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
	int row = pos / 8;
	int col = pos % 8;
	// TOP
	if (row > 0)
	{
		for (int index = pos - 8; index > pos - row * 8; index -= 8)
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
	if (row < 7)
	{
		for (int index = pos + 8; index < pos + (8 - row) * 8; index += 8)
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
