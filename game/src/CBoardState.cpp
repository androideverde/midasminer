#include <CBoardState.h>

namespace CBoardStateInternal {
	std::vector<int> SampleBoard = {
		1, 2, 3, 4, 5, 1, 2, 3,
		4, 5, 1, 2, 3, 4, 5, 1,
		2, 3, 1, 5, 1, 2, 3, 4,
		5, 1, 2, 3, 4, 5, 1, 2,
		3, 4, 1, 1, 2, 3, 4, 5,
		1, 2, 3, 4, 5, 1, 2, 3,
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

int CBoardState::GetIndexFromCoords(SBoardCoords coords) const
{
	return coords.col + BOARD_SIZE * coords.row;
}

SBoardCoords CBoardState::GetCoordsFromIndex(int index) const
{
	SBoardCoords coords;
	coords.col = index % BOARD_SIZE;
	coords.row = index / BOARD_SIZE;
	return coords;
}

std::vector<int> CBoardState::GetNeighbours(int pos, bool orientation, bool direction) const
{
	std::vector<int> result;
	TileType tile = mBoardState[pos];
	int row = GetCoordsFromIndex(pos).row;
	int col = GetCoordsFromIndex(pos).col;
	int loopOrientation;
	orientation ? loopOrientation = col : loopOrientation = row; // true = HORIZONTAL, false = VERTICAL
	int loopIndex;
	direction ? loopIndex = loopOrientation : loopIndex = BOARD_SIZE - loopOrientation - 1; // true = TO_LOW, false = TO_HIGH
	for (int i = 0; i < loopIndex; i++)
	{
		int nextStep;
		direction ? nextStep = loopOrientation - 1 - i : nextStep = loopOrientation + 1 + i;
		SBoardCoords neighbour;
		orientation ? neighbour = {row, nextStep} : neighbour = {nextStep, col};
		int index = GetIndexFromCoords(neighbour);
		TileType neighbourTile = mBoardState[index];
		if (neighbourTile == tile)
		{
			result.push_back(index);
		}
		else
		{
			break;
		}
	}
	return result;
}

std::vector<int> CBoardState::GetRowNeighboursSameAsTile(int pos) const
{
	std::vector<int> left = GetNeighbours(pos, true, true);
	std::vector<int> right = GetNeighbours(pos, true, false);
	std::vector<int> result;
	result.push_back(pos);
	std::merge(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(result));
	std::sort(result.begin(), result.end());
	return result;
}

std::vector<int> CBoardState::GetColNeighboursSameAsTile(int pos) const
{
	std::vector<int> top = GetNeighbours(pos, false, true);
	std::vector<int> bottom = GetNeighbours(pos, false, false);
	std::vector<int> result;
	result.push_back(pos);
	std::merge(top.begin(), top.end(), bottom.begin(), bottom.end(), std::back_inserter(result));
	std::sort(result.begin(), result.end());
	return result;
}
