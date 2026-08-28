#include "player.h"

#include <cmath>
#include <iostream>

#include "constants.h"

// m_pos is currently vec2f, but should be vec2int
Player::Player() : m_pos(2.5f, 2.5f) { // Start in the middle of cell (2, 2)
    m_speed = 1.0f;                    // Move 1 map tiles per second
    m_TurnRate = 2.0f;                 // Radians per second

    m_lookAngle = 140.0f;
    m_lookAngleRad = m_lookAngle * DEG_TO_RAD;

    m_FOV = 90.0f;
    m_FOVRad = m_FOV * DEG_TO_RAD;
    m_FOVBy2 = m_FOV / 2.0f;
    m_FOVBy2Rad = m_FOVBy2 * DEG_TO_RAD;
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
    std::cout << "player pos: (x: " << m_pos.x << ", y: " << m_pos.y << ")\n";
}

bool Player::colliding(Vec2f newPos) {
    int mapX = static_cast<int>(newPos.x);
    int mapY = static_cast<int>(newPos.y);

    if (mapX < 0 || mapX >= KMapWidth || mapY < 0 || mapY >= KMapHeight) {
        return true;
    }

    return KMap[mapY][mapX] != 0;
}

void Player::MoveForward(double dt) {
    Vec2f direction = Vec2f(cosf(m_lookAngleRad), sinf(m_lookAngleRad));
    Vec2f toMove = direction * (m_speed * static_cast<float>(dt));
    move(toMove);
}

void Player::MoveBackward(double dt) {
    Vec2f direction = Vec2f(cosf(m_lookAngleRad), sinf(m_lookAngleRad));
    Vec2f toMove = direction * (-m_speed * static_cast<float>(dt));
    move(toMove);
}
//
// void Player::MoveLeft(double dt) {
//     Vec2f toMove = Vec2f(1.0f, 0.0f);
//     toMove *= -1 * m_speed * dt;
//     move(toMove);
// }
//
// void Player::MoveRight(double dt) {
//     Vec2f toMove = Vec2f(1.0f, 0.0f);
//     toMove *= m_speed * dt;
//     move(toMove);
// }
//
void Player::TurnRight(double dt) {
    m_lookAngleRad += dt * m_TurnRate;
    std::cout << "angle: " << m_lookAngleRad << std::endl;
}

void Player::TurnLeft(double dt) {
    m_lookAngleRad -= dt * m_TurnRate;
    std::cout << "angle: " << m_lookAngleRad << std::endl;
}

// SDL_FRect *Player::GetPlayerFRect() {
//     return &m_playerFRect;
// }
Vec2f Player::Posf() {
    // return Vec2f{m_playerFRect.x, m_playerFRect.y};
    return m_pos;
}
