#pragma once

#include <functional>
#include <vector>
#include <set>

#include "any.hpp"

struct Element {
    Any value;
    size_t weight;

    Element(Any &value, size_t weight) noexcept(true) : value(std::move(value)), weight(weight) {}

    bool operator < (const Element& right) const {
        return weight < right.weight;
    }
};

/** Function to perform map over Any objects */
typedef std::function<void(Any&)> map_any_fn;
/** Function to perform map over Any objects and return new weight. */
typedef std::function<size_t(Any&, size_t)> map_any_w_weight_fn;

/**
 * Storage of Any objects.
 */
class Master {
    public:
        /**
         * Creates new instance with default weight @ref DEFAULT_WEIGHT.
         */
        Master() noexcept(true) : Master(Master::DEFAULT_WEIGHT) {};

        /**
         * Creates new instance with custom default weight.
         *
         * @param weight Default weight setting.
         */
        explicit Master(size_t weight) noexcept(true) : default_weight(weight) {
        };

        /**
         * Adds element with default weight.
         *
         * @param value Value to store.
         * @returns self reference.
         */
        template<typename T>
        Master& push(const T& value) {
            return push(value, default_weight);
        }

        /**
         * Adds element and assign specified weight.
         *
         * @param value Value to store.
         * @param weight Weight to assign.
         * @returns self reference.
         */
        template<typename T>
        Master& push(const T& value, size_t weight) {
            const auto element = Element(Any(value), weight);
            values.push_back(std::move(element));
            sorted = false;
            return *this;
        }

        /**
         * @returns Set of used weights.
         */
        auto weights() const {
            std::set<size_t> weights;

            for (const auto& element : values) {
                weights.insert(element.weight);
            }

            return weights;
        }

        /**
         * @returns Vector of items
         * @param weight Desired weight of items.
         */
        auto items_by(size_t weight) const {
            std::vector<Any> result;

            for (auto element : values) {
                if (element.weight == weight) {
                    result.push_back(element.value);
                }
            }

            return result;
        }

        /**
         * Runs function over elements in ordered manner.
         */
        void map(map_any_fn fn) {
            if (!sorted) {
                std::sort(values.begin(), values.end());
                sorted = true;
            }

            for (auto& element : values) {
                fn(element.value);
            }
        }

        void map(map_any_w_weight_fn fn) {
            for (auto& element : values) {
                element.weight = fn(element.value, element.weight);
            }

            sorted = false;
        }

        /**
         * Default weight that is used unless user provides own.
         */
        static const size_t DEFAULT_WEIGHT = 10;

    private:
        bool sorted = false;
        size_t default_weight;
        std::vector<Element> values;
};
