#include <gtest/gtest.h>

// Sample function to test
int add(int a, int b)
{
    return a + b;
}

// sample test
TEST(AdditionTest, HandlesPositiveInput)
{
    EXPECT_EQ(add(1, 2), 3);
    EXPECT_EQ(add(10, 20), 30);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
