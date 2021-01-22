#pragma once

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
}