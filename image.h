#pragma once

#include "common.h"
#include <vector>
#include <random>

class Image {
public:
    Image(int width, int height);
    auto makeVoronoi(int cells, const Palette& palette, Style style) -> void;

public:
    const int width;
    const int height;

private:
    auto generateRegionPoints(int num_regions) -> void;
    auto generateRegionColors(const Palette& palette) -> void;
    auto fillRegions(Style style) -> void;
    auto generate() -> void;

    auto calculateDistance(Style style, const Point& from_point, const Point& to_point) -> double;
    auto manhattanDistance(const Point& from_point, const Point& to_point) -> double;
    auto euclideanDistance(const Point& from_point, const Point& to_point) -> double;
    auto minkowskiDistance(const Point& from_point, const Point& to_point) -> double;

    auto draw(const Point& point) -> void;
    auto draw(int i, Color color) -> void;

    auto outputHeader() const -> void;
    auto outputPixels() const -> void;
    auto outputPixel(const Color& color) const -> void;

private:
    std::vector<Point> points;
    std::vector<Color> pixels;
};

template <typename T>
auto generateRandomNumber(T min, T max) -> T {
    std::random_device rd;
    std::uniform_int_distribution<T> dist(min, max - 1);
    return dist(rd);
}

auto generateRandomPoint(int width, int height) -> Point;
auto generateRandomColor(const Palette& palette) -> Color;
auto getRandomColorFromPalette(const Palette& palette) -> Color;