#include <CBoardState.h>

namespace CBoardStateInternal {
	std::vector<int> SampleBoard = {
		1, 2, 3, 4, 5, 1, 2, 3,
		4, 5, 1, 2, 3, 4, 5, 1,
		2, 3, 4, 5, 1, 2, 3, 4,
		5, 1, 2, 3, 4, 5, 1, 2,
		3, 4, 5, 1, 2, 3, 4, 5,
		1, 2, 3, 4, 5, 1, 2, 3,
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
