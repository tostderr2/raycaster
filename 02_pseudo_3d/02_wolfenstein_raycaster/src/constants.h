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
constexpr Color teal = {
    .r = 145 / 255.0f,
    .g = 217 / 255.0f,
    .b = 214 / 255.0f,
    .a = 1.0f,
};

constexpr size_t WIN_WD_G = 1080;
constexpr size_t WIN_HT_G = 720;

constexpr size_t MAP_WD_G = 12;
constexpr size_t MAP_HT_G = 12;
constexpr size_t DRAW_BOX_WD_G = WIN_WD_G / MAP_WD_G;
constexpr size_t DRAW_BOX_HT_G = WIN_HT_G / MAP_HT_G;

constexpr float MAP_SPACE_STEP_RATIO_X = MAP_WD_G / (float)WIN_WD_G;
constexpr float MAP_SPACE_STEP_RATIO_Y = MAP_HT_G / (float)WIN_HT_G;

const int MAP_G[MAP_HT_G][MAP_WD_G] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},

    {1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1},

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
