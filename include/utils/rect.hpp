#pragma once
#include "raylib.h"

namespace utils {

inline Vector2 rect_center(Rectangle r) {
    return { r.x + r.width * 0.5f, r.y + r.height * 0.5f };
}

inline Rectangle rect_from_center(Vector2 c, float w, float h) {
    return { c.x - w * 0.5f, c.y - h * 0.5f, w, h };
}

// dx/dy positiu fa el rect més gran, negatiu el fa més petit
inline Rectangle rect_inflate(Rectangle r, float dx, float dy) {
    return { r.x - dx, r.y - dy, r.width + 2.0f * dx, r.height + 2.0f * dy };
}

} // namespace utils
