#include "gtest/gtest.h"

#include <CBoardState.h>
#include <ETileType.h>
#include <CCandyGenerator.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(board, check_refill)
{
	class TestCandyGenerator : public CCandyGenerator
	{
	public:
		TileType GenerateCandy() const override
		{
			return TileType::BLUE;
		}
	};
	std::vector<int> testBoard = {
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 0, 3, 4, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
	};
	CBoardState boardState(8, std::make_unique<const TestCandyGenerator>());
	boardState.SetupBoard(testBoard);
	boardState.Refill();
	EXPECT_EQ(boardState.GetTile({0, 1}), TileType::BLUE);
}
