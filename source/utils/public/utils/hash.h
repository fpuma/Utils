#pragma once

#include <functional>

namespace puma
{
    //Based on Boost 1.75.0 boost/container_hash/hash.hpp

    template <class T>
    inline void hashCombine( std::size_t& seed, const T& v )
    {
        std::hash<T> hasher;
        seed ^= hasher( v ) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
}