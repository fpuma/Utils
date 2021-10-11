#pragma once

#include <utils/containers/containedvector.h>
#include <utils/geometry/shapes/shapedefs.h>
#include <utils/geometry/vec2.h>
#include <utils/numerictypes.h>

namespace puma
{
    struct Extent
    {
        s32 width = 0;
        s32 height = 0;
        s32 xPos = 0;
        s32 yPos = 0;
    };

    struct Rect
    {
        s32 width = 0;
        s32 height = 0;
    };

    struct ScreenPos
    {
        s32 xCoord = 0;
        s32 yCoord = 0;
    };

    struct Rectangle
    {
        Vec2 upperBoundary;
        Vec2 lowerBoundary;
    };

    using ShapeVerticesList = ContainedVector<Vec2, kMaxVerticesPerShape>;
    using ShapeScreenPointsList = ContainedVector<ScreenPos, kMaxVerticesPerShape>;
}