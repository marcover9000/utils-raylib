#pragma once
#include <cmath>

namespace utils {

// raylib ja porta Lerp/Clamp; aquí només el que falta

inline float map_range(float v, float a1, float b1, float a2, float b2) {
    return a2 + (v - a1) * (b2 - a2) / (b1 - a1);
}

// embolica v dins [min, max)
inline float wrap(float v, float min, float max) {
    float r = max - min;
    return min + std::fmod(std::fmod(v - min, r) + r, r);
}

inline int sign(float v) {
    return (v > 0.0f) - (v < 0.0f);
}

inline bool approx_equal(float a, float b, float eps = 1e-5f) {
    return std::fabs(a - b) < eps;
}

// easings — t de 0 a 1
inline float ease_in_quad(float t)     { return t * t; }
inline float ease_out_quad(float t)    { return 1.0f - (1.0f - t) * (1.0f - t); }
inline float ease_in_out_quad(float t) { return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) * 0.5f; }

inline float ease_in_cubic(float t)    { return t * t * t; }
inline float ease_out_cubic(float t)   { float u = 1.0f - t; return 1.0f - u * u * u; }
inline float ease_in_out_cubic(float t){ return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) * 0.5f; }

inline float ease_in_sine(float t)     { return 1.0f - std::cos((t * 3.14159265f) * 0.5f); }
inline float ease_out_sine(float t)    { return std::sin((t * 3.14159265f) * 0.5f); }
inline float ease_in_out_sine(float t) { return -(std::cos(3.14159265f * t) - 1.0f) * 0.5f; }

// bota al final
inline float ease_out_bounce(float t) {
    const float n1 = 7.5625f;
    const float d1 = 2.75f;
    if (t < 1.0f / d1)       { return n1 * t * t; }
    else if (t < 2.0f / d1)  { t -= 1.5f / d1;   return n1 * t * t + 0.75f; }
    else if (t < 2.5f / d1)  { t -= 2.25f / d1;  return n1 * t * t + 0.9375f; }
    else                     { t -= 2.625f / d1; return n1 * t * t + 0.984375f; }
}
inline float ease_in_bounce(float t) {
    return 1.0f - ease_out_bounce(1.0f - t);
}

// goma elàstica al final
inline float ease_out_elastic(float t) {
    const float c4 = (2.0f * 3.14159265f) / 3.0f;
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

// passa una mica del valor i torna (overshoot)
inline float ease_in_back(float t) {
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    return c3 * t * t * t - c1 * t * t;
}
inline float ease_out_back(float t) {
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    float u = t - 1.0f;
    return 1.0f + c3 * u * u * u + c1 * u * u;
}

} // namespace utils
