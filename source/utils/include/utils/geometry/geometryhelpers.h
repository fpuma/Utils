#pragma once

#include "vec2.h"
#include <cmath>

namespace puma
{
    constexpr float PI = 3.1416f;

    class GeometryHelpers
    {
    public:

        static float degreesToRadians( float _degrees )
        {
            return (_degrees * PI) / 180.0f;
        }

        static float radiansToDegrees( float _radians )
        {
            return (_radians * 180.0f) / PI;
        }

        static Vec2 rotatePoint2D( const Vec2& _point, const float& _radians )
        {
            return { (_point.x * std::cosf( _radians )) - (_point.y * std::sinf( _radians )),
                     (_point.y * std::cosf( _radians )) + (_point.x * std::sinf( _radians )) };
        }
    };
}