#ifndef BITS_H
#define BITS_H

#include <stdint.h>

static inline int lzcnt64(uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_clzll(x);
#else
    int count = 0;
    for (int i = 63; i >= 0; i--) {
        if ((x >> i) & 1ULL) break;
        count++;
    }
    return count;
#endif
}

#endif 