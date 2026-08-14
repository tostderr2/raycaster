#pragma once

struct Vec2 {
    int x, y;
};

class Player {

public:
  void moveForward(double dt);
  void moveBackward(double dt);
  void moveLeft(double dt);
  void moveRight(double dt);

private:
    Vec2 pos;
	Vec2 speed;
	float lookAngle;
};
