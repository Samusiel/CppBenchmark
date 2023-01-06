#include <gtest/gtest.h>

#include <reflection/MemberCount.hpp>

struct Test1 {
    int i;
    int j;
    int k;
};

struct Test2 {
    int i, j, k, w, n;
};

TEST(Reflection, MemberCount) {
    ASSERT_TRUE(Reflection::memberCount<Test1>() == 3);
    ASSERT_TRUE(Reflection::memberCount<Test2>() == 5);
}