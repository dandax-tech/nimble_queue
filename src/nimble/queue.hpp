#pragma once

#include <atomic>
#include <memory>
#include <vector>
#include <thread>

#include "atomic_bitset.hpp"

namespace nimble {

class first_last {
private:
    // TODO: hide in a sub class
    std::atomic<uint64_t> first_
    __attribute__((__aligned__(128)));

    std::atomic<uint64_t> last_
    __attribute__((__aligned__(128)));

public:
    first_last() : first_(0), last_(0) {}

    void grab(uint64_t &first, uint64_t &last) const {
        // preserve order
        first = first_;
        last = last_;
    }

    uint64_t advance_last() {
        return last_.fetch_add(1);
    }

    bool advance_first(uint64_t first) {
        return first_.compare_exchange_strong(first, first + 1);
    }
};

template<typename T>
class queue {

    first_last first_last;

    std::vector<T> buffer;

    atomic_bitset<> buffer_ready;

#ifdef DEBUG
    uint64_t again1 = 0;
    uint64_t again2 = 0;
#endif

    public:
        queue(size_t capacity)
            : buffer(capacity), buffer_ready(capacity)
        {
        }

        size_t capacity() const {
            return buffer.capacity();
        }

        size_t size() const {
            uint64_t first, last;
            first_last.grab(first, last);

            return last - first;
        }

        bool try_push(const T& val) {
            uint64_t first, last;
            first_last.grab(first, last);

            size_t cap = capacity();
            if (last - first >= cap) return false; // no room

            uint64_t place_at = first_last.advance_last() % cap;
            buffer[place_at] = val;
            buffer_ready.set(place_at);

            return true;
        }

        bool try_pop(T& out) {
        again:
            uint64_t first, last;
            first_last.grab(first, last);
            if (last == first) return false; // empty

            auto cap = capacity();
            auto at = first % cap;

            // still writing ?
            if (false == buffer_ready.get(at)) {
#ifdef DEBUG
                again1++;
#endif
                goto again;
            } 

            // did someone else read this
            if (false == first_last.advance_first(first)) {
#ifdef DEBUG
                again2++;
#endif
                goto again;
            }

            // we got it!
            out = buffer[at];
            buffer_ready.clear(at);
            return true;
        }

        void dump_stats(std::ostream &out) const {
            uint64_t first, last;
            first_last.grab(first, last);
            out << "reads=" << first
                << " writes=" << last
                << " q size=" << (last-first)
#ifdef DEBUG
                << " again1=" << again1
                << " again2=" << again2
#endif
                << std::endl;
        }

        void dump(std::ostream &out) const {
            uint64_t first, last;
            first_last.grab(first, last);

            uint64_t first_at = first % capacity();
            uint64_t last_at = last % capacity();

            for (int i = 0; i < buffer.size(); i++) {
                if (i == last_at)
                    out << " ] ";
                if (i == first_at)
                    out << " [ ";
                
                out << i << ' ';
                if (i % 10 == 9) {
                    out << std::endl;
                }
            }

            out << std::endl;
            buffer_ready.dump(out);
            dump_stats(out);
        }
    };
}