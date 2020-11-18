#include <CBoardState.h>

#include <Utils.h>
#include <cassert>

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
	mCandies.clear();
	mTiles.clear();
	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
	{
		mTiles.emplace_back(GetCoordsFromIndex(i));
	}
	SetupBoard(CBoardStateInternal::SampleBoard);
}

SDL_Point CBoardState::ResetCandyPos(int index) const
{
	SDL_Point point;
	SBoardCoords coords = GetCoordsFromIndex(index);
	point.x = coords.col * TILE_SIZE + BOARD_ORIGIN.x;
	point.y = coords.row * TILE_SIZE + BOARD_ORIGIN.y;
	return point;
}

void CBoardState::SetupBoard(const std::vector<int>& boardDefinition)
{
	assert(boardDefinition.size() == mTiles.size());
	for (int i = 0; i < boardDefinition.size(); i++)
	{
		std::unique_ptr<CCandy> candy = std::make_unique<CCandy>(static_cast<CandyType>(boardDefinition[i]), ResetCandyPos(i));
		mTiles[i].SetCandy(candy.get());
		mCandies.push_back(std::move(candy));
	}
}

CandyType CBoardState::GetCandyType(SBoardCoords coords) const
{
	return mTiles[GetIndexFromCoords(coords)].GetCandy()->GetType();
}

CCandy* CBoardState::GetCandy(SBoardCoords coords) const
{
	return mTiles[GetIndexFromCoords(coords)].GetCandy();
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
	CandyType tile = GetCandyType(coords);
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
		CandyType neighbourTile = GetCandyType(neighbour);
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

std::map<CCandy*, int> CBoardState::GetFallingCandies() const
{
	std::map<CCandy*, int> fallingCandies;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		int row = BOARD_SIZE - 1 - i;
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			CCandy* candy = GetCandy({row, col});
			//CandyType tile = GetCandyType({row, col});
			if (candy == nullptr)
			{
				printf("found empty tile (%d, %d)\n", row, col);
				int emptiesInCol = CountColNeighboursSameAsTile({row, col});
				for (int i = 0; i < row - emptiesInCol; i++)
				{
					fallingCandies.insert(std::pair<CCandy*, int>(GetCandy({row - i - emptiesInCol, col}), emptiesInCol));
				}
			}
		}
	}
	return fallingCandies;
}

std::map<std::unique_ptr<CCandy>, int> CBoardState::GenerateNewCandies() const
{
	std::map<std::unique_ptr<CCandy>, int> newCandies;
	// find all empties in the board
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		int row = BOARD_SIZE - 1 - i;
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			CandyType tile = GetCandyType({row, col});
			if (tile == CandyType::EMPTY)
			{
				printf("found empty tile (%d, %d)\n", row, col);
				int emptiesInCol = CountColNeighboursSameAsTile({row, col});
				for (int i = 0; i <= emptiesInCol; i++)
				{
					CandyType newTile = mCandyGenerator->GenerateCandy();
					SDL_Point pos = ResetCandyPos(col);
					pos.y -= TILE_SIZE;
					std::unique_ptr<CCandy> candy = std::make_unique<CCandy>(newTile, pos);
					//newCandies.insert({std::move(candy), emptiesInCol - i - 1});
				}
			}
		}
	}
	return newCandies;
}

std::vector<CCandy*> CBoardState::Refill()
{
	std::vector<CCandy*> fallingCandies;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		int row = BOARD_SIZE - 1 - i;
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			CandyType tile = GetCandyType({row, col});
			if (tile == CandyType::EMPTY)
			{
				printf("found empty tile (%d, %d)\n", row, col);
				//find out how many empties in col
				//generate as many new candies as needed for that col
				//shift col down by amount of empties in col
				//appear new candy + fall to place <-- repeat until all new candies are drawn
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
	CCandy* candy_1 = mTiles[index_1].GetCandy();
	CCandy* candy_2 = mTiles[index_2].GetCandy();
	mTiles[index_1].SetCandy(candy_2);
	mTiles[index_2].SetCandy(candy_1);
	printf("swap done! (%d, %d) %s <-> (%d, %d) %s\n", tileCoords_1.row, tileCoords_1.col, GetTileName(candy_1->GetType()).c_str(), tileCoords_2.row, tileCoords_2.col, GetTileName(candy_2->GetType()).c_str());
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
	CandyType newTile = mCandyGenerator->GenerateCandy();
	GetCandy(coords)->SetType(newTile);
	std::string tileName = GetTileName(newTile);
	printf("refilled (%d, %d) with %s\n", coords.row, coords.col, tileName.c_str());
}

std::string CBoardState::GetTileName(CandyType tile) const
{
	switch (tile) {
		case CandyType::BLUE:
			return "BLUE";
			break;
		case CandyType::GREEN:
			return "GREEN";
			break;
		case CandyType::RED:
			return "RED";
			break;
		case CandyType::YELLOW:
			return "YELLOW";
			break;
		case CandyType::PURPLE:
			return "PURPLE";
			break;
		case CandyType::EMPTY:
			return "EMPTY";
			break;
	}
}
