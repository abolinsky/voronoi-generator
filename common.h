#pragma once

#include <vector>

struct Color {
    uint8_t r {};
    uint8_t g {};
    uint8_t b {};
};

using Palette = std::vector<Color>;
const Palette default_palette {{ {116, 0, 184}, {105, 48, 195}, {94, 96, 206}, {83, 144, 217}, {78, 168, 222}, {72, 191, 227}, {86, 207, 225}, {100, 223, 223}, {114, 239, 221}, {128, 255, 219} }};

struct Point {
    int x {};
    int y {};
    Color color {};
};

auto print(const Point& point) -> void;

enum class Style { euclidean, manhattan, minkowski };