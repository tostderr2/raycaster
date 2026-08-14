#pragma once

#include <cstdint>
#include <string>
#include <termios.h>
#include <vector>

typedef struct termios termios_t;

// ---- global terminal / window state ----
extern termios_t g_OrigTermios;
extern int g_ScreenWidth;
extern int g_ScreenHeight;
extern volatile bool g_ScreenResized; // set by the SIGWINCH handler
extern volatile bool g_ShowMinimap;   // toggled at runtime with 'M'

// ---- world constants ----
constexpr int c_MapWidth = 20;
constexpr int c_MapHeight = 20;
// A terminal character cell is roughly twice as tall as it is wide, so a
// wall of "true" height 1.0 needs to be stretched vertically by this much
// or it looks squashed. Tweak to taste for your font/terminal.
constexpr float c_AspectRatio = 2.0f;

struct Vec2
{
	float x;
	float y;
};

struct RGB
{
	uint8_t r, g, b;
};

// A single on-screen cell. If `ch` is 0 we draw a solid colored block
// (used for the 3D scene: walls/floor/sky). Otherwise we draw the literal
// ASCII character `ch` in `color` (used for HUD text and the minimap).
struct Cell
{
	char ch = 0;
	RGB color{0, 0, 0};
};

struct Player
{
	Vec2 pos = {1.7f, 1.7f};
	float currentAngle = 0.0f;
	float fov = 1.0472f; // 60 degrees
	float maxDepth = 20.0f;
	float moveSpeed = 17.0f;
	float sprintMultiplier = 1.8f;
	float turnSpeed = 8.5f;
};

// ---- terminal / lifecycle ----
void HandleResize(int sig);
void SetScreenToTerminalSize(int *width, int *height);
void CrashHandler(int signum);
void ClearScreen();

// ---- small math helpers ----
float Clamp(float v, float lo, float hi);
RGB LerpRGB(const RGB &a, const RGB &b, float t);

// Base color for a wall tile character. `darkSide` shades a wall face
// darker than the other so adjacent faces of the same wall read as
// distinct surfaces under a fixed "sun" direction, like Wolfenstein-style
// N/S vs E/W shading.
RGB WallColor(wchar_t wallType, bool darkSide);

// Moves the player by (dx, dy), testing the X and Y axes independently.
// This lets you slide along a wall you're moving into diagonally instead
// of stopping dead, which is what a single combined-axis test gives you.
void TryMovePlayer(Vec2 &pos, float dx, float dy, const std::wstring &map,
					int mapWidth, int mapHeight);

// ---- drawing into the Cell buffer (screen-space, not terminal escapes) ----
void DrawText(std::vector<Cell> &screen, int screenWidth, int screenHeight,
			  int startX, int startY, const std::string &text, RGB color);

void DrawMiniMap(std::vector<Cell> &screen, int screenWidth, int screenHeight,
				  const std::wstring &map, int mapWidth, int mapHeight,
				  const Vec2 &playerPos, float playerAngle);
