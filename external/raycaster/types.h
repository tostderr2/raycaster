#pragma once

#include <SDL3/SDL_stdinc.h>
#include <cassert>
#include <cmath>

namespace rc {

struct RcHit{
	float perpDist;
	int hitX;
	int hitY;
	int mapValue;

};

struct RcMap{
	const int *cells;
	int width;
	int height;
};

struct Vec2f {
    float x{0.0f};
    float y{0.0f};

    constexpr Vec2f() = default;
    constexpr Vec2f(float x_val, float y_val) : x(x_val), y(y_val) {}

    // Vector-Vector 
    [[nodiscard]] constexpr Vec2f operator+(Vec2f rhs) const { return {x + rhs.x, y + rhs.y}; }
    [[nodiscard]] constexpr Vec2f operator-(Vec2f rhs) const { return {x - rhs.x, y - rhs.y}; }
    [[nodiscard]] constexpr Vec2f operator*(Vec2f rhs) const { return {x * rhs.x, y * rhs.y}; }
    [[nodiscard]] constexpr Vec2f operator/(Vec2f rhs) const { return {x / rhs.x, y / rhs.y}; }

    // scalar
    [[nodiscard]] constexpr Vec2f operator*(float scale) const { return {x * scale, y * scale}; }
    [[nodiscard]] constexpr Vec2f operator/(float scale) const {
		assert(scale);
        float inv = 1.0f / scale; 
        return {x * inv, y * inv};
    }

    // compound 
    constexpr Vec2f& operator+=(Vec2f rhs) { x += rhs.x; y += rhs.y; return *this; }
    constexpr Vec2f& operator-=(Vec2f rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    constexpr Vec2f& operator*=(Vec2f rhs) { x *= rhs.x; y *= rhs.y; return *this; }
    constexpr Vec2f& operator*=(float scale) { x *= scale; y *= scale; return *this; }
    constexpr Vec2f& operator/=(float scale) {
		assert(scale);
        float inv = 1.0f / scale;
        x *= inv; y *= inv;
        return *this;
    }

    // unary Negation
    [[nodiscard]] constexpr Vec2f operator-() const { return {-x, -y}; }

    // subscripting
    [[nodiscard]] constexpr float operator[](size_t idx) const { return (&x)[idx]; }
    [[nodiscard]] constexpr float& operator[](size_t idx) { return (&x)[idx]; }
};

// Enable float * Vec2f (Commutative)
[[nodiscard]] inline constexpr Vec2f operator*(float scale, Vec2f v) {
    return v * scale;
}

[[nodiscard]] inline constexpr float dot(Vec2f a, Vec2f b) {
    return a.x * b.x + a.y * b.y;
}

[[nodiscard]] inline constexpr float length_sq(Vec2f v) {
    return dot(v, v);
}

[[nodiscard]] inline float length(Vec2f v) {
    return std::sqrt(length_sq(v));
}

[[nodiscard]] inline Vec2f normalize(Vec2f v) {
    float len = length(v);
    return len > 0.0f ? v / len : Vec2f{0.0f, 0.0f};
}

} // namespace rc
