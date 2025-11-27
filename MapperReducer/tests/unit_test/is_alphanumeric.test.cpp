#include "utils.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace utils;

TEST(IsAlphanumericTest, HandlesAlphabeticCharacters) {
  EXPECT_TRUE(isAlphanumeric('A'));
  EXPECT_TRUE(isAlphanumeric('Z'));
  EXPECT_TRUE(isAlphanumeric('a'));
  EXPECT_TRUE(isAlphanumeric('z'));
}

TEST(IsAlphanumericTest, HandlesNumericCharacters) {
  EXPECT_TRUE(isAlphanumeric('0'));
  EXPECT_TRUE(isAlphanumeric('9'));
}

TEST(IsAlphanumericTest, HandlesNonAlphanumericCharacters) {
  EXPECT_FALSE(isAlphanumeric(','));
  EXPECT_FALSE(isAlphanumeric(' '));
  EXPECT_FALSE(isAlphanumeric('!'));
  EXPECT_FALSE(isAlphanumeric('@'));
  EXPECT_FALSE(isAlphanumeric('\n'));
}
