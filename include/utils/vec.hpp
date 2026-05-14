#pragma once
#include "raylib.h"
#include <cmath>

// operadors per Vector2 al namespace global (Vector2 ve de raylib, que és C)

inline Vector2 operator+(Vector2 a, Vector2 b) { return { a.x + b.x, a.y + b.y }; }
inline Vector2 operator-(Vector2 a, Vector2 b) { return { a.x - b.x, a.y - b.y }; }
inline Vector2 operator-(Vector2 v)            { return { -v.x, -v.y }; }
inline Vector2 operator*(Vector2 v, float s)   { return { v.x * s, v.y * s }; }
inline Vector2 operator*(float s, Vector2 v)   { return { v.x * s, v.y * s }; }
inline Vector2 operator/(Vector2 v, float s)   { return { v.x / s, v.y / s }; }

inline Vector2& operator+=(Vector2& a, Vector2 b) { a.x += b.x; a.y += b.y; return a; }
inline Vector2& operator-=(Vector2& a, Vector2 b) { a.x -= b.x; a.y -= b.y; return a; }
inline Vector2& operator*=(Vector2& v, float s)   { v.x *= s; v.y *= s; return v; }
inline Vector2& operator/=(Vector2& v, float s)   { v.x /= s; v.y /= s; return v; }

inline bool operator==(Vector2 a, Vector2 b) { return a.x == b.x && a.y == b.y; }
inline bool operator!=(Vector2 a, Vector2 b) { return !(a == b); }

namespace utils {

// versions sense sqrt — més ràpid per comparar distàncies
inline float length_sq(Vector2 v) { return v.x * v.x + v.y * v.y; }
inline float dist_sq(Vector2 a, Vector2 b) {
    float dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// àlies curts amb sqrt (raylib ja en té versions, però aquí tens-ho a mà)
inline float length(Vector2 v) { return std::sqrt(length_sq(v)); }
inline float dist(Vector2 a, Vector2 b) { return std::sqrt(dist_sq(a, b)); }

} // namespace utils
