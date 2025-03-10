#pragma ONCE

#include <cstddef>
#include <atomic>
#include <new>

//static_assert(sizeof(unsigned long) == 8, "unsigned long is not 8 bytes!");



namespace nimble {
    template<typename T, size_t CAPACITY> class queue {
    private:
        std::atomic<unsigned long> first
            __attribute__((__aligned__(128)));

        std::atomic<unsigned long> last
            __attribute__((__aligned__(128)));

        std::atomic<unsigned long> last_inflight
            __attribute__((__aligned__(128)));


        T buffer[CAPACITY];

    public:
        queue() : first(0), last(0), last_inflight(0) {
        }

        size_t capacity() const {
            return CAPACITY;
        }

        size_t size() const {
            unsigned long l = last;
            unsigned long f = first;

            return l - f;
        }

        bool try_push(const T& val) {
            unsigned long l = last_inflight;
            unsigned long f = first;
            unsigned long s_inflight = l - f;

            if (s_inflight == CAPACITY) return false;
            if (s_inflight > CAPACITY) throw std::range_error("Overflow, shouldn't happen");

            unsigned long place_at = last_inflight.fetch_add(1);
            unsigned long place_at_mod = place_at % CAPACITY;
        }

    
    };
}