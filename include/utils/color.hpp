#pragma once
#include "raylib.h"
#include <cstdint>

namespace utils {

// format 0xRRGGBBAA
inline Color from_hex(uint32_t hex) {
    return {
        (unsigned char)((hex >> 24) & 0xFF),
        (unsigned char)((hex >> 16) & 0xFF),
        (unsigned char)((hex >> 8)  & 0xFF),
        (unsigned char)( hex        & 0xFF)
    };
}

// a de 0 a 1
inline Color with_alpha(Color c, float a) {
    if (a < 0.0f) a = 0.0f;
    if (a > 1.0f) a = 1.0f;
    c.a = (unsigned char)(a * 255.0f);
    return c;
}

inline Color color_lerp(Color a, Color b, float t) {
    return {
        (unsigned char)(a.r + (b.r - a.r) * t),
        (unsigned char)(a.g + (b.g - a.g) * t),
        (unsigned char)(a.b + (b.b - a.b) * t),
        (unsigned char)(a.a + (b.a - a.a) * t)
    };
}

} // namespace utils
