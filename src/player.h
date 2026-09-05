#pragma once

#include <cstdlib>

#include <SDL3/SDL_rect.h>

#include "raycaster/types.h"

class Player {

  public:
    Player();
    void MoveForward(float dt);
    void MoveBackward(float dt);
    // void MoveLeft(float dt);
    // void MoveRight(float dt);
    void TurnRight(float dt);
    void TurnLeft(float dt);
    // SDL_FRect *GetPlayerFRect();

    float m_lookAngle;
    float m_lookAngleRad;
    float m_FOV;
    float m_FOVRad;
    float m_FOVBy2;
    float m_FOVBy2Rad;
    float m_TurnRate;
    rc::Vec2f m_pos;

    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;

  private:
    void move(rc::Vec2f toMove);
    bool colliding(rc::Vec2f newPos);

    float m_speed;
    // SDL_FRect m_playerFRect;
};
