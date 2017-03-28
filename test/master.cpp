#include "gtest/gtest.h"
#include "lib/master.hpp"

#include <set>

using namespace ::testing;

TEST(Master, InitDefault) {
    auto master = Master();

    master.push(1).push(2).push(3).push(11, 20).push(12, 20);

    const std::set<size_t> expected_weights = {Master::DEFAULT_WEIGHT, 20};
    const auto weights = master.weights();

    ASSERT_EQ(weights, expected_weights);
}

TEST(Master, InitNonDefault) {
    const size_t default_weight = 25;
    auto master = Master(default_weight);

    master.push(1).push(2).push(3).push(11, 20).push(12, 20);

    const std::set<size_t> expected_weights = {default_weight, 20};
    const auto weights = master.weights();

    ASSERT_EQ(weights, expected_weights);
}

TEST(Master, SelectItemsByWeight) {
    const size_t default_weight = 25;
    auto master = Master(default_weight);

    master.push(1).push(2).push(3).push(11, 20).push(12, 20);

    const std::vector<int> expected_items = {1, 2, 3};
    const auto items_any = master.items_by(default_weight);

    ASSERT_EQ(items_any.size(), expected_items.size());

    std::vector<int> items_ints;
    for (auto& item : items_any) {
        items_ints.push_back(item.value<int>());
    }
    std::sort(items_ints.begin(), items_ints.end());

    ASSERT_EQ(expected_items, items_ints);

    const auto items_any_empty = master.items_by(0xFFFF);
    ASSERT_EQ(items_any_empty.size(), 0);
}

TEST(Master, Map) {
    auto master = Master();
    master.push(1, 20).push(2, 20).push(3).push(11).push(12, 20);

    const std::vector<int> expected_items = {3, 11, 1, 2, 12};
    size_t expected_idx = 0;

    master.map([&expected_items = expected_items, &expected_idx = expected_idx](Any& any) {
        const auto value = any.value<int>();
        ASSERT_EQ(value, expected_items[expected_idx]);
        expected_idx++;
    });
}

TEST(Master, MapChangeWeight) {
    auto master = Master();
    master.push(1, 20).push(2, 20).push(3).push(11).push(12, 20);

    master.map([](Any& any, size_t weight) {
        return weight + 10;
    });

    const std::set<size_t> expected_weights = {Master::DEFAULT_WEIGHT + 10, 30};
    const auto weights = master.weights();

    ASSERT_EQ(weights, expected_weights);

    const std::vector<int> expected_items = {1, 2, 12};
    const auto items_any = master.items_by(30);

    ASSERT_EQ(items_any.size(), expected_items.size());

    std::vector<int> items_ints;
    for (auto& item : items_any) {
        items_ints.push_back(item.value<int>());
    }
    std::sort(items_ints.begin(), items_ints.end());

    ASSERT_EQ(expected_items, items_ints);

}
