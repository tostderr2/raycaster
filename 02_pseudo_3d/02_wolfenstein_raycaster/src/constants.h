#pragma once

#include <cstddef>

float constexpr DEG_TO_RAD = 3.14159265f / 180.0f;

struct Color {
    float r;
    float g;
    float b;
    float a;
};

// color constants
constexpr Color KTeal = {
    .r = 145 / 255.0f,
    .g = 217 / 255.0f,
    .b = 214 / 255.0f,
    .a = 1.0f,
};

constexpr size_t KWinWidth = 1080;
constexpr size_t KWinHeight = 720;

constexpr size_t KMapWidth = 12;
constexpr size_t KMapHeight = 12;
constexpr size_t KDrawBoxWidth = KWinWidth / KMapWidth;
constexpr size_t KDrawBoxHeight = KWinHeight / KMapHeight;

constexpr float KMapSpaceStepRatioX = KMapWidth / (float)KWinWidth;
constexpr float KMapSpaceStepRatioY = KMapHeight / (float)KWinHeight;

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
const int KMap[KMapHeight][KMapWidth] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},

    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},

    {1, 0, 1, 0, 0, 0, 0, 0, 2, 2, 2, 1},

    {1, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 1},

    {1, 0, 1, 1, 1, 1, 0, 0, 0, 2, 0, 1},

    {1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 1},

    {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1},

    {1, 0, 0, 1, 1, 1, 0, 3, 0, 0, 0, 1},

    {1, 0, 0, 1, 0, 0, 0, 3, 3, 3, 3, 1},

    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},

    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
