#include <iostream>
#include <map>
#include <cmath>
#include "allocator.h"
#include "container.h"

int factorial(int i) {
    return (int) tgamma(i + 1);
}

void exmpleMap() {
    auto tempMap = std::map<int, int>{};
    for (int i = 0; i < 10; i++) {
        tempMap[i] = factorial(i);
    }
    tempMap.clear();
}

void exmpleMapWithAllocator() {
    auto tempMap = std::map<int, int, std::less<int>, CustomAllocator<std::pair<const int, int>, 10>>{};
    for (int i = 0; i < 10; i++) {
        tempMap[i] = factorial(i);
    }
    for (const auto &item : tempMap) {
        std::cout << item.first << " " << item.second << std::endl;
    }
    tempMap.clear();
}

void exmpleContainer() {
    auto tempContainer = container<int>{};
    for (int i = 0; i < 10; i++) {
        tempContainer.push(factorial(i));
    }
    tempContainer.clear();
}

void exmpleContainerWithAllocator() {
    auto tempContainer = container<int, CustomAllocator<element<int>, 10>>{};
    for (int i = 0; i < 10; i++) {
        tempContainer.push((i));
    }
    for (const auto &item : tempContainer) {
        std::cout << item.data << std::endl;
    }
    tempContainer.clear();
}

int main() {
    try {
        exmpleMap();
        exmpleMapWithAllocator();
        exmpleContainer();
        exmpleContainerWithAllocator();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
