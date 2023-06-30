#include "argument_parser.h"
#include "common.h"
#include "image.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

auto handleArguments(int argc, char** argv) -> std::tuple<int, int, int, Palette, Style>;
auto makeErrorMessage(const std::string& message) -> std::string;
auto makeUsageMessage(const std::string& program) -> std::string;

auto parsePalette(const std::string& filename) -> Palette;
auto openFile(const std::string& filename) -> std::ifstream;
auto createPaletteFromFile(std::ifstream& file) -> Palette;

auto parseStyle(const std::string& style) -> Style;


auto main(int argc, char** argv) -> int {
    const auto [ width, height, cells, palette, style ] = handleArguments(argc, argv);
    Image image { width, height };
    image.makeVoronoi(cells, palette, style);
}

auto handleArguments(int argc, char** argv) -> std::tuple<int, int, int, Palette, Style> {
    ArgumentParser arg_parser;
    if (!arg_parser.init(argc, argv)) {
        std::cerr << makeErrorMessage("The arguments could not be parsed") << std::endl;
        std::exit(-1);
    }

    if (arg_parser.helpRequested()) {
        std::cout << makeUsageMessage(argv[0]) << std::endl;
        std::exit(0);
    }

    const auto width { arg_parser.get<int>(0) };
    const auto height { arg_parser.get<int>(1) };
    const auto cells { arg_parser.get<int>(2) };
    if (!width || !height || !cells) {
        std::cerr << makeErrorMessage("The required arguments could not be parsed") << std::endl;
        std::exit(-1);
    }

    const auto palette_str { arg_parser.getOptional<std::string>("p", "") };
    const auto style_str { arg_parser.getOptional<std::string>("s", "") };
    if (!palette_str || !style_str) {
        std::cerr << makeErrorMessage("The optional arguments could not be parsed") << std::endl;
        std::exit(-1);
    }

    const Palette palette { parsePalette(*palette_str)};
    const Style style { parseStyle(*style_str) };

    return { *width, *height, *cells, palette, style };
}

auto makeErrorMessage(const std::string& message) -> std::string {
    return message + ". For help, use -h.";
}

auto makeUsageMessage(const std::string& program) -> std::string {
    const std::string usage("Usage: " + std::string(program) + " width height cells [-p palette] [-s style]\n"
                            "    width: desired image width\n"
                            "    height: desired image height\n"
                            "    cells: desired number of voronoi cells\n"
                            "    -p palette: where palette is a file containing one color per line in rgb, e.g. 255 0 127\n"
                            "    -s style: where style is one of the following: (ma)nhattan (default), (eu)clidean, or (mi)nkowski\n");
    return usage;
}

auto parsePalette(const std::string& filename) -> Palette {
    if (filename.empty()) {
        return default_palette;
    }

    auto file { openFile(filename) };
    return createPaletteFromFile(file);
}

auto openFile(const std::string& filename) -> std::ifstream {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << makeErrorMessage("Cannot open file " + filename) << std::endl;
        std::exit(-1);
    }

    return file;
}

auto createPaletteFromFile(std::ifstream& file) -> Palette {
    Palette palette;

    std::string line;
    while (std::getline(file, line)) {
        auto iss = std::istringstream { line };

        int r, g, b;
        if (!(iss >> r >> g >> b)) {
            std::cerr << makeErrorMessage("Cannot parse colors: " + line) << std::endl;
            std::exit(-1);
        }

        Color color { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b) };
        palette.push_back(color);
    }

    return palette;
}

auto parseStyle(const std::string& style) -> Style {
    if (style.empty() || style == "ma") {
        return Style::manhattan;
    } else if (style == "eu") {
        return Style::euclidean;
    } else if (style == "mi") {
        return Style::minkowski;
    } else {
        std::cerr << makeErrorMessage(style + " is an invalid style") << std::endl;
        std::exit(-1);
    }
}