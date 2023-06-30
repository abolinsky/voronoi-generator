#include "common.h"

#include <iostream>

auto print(const Point& point) -> void {
    std::cout << "{" << point.x << ", " << point.y << ", " << "} "
              << "{" << point.color.r << ", " << point.color.g << ", " << point.color.b << "}" << std::endl;
}