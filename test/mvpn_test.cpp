#include <gtest/gtest.h>

int add(int x, int y) {
  return x + y;
}

TEST(AddTest, Test1) {
  ASSERT_EQ(2, add(1, 1));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
