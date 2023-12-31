#include "image.h"

#include <iostream>
#include <limits>
#include <thread>

Image::Image(int width, int height) : width(width), height(height), pixels(width * height) {}

auto Image::makeVoronoi(int cells, const Palette& palette, Style style) -> void {
    generateRegionPoints(cells);
    generateRegionColors(palette);
    fillRegions(style);
}

auto Image::generateRegionPoints(int regions) -> void {
    points.reserve(regions);
    for (int i {}; i < regions; ++i) {
        points.emplace_back(generateRandomPoint(width, height));
    }
}

auto Image::generateRegionColors(const Palette& palette) -> void {
    for (auto& point : points) {
        point.color = getRandomColorFromPalette(palette);
    }
}

// TODO break this function up and use async
auto Image::fillRegions(Style style) -> void {
    const auto threads { static_cast<int>(std::thread::hardware_concurrency()) };
    std::vector<std::thread> thread_pool;
    thread_pool.reserve(threads);

    const auto pixels_per_thread { (width * height) / threads };
    for (int i {}; i < threads; ++i) {
        const auto begin_pixel { i * pixels_per_thread };
        const auto end_pixel { (i == threads - 1) ? (width * height) : (i + 1) * pixels_per_thread };

        thread_pool.emplace_back([&, i, begin_pixel, end_pixel]{
            for (auto pixel_index { begin_pixel }; pixel_index < end_pixel; ++pixel_index) {
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
        });
    }

    for (auto& thread : thread_pool) {
        thread.join();
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
    return static_cast<double>(abs(to_point.x - from_point.x) + abs(to_point.y - from_point.y));
}

auto Image::euclideanDistance(const Point& from_point, const Point& to_point) -> double {
    return sqrt(pow(to_point.x - from_point.x, 2) + pow(to_point.y - from_point.y, 2));
}

auto Image::minkowskiDistance(const Point& from_point, const Point& to_point) -> double {
    constexpr double p { 0.66 };
    return pow(pow(abs(to_point.x - from_point.x), p) + pow(abs(to_point.y - from_point.y), p), (1/p));
}

auto Image::draw(const Point& point) -> void {
    draw(point.y * width + point.x, point.color);
}

auto Image::draw(int pixel_index, Color color) -> void {
    pixels[pixel_index] = color;
}

auto Image::write(std::string_view filename) const -> void {
    std::ofstream fstream(filename.data());
    writeHeader(fstream);
    writePixels(fstream);
}

auto Image::writeHeader(std::ofstream& fstream) const -> void {
    fstream << "P3\n" << width << ' ' << height << "\n255\n";
}

auto Image::writePixels(std::ofstream& fstream) const -> void {
    for (Color pixel : pixels) {
        fstream << static_cast<int>(pixel.r) << " "
                << static_cast<int>(pixel.g) << " "
                << static_cast<int>(pixel.b) << '\n';
    }
}