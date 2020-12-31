#pragma once

#include <utils/geometry/shapes/cirlce.h>
#include <utils/geometry/shapes/rectangle.h>

namespace puma
{
    enum class ShapeType
    {
        Circle,
        Rectangle,
    };

    union Shape
    {
        Shape() {}
        Circle circle = {};
        Rectangle rectangle;
    };
}