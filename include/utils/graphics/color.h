#pragma once

namespace puma
{
    struct Color
    {
        unsigned char red = 0;
        unsigned char green = 0;
        unsigned char blue = 0;
        unsigned char alpha = 255;

        static Color Red() { return { 255,   0,   0, 255 }; }
        static Color Green() { return { 0, 255,   0, 255 }; }
        static Color Blue() { return { 0,   0, 255, 255 }; }
        static Color Black() { return { 0,   0,   0, 255 }; }
        static Color White() { return { 255, 255, 255, 255 }; }

        bool operator == ( const Color& _other ) const
        {
            return  red == _other.red &&
                green == _other.green &&
                blue == _other.blue &&
                alpha == _other.alpha;
        }
    };
}