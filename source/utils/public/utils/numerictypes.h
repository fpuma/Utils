#pragma once

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

    constexpr u8  kMaxU8  = (u8) ~0x0;
    constexpr u16 kMaxU16 = (u16)~0x0;
    constexpr u32 kMaxU32 = (u32)~0x0;
    constexpr u64 kMaxU64 = (u64)~0x0;

    constexpr s8  kMaxS8  = (s8) ~0x0;
    constexpr s16 kMaxS16 = (s16)~0x0;
    constexpr s32 kMaxS32 = (s32)~0x0;
    constexpr s64 kMaxS64 = (s64)~0x0;
}