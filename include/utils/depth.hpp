#pragma once
// sistema de profunditat semi-3D estil FF Tactics: grid isomètric/dimetric
// amb alçada per tile i depth-sort per pintar sprites en l'ordre correcte.
//
// EN DESENVOLUPAMENT — no inclòs a utils.hpp encara.

#include "raylib.h"
#include "math.hpp"
#include "color.hpp"
#include "vec.hpp"
#include <vector>
#include <functional>
#include <algorithm>

namespace utils {

// posició dins el grid: x,y son coords de tile (float per moviments suaus),
// z és l'alçada en "nivells" (cada nivell = height_step píxels).
struct TilePos {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

// projecció isomètrica configurable.
// per dimetric 2:1 (FF Tactics-ish): tile_w = 2 * tile_h.
struct IsoProjection {
    float tile_w     = 64.0f;   // amplada del rombo del tile
    float tile_h     = 32.0f;   // alçada del rombo del tile (eix Y projectat)
    float height_step = 16.0f;  // píxels que puja cada nivell de z
    Vector2 origin   = { 0.0f, 0.0f }; // posició a pantalla de (0,0,0)
};

// tile → pantalla
inline Vector2 tile_to_screen(float tx, float ty, float z, IsoProjection p) {
    return {
        p.origin.x + (tx - ty) * (p.tile_w * 0.5f),
        p.origin.y + (tx + ty) * (p.tile_h * 0.5f) - z * p.height_step
    };
}
inline Vector2 tile_to_screen(TilePos t, IsoProjection p) {
    return tile_to_screen(t.x, t.y, t.z, p);
}

// pantalla → tile (assumeix z = 0, útil per saber sobre quin tile està el ratolí)
inline Vector2 screen_to_tile(Vector2 screen, IsoProjection p) {
    float sx = (screen.x - p.origin.x) / (p.tile_w * 0.5f);
    float sy = (screen.y - p.origin.y) / (p.tile_h * 0.5f);
    return { (sx + sy) * 0.5f, (sy - sx) * 0.5f };
}

// clau d'ordenació: més gran = es pinta després (queda a sobre)
inline float depth_key(float tx, float ty, float z) {
    return tx + ty + z * 0.5f;
}
inline float depth_key(TilePos t) { return depth_key(t.x, t.y, t.z); }

// cua de drawables ordenats per profunditat.
// uses: push elements durant l'update, flush() al final del frame.
class DepthBuffer {
    struct Item { float key; std::function<void()> draw; };
    std::vector<Item> items;
public:
    void push(float key, std::function<void()> fn) {
        items.push_back({ key, std::move(fn) });
    }
    void push_at(float tx, float ty, float z, std::function<void()> fn) {
        push(depth_key(tx, ty, z), std::move(fn));
    }
    void push_at(TilePos t, std::function<void()> fn) {
        push(depth_key(t), std::move(fn));
    }
    void flush() {
        std::sort(items.begin(), items.end(),
                  [](const Item& a, const Item& b){ return a.key < b.key; });
        for (auto& i : items) i.draw();
        items.clear();
    }
    void clear() { items.clear(); }
    size_t size() const { return items.size(); }
};

// les 4 cantonades de la cara superior d'un tile
struct TileQuad {
    Vector2 back;   // (tx,   ty)
    Vector2 right;  // (tx+1, ty)
    Vector2 front;  // (tx+1, ty+1)
    Vector2 left;   // (tx,   ty+1)
};
inline TileQuad tile_top_quad(float tx, float ty, float z, IsoProjection p) {
    return {
        tile_to_screen(tx,        ty,        z, p),
        tile_to_screen(tx + 1.0f, ty,        z, p),
        tile_to_screen(tx + 1.0f, ty + 1.0f, z, p),
        tile_to_screen(tx,        ty + 1.0f, z, p)
    };
}

// cub-tile: cara superior + dues cares laterals visibles (la cara nord-oest
// i la cara nord-est queden ocultes per la projecció)
inline void draw_tile_cube(float tx, float ty, float z_top, float z_bottom,
                           IsoProjection p,
                           Color top, Color left_face, Color right_face) {
    TileQuad t = tile_top_quad(tx, ty, z_top, p);
    Vector2 bl = tile_to_screen(tx,        ty + 1.0f, z_bottom, p);
    Vector2 bf = tile_to_screen(tx + 1.0f, ty + 1.0f, z_bottom, p);
    Vector2 br = tile_to_screen(tx + 1.0f, ty,        z_bottom, p);

    // cara superior (rombo)
    DrawTriangle(t.back, t.left,  t.front, top);
    DrawTriangle(t.back, t.front, t.right, top);

    // cara esquerra (mira cap a SW)
    DrawTriangle(t.left,  bl, bf,     left_face);
    DrawTriangle(t.left,  bf, t.front, left_face);

    // cara dreta (mira cap a SE)
    DrawTriangle(t.front, bf, br,     right_face);
    DrawTriangle(t.front, br, t.right, right_face);
}

// wireframe del cub, útil per debug del grid
inline void draw_tile_wire(float tx, float ty, float z_top, float z_bottom,
                           IsoProjection p, Color c) {
    TileQuad t = tile_top_quad(tx, ty, z_top, p);
    Vector2 bb = tile_to_screen(tx,        ty,        z_bottom, p);
    Vector2 bl = tile_to_screen(tx,        ty + 1.0f, z_bottom, p);
    Vector2 bf = tile_to_screen(tx + 1.0f, ty + 1.0f, z_bottom, p);
    Vector2 br = tile_to_screen(tx + 1.0f, ty,        z_bottom, p);

    DrawLineV(t.back, t.right, c); DrawLineV(t.right, t.front, c);
    DrawLineV(t.front, t.left, c); DrawLineV(t.left, t.back, c);
    DrawLineV(t.back, bb, c); DrawLineV(t.right, br, c);
    DrawLineV(t.front, bf, c); DrawLineV(t.left, bl, c);
    DrawLineV(bb, br, c); DrawLineV(br, bf, c);
    DrawLineV(bf, bl, c); DrawLineV(bl, bb, c);
}

// pinta el contorn de la cara superior d'un tile (ressaltar tile sota el ratolí)
inline void draw_tile_top_outline(float tx, float ty, float z, IsoProjection p, Color c) {
    TileQuad q = tile_top_quad(tx, ty, z, p);
    DrawLineV(q.back, q.right, c);
    DrawLineV(q.right, q.front, c);
    DrawLineV(q.front, q.left, c);
    DrawLineV(q.left, q.back, c);
}

// dibuixa un sprite ancorat dins el tile. anchor (0.5, 1.0) = centre-baix
// que és l'estil clàssic FF Tactics: peus al centre del tile.
inline void draw_sprite_at(Texture2D tex, TilePos t, IsoProjection p,
                           Color tint = WHITE,
                           Vector2 anchor = { 0.5f, 1.0f }) {
    Vector2 center = tile_to_screen(t.x + 0.5f, t.y + 0.5f, t.z, p);
    Vector2 dst = {
        center.x - tex.width  * anchor.x,
        center.y - tex.height * anchor.y
    };
    DrawTextureV(tex, dst, tint);
}

// salt d'un tile a un altre amb arc parabòlic en z.
// XY amb easing in/out (utils::math), Z amb paràbola simètrica.
// t va de 0 (origen) a 1 (destí). arc_height: alçada extra del salt en nivells.
inline TilePos tile_hop(TilePos from, TilePos to, float arc_height, float t) {
    float te = ease_in_out_quad(t);
    return {
        from.x + (to.x - from.x) * te,
        from.y + (to.y - from.y) * te,
        from.z + (to.z - from.z) * t + arc_height * 4.0f * t * (1.0f - t)
    };
}

// ombreja un color en funció de l'alçada per donar sensació de profunditat.
// strength > 0 → més clar com més alt; strength < 0 → més fosc.
inline Color height_shade(Color base, float z, float strength = 0.05f) {
    float t = z * strength;
    if (t > 0.0f) {
        if (t > 1.0f) t = 1.0f;
        return color_lerp(base, WHITE, t);
    } else {
        t = -t;
        if (t > 1.0f) t = 1.0f;
        return color_lerp(base, BLACK, t);
    }
}

} // namespace utils
