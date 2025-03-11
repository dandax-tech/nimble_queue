#include <memory>
#include <iostream>
#include <numeric>
#include "ios_guard.hpp"

#include "atomic_bitset_calculator.hpp"

namespace nimble {

template <
    typename atomic_type_t = uint32_t,
    typename Allocator = std::allocator<std::atomic<atomic_type_t>>
    >
class atomic_bitset
{
    using calculator_t = atomic_bitset_calculator<atomic_type_t>;
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
    class atomic_bitset_accessor {
        atomic_bitset &bitset;
        size_t index;

    public:    
        // TODO: make it take a page/mask instead of index
        atomic_bitset_accessor(atomic_bitset &bitset_, size_t index_)
            : bitset(bitset_)
            , index(index_)
             {}

        atomic_bitset_accessor operator=(bool value) {
            bitset.set(index, value);
            return *this;
        }

        operator bool() const {
            return bitset.get(index);
        }
    };

    public:

        bool get(size_t index) const {
            auto pb = calculate_index(index);
            const auto& page = stripes[pb.stripe][pb.page];
            return (page & pb.mask) != 0;
        }

        void set(size_t index, bool value) {
            auto pb = calculate_index(index);
            if (value) {
                stripes[pb.stripe][pb.page] |= pb.mask;
            } else {
                stripes[pb.stripe][pb.page] &= ~pb.mask;
            }          
        }

        atomic_bitset_accessor operator[](size_t index) {
            return atomic_bitset_accessor(*this, index);
        }

        bool operator[](size_t index) const {
            return this->get(index);
        }

private:
    size_t size;

    std::array<stripe_t, calculator_t::number_of_stripes()> stripes;
};

}