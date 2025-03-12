#include <nimble/atomic_bitset_calculator.hpp>
#include "common.hpp"

using calculator = nimble::atomic_bitset_calculator<3, uint8_t>;

int main() {
    typedef calculator::bit_index  bit_index;

    TestStatementEquals([](){ return calculator::bits_in_one_atom(); }, 8, "Bits in atom");
    TestStatementEquals([](){ return calculator::bits_in_one_stripe(); }, 8 * 3, "Bits in stripe");
    TestStatementEquals([](){ return calculator::calculate_index(0); }, bit_index{.page = 0, .mask = (1 << 0),.stripe = 0}, "Index 0");
    TestStatementEquals([](){ return calculator::calculate_index(1); }, bit_index{.page = 0, .mask = (1 << 0),.stripe = 1}, "Index 1");
    TestStatementEquals([](){ return calculator::calculate_index(2); }, bit_index{.page = 0, .mask = (1 << 0),.stripe = 2}, "Index 2");
    TestStatementEquals([](){ return calculator::calculate_index(3); }, bit_index{.page = 0, .mask = (1 << 1),.stripe = 0}, "Index 3");
    TestStatementEquals([](){ return calculator::calculate_index(4); }, bit_index{.page = 0, .mask = (1 << 1),.stripe = 1}, "Index 4");

    TestStatementEquals([](){ return calculator::calculate_index(23); }, bit_index{.page = 0, .mask = (1 << 7),.stripe = 2}, "Index 23");
    TestStatementEquals([](){ return calculator::calculate_index(24); }, bit_index{.page = 1, .mask = (1 << 0),.stripe = 0}, "Index 24");


    TestStatementEquals([](){return calculator::calculate_pages_per_stripe(0);}, 0, "Size 0");
    TestStatementEquals([](){return calculator::calculate_pages_per_stripe(1);}, 1, "Size 1");
    TestStatementEquals([](){return calculator::calculate_pages_per_stripe(23);}, 1, "Size 23");
    TestStatementEquals([](){return calculator::calculate_pages_per_stripe(24);}, 1, "Size 24");
    TestStatementEquals([](){return calculator::calculate_pages_per_stripe(25);}, 2, "Size 25");

    std::cout << "PASSED: " << passes << " FAILED: " << fails << std::endl;

}
