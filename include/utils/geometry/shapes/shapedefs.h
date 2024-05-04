#pragma once

#include <utils/numerictypes.h>

namespace puma
{
    enum class ShapeType
    {
        Chain,
        Circle,
        Polygon,
    };

    constexpr u32 kMaxVerticesPerShape = 8;
}