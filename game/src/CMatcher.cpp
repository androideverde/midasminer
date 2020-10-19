#include <CMatcher.h>

CMatcher::CMatcher(CBoardState& state)
	: mState(state)
{
}

void CMatcher::DoMatchInTile(SBoardCoords coords)
{
	std::set<SBoardCoords> matches = mState.GetNeighboursSameAsTile(coords);
	for (SBoardCoords tileCoords : matches)
	{
		mState.SetTile(tileCoords, TileType::EMPTY);
	}
}

bool CMatcher::IsMatchInTile(SBoardCoords coords)
{
	if (mState.CountRowNeighboursSameAsTile(coords) >= 3)
	{
		printf("row match of %s in (%d, %d)!\n", mState.GetTileName(mState.GetTile(coords)).c_str(), coords.row, coords.col);
		return true;
	}
	if (mState.CountColNeighboursSameAsTile(coords) >= 3)
	{
		printf("col match of %s in (%d, %d)!\n", mState.GetTileName(mState.GetTile(coords)).c_str(), coords.row, coords.col);
		return true;
	}
	return false;
}
