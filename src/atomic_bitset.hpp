#pragma once

#include <memory>
#include <iostream>
#include <numeric>
#include "ios_guard.hpp"

#include "atomic_bitset_calculator.hpp"

namespace nimble {

template <
    size_t STRIPES = 16, // TODO: function of cache size
    typename atomic_type_t = uint32_t,
    typename Allocator = std::allocator<std::atomic<atomic_type_t>>
    >
class atomic_bitset
{
    using calculator_t = atomic_bitset_calculator<STRIPES, atomic_type_t>;
    using stripe_t = std::vector<std::atomic<atomic_type_t>, Allocator>;

public:
    atomic_bitset(const atomic_bitset& copy) = delete;
    void operator=(const atomic_bitset& copy) = delete;

    atomic_bitset(size_t size) : size(size)
    {
        for (auto &s : stripes) {
            s = stripe_t(calculator_t::calculate_pages_per_stripe(size));
        }
    }

private:

    calculator_t::bit_index calculate_index(size_t bit_number) const {
        // TODO: DEBUG only ?
        if (bit_number < 0 || bit_number >= size) throw std::out_of_range("Out of range");

        auto ret = calculator_t::calculate_index(bit_number);

#ifdef DEBUG
        if (ret.stripe >= calculator_t::number_of_stripes()) throw std::exception();
        if (ret.mask == 0) throw std::exception();
        if (ret.page >= stripes[ret.stripe].size()) throw std::exception();
#endif

        return ret;
    }

public:
    public:

        bool get(size_t index) const {
            auto pb = calculate_index(index);
            const auto& page = stripes[pb.stripe][pb.page];
            return (page & pb.mask) != 0;
        }

        void set(size_t index) {
            auto pb = calculate_index(index);
            stripes[pb.stripe][pb.page] |= pb.mask;
        }

        void clear(size_t index) {
            auto pb = calculate_index(index);
            stripes[pb.stripe][pb.page] &= ~pb.mask;
        }
private:
    size_t size;

    std::array<stripe_t, STRIPES> stripes;
};

}