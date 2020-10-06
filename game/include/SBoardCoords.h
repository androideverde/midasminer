#pragma once

struct SBoardCoords {
	int row;
	int col;
	bool operator==(const SBoardCoords& comp) const
	{
		return row == comp.row && col == comp.col;
	}
};
