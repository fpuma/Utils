#pragma once

#include <utils/numerictypes.h>

#include <math.h>

namespace puma
{

    struct Vec2i
    {
        Vec2i() {}

        Vec2i(int _x, int _y)
            : x(_x)
            , y(_y)
        {
        }

        Vec2i operator + (const Vec2i& _other) const
        {
            return { x + _other.x, y + _other.y };
        }

        Vec2i operator - (const Vec2i& _other) const
        {
            return { x - _other.x, y - _other.y };
        }

        Vec2i operator - ()
        {
            return { -x, -y };
        }

        Vec2i operator * (int _scale) const
        {
            return { x * _scale, y * _scale };
        }

        Vec2i& operator = (const Vec2i& _other)
        {
            x = _other.x;
            y = _other.y;
            return *this;
        }

        float dot( const Vec2i& _other ) const
        {
            return (static_cast<float>(x) * static_cast<float>(_other.x)) + (static_cast<float>(y) * static_cast<float>(_other.y));
        }

        float squaredLength() const
        {
            float fx = static_cast<float>(x);
	        float fy = static_cast<float>(y);
            return (fx * fx) + (fy * fy);
        }

        float length() const
        {
            return sqrtf(squaredLength());
        }

        u32 manhattanLength() const
        {
            u32 ux = x < 0 ? -x : x;
            u32 uy = y < 0 ? -y : y;
            return ux + uy;
        }

        int x = 0;
        int y = 0;
    };
}