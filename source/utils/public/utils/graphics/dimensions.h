#pragma once

#include <utils/numerictypes.h>

namespace puma
{
    struct Extent
    {
        s32 width;
        s32 height;
        s32 xPos;
        s32 yPos;
    };

    struct Rect
    {
        s32 width = 0;
        s32 height = 0;
    };
}