#include <chrono>
#include <clocale>
#include <cmath>
#include <csignal>
#include <cstdio>
#include <thread>
#include <unistd.h>

#include "Helper.h"
#include "LinuxTerminal.h"

using namespace std;

// Map legend: '.' = floor, '#' = plain stone, '1'/'2'/'3' = colored wall
// types (see WallColor in Helper.cpp). Must be exactly c_MapWidth *
// c_MapHeight characters.
static const std::wstring g_Map = L"####################"
								   L"#..................#"
								   L"#............2.....#"
								   L"#.11111......2.....#"
								   L"#............2.....#"
								   L"#............2.....#"
								   L"#.....3333.........#"
								   L"#........#.........#"
								   L"#........#.........#"
								   L"#........#.........#"
								   L"#..................#"
								   L"#..................#"
								   L"#...........1111...#"
								   L"#...........2......#"
								   L"#...........2......#"
								   L"#......333333......#"
								   L"#..................#"
								   L"#..................#"
								   L"#..................#"
								   L"####################";

// Distance, from the horizon (screen center row), at which a ceiling/floor
// pixel sits in world units. Derived from the same perspective relation
// used for wall height: ceiling = height/2 - (aspect*height)/dist.
// Both ceiling and floor pixels use this, since they're mirror images of
// each other around the horizon row.
static float RowDistance(int y, int height)
{
	float diff = fabsf((float)y - height / 2.0f);
	if (diff < 0.5f) // avoid dividing by ~0 right at the horizon line
		return 1000.0f;
	return (c_AspectRatio * height) / diff;
}

// Casts one ray using DDA (Digital Differential Analysis): step cell by
// cell through the grid along the ray instead of marching in small fixed
// distance increments. This is both exact (no step-size vs. accuracy
// tradeoff) and touches only the cells the ray actually crosses, so it's
// far cheaper than incremental marching.
struct RayHit
{
	float perpDist; // perpendicular distance to the wall (fisheye-free)
	wchar_t wallType;
	bool sideHit; // false = hit stepping in X, true = hit stepping in Y
	bool isEdge;  // near a tile boundary -> draw a mortar/seam line
};

static RayHit CastRay(float rayAngle, const Vec2 &pos, float maxDepth)
{
	float rayDirX = sinf(rayAngle);
	float rayDirY = cosf(rayAngle);

	int mapX = (int)pos.x;
	int mapY = (int)pos.y;

	float deltaDistX = (rayDirX == 0.0f) ? 1e30f : fabsf(1.0f / rayDirX);
	float deltaDistY = (rayDirY == 0.0f) ? 1e30f : fabsf(1.0f / rayDirY);

	int stepX, stepY;
	float sideDistX, sideDistY;

	if (rayDirX < 0)
	{
		stepX = -1;
		sideDistX = (pos.x - mapX) * deltaDistX;
	}
	else
	{
		stepX = 1;
		sideDistX = (mapX + 1.0f - pos.x) * deltaDistX;
	}

	if (rayDirY < 0)
	{
		stepY = -1;
		sideDistY = (pos.y - mapY) * deltaDistY;
	}
	else
	{
		stepY = 1;
		sideDistY = (mapY + 1.0f - pos.y) * deltaDistY;
	}

	bool sideHit = false;
	wchar_t wallType = L'.';
	bool outOfBounds = false;

	while (wallType == L'.')
	{
		if (sideDistX < sideDistY)
		{
			sideDistX += deltaDistX;
			mapX += stepX;
			sideHit = false;
		}
		else
		{
			sideDistY += deltaDistY;
			mapY += stepY;
			sideHit = true;
		}

		if (mapX < 0 || mapX >= c_MapWidth || mapY < 0 || mapY >= c_MapHeight)
		{
			outOfBounds = true;
			break;
		}
		wallType = g_Map[mapY * c_MapWidth + mapX];
	}

	if (outOfBounds)
		return RayHit{maxDepth, L'.', sideHit, false};

	float perpDist = sideHit ? (sideDistY - deltaDistY) : (sideDistX - deltaDistX);
	if (perpDist > maxDepth)
		perpDist = maxDepth;

	// Exact fractional position where the ray crosses the wall face; near
	// 0 or 1 means we're near the seam between two adjacent tiles.
	float wallX = sideHit ? (pos.x + perpDist * rayDirX)
						   : (pos.y + perpDist * rayDirY);
	wallX -= floorf(wallX);
	bool isEdge = (wallX < 0.03f || wallX > 0.97f);

	return RayHit{perpDist, wallType, sideHit, isEdge};
}

// Ray-casts and shades one screen column into `screen`.
static void RenderColumn(std::vector<Cell> &screen, int x, int width,
						  int height, const Player &player)
{
	float rayAngle = (player.currentAngle - player.fov / 2.0f) +
					  ((float)x / width) * player.fov;

	RayHit hit = CastRay(rayAngle, player.pos, player.maxDepth);

	int ceiling = (int)(height / 2.0f -
						 (c_AspectRatio * height) / hit.perpDist);
	int floorEdge = height - ceiling;
	if (ceiling < 0)
		ceiling = 0;
	if (floorEdge >= height)
		floorEdge = height - 1;

	// Wall color: base palette color, darkened by which axis we hit and
	// by distance fog, with a darker seam line at tile boundaries.
	const RGB fogColor{10, 10, 22};
	float fog = Clamp(1.0f - hit.perpDist / player.maxDepth, 0.12f, 1.0f);
	RGB wallColor = LerpRGB(fogColor, WallColor(hit.wallType, hit.sideHit), fog);
	if (hit.isEdge)
		wallColor = LerpRGB(RGB{0, 0, 0}, wallColor, 0.45f);

	const RGB skyNear{40, 50, 90};
	const RGB skyFar{8, 8, 20};
	const RGB floorNear{90, 80, 60};
	const RGB floorFar{15, 14, 12};

	for (int y = 0; y < height; ++y)
	{
		Cell cell;
		if (y < ceiling)
		{
			float dist = RowDistance(y, height);
			float f = Clamp(1.0f - dist / player.maxDepth, 0.05f, 1.0f);
			cell.color = LerpRGB(skyFar, skyNear, f);
		}
		else if (y <= floorEdge)
		{
			cell.color = wallColor;
		}
		else
		{
			float dist = RowDistance(y, height);
			float f = Clamp(1.0f - dist / player.maxDepth, 0.05f, 1.0f);
			RGB c = LerpRGB(floorFar, floorNear, f);
			// Cheap checkerboard: darken alternating distance bands so the
			// floor reads as a receding grid instead of a flat gradient.
			if (((int)(dist * 2.0f)) % 2 == 0)
				c = LerpRGB(RGB{0, 0, 0}, c, 0.85f);
			cell.color = c;
		}
		screen[y * width + x] = cell;
	}
}

int main()
{
	std::setlocale(LC_ALL, "");

	// ---- crash / signal interception, so we always restore the terminal ----
	struct sigaction sa{};
	sa.sa_handler = CrashHandler;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGSEGV, &sa, nullptr);
	sigaction(SIGABRT, &sa, nullptr);
	sigaction(SIGINT, &sa, nullptr);
	sigaction(SIGTERM, &sa, nullptr);

	EnableRawMode(g_OrigTermios);
	signal(SIGWINCH, HandleResize);

	int width = g_ScreenWidth;
	int height = g_ScreenHeight;
	SetScreenToTerminalSize(&width, &height);

	std::vector<Cell> screen((size_t)width * height);
	Player player;

	auto prevTime = chrono::steady_clock::now();
	bool running = true;
	bool minimapKeyHeld = false; // edge-detects the 'M' toggle

	while (running)
	{
		auto now = chrono::steady_clock::now();
		float dt = chrono::duration<float>(now - prevTime).count();
		prevTime = now;
		if (dt <= 0.0f)
			dt = 1.0f / 1000.0f; // guard div-by-zero on the FPS readout

		if (g_ScreenResized)
		{
			SetScreenToTerminalSize(&width, &height);
			screen.assign((size_t)width * height, Cell{});
			ClearScreen();
			g_ScreenResized = false;
		}

		// ---- input: drain every buffered byte this frame (non-blocking) ----
		bool aPressed = false, dPressed = false, wPressed = false,
			 sPressed = false, qPressed = false, ePressed = false,
			 spacePressed = false, mPressed = false, escPressed = false;
		char ch;
		while (read(STDIN_FILENO, &ch, 1) > 0)
		{
			switch (ch)
			{
			case 'a': case 'A': aPressed = true; break;
			case 'd': case 'D': dPressed = true; break;
			case 'w': case 'W': wPressed = true; break;
			case 's': case 'S': sPressed = true; break;
			case 'q': case 'Q': qPressed = true; break; // strafe left
			case 'e': case 'E': ePressed = true; break; // strafe right
			case ' ': spacePressed = true; break;		  // sprint
			case 'm': case 'M': mPressed = true; break;  // toggle minimap
			case 27: escPressed = true; break;			  // ESC -> quit
			}
		}
		if (escPressed)
			running = false;

		if (mPressed && !minimapKeyHeld)
			g_ShowMinimap = !g_ShowMinimap;
		minimapKeyHeld = mPressed;

		if (aPressed)
			player.currentAngle -= player.turnSpeed * dt;
		if (dPressed)
			player.currentAngle += player.turnSpeed * dt;

		// Build a combined movement vector from all held movement keys so
		// e.g. forward+strafe moves diagonally at normal speed rather than
		// the old model of testing/reverting one intent at a time.
		float speed = player.moveSpeed *
					  (spacePressed ? player.sprintMultiplier : 1.0f);
		float moveX = 0.0f, moveY = 0.0f;
		if (wPressed)
		{
			moveX += sinf(player.currentAngle);
			moveY += cosf(player.currentAngle);
		}
		if (sPressed)
		{
			moveX -= sinf(player.currentAngle);
			moveY -= cosf(player.currentAngle);
		}
		if (ePressed) // strafe right: forward direction rotated +90 deg
		{
			moveX += sinf(player.currentAngle + (float)M_PI_2);
			moveY += cosf(player.currentAngle + (float)M_PI_2);
		}
		if (qPressed) // strafe left
		{
			moveX += sinf(player.currentAngle - (float)M_PI_2);
			moveY += cosf(player.currentAngle - (float)M_PI_2);
		}
		if (moveX != 0.0f || moveY != 0.0f)
		{
			// Normalize so diagonal movement isn't faster than axis-aligned.
			float len = sqrtf(moveX * moveX + moveY * moveY);
			moveX = moveX / len * speed * dt;
			moveY = moveY / len * speed * dt;
			TryMovePlayer(player.pos, moveX, moveY, g_Map, c_MapWidth,
						  c_MapHeight);
		}

		// ---- render ----
		auto tRayStart = chrono::steady_clock::now();
		for (int x = 0; x < width; ++x)
			RenderColumn(screen, x, width, height, player);
		float raycastMs = chrono::duration<float, std::milli>(
							   chrono::steady_clock::now() - tRayStart)
							   .count();

		// drawMs is last frame's draw time -- this frame's isn't known
		// until after DrawFrame runs below, one frame of lag is fine for
		// a debug readout.
		static float drawMs = 0.0f;
		char buf[161];
		snprintf(buf, sizeof(buf),
				 "FPS: %4.1f | Pos: (%4.1f, %4.1f) | Angle: %5.1f deg | "
				 "ray: %4.1fms | draw: %4.1fms",
				 1.0f / dt, player.pos.x, player.pos.y,
				 player.currentAngle * (180.0f / (float)M_PI), raycastMs,
				 drawMs);

		const RGB hudColor{120, 230, 160};
		int statsY = height - 2;
		DrawText(screen, width, height, 0, statsY, buf, hudColor);
		DrawText(screen, width, height, 0, statsY + 1,
				 "W/S move  A/D turn  Q/E strafe  Space sprint  M map  "
				 "ESC quit",
				 hudColor);

		if (g_ShowMinimap)
			DrawMiniMap(screen, width, height, g_Map, c_MapWidth,
						c_MapHeight, player.pos, player.currentAngle);

		auto tDrawStart = chrono::steady_clock::now();
		DrawFrame(screen, width, height);
		drawMs = chrono::duration<float, std::milli>(
					 chrono::steady_clock::now() - tDrawStart)
					 .count();

		// Cap the frame rate so we don't peg a CPU core spinning on a
		// render that, with the batched writer, is now cheap enough to
		// run far faster than the eye (or the terminal) needs.
		constexpr auto targetFrameTime = chrono::milliseconds(6); // ~166fps
		auto elapsedThisFrame = chrono::steady_clock::now() - now;
		if (elapsedThisFrame < targetFrameTime)
			std::this_thread::sleep_for(targetFrameTime - elapsedThisFrame);
	}

	linuxTerminalClose(true, g_OrigTermios);
	return 0;
}
