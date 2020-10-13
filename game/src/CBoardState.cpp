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

TileType CBoardState::GetTile(SBoardCoords coords) const
{
	int index = GetIndexFromCoords(coords);
	return mBoardState[index];
}

void CBoardState::SetTile(SBoardCoords coords, TileType type)
{
	int index = GetIndexFromCoords(coords);
	mBoardState[index] = type;
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

std::vector<int> CBoardState::GetNeighbours(SBoardCoords coords, EDirection direction) const
{
	std::vector<int> result;
	TileType tile = GetTile(coords);
	int row = coords.row;
	int col = coords.col;
	int loopIndex;
	switch (direction) {
		case CBoardState::EDirection::LEFT:
			loopIndex = col;
			break;
		case CBoardState::EDirection::RIGHT:
			loopIndex = BOARD_SIZE - col - 1;
			break;
		case CBoardState::EDirection::UP:
			loopIndex = row;
			break;
		case CBoardState::EDirection::DOWN:
			loopIndex = BOARD_SIZE - row - 1;
			break;
	}
	for (int i = 0; i < loopIndex; i++)
	{
		int nextStep;
		SBoardCoords neighbour;
		switch (direction) {
			case CBoardState::EDirection::LEFT:
				nextStep = col - 1 - i;
				neighbour = {row, nextStep};
				break;
			case CBoardState::EDirection::RIGHT:
				nextStep = col + 1 + i;
				neighbour = {row, nextStep};
				break;
			case CBoardState::EDirection::UP:
				nextStep = row - 1 - i;
				neighbour = {nextStep, col};
				break;
			case CBoardState::EDirection::DOWN:
				nextStep = row + 1 + i;
				neighbour = {nextStep, col};
				break;
		}
		TileType neighbourTile = GetTile(neighbour);
		if (neighbourTile == tile)
		{
			result.push_back(GetIndexFromCoords(neighbour));
		}
		else
		{
			break;
		}
	}
	return result;
}

std::vector<int> CBoardState::GetRowNeighboursSameAsTile(SBoardCoords coords) const
{
	std::vector<int> left = GetNeighbours(coords, CBoardState::EDirection::LEFT);
	std::vector<int> right = GetNeighbours(coords, CBoardState::EDirection::RIGHT);
	std::vector<int> result;
	result.push_back(GetIndexFromCoords(coords));
	std::merge(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(result));
	std::sort(result.begin(), result.end());
	return result;
}

std::vector<int> CBoardState::GetColNeighboursSameAsTile(SBoardCoords coords) const
{
	std::vector<int> top = GetNeighbours(coords, CBoardState::EDirection::UP);
	std::vector<int> bottom = GetNeighbours(coords, CBoardState::EDirection::DOWN);
	std::vector<int> result;
	result.push_back(GetIndexFromCoords(coords));
	std::merge(top.begin(), top.end(), bottom.begin(), bottom.end(), std::back_inserter(result));
	std::sort(result.begin(), result.end());
	return result;
}

int CBoardState::CountRowNeighboursSameAsTile(SBoardCoords coords) const
{
	std::vector<int> neighbours = GetRowNeighboursSameAsTile(coords);
	return neighbours.size();
}

int CBoardState::CountColNeighboursSameAsTile(SBoardCoords coords) const
{
	std::vector<int> neighbours = GetColNeighboursSameAsTile(coords);
	return neighbours.size();
}

std::set<SBoardCoords> CBoardState::GetNeighboursSameAsTile(SBoardCoords coords) const
{
	std::vector<int> matchInRow = GetRowNeighboursSameAsTile(coords);
	std::vector<int> matchInCol = GetColNeighboursSameAsTile(coords);
	std::vector<int> merged;
	std::merge(matchInRow.begin(), matchInRow.end(), matchInCol.begin(), matchInCol.end(), std::back_inserter(merged));
	std::sort(merged.begin(), merged.end());
	std::set<SBoardCoords> matchGroup;
	for (int index : merged)
	{
		matchGroup.insert(GetCoordsFromIndex(index));
	}
	return matchGroup;
}

void CBoardState::Refill()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		int row = BOARD_SIZE - 1 - i;
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			TileType tile = GetTile({row, col});
			if (tile == TileType::EMPTY)
			{
				printf("found empty tile\n");
				ShiftColumnDown({row, col});
				AddNewCandy({0, col});
			}
		}
	}
}

void CBoardState::ShiftColumnDown(SBoardCoords coords)
{
	for (int i = 0; i < coords.row; i++)
	{
		SBoardCoords current;
		current.row = coords.row - i;
		current.col = coords.col;
		SBoardCoords above;
		above.row = coords.row - 1 - i;
		above.col = coords.col;
		TileType tile = GetTile(above);
		SetTile(current, tile);
		SetTile(above, TileType::EMPTY);
	}
}

void CBoardState::AddNewCandy(SBoardCoords coords)
{
	SetTile(coords, TileType::RED);
}
