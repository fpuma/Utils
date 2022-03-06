#pragma once

#include <math.h>

namespace puma
{

    struct Vec2
    {
        Vec2 operator + ( const Vec2& _other ) const
        {
            return { x + _other.x, y + _other.y };
        }

        Vec2 operator - ( const Vec2& _other ) const
        {
            return { x - _other.x, y - _other.y };
        }

        Vec2 operator - ()
        {
            return { -x, -y };
        }

        Vec2 operator * ( float _scale ) const
        {
            return { x * _scale, y * _scale };
        }

        Vec2& operator = ( const Vec2& _other )
        {
            x = _other.x;
            y = _other.y;
            return *this;
        }

        float dot( const Vec2& _other ) const
        {
            return (x * _other.x) + (y * _other.y);
        }

        float squaredLength() const
        {
            return (x * x) + (y * y);
        }

        float length() const
        {
            return sqrtf( squaredLength() );
        }

        Vec2 normalize() const
        {
            float vecLength = length();
            return { x / vecLength, y / vecLength };
        }

        float x = 0.0f;
        float y = 0.0f;
    };
}