#include "player.h"

#include <cmath>
#include <cstring>
#include <iostream>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#include "constants.h"

// m_pos is currently vec2f, but should be vec2int
Player::Player() : m_pos(1.2f, 1.2f) { // Start inside the cell
    m_speed = 2.7f;                    // Move 2.7 map tiles per second
    m_TurnRate = 3.0f;                 // Radians per second

    m_lookAngleDegrees = 0.0f;
    m_lookAngle = m_lookAngleDegrees * DEG_TO_RAD;

    m_FOVDegrees = 90.0f;
    m_FOV = m_FOVDegrees * DEG_TO_RAD;
    m_FOVBy2Degrees = m_FOVDegrees / 2.0f;
    m_FOVBy2 = m_FOVBy2Degrees * DEG_TO_RAD;
} //
// Player::Player(SDL_FRect rect) : m_pos(1080 / 2.0f, 720.0f / 2) {
//     m_playerFRect = rect;
// }
//
void Player::move(Vec2f delta) {
    Vec2f targetPos = m_pos + delta;
    if (!colliding(targetPos)) {
        m_pos = targetPos;
        // for debug
        // m_playerFRect.x = m_pos.x;
        // m_playerFRect.y = m_pos.y;
    }
    // std::cout << "player pos: (x: " << m_pos.x << ", y: " << m_pos.y << ")\n";
}

bool Player::colliding(Vec2f newPos) {
    size_t mapX = static_cast<size_t>(newPos.x);
    size_t mapY = static_cast<size_t>(newPos.y);

    if (mapX < 0 || mapX >= KMapWidth || mapY < 0 || mapY >= KMapHeight) {
        return true;
    }

    return KMap[mapY][mapX] != 0;
}

void Player::MoveForward(float dt) {
    Vec2f direction = Vec2f(cosf(m_lookAngle), sinf(m_lookAngle));
    Vec2f toMove = direction * (m_speed * static_cast<float>(dt));
    move(toMove);
}

void Player::MoveBackward(float dt) {
    Vec2f direction = Vec2f(cosf(m_lookAngle), sinf(m_lookAngle));
    Vec2f toMove = direction * (-m_speed * static_cast<float>(dt));
    move(toMove);
}
//
// void Player::MoveLeft(float dt) {
//     Vec2f toMove = Vec2f(1.0f, 0.0f);
//     toMove *= -1 * m_speed * dt;
//     move(toMove);
// }
//
// void Player::MoveRight(float dt) {
//     Vec2f toMove = Vec2f(1.0f, 0.0f);
//     toMove *= m_speed * dt;
//     move(toMove);
// }
//
void Player::TurnRight(float dt) {
    m_lookAngle += dt * m_TurnRate;
    if (m_lookAngle > 2 * PI) {
        m_lookAngle -= 2 * PI;
    }
}

void Player::TurnLeft(float dt) {
    m_lookAngle -= dt * m_TurnRate;
    if (m_lookAngle < 0.0f) {
        m_lookAngle += 2 * PI;
    }
}

void Player::Shoot(float dt) {
    // todo:
    // create an entity manager that has enemy and bullets
    // use that manager to spawn a bullet.
    // with players current pos as its parameters
    // rendering it and updating will happen in that manager
    // it will have a vector of entities that will be looped to
    // check if it is visible or behind the hitPoints of that row
    // and render
    // so it will need a reference to hitPoints vector
    // have done this in shapes and shooter, check it for reference

    static Uint64 lastFireTime = 0;
    static Uint64 fireRate = 100; // one bullet every 100 ms?
    Uint64 current = SDL_GetTicks();

    if (current - lastFireTime >= fireRate) {
        std::cout << "Player shot a bullet!\n";
        lastFireTime = current;

        // // test: intentional ram drain.
        // // allocate 50 mb every frame
        // size_t size = 1000 * 1024 * 1024;
        // char *massiveLeak = new char[size];
        // std::memset(massiveLeak, 'a', size);
    }
}
// SDL_FRect *Player::GetPlayerFRect() {
//     return &m_playerFRect;
// }
// Vec2f Player::Posf() {
//     // return Vec2f{m_playerFRect.x, m_playerFRect.y};
//     return m_pos;
// }
