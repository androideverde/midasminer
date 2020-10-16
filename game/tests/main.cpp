#include "gtest/gtest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(simple_suite, always_fails)
{
	EXPECT_EQ(0, 1);
}

TEST(simple_suite, always_passes)
{
	EXPECT_EQ(0, 0);
}
