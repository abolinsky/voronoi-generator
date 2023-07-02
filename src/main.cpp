#include "utilities.h"
#include "image.h"

auto main(int argc, char** argv) -> int {
    const auto [ width, height, cells, palette, style, filename ] = handleArguments(argc, argv);

    Image image { width, height };
    image.makeVoronoi(cells, palette, style);
    image.write(filename);
}