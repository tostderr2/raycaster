#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>

namespace rc {

struct RcHit {
   float perpDist;
    int hitX;
    int hitY;
    int mapValue;
};

struct RcMap {
    const int *cells;
    int width;
    int height;
};

// Generic 2D Vector template
template <typename T>
struct Vec2 {
    T x{0};
    T y{0};

    constexpr Vec2() = default;
    constexpr Vec2(T x_val, T y_val) : x(x_val), y(y_val) {}

    // Vector-Vector Arithmetic
    [[nodiscard]] constexpr Vec2 operator+(Vec2 rhs) const { return {x + rhs.x, y + rhs.y}; }
    [[nodiscard]] constexpr Vec2 operator-(Vec2 rhs) const { return {x - rhs.x, y - rhs.y}; }
    [[nodiscard]] constexpr Vec2 operator*(Vec2 rhs) const { return {x * rhs.x, y * rhs.y}; }
    [[nodiscard]] constexpr Vec2 operator/(Vec2 rhs) const { return {x / rhs.x, y / rhs.y}; }

    // Scalar Arithmetic
    [[nodiscard]] constexpr Vec2 operator*(T scale) const { return {x * scale, y * scale}; }
    [[nodiscard]] constexpr Vec2 operator/(T scale) const {
        assert(scale != T{0});
        if constexpr (std::is_floating_point_v<T>) {
            T inv = T{1} / scale;
            return {x * inv, y * inv};
        } else {
            return {x / scale, y / scale};
        }
    }

    // Compound Operators
    constexpr Vec2& operator+=(Vec2 rhs) { x += rhs.x; y += rhs.y; return *this; }
    constexpr Vec2& operator-=(Vec2 rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    constexpr Vec2& operator*=(Vec2 rhs) { x *= rhs.x; y *= rhs.y; return *this; }
    constexpr Vec2& operator/=(Vec2 rhs) { x /= rhs.x; y /= rhs.y; return *this; }
    constexpr Vec2& operator*=(T scale) { x *= scale; y *= scale; return *this; }
    constexpr Vec2& operator/=(T scale) {
        assert(scale != T{0});
        if constexpr (std::is_floating_point_v<T>) {
            T inv = T{1} / scale;
            x *= inv; y *= inv;
        } else {
            x /= scale; y /= scale;
        }
        return *this;
    }

    // Unary Negation
    [[nodiscard]] constexpr Vec2 operator-() const { return {-x, -y}; }

    // Subscripting
    [[nodiscard]] constexpr T operator[](std::size_t idx) const { return (&x)[idx]; }
    [[nodiscard]] constexpr T& operator[](std::size_t idx) { return (&x)[idx]; }

    // Explicit Conversion Operator (e.g., Vec2f to Vec2i)
    template <typename U>
    [[nodiscard]] constexpr explicit operator Vec2<U>() const {
        return {static_cast<U>(x), static_cast<U>(y)};
    }
};

// Scalar multiplication (Commutative: scale * vec)
template <typename T>
[[nodiscard]] inline constexpr Vec2<T> operator*(T scale, Vec2<T> v) {
    return v * scale;
}

// Vector utility functions
template <typename T>
[[nodiscard]] inline constexpr T dot(Vec2<T> a, Vec2<T> b) {
    return a.x * b.x + a.y * b.y;
}

template <typename T>
[[nodiscard]] inline constexpr T length_sq(Vec2<T> v) {
    return dot(v, v);
}

template <typename T>
[[nodiscard]] inline float length(Vec2<T> v) {
    return std::sqrt(static_cast<float>(length_sq(v)));
}

template <typename T>
[[nodiscard]] inline Vec2<float> normalize(Vec2<T> v) {
    float len = length(v);
    return len > 0.0f ? Vec2<float>{v.x / len, v.y / len} : Vec2<float>{0.0f, 0.0f};
}

// Type Aliases for convenient usage
using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;

} // namespace rc
