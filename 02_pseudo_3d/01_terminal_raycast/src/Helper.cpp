#include "Helper.h"
#include "LinuxTerminal.h"
#include <cmath>
#include <csignal>
#include <cstring>
#include <sys/ioctl.h>
#include <unistd.h>

// Default screen dimensions before the first resize query overwrites them.
int g_ScreenWidth = 240;
int g_ScreenHeight = 110;
volatile bool g_ScreenResized = false;
volatile bool g_ShowMinimap = false;
termios_t g_OrigTermios{};

// Ignoring write()'s return value is fine here -- these are best-effort
// terminal writes on shutdown/clear paths, not something worth retrying.
static void WriteIgnoreResult(int fd, const char *data, size_t len)
{
	ssize_t ignored = write(fd, data, len);
	(void)ignored;
}

void HandleResize(int sig)
{
	(void)sig;
	g_ScreenResized = true;
}

void SetScreenToTerminalSize(int *width, int *height)
{
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0 &&
		w.ws_row > 0)
	{
		*width = w.ws_col;
		*height = w.ws_row;
		g_ScreenWidth = w.ws_col;
		g_ScreenHeight = w.ws_row;
	}
}

void CrashHandler(int signum)
{
	linuxTerminalClose(true, g_OrigTermios);

	if (signum == SIGSEGV)
	{
		const char *msg = "\n[CRASH] Segmentation fault detected!\n";
		WriteIgnoreResult(STDERR_FILENO, msg, strlen(msg));
	}

	_Exit(signum);
}

void ClearScreen()
{
	const char *clear = "\x1b[2J";
	WriteIgnoreResult(STDOUT_FILENO, clear, strlen(clear));
}

float Clamp(float v, float lo, float hi)
{
	if (v < lo)
		return lo;
	if (v > hi)
		return hi;
	return v;
}

RGB LerpRGB(const RGB &a, const RGB &b, float t)
{
	t = Clamp(t, 0.0f, 1.0f);
	return RGB{
		(uint8_t)(a.r + (b.r - a.r) * t),
		(uint8_t)(a.g + (b.g - a.g) * t),
		(uint8_t)(a.b + (b.b - a.b) * t),
	};
}

RGB WallColor(wchar_t wallType, bool darkSide)
{
	RGB base;
	switch (wallType)
	{
	case L'1':
		base = {178, 50, 40};
		break; // brick red
	case L'2':
		base = {70, 110, 190};
		break; // slate blue
	case L'3':
		base = {70, 150, 70};
		break; // mossy green
	default:
		base = {150, 150, 155}; // plain stone ('#')
	}

	if (darkSide)
	{
		base.r = (uint8_t)(base.r * 0.65f);
		base.g = (uint8_t)(base.g * 0.65f);
		base.b = (uint8_t)(base.b * 0.65f);
	}
	return base;
}

// A tile blocks movement unless it's explicitly floor ('.').
static bool IsWallTile(wchar_t tile)
{
	return tile != L'.';
}

static bool IsBlocked(const Vec2 &pos, const std::wstring &map, int mapWidth,
					   int mapHeight)
{
	int x = (int)pos.x;
	int y = (int)pos.y;
	if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
		return true;
	return IsWallTile(map[y * mapWidth + x]);
}

void TryMovePlayer(Vec2 &pos, float dx, float dy, const std::wstring &map,
					int mapWidth, int mapHeight)
{
	// Test each axis independently so a diagonal move into a wall corner
	// slides along it instead of being rejected outright.
	Vec2 afterX = {pos.x + dx, pos.y};
	if (!IsBlocked(afterX, map, mapWidth, mapHeight))
		pos.x = afterX.x;

	Vec2 afterY = {pos.x, pos.y + dy};
	if (!IsBlocked(afterY, map, mapWidth, mapHeight))
		pos.y = afterY.y;
}

void DrawText(std::vector<Cell> &screen, int screenWidth, int screenHeight,
			  int startX, int startY, const std::string &text, RGB color)
{
	if (startY < 0 || startY >= screenHeight)
		return;

	for (size_t i = 0; i < text.length(); ++i)
	{
		int targetX = startX + (int)i;
		if (targetX >= 0 && targetX < screenWidth)
			screen[startY * screenWidth + targetX] = Cell{text[i], color};
	}
}

void DrawMiniMap(std::vector<Cell> &screen, int screenWidth, int screenHeight,
				  const std::wstring &map, int mapWidth, int mapHeight,
				  const Vec2 &playerPos, float playerAngle)
{
	const RGB wallCol{200, 200, 60};
	const RGB floorCol{60, 60, 60};
	const RGB playerCol{255, 60, 60};

	for (int y = 0; y < mapHeight; ++y)
	{
		if (y >= screenHeight)
			break;
		for (int x = 0; x < mapWidth; ++x)
		{
			if (x >= screenWidth)
				break;

			wchar_t tile = map[y * mapWidth + x];
			int i = y * screenWidth + x;
			screen[i] = IsWallTile(tile) ? Cell{'#', wallCol}
										  : Cell{'.', floorCol};
		}
	}

	int playerX = (int)playerPos.x;
	int playerY = (int)playerPos.y;
	if (playerX < 0 || playerX >= mapWidth || playerY < 0 ||
		playerY >= mapHeight || playerY >= screenHeight ||
		playerX >= screenWidth)
		return;

	// Angle 0 points along +Y (north/"down" in array terms), matching how
	// the raycaster below defines forward. Pick one of 4 glyphs by octant.
	float angle = fmodf(playerAngle, 2.0f * (float)M_PI);
	if (angle < 0)
		angle += 2.0f * (float)M_PI;

	char arrow;
	if (angle >= 0.25f * M_PI && angle < 0.75f * M_PI)
		arrow = 'v';
	else if (angle >= 0.75f * M_PI && angle < 1.25f * M_PI)
		arrow = '<';
	else if (angle >= 1.25f * M_PI && angle < 1.75f * M_PI)
		arrow = '^';
	else
		arrow = '>';

	screen[playerY * screenWidth + playerX] = Cell{arrow, playerCol};
}
