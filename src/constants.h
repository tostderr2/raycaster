#pragma once

#include <cstddef>

float constexpr DEG_TO_RAD = 3.14159265f / 180.0f;
constexpr size_t KWinWidth = 1080;
constexpr size_t KWinHeight = 720;

constexpr size_t KMapWidth = 12;
constexpr size_t KMapHeight = 12;
constexpr size_t KDrawBoxWidth = KWinWidth / KMapWidth;
constexpr size_t KDrawBoxHeight = KWinHeight / KMapHeight;

constexpr float KMapSpaceStepRatioX = KMapWidth / static_cast<float>(KWinWidth);
constexpr float KMapSpaceStepRatioY = KMapHeight / static_cast<float>(KWinHeight);

struct Color {
    float r;
    float g;
    float b;
    float a;
};

// Color constants
constexpr Color KTeal = {
    .r = 145 / 255.0f, .g = 217 / 255.0f, .b = 214 / 255.0f, .a = 1.0f}; // sky background
constexpr Color KMudFloor = {
    .r = 210 / 255.0f, .g = 180 / 255.0f, .b = 140 / 255.0f, .a = 1.0f}; // CS:GO De_Dust sandy mud
constexpr Color KStoneWall = {
    .r = 128 / 255.0f, .g = 128 / 255.0f, .b = 128 / 255.0f, .a = 1.0f}; // Solid Grey
constexpr Color KRedWall = {
    .r = 200 / 255.0f, .g = 45 / 255.0f, .b = 45 / 255.0f, .a = 1.0f}; // Vibrant Red
constexpr Color KIceBlueWall = {
    .r = 065 / 255.0f, .g = 122 / 255.0f, .b = 145 / 255.0f, .a = 1.0f}; // Frozen Ice Blue
constexpr Color KDefaultWhite = {.r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f};

enum ColorsPresets {
    MudFloor_ID = 0,  // Walkable floor
    StoneWall_ID = 1, // Boundary walls
    RedWall_ID = 2,   // Red obstacles
    IceBlue_ID = 3,   // Ice Blue obstacles
};

// 0 -> MudFloor (de_dust sand)
// 1 -> Stone grey
// 2 -> Red
// 3 -> Ice blue
const int KMap[KMapHeight][KMapWidth] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1}, {1, 0, 2, 0, 0, 0, 3, 0, 2, 2, 2, 1},
    {1, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 1}, {1, 0, 2, 2, 2, 2, 0, 0, 0, 2, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 2, 2, 2, 0, 3, 0, 0, 0, 1}, {1, 0, 0, 2, 0, 0, 0, 3, 3, 3, 3, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

// const int KMap[KMapHeight][KMapWidth] = {
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1},
//
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1},
//
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
// };
