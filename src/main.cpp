#include <iostream>
#include <string>

#include "lib/master.hpp"

int main(int argc, char *argv[]) {
    auto master = Master();

    master.push(10).push(11).push(12);
    master.push(60, 60).push(61, 60);
    const auto weights = master.weights();

    std::cout << "Weights: ";
    for (const auto& weight : weights) {
        std::cout << weight << ", ";
    }
    std::cout << "\n";

    const auto elements = master.items_by(10);
    std::cout << "Elements by weight 10:";
    for (const auto& element : elements) {
        std::cout << element.value<int>() << ", ";
    }
    std::cout << "\n";

    std::cout << "enumerate elements:\n";
    master.map([](Any& element) { std::cout << element.value<int>() << ";\n"; });

    master.map([](Any& element, size_t weight) { return 666; });
    std::cout << "Elements by weight 10:";
    for (const auto& element : master.items_by(10)) {
        std::cout << element.value<int>() << ", ";
    }
    std::cout << "\n";

    return 0;
}
