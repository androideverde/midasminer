#pragma once

struct SBoardCoords {
	int row = -1;
	int col = -1;
	bool operator==(const SBoardCoords& comp) const
	{
		return row == comp.row && col == comp.col;
	}
	bool operator<(const SBoardCoords& comp) const
	{
		if (row < comp.row)
		{
			return true;
		}
		else if (row == comp.row)
		{
			if (col < comp.col)
			{
				return true;
			}
		}
		return false;
	}
};
