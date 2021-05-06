#include "gtest/gtest.h"

#include <CBoardState.h>
#include <CCandy.h>
#include <CAnimationSystem.h>
#include <CRefiller.h>
#include <CMatcher.h>
#include <CCandyGenerator.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(board, refills_tile)
{
	class TestCandyGenerator : public CCandyGenerator
	{
	public:
		CandyType GenerateCandy() const override
		{
			return CandyType::BLUE;
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
	CBoardState board(8, 43, 0, 0, std::make_unique<const TestCandyGenerator>());
	board.SetupBoard(testBoard);
	CAnimationSystem animSys; // TODO: can be mocked
	CRefiller refiller(board, animSys);
	EXPECT_TRUE(refiller.IsRefillPending());
	refiller.RefillBoard();
	EXPECT_EQ(board.GetTile({0, 1}).GetCandy()->GetType(), CandyType::BLUE);
}

TEST(board, no_refill_available)
{
	std::vector<int> testBoard = {
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 3, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
	};
	CBoardState board(8, 43, 0, 0, std::make_unique<CCandyGenerator>()); // TODO: candyGen can be mocked
	board.SetupBoard(testBoard);
	CAnimationSystem animSys; // TODO: can be mocked
	CRefiller refiller(board, animSys);
	EXPECT_FALSE(refiller.IsRefillPending());
}

TEST(board, no_match_available)
{
	std::vector<int> testBoard = {
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 3, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
	};
	CBoardState board(8, 43, 0, 0, std::make_unique<CCandyGenerator>()); // TODO: candyGen can be mocked
	board.SetupBoard(testBoard);
	CAnimationSystem animSys; // TODO: can be mocked
	CScoringSystem scoringSys; // TODO: can be mocked
	CMatcher matcher(board, animSys, scoringSys);
	EXPECT_FALSE(matcher.IsMatchPending());
}

TEST(board, row_match_triggered)
{
	std::vector<int> testBoard = {
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 4, 4, 2, 3,
		1, 2, 3, 3, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
	};
	CBoardState board(8, 43, 0, 0, std::make_unique<CCandyGenerator>()); // TODO: candyGen can be mocked
	board.SetupBoard(testBoard);
	CAnimationSystem animSys; // TODO: can be mocked
	CScoringSystem scoringSys; // TODO: can be mocked
	CMatcher matcher(board, animSys, scoringSys);
	EXPECT_TRUE(matcher.IsMatchPending());
	EXPECT_FALSE(matcher.IsMatchInTile({0, 0}));
	EXPECT_TRUE(matcher.IsMatchInTile({3, 3}));
	EXPECT_TRUE(matcher.IsMatchInTile({3, 4}));
	EXPECT_TRUE(matcher.IsMatchInTile({3, 5}));
	matcher.DoMatchInTile({3, 3});
	EXPECT_EQ(board.GetTile({3, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({3, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({3, 5}).GetCandy(), nullptr);
}

TEST(board, col_match_triggered)
{
	std::vector<int> testBoard = {
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
		2, 3, 2, 4, 1, 2, 3, 1,
		1, 2, 3, 2, 5, 1, 2, 3,
		1, 2, 3, 3, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
	};
	CBoardState board(8, 43, 0, 0, std::make_unique<CCandyGenerator>()); // TODO: candyGen can be mocked
	board.SetupBoard(testBoard);
	CAnimationSystem animSys; // TODO: can be mocked
	CScoringSystem scoringSys; // TODO: can be mocked
	CMatcher matcher(board, animSys, scoringSys);
	EXPECT_TRUE(matcher.IsMatchPending());
	EXPECT_TRUE(matcher.IsMatchInTile({0, 3}));
	EXPECT_TRUE(matcher.IsMatchInTile({1, 3}));
	EXPECT_TRUE(matcher.IsMatchInTile({2, 3}));
	matcher.DoMatchInTile({1, 3});
	EXPECT_EQ(board.GetTile({0, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({1, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({2, 3}).GetCandy(), nullptr);
}

TEST(board, t_match_triggered_on_row)
{
	std::vector<int> testBoard = {
		1, 2, 4, 4, 4, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
		2, 3, 2, 4, 1, 2, 3, 1,
		1, 2, 3, 2, 5, 1, 2, 3,
		1, 2, 3, 3, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
	};
	CBoardState board(8, 43, 0, 0, std::make_unique<CCandyGenerator>()); // TODO: candyGen can be mocked
	board.SetupBoard(testBoard);
	CAnimationSystem animSys; // TODO: can be mocked
	CScoringSystem scoringSys; // TODO: can be mocked
	CMatcher matcher(board, animSys, scoringSys);
	EXPECT_TRUE(matcher.IsMatchPending());
	EXPECT_TRUE(matcher.IsMatchInTile({0, 2}));
	EXPECT_TRUE(matcher.IsMatchInTile({0, 3}));
	EXPECT_TRUE(matcher.IsMatchInTile({0, 4}));
	EXPECT_TRUE(matcher.IsMatchInTile({1, 3}));
	EXPECT_TRUE(matcher.IsMatchInTile({2, 3}));
	matcher.DoMatchInTile({0, 3});
	EXPECT_EQ(board.GetTile({0, 2}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({1, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({2, 3}).GetCandy(), nullptr);
}

TEST(board, t_match_triggered_on_col)
{
	std::vector<int> testBoard = {
		1, 2, 4, 4, 4, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
		2, 3, 2, 4, 1, 2, 3, 1,
		1, 2, 3, 2, 5, 1, 2, 3,
		1, 2, 3, 3, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
	};
	CBoardState board(8, 43, 0, 0, std::make_unique<CCandyGenerator>()); // TODO: candyGen can be mocked
	board.SetupBoard(testBoard);
	CAnimationSystem animSys; // TODO: can be mocked
	CScoringSystem scoringSys; // TODO: can be mocked
	CMatcher matcher(board, animSys, scoringSys);
	EXPECT_TRUE(matcher.IsMatchPending());
	EXPECT_TRUE(matcher.IsMatchInTile({0, 2}));
	EXPECT_TRUE(matcher.IsMatchInTile({0, 3}));
	EXPECT_TRUE(matcher.IsMatchInTile({0, 4}));
	EXPECT_TRUE(matcher.IsMatchInTile({1, 3}));
	EXPECT_TRUE(matcher.IsMatchInTile({2, 3}));
	matcher.DoMatchInTile({0, 3});
	EXPECT_EQ(board.GetTile({0, 2}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({1, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({2, 3}).GetCandy(), nullptr);
}

TEST(board, t_match_triggered_on_row_full_board)
{
	std::vector<int> testBoard = {
		1, 2, 4, 4, 4, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
		2, 3, 2, 4, 1, 2, 3, 1,
		1, 2, 3, 2, 5, 1, 2, 3,
		1, 2, 3, 3, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
	};
	CBoardState board(8, 43, 0, 0, std::make_unique<CCandyGenerator>()); // TODO: candyGen can be mocked
	board.SetupBoard(testBoard);
	CAnimationSystem animSys; // TODO: can be mocked
	CScoringSystem scoringSys; // TODO: can be mocked
	CMatcher matcher(board, animSys, scoringSys);
	EXPECT_TRUE(matcher.IsMatchPending());
	EXPECT_TRUE(matcher.IsMatchInTile({0, 2}));
	EXPECT_TRUE(matcher.IsMatchInTile({0, 3}));
	EXPECT_TRUE(matcher.IsMatchInTile({0, 4}));
	EXPECT_TRUE(matcher.IsMatchInTile({1, 3}));
	EXPECT_TRUE(matcher.IsMatchInTile({2, 3}));
	matcher.DoMatch();
	EXPECT_EQ(board.GetTile({0, 2}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({1, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({2, 3}).GetCandy(), nullptr);
}

TEST(board, t_match_triggered_on_col_full_board)
{
	std::vector<int> testBoard = {
		1, 2, 4, 4, 4, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
		2, 3, 2, 4, 1, 2, 3, 1,
		1, 2, 3, 2, 5, 1, 2, 3,
		1, 2, 3, 3, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
	};
	CBoardState board(8, 43, 0, 0, std::make_unique<CCandyGenerator>()); // TODO: candyGen can be mocked
	board.SetupBoard(testBoard);
	CAnimationSystem animSys; // TODO: can be mocked
	CScoringSystem scoringSys; // TODO: can be mocked
	CMatcher matcher(board, animSys, scoringSys);
	EXPECT_TRUE(matcher.IsMatchPending());
	EXPECT_TRUE(matcher.IsMatchInTile({0, 2}));
	EXPECT_TRUE(matcher.IsMatchInTile({0, 3}));
	EXPECT_TRUE(matcher.IsMatchInTile({0, 4}));
	EXPECT_TRUE(matcher.IsMatchInTile({1, 3}));
	EXPECT_TRUE(matcher.IsMatchInTile({2, 3}));
	matcher.DoMatch();
	EXPECT_EQ(board.GetTile({0, 2}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({1, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({2, 3}).GetCandy(), nullptr);
}

TEST(board, row_match_with_neighbour)
{
	std::vector<int> testBoard = {
		1, 2, 5, 5, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
		2, 3, 2, 1, 1, 2, 3, 1,
		1, 2, 3, 2, 5, 1, 2, 3,
		1, 2, 3, 3, 5, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
	};
	CBoardState board(8, 43, 0, 0, std::make_unique<CCandyGenerator>()); // TODO: candyGen can be mocked
	board.SetupBoard(testBoard);
	CAnimationSystem animSys; // TODO: can be mocked
	CScoringSystem scoringSys; // TODO: can be mocked
	CMatcher matcher(board, animSys, scoringSys);
	EXPECT_TRUE(matcher.IsMatchPending());
	EXPECT_TRUE(matcher.IsMatchInTile({0, 2}));
	EXPECT_TRUE(matcher.IsMatchInTile({0, 3}));
	EXPECT_TRUE(matcher.IsMatchInTile({0, 4}));
	EXPECT_FALSE(matcher.IsMatchInTile({1, 4}));
	matcher.DoMatchInTile({0, 3});
	EXPECT_EQ(board.GetTile({0, 2}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({1, 4}).GetCandy()->GetType(), CandyType::PURPLE);
}

TEST(board, col_match_with_neighbour)
{
	std::vector<int> testBoard = {
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
		2, 3, 4, 5, 2, 2, 3, 1,
		1, 2, 3, 2, 2, 1, 2, 3,
		1, 2, 3, 3, 2, 1, 2, 3,
		2, 3, 4, 5, 1, 2, 3, 1,
		1, 2, 3, 4, 5, 1, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
	};
	CBoardState board(8, 43, 0, 0, std::make_unique<CCandyGenerator>()); // TODO: candyGen can be mocked
	board.SetupBoard(testBoard);
	CAnimationSystem animSys; // TODO: can be mocked
	CScoringSystem scoringSys; // TODO: can be mocked
	CMatcher matcher(board, animSys, scoringSys);
	EXPECT_TRUE(matcher.IsMatchPending());
	EXPECT_TRUE(matcher.IsMatchInTile({2, 4}));
	EXPECT_TRUE(matcher.IsMatchInTile({3, 4}));
	EXPECT_TRUE(matcher.IsMatchInTile({4, 4}));
	EXPECT_FALSE(matcher.IsMatchInTile({3, 3}));
	EXPECT_FALSE(matcher.IsMatchInTile({2, 5}));
	matcher.DoMatchInTile({2, 4});
	EXPECT_EQ(board.GetTile({2, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({3, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({4, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({3, 3}).GetCandy()->GetType(), CandyType::GREEN);
	EXPECT_EQ(board.GetTile({2, 5}).GetCandy()->GetType(), CandyType::GREEN);
}

TEST(board, extreme_match)
{
	// board state is coming from a refill of a previous match in 0,2 - 4,2 that has been refilled with color 1
	std::vector<int> testBoard = {
		1, 1, 1, 1, 1, 2, 2, 3,
		1, 1, 1, 1, 1, 3, 2, 3,
		2, 3, 1, 5, 4, 2, 3, 1,
		1, 1, 1, 1, 1, 4, 2, 3,
		1, 1, 1, 1, 1, 5, 2, 3,
		2, 3, 1, 5, 4, 2, 3, 1,
		1, 2, 1, 2, 1, 3, 2, 3,
		1, 2, 3, 4, 5, 1, 2, 3,
	};
	CBoardState board(8, 43, 0, 0, std::make_unique<CCandyGenerator>());
	board.SetupBoard(testBoard);
	CAnimationSystem animSys; // TODO: can be mocked
	CScoringSystem scoringSys; // TODO: can be mocked
	CMatcher matcher(board, animSys, scoringSys);
	matcher.DoMatch();
	EXPECT_EQ(board.GetTile({0, 0}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 1}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 2}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({0, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({1, 0}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({1, 1}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({1, 2}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({1, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({1, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({2, 2}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({3, 0}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({3, 1}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({3, 2}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({3, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({3, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({4, 0}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({4, 1}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({4, 2}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({4, 3}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({4, 4}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({5, 2}).GetCandy(), nullptr);
	EXPECT_EQ(board.GetTile({6, 2}).GetCandy(), nullptr);
}
