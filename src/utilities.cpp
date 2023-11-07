#include "utilities.h"
#include "argument_parser.h"

const Palette default_palette {{ {116, 0, 184}, {105, 48, 195}, {94, 96, 206}, {83, 144, 217}, {78, 168, 222}, {72, 191, 227}, {86, 207, 225}, {100, 223, 223}, {114, 239, 221}, {128, 255, 219} }};
const std::string default_filename { "image.ppm" };

auto handleArguments(int argc, char** argv) -> std::tuple<int, int, int, Palette, Style, std::string> {
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
    const auto output { arg_parser.getOptional<std::string>("o", default_filename) };
    if (!palette_str || !style_str || !output) {
        std::cerr << makeErrorMessage("The optional arguments could not be parsed") << std::endl;
        std::exit(-1);
    }

    const Palette palette { parsePalette(*palette_str)};
    const Style style { parseStyle(*style_str) };

    return { *width, *height, *cells, palette, style, *output };
}

auto makeErrorMessage(const std::string& message) -> std::string {
    return message + ". For help, use -h.";
}

auto makeUsageMessage(const std::string& program) -> std::string {
    const std::string usage("Usage: " + std::string(program) + " <width> <height> <cells> [-p palette] [-s style] [-o output]\n"
                            "    width: desired image width\n"
                            "    height: desired image height\n"
                            "    cells: desired number of voronoi cells\n"
                            "    -p palette: where palette is a file containing one color per line in rgb, e.g. 255 0 127\n"
                            "    -s style: where style is one of the following: (ma)nhattan (default), (eu)clidean, or (mi)nkowski\n"
                            "    -o output: where output is the name of the generated image file. Defaults to image.ppm\n");
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
        if (line.size() != 7 || line[0] != '#') {
            std::cerr << makeErrorMessage("Invalid hex color code: " + line) << std::endl;
            std::exit(-1);
        }

        int r, g, b;
        try {
            r = std::stoi(line.substr(1, 2), nullptr, 16);
            g = std::stoi(line.substr(3, 2), nullptr, 16);
            b = std::stoi(line.substr(5, 2), nullptr, 16);
        } catch (const std::invalid_argument& ia) {
            std::cerr << makeErrorMessage("Cannot parse hex color code: " + line) << std::endl;
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

auto getRandomColorFromPalette(const Palette& palette) -> Color {
    const auto index { generateRandomNumber<int>(0, palette.size()) };
    return palette[index];
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
