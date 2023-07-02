#pragma once

#include "utilities.h"

#include <vector>
#include <string_view>
#include <fstream>


class Image {
public:
    Image(int width, int height);
    auto makeVoronoi(int cells, const Palette& palette, Style style) -> void;
    auto write(std::string_view filename) const -> void;

public:
    const int width;
    const int height;

private:
    auto generateRegionPoints(int regions) -> void;
    auto generateRegionColors(const Palette& palette) -> void;
    auto fillRegions(Style style) -> void;

    auto calculateDistance(Style style, const Point& from_point, const Point& to_point) -> double;
    auto manhattanDistance(const Point& from_point, const Point& to_point) -> double;
    auto euclideanDistance(const Point& from_point, const Point& to_point) -> double;
    auto minkowskiDistance(const Point& from_point, const Point& to_point) -> double;

    auto draw(const Point& point) -> void;
    auto draw(int i, Color color) -> void;

    auto writeHeader(std::ofstream& fstream) const -> void;
    auto writePixels(std::ofstream& fstream) const -> void;

private:
    std::vector<Point> points;
    std::vector<Color> pixels;
};