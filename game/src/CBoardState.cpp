#include <CBoardState.h>

#include <Utils.h>
#include <CCandyGenerator.h>

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

CBoardState::CBoardState(int size, int tileSize, int originX, int originY, std::unique_ptr<const CCandyGenerator> candyGenerator)
	: BOARD_SIZE(size)
	, TILE_SIZE(tileSize)
	, BOARD_ORIGIN({originX, originY})
	, mCandyGenerator(std::move(candyGenerator))
{
	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
	{
		SDL_Point candyPos = ResetCandyPos(i);
		mCandies.push_back(std::make_unique<CCandy>(TileType::EMPTY, candyPos));
	}
	SetupBoard(CBoardStateInternal::SampleBoard);
}

SDL_Point CBoardState::ResetCandyPos(int index)
{
	SDL_Point point;
	SBoardCoords coords = GetCoordsFromIndex(index);
	point.x = coords.col * TILE_SIZE + BOARD_ORIGIN.x;
	point.y = coords.row * TILE_SIZE + BOARD_ORIGIN.y;
	return point;
}

void CBoardState::SetupBoard(const std::vector<int>& boardDefinition)
{
	int index = 0;
	for (int value : boardDefinition)
	{
		mCandies[index]->SetType(static_cast<TileType>(value));
		index++;
	}
}

TileType CBoardState::GetTile(SBoardCoords coords) const
{
	int index = GetIndexFromCoords(coords);
	return mCandies[index]->GetType();
}

CCandy* CBoardState::GetCandy(SBoardCoords coords)
{
	int index = GetIndexFromCoords(coords);
	return mCandies[index].get();
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

std::vector<CCandy*> CBoardState::Refill()
{
	std::vector<CCandy*> fallingCandies;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		int row = BOARD_SIZE - 1 - i;
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			TileType tile = GetTile({row, col});
			if (tile == TileType::EMPTY)
			{
				printf("found empty tile (%d, %d)\n", row, col);
				std::vector<SBoardCoords> fallingTiles = ShiftColumnDown({row, col});
				AddNewCandy({0, col});
				fallingTiles.push_back({0, col});
				for (SBoardCoords coord : fallingTiles)
				{
					fallingCandies.push_back(GetCandy(coord));
				}
			}
		}
	}
	return fallingCandies;
}

void CBoardState::Swap(SBoardCoords tileCoords_1, SBoardCoords tileCoords_2)
{
	int index_1 = GetIndexFromCoords(tileCoords_1);
	int index_2 = GetIndexFromCoords(tileCoords_2);
	std::swap(mCandies[index_1], mCandies[index_2]);
	mCandies[index_1]->SetPos(ResetCandyPos(index_1));
	mCandies[index_2]->SetPos(ResetCandyPos(index_2));
}

std::vector<SBoardCoords> CBoardState::ShiftColumnDown(SBoardCoords coords)
{
	std::vector<SBoardCoords> fallingTiles;
	for (int i = 0; i < coords.row; i++)
	{
		SBoardCoords current;
		SBoardCoords above;
		current.row = coords.row - i;
		above.row = coords.row - 1 - i;
		current.col = above.col = coords.col;
		fallingTiles.push_back(current);
		Swap(current, above);
	}
	return fallingTiles;
}

void CBoardState::AddNewCandy(SBoardCoords coords)
{
	TileType newTile = mCandyGenerator->GenerateCandy();
	GetCandy(coords)->SetType(newTile);
	std::string tileName = GetTileName(newTile);
	printf("refilled (%d, %d) with %s\n", coords.row, coords.col, tileName.c_str());
}

std::string CBoardState::GetTileName(TileType tile) const
{
	switch (tile) {
		case TileType::BLUE:
			return "BLUE";
			break;
		case TileType::GREEN:
			return "GREEN";
			break;
		case TileType::RED:
			return "RED";
			break;
		case TileType::YELLOW:
			return "YELLOW";
			break;
		case TileType::PURPLE:
			return "PURPLE";
			break;
		case TileType::EMPTY:
			return "EMPTY";
			break;
	}
}
