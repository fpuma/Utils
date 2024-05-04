#pragma once

#include <limits>

namespace puma
{
    using u8    = unsigned char;
    using u16   = unsigned short;
    using u32   = unsigned int;
    using u64   = unsigned long long;

    using s8    = char;
    using s16   = short;
    using s32   = int;
    using s64   = long long;

    constexpr u8  kMaxU8  = std::numeric_limits<u8>::max();
    constexpr u16 kMaxU16 = std::numeric_limits<u16>::max();
    constexpr u32 kMaxU32 = std::numeric_limits<u32>::max();
    constexpr u64 kMaxU64 = std::numeric_limits<u64>::max();

    constexpr s8  kMaxS8  = std::numeric_limits<s8>::max();
    constexpr s16 kMaxS16 = std::numeric_limits<s16>::max();
    constexpr s32 kMaxS32 = std::numeric_limits<s32>::max();
    constexpr s64 kMaxS64 = std::numeric_limits<s64>::max();
}