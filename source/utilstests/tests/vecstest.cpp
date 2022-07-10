#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <utils/geometry/vec2.h>
#include <utils/geometry/vec3.h>

using namespace puma;

TEST( Geometry, Vecs )
{
    Vec2 vec2;
    Vec3 vec3;

    vec2 = { 5.0f, 4.0f };
    vec3 = { 4.0f, 3.0f, 2.0f };

    vec2 = vec2 - Vec2( { 1.0f, 1.0f } );

    EXPECT_FLOAT_EQ( vec2.x, 4.0f );
    EXPECT_FLOAT_EQ( vec2.y, 3.0f );

    vec3 = vec3 - Vec3( { 1.0f, 1.0f, 1.0f } );

    EXPECT_FLOAT_EQ( vec3.x, 3.0f );
    EXPECT_FLOAT_EQ( vec3.y, 2.0f );
    EXPECT_FLOAT_EQ( vec3.z, 1.0f );

    EXPECT_FLOAT_EQ( vec2.normalize().length(), 1.0f );
    EXPECT_FLOAT_EQ( vec3.normalize().length(), 1.0f );
}