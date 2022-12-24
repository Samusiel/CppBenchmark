#include <gtest/gtest.h>

#include <math/Hash.hpp>
#include <string>

TEST(Hash, StringHash) {
    constexpr std::string_view str0{"Hello World"};
    constexpr std::string_view str1{""};
    constexpr std::string_view str2{"Not a string"};
    constexpr std::string_view str3{"Just to check"};
    std::string str{str3.data()};
    ASSERT_TRUE(Math::compileTimeHash(str0) == Math::hash(str0));
    ASSERT_TRUE(Math::compileTimeHash(str1) == Math::hash(str1));
    ASSERT_TRUE(Math::compileTimeHash(str2) == Math::hash(str2));
    ASSERT_TRUE(Math::compileTimeHash(str3) == Math::hash(str.c_str()));
}