#pragma once

#include <iostream>


class ios_hexguard {
public:
    ios_hexguard(std::ostream &out) : out(out) {
        originalFlags = out.flags();
    }

    ~ios_hexguard() {
        out.flags(originalFlags);
    }

private:
    std::ostream &out;
    std::ios_base::fmtflags originalFlags;
};