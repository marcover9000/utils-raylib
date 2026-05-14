#pragma once
#include "raylib.h"
#include <vector>
#include <cstddef>

namespace utils {

inline void rand_seed(unsigned int seed) {
    SetRandomSeed(seed);
}

inline int rand_int(int min, int max) {
    return GetRandomValue(min, max);
}

// float dins [min, max]
inline float rand_float(float min = 0.0f, float max = 1.0f) {
    return min + (max - min) * (GetRandomValue(0, 1000000) / 1000000.0f);
}

// true amb probabilitat p (0..1)
inline bool rand_chance(float p) {
    return rand_float(0.0f, 1.0f) < p;
}

inline bool rand_bool() {
    return GetRandomValue(0, 1) == 1;
}

// agafa un element a l'atzar
template <typename T>
inline T& rand_pick(std::vector<T>& v) {
    return v[GetRandomValue(0, (int)v.size() - 1)];
}

template <typename T>
inline const T& rand_pick(const std::vector<T>& v) {
    return v[GetRandomValue(0, (int)v.size() - 1)];
}

// escull segons pesos (weights[i] és el pes relatiu de items[i])
template <typename T>
inline const T& rand_pick_weighted(const std::vector<T>& items, const std::vector<float>& weights) {
    float total = 0.0f;
    for (float w : weights) total += w;
    float r = rand_float(0.0f, total);
    float acc = 0.0f;
    for (std::size_t i = 0; i < items.size(); ++i) {
        acc += weights[i];
        if (r < acc) return items[i];
    }
    return items.back();
}

} // namespace utils
