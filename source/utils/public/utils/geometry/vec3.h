#pragma once

namespace puma
{

    class Vec3
    {
    public: 
        Vec3() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
        
        Vec3( float _x, float _y )
            : x( _x )
            , y( _y )
            , z( 0.0f ) {}

        Vec3( float _x, float _y, float _z )
            : x( _x )
            , y( _y ) 
            , z( _z ){}
                
        Vec3 operator + ( const Vec3& _other ) const
        {
            return Vec3( x + _other.x, y + _other.y, z + _other.z );
        }

        Vec3 operator - ( const Vec3& _other ) const
        {
            return Vec3( x - _other.x, y - _other.y, z - _other.z );
        }

        Vec3 operator - ()
        {
            return Vec3( -x, -y, -z );
        }

        Vec3 operator * ( float _scale ) const
        {
            return Vec3( x * _scale, y * _scale, z * _scale );
        }

        Vec3& operator = ( const Vec3& _other )
        {
            x = _other.x;
            y = _other.y;
            z = _other.z;
            return *this;
        }

        float dot( const Vec3& _other ) const
        {
            return (x * _other.x) + (y * _other.y) + (z * _other.z);
        }

        float squaredLength() const
        {
            return (x * x) + (y * y) + (z * z);
        }

        float x = 0;
        float y = 0;
        float z = 0;
    };
}