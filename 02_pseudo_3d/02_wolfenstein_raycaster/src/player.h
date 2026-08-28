#pragma once

#include <SDL3/SDL_rect.h>

struct Vec2i {
    int x, y;
};
struct Vec3i {
    int x, y, z;
};
struct Vec4i {
    int r, g, b, a;
};

struct Vec2f {
    Vec2f() : x(0.0f), y(0.0f) {
    }
    Vec2f(float x_val, float y_val) : x(x_val), y(y_val) {
    }

    float x, y;

    Vec2f operator+(Vec2f rhs) {
        return Vec2f(x + rhs.x, y + rhs.y);
    }
    void operator+=(Vec2f rhs) {
        x += rhs.x;
        y += rhs.y;
    }
    void operator-=(Vec2f rhs) {
        x -= rhs.x;
        y -= rhs.y;
    }
    Vec2f operator*(Vec2f rhs) {
        return Vec2f(x * rhs.x, y * rhs.y);
    }
    void operator*=(Vec2f rhs) {
        x *= rhs.x;
        y *= rhs.y;
    }
    void operator*=(float scale) {
        x *= scale;
        y *= scale;
    }
    Vec2f operator*(float scale) {
        return Vec2f(x * scale, y * scale);
    }

    void operator/=(float scale) {
        if (scale != 0.0) {
            x /= scale;
            y /= scale;
        }
    }
};
struct Vec3f {
    float x, y, z;
};
struct Vec4f {
    float x, y, z, w;
};

class Player {

  public:
    Player();
    void MoveForward(double dt);
    void MoveBackward(double dt);
    // void MoveLeft(double dt);
    // void MoveRight(double dt);
    void TurnRight(double dt);
    void TurnLeft(double dt);
    // SDL_FRect *GetPlayerFRect();
    Vec2f Posf();

    float m_lookAngle;
    float m_lookAngleRad;
    float m_FOV;
    float m_FOVRad;
    float m_FOVBy2;
    float m_FOVBy2Rad;
    float m_TurnRate;

    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;

  private:
    void move(Vec2f toMove);
    bool colliding(Vec2f newPos);

    Vec2f m_pos;
    float m_speed;
    // SDL_FRect m_playerFRect;
};
