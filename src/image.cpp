#include "image.h"

#include <iostream>
#include <limits>

Image::Image(int width, int height) : width(width), height(height), pixels(width * height) {}

auto Image::makeVoronoi(int cells, const Palette& palette, Style style) -> void {
    generateRegionPoints(cells);
    generateRegionColors(palette);
    fillRegions(style);
}

auto Image::write(std::string_view filename) const -> void {
    std::ofstream fstream(filename.data());
    writeHeader(fstream);
    writePixels(fstream);
}

auto Image::generateRegionPoints(int num_regions) -> void {
    for (int i {}; i < num_regions; ++i) {
        auto point { generateRandomPoint(width, height) };
        points.push_back(point);
    }
}

auto Image::generateRegionColors(const Palette& palette) -> void {
    for (auto& point : points) {
        point.color = generateRandomColor(palette);
    }
}

auto Image::fillRegions(Style style) -> void {
    for (int pixel_index {}; pixel_index < width * height; ++pixel_index) {
        Point current_pixel { pixel_index % width, pixel_index / width };

        Point closest_point { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
        double closest_distance { std::numeric_limits<double>::max() };

        for (const auto& point : points) {
            double current_distance { calculateDistance(style, current_pixel, point) };

            if (current_distance < closest_distance) {
                closest_distance = current_distance;
                closest_point = point;
            }
        }

        draw(pixel_index, closest_point.color);
    }
}

auto Image::calculateDistance(Style style, const Point& from_point, const Point& to_point) -> double {
    switch (style) {
        case Style::euclidean : return euclideanDistance(from_point, to_point);
        case Style::manhattan : return manhattanDistance(from_point, to_point);
        case Style::minkowski : return minkowskiDistance(from_point, to_point);
        default : return euclideanDistance(from_point, to_point);
    }
}
auto Image::manhattanDistance(const Point& from_point, const Point& to_point) -> double {
    double distance { static_cast<double>(abs(to_point.x - from_point.x) + abs(to_point.y - from_point.y)) };
    return distance;
}

auto Image::euclideanDistance(const Point& from_point, const Point& to_point) -> double {
    double distance { sqrt(pow(to_point.x - from_point.x, 2) + pow(to_point.y - from_point.y, 2)) };
    return distance;
}

auto Image::minkowskiDistance(const Point& from_point, const Point& to_point) -> double {
    constexpr double p { 0.66 };
    double distance { pow(pow(abs(to_point.x - from_point.x), p) + pow(abs(to_point.y - from_point.y), p), (1/p)) };
    return distance;
}

auto Image::draw(const Point& point) -> void {
    draw(point.y * width + point.x, point.color);
}

auto Image::draw(int pixel_index, Color color) -> void {
    pixels[pixel_index] = color;
}

auto Image::writeHeader(std::ofstream& fstream) const -> void {
    fstream << "P3\n" << width << ' ' << height << "\n255\n";
}

auto Image::writePixels(std::ofstream& fstream) const -> void {
    for (const Color& pixel : pixels) {
        fstream << static_cast<int>(pixel.r) << " "
                << static_cast<int>(pixel.g) << " "
                << static_cast<int>(pixel.b) << '\n';
    }
}

auto getRandomColorFromPalette(const Palette& palette) -> Color {
    const auto index { generateRandomNumber<int>(0, palette.size()) };
    return palette[index];
}

auto generateRandomColor(const Palette& palette) -> Color {
    if (palette.empty()) {
        const auto gray { generateRandomNumber<uint8_t>(0, 255) };
        return { gray, gray, gray };
    }

    return getRandomColorFromPalette(palette);
}

auto generateRandomPoint(int width, int height) -> Point {
    const int x { generateRandomNumber(0, width) };
    const int y { generateRandomNumber(0, height) };
    return { x, y };
}

auto print(const Point& point) -> void {
    std::cout << "{" << point.x << ", " << point.y << ", " << "} "
              << "{" << point.color.r << ", " << point.color.g << ", " << point.color.b << "}" << std::endl;
}
