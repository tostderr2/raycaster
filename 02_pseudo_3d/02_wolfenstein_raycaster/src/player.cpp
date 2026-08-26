#include "player.h"
#include <iostream>

#include "constants.h"

Player::Player() : m_pos(1080 / 2.0f, 720.0f / 2) {
    m_speed = 100.0f;
    m_lookAngle= 140.0f;
    m_lookAngleRad = m_lookAngle * DEG_TO_RAD;
    m_TurnRate = 5;
    m_FOV = 90.0f;
	m_FOVRad = m_FOV * DEG_TO_RAD;
    m_FOVBy2= m_FOV / 2;
	m_FOVBy2Rad = m_FOVBy2 * DEG_TO_RAD;

    m_playerFRect = {m_pos.x, m_pos.y, 10, 10};
}
//
// Player::Player(SDL_FRect rect) : m_pos(1080 / 2.0f, 720.0f / 2) {
//     m_playerFRect = rect;
// }
//
void Player::move(Vec2f toMove) {

    if (!colliding(m_pos + toMove)) {
        m_pos += toMove;
        // for debug
        m_playerFRect.x = m_pos.x;
        m_playerFRect.y = m_pos.y;
    }
}

bool Player::colliding(Vec2f newPos) {

    // convert the window space x and y to map x and  y
    int map_x = newPos.x * MAP_SPACE_STEP_RATIO_X;
    int map_x_end = (newPos.x + m_playerFRect.w) * MAP_SPACE_STEP_RATIO_X; // there is a gap here, interesting
    int map_y = newPos.y * MAP_SPACE_STEP_RATIO_Y;
    int map_y_end = (newPos.y + m_playerFRect.h) * MAP_SPACE_STEP_RATIO_Y;
    // absolutely ignoring the rect width and ht for collision as this is temporary
    if (MAP_G[map_y][map_x] == 0 && MAP_G[map_y_end][map_x_end] == 0) {
        return false;
    }

    return true;
}

void Player::MoveForward(double dt) {
    Vec2f toMove = Vec2f(0, 1.0f);
    toMove *= -1 * m_speed * dt;
    move(toMove);
}

void Player::MoveBackward(double dt) {
    Vec2f toMove = Vec2f(0, 1.0f);
    toMove *= m_speed * dt;
    move(toMove);
}

void Player::MoveLeft(double dt) {
    Vec2f toMove = Vec2f(1.0f, 0.0f);
    toMove *= -1 * m_speed * dt;
    move(toMove);
}

void Player::MoveRight(double dt) {
    Vec2f toMove = Vec2f(1.0f, 0.0f);
    toMove *= m_speed * dt;
    move(toMove);
}

void Player::TurnRight(double dt) {
    m_lookAngleRad += dt * m_TurnRate;
	std::cout << "angle: " << m_lookAngleRad << std::endl;
}

void Player::TurnLeft(double dt) {
    m_lookAngleRad -= dt * m_TurnRate;
	std::cout << "angle: " << m_lookAngleRad << std::endl;
}

SDL_FRect *Player::GetPlayerFRect() {
    return &m_playerFRect;
}
Vec2f Player::Posf() {
    return Vec2f{m_playerFRect.x, m_playerFRect.y};
}
