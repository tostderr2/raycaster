#pragma once

#include <cstddef>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_stdinc.h>

float constexpr DEG_TO_RAD = 3.14159265f / 180.0f;
constexpr size_t KWinWidth = 1080;
constexpr size_t KWinHeight = 720;

constexpr size_t KMapWidth = 24;
constexpr size_t KMapHeight = 24;
constexpr size_t KDrawBoxWidth = KWinWidth / KMapWidth;
constexpr size_t KDrawBoxHeight = KWinHeight / KMapHeight;

constexpr float KMapSpaceStepRatioX = KMapWidth / static_cast<float>(KWinWidth);
constexpr float KMapSpaceStepRatioY = KMapHeight / static_cast<float>(KWinHeight);

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
};


// Color constants
constexpr Color KEmpty = {.r = 255, .g = 255, .b = 255, .a = 0}; // Fixed Alpha transparency

constexpr SDL_FColor KTeal = {
    .r = 145 / 255.0f, .g = 217 / 255.0f, .b = 214 / 255.0f, .a = 255 / 255.0f}; // sky background

constexpr SDL_FColor KOrange = {
    .r = 181 / 255.0f, .g = 93 / 255.0f, .b = 11 / 255.0f, .a = 255 / 255.0f};

constexpr Color KMudFloor = {.r = 210, .g = 180, .b = 140, .a = 255}; // CS:GO De_Dust sandy mud

constexpr Color KStoneWall = {.r = 128, .g = 128, .b = 128, .a = 255}; // Solid Grey

constexpr Color KMaroonWall = {.r = 155, .g = 45, .b = 45, .a = 255}; // Vibrant Red

// hack: not really blue here, experimenting
constexpr Color KIceBlueWall = {
    .r = 1, .g = 201, .b = 200, .a = 255}; // Fixed octal formatting literal

constexpr Color KDefaultWhite = {.r = 255, .g = 255, .b = 255, .a = 255};

constexpr Color KGreen = {.r = 57, .g = 255, .b = 20, .a = 255};
constexpr Color KPurple = {.r = 155, .g = 38, .b = 182, .a = 255};

enum ColorsPresets {
    Empty_ID = -1,
    MudFloor_ID = 0,  // Walkable floor
    StoneWall_ID = 1, // Boundary walls
    RedWall_ID = 2,   // Red obstacles
    IceBlue_ID = 3,   // Ice Blue obstacles
    Purple_ID = 4,    // Purple obstacles
    Green_ID = 5,     // Green obstacles
};

// 0 -> MudFloor (de_dust sand)
// 1 -> Stone grey
// 2 -> Red
// 3 -> Ice blue
// 4 -> Purple
// 5 -> Green
const int KMap[KMapHeight][KMapWidth] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 1},
    {1, 0, 5, 0, 5, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 2, 2, 0, 1},
    {1, 0, 5, 0, 0, 0, 3, 0, 3, 0, 2, 2, 2, 2, 0, 4, 0, 0, 4, 0, 2, 0, 0, 1},
    {1, 0, 5, 5, 0, 0, 3, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 1},
    {1, 0, 0, 0, 0, 0, 3, 3, 3, 0, 2, 0, 0, 2, 0, 5, 5, 5, 5, 0, 0, 0, 0, 1},
    {1, 0, 2, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 5, 0, 0, 5, 0, 3, 3, 0, 1},
    {1, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 3, 3, 0, 1},
    {1, 0, 2, 0, 2, 2, 2, 4, 4, 4, 0, 0, 0, 0, 0, 5, 5, 0, 5, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 3, 3, 0, 1, 1, 2, 2, 2, 0, 4, 4, 0, 1},
    {1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 0, 2, 0, 0, 0, 0, 1},
    {1, 1, 3, 3, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 5, 0, 0, 5, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 0, 5, 5, 0, 1},
    {1, 0, 4, 4, 0, 5, 0, 0, 5, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 5, 5, 0, 1},
    {1, 0, 4, 4, 0, 5, 5, 0, 5, 0, 2, 0, 0, 2, 0, 3, 3, 3, 3, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 3, 0, 0, 3, 0, 2, 2, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 2, 2, 0, 1},
    {1, 0, 1, 0, 0, 1, 0, 4, 4, 4, 4, 0, 0, 0, 0, 3, 3, 0, 3, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 1, 0, 4, 0, 0, 4, 0, 5, 5, 0, 0, 0, 0, 0, 0, 3, 3, 0, 1},
    {1, 0, 1, 1, 0, 1, 0, 4, 0, 0, 4, 0, 5, 5, 0, 2, 2, 2, 2, 0, 3, 3, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
