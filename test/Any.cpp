#include "gtest/gtest.h"
#include "lib/Any.hpp"

TEST(Any, InitInt) {
    const auto some_int = 1;
    const Any any_int = some_int;

    ASSERT_TRUE(any_int);
    ASSERT_EQ(any_int.type(), typeid(some_int));
    ASSERT_NE(any_int.value_ptr<int>(), nullptr);
    ASSERT_EQ(any_int.value<int>(), some_int);

    ASSERT_EQ(any_int.value_ptr<std::string>(), nullptr);
}

TEST(Any, InitString) {
    const auto some_string = std::string("test");
    const Any any_str = some_string;

    ASSERT_TRUE(any_str);
    ASSERT_EQ(any_str.type(), typeid(some_string));
    ASSERT_NE(any_str.value_ptr<std::string>(), nullptr);
    ASSERT_EQ(any_str.value<std::string>(), some_string);

    ASSERT_EQ(any_str.value_ptr<int>(), nullptr);
}

TEST(Any, InitEmpty) {
    const Any any;

    ASSERT_FALSE(any);
    ASSERT_EQ(any.type(), typeid(void));
}
