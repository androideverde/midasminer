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
		mTiles.emplace_back(CTile(GetCoordsFromIndex(i)));
	}
	SetupBoard(CBoardStateInternal::SampleBoard);
}

SDL_Point CBoardState::GetTilePos(int index) const
{
	return GetTilePos(GetCoordsFromIndex(index));
}

SDL_Point CBoardState::GetTilePos(SBoardCoords coords) const
{
	SDL_Point point;
	point.x = coords.col * TILE_SIZE + BOARD_ORIGIN.x;
	point.y = coords.row * TILE_SIZE + BOARD_ORIGIN.y;
	return point;
}

void CBoardState::SetupBoard(const std::vector<int>& boardDefinition)
{
	assert(boardDefinition.size() == mTiles.size());
	for (int i = 0; i < boardDefinition.size(); i++)
	{
		CandyType candyColor = static_cast<CandyType>(boardDefinition[i]);
		if (candyColor == CandyType::EMPTY)
		{
			mTiles[i].SetCandy(nullptr);
		}
		else
		{
			std::unique_ptr<CCandy> candy = std::make_unique<CCandy>(candyColor, GetTilePos(i));
			mTiles[i].SetCandy(candy.get());
			mCandies.push_back(std::move(candy));
		}
	}
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
		CCandy* candy = GetTile(coords).GetCandy();
		CCandy* neighbourCandy = GetTile(neighbour).GetCandy();
		if (candy == neighbourCandy)
		{
			result.push_back(GetIndexFromCoords(neighbour));
			continue;
		}
		if (candy == nullptr || neighbourCandy == nullptr )
		{
			break;
		}
		if (neighbourCandy->GetType() == candy->GetType())
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
	return GetRowNeighboursSameAsTile(coords).size();
}

int CBoardState::CountColNeighboursSameAsTile(SBoardCoords coords) const
{
	return GetColNeighboursSameAsTile(coords).size();
}

std::set<SBoardCoords> CBoardState::GetMatchedNeighboursSameAsTile(SBoardCoords coords) const
{
	std::vector<int> matchInRow = GetRowNeighboursSameAsTile(coords);
	std::vector<int> matchInCol = GetColNeighboursSameAsTile(coords);
	std::vector<int> merged;
	if (matchInRow.size() >= 3 && matchInCol.size() >= 3)
	{
		std::merge(matchInRow.begin(), matchInRow.end(), matchInCol.begin(), matchInCol.end(), std::back_inserter(merged));
	}
	else
	{
		if (matchInRow.size() >= 3)
		{
			merged = matchInRow;
		}
		else
		{
			merged = matchInCol;
		}
	}
	std::sort(merged.begin(), merged.end());
	std::set<SBoardCoords> matchGroup;
	for (int index : merged)
	{
		matchGroup.insert(GetCoordsFromIndex(index));
	}
	return matchGroup;
}

void CBoardState::Swap(SBoardCoords tileCoords_1, SBoardCoords tileCoords_2)
{
	int index_1 = GetIndexFromCoords(tileCoords_1);
	int index_2 = GetIndexFromCoords(tileCoords_2);
	CCandy* candy_1 = mTiles[index_1].GetCandy();
	CCandy* candy_2 = mTiles[index_2].GetCandy();
	mTiles[index_1].SetCandy(candy_2);
	mTiles[index_2].SetCandy(candy_1);
	std::string candyName_1 = candy_1 == nullptr ? "EMPTY" : candy_1->GetName();
	std::string candyName_2 = candy_2 == nullptr ? "EMPTY" : candy_2->GetName();
	printf("swap done! (%d, %d) %s <-> (%d, %d) %s\n", tileCoords_1.row, tileCoords_1.col, candyName_1.c_str(), tileCoords_2.row, tileCoords_2.col, candyName_2.c_str());
}

void CBoardState::AddNewCandy(SBoardCoords coords)
{
	CandyType newTile = mCandyGenerator->GenerateCandy();
	// the newly created candy is placed in row -1
	SDL_Point spawnPos = GetTilePos(coords);
	spawnPos.y -= TILE_SIZE;
	std::unique_ptr<CCandy> candy = std::make_unique<CCandy>(newTile, spawnPos);
	GetTile(coords).SetCandy(candy.get());
	printf("refilled (%d, %d) with %s\n", coords.row, coords.col, candy->GetName().c_str());
	mCandies.push_back(std::move(candy));
}
