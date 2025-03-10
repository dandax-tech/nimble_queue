#include <iostream>
#include <format>
#include <unordered_set>
#include <cmath>

#define private public

#include "atomic_bitset.hpp"
#include "tester.hpp"

auto get_new_bitset(size_t size)
{
    return nimble::atomic_bitset(size);
}


int main() {
    nimble::atomic_bitset bitset = get_new_bitset(10000);

    for (int i = 0; i < 100; i++) {
        TestStatementEquals([&](){ return bitset.get(i); }, false, std::format("step1 {}", i) );
        bitset.set(i, true);
        TestStatementEquals([&](){ return bitset.get(i); }, true, std::format("step2 {}", i) );
        bitset.set(i, false);
        TestStatementEquals([&](){ return bitset.get(i); }, false, std::format("step3 {}", i) );
        bitset[i] = true;
        TestStatementEquals([&](){ return bitset[i]; }, true, std::format("step4 {}", i) );
        bitset[i] = false;
    }

    TestStatementThrows<std::exception>([&](){ return bitset.get(10000); }, "Out of bounds");

    std::cout << "PASSED: " << passes << " FAILED: " << fails << std::endl;
}