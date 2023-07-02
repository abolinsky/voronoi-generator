#pragma once

#include <vector>
#include <tuple>
#include <string>
#include <fstream>
#include <random>

struct Color {
    uint8_t r {};
    uint8_t g {};
    uint8_t b {};
};

using Palette = std::vector<Color>;


struct Point {
    int x {};
    int y {};
    Color color {};
};

auto print(const Point& point) -> void;


enum class Style { euclidean, manhattan, minkowski };


auto handleArguments(int argc, char** argv) -> std::tuple<int, int, int, Palette, Style, std::string>;

auto makeErrorMessage(const std::string& message) -> std::string;
auto makeUsageMessage(const std::string& program) -> std::string;

auto openFile(const std::string& filename) -> std::ifstream;
auto createPaletteFromFile(std::ifstream& file) -> Palette;
auto parsePalette(const std::string& filename) -> Palette;
auto parseStyle(const std::string& style) -> Style;

template <typename T>
auto generateRandomNumber(T min, T max) -> T {
    std::random_device rd;
    std::uniform_int_distribution<T> dist(min, max - 1);
    return dist(rd);
}

auto generateRandomPoint(int width, int height) -> Point;
auto getRandomColorFromPalette(const Palette& palette) -> Color;