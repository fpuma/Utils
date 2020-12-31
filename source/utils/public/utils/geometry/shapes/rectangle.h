#pragma once

#include <utils/geometry/vec2.h>

namespace puma
{
    struct Rectangle
    {
        Vec2 upperBoundary;
        Vec2 lowerBoundary;
    };
}