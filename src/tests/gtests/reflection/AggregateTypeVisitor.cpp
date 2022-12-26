#include <gtest/gtest.h>

#include <reflection/AggregateTypeVisitor.hpp>
#include <sstream>

TEST(Reflection, Visit) {
    // Simple struct
    struct Test1 {
        int i;
        int j;
        int k;
    };
    Test1 t1 {
        .i = 1,
        .j = 2,
        .k = 3
    };
    int valueToCheck = 1;
    auto localVisit = [&](const auto& value) {
        ASSERT_TRUE(valueToCheck++ == value);
    };
    Reflection::visit([&](const auto& ... values) {
        ((localVisit(values)), ...);
    }, t1);

    // Different fields
    struct Test3 {
        std::string name;
        int value;
        size_t id;
    };
    Test3 initialValue {
        .name = "Hello World",
        .value = 5,
        .id = 666
    };
    std::string result = "Hello World5666";
    std::ostringstream output;
    Reflection::visit([&](const auto& ... values) {
        ((output << values), ...);
    }, initialValue);
    ASSERT_TRUE(result == output.str());
}

TEST(Reflection, MakeTuple) {
    struct Test {
        int value;
        size_t id;
        std::string name;
        std::vector<int> randomValues;
    };
    Test t {
        .value = 666,
        .id = 123456789,
        .name = "Hello World",
        .randomValues = {9, 8, 7, 6, 5, 4, 3, 2, 1}
    };

    auto compareVectors = [](const auto& v1, const auto& v2) {
        if (v1.size() != v2.size()) return false;
        for (int i = 0; i < v1.size(); ++i) {
            if (v1[i] != v2[i]) return false;
        }
        return true;
    };
    auto testTuple = Reflection::makeTuple(t);
    ASSERT_TRUE(std::get<0>(testTuple) == t.value);
    ASSERT_TRUE(std::get<1>(testTuple) == t.id);
    ASSERT_TRUE(std::get<2>(testTuple) == t.name);
    ASSERT_TRUE(compareVectors(std::get<3>(testTuple), t.randomValues));
}