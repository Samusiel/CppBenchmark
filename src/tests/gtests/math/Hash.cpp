#include <gtest/gtest.h>

#include <math/Hash.hpp>

TEST(Hash, StringHash) {
    ASSERT_TRUE(Math::Compile::hash("Hello World") == Math::Runtime::hash("Hello World"));
    ASSERT_TRUE(Math::Compile::hash("") == Math::Runtime::hash(""));
    ASSERT_TRUE(Math::Compile::hash("Not a string") == Math::Runtime::hash("Not a string"));
}