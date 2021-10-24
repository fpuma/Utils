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

        static Vec2 rotatePoint2D( const Vec2& _point, const float& _degrees )
        {
            float radians = degreesToRadians( _degrees );
            return { (_point.x * std::cosf( radians )) - (_point.y * std::sinf( radians )),
                     (_point.y * std::cosf( radians )) + (_point.x * std::sinf( radians )) };
        }
    };
}