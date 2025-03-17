#include <iostream>
#include <ranges>

#include <nimble/queue.hpp>

#include "common.hpp"

void usage(const char* prog)
{
    std::cerr << "Usage: " << prog << " <size> <writes> <reads>" << std::endl;
    exit(1);
}

void error(const char* error, int arg)
{
    std::cerr << error << ' ' << arg << std::endl;
    exit(1);
}

int main(int argc, const char **argv)
{
    if (argc != 4) usage(argv[0]);

    int size = atoi(argv[1]);
    int writes = atoi(argv[2]);
    int reads = atoi(argv[3]);

    if (writes < 0 || reads < 0 || size <= 0 || reads > writes || reads - writes > size) usage(argv[0]);

    nimble::queue<int> q(size);

    for (int w : std::views::iota(0, writes)) {
        if (false == q.try_push(w)) error("Can't push", w);
    }

    for (int r : std::views::iota(0, reads)) {
        int v;
        if (false == q.try_pop(v)) error("Can't pop", r);
    }

    q.dump(std::cout);

    exit(0);
}