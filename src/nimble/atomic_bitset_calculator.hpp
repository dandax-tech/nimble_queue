#pragma once

#include <memory>
#include <format>
#include "ios_guard.hpp"

namespace nimble {

template <int STRIPES, typename ATOM_T>
struct atomic_bitset_calculator {  
    constexpr static size_t bits_in_one_atom() {
        return sizeof(ATOM_T) * 8;
    }

    constexpr static size_t number_of_stripes() {
        return STRIPES;
    }

    constexpr static size_t bits_in_one_stripe() {
        return bits_in_one_atom() * number_of_stripes();
    }

    constexpr static size_t calculate_pages_per_stripe(size_t size) {
        size_t one_stripe_size = (size + (bits_in_one_stripe()) - 1) / bits_in_one_stripe();
        return one_stripe_size;
    }

    struct bit_index {
        size_t page;
        ATOM_T mask;
        int stripe;

        bool operator==(const bit_index &rhs) {
            return page == rhs.page && stripe == rhs.stripe && mask == rhs.mask;
        }
    
        friend std::ostream &operator<<(std::ostream& out, const bit_index &value) {
            auto _ = ios_hexguard(out);
            return out << "{P" << std::dec << value.page << ",M" << std::hex << (uint64_t)value.mask << ",S" << std::dec << value.stripe << "}";
        }
    };

    constexpr bit_index static calculate_index(size_t bit_number) {
        size_t page = bit_number / bits_in_one_stripe();
        int bit = (bit_number / number_of_stripes()) % bits_in_one_atom();
        int stripe = bit_number % number_of_stripes();
        ATOM_T mask = (ATOM_T)1 << bit;

        return  { .page = page, .mask = mask, .stripe = stripe};
    }
};

}