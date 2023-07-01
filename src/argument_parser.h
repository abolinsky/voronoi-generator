#pragma once

#include <sstream>
#include <string_view>
#include <iostream>
#include <optional>
#include <vector>
#include <unordered_map>

class ArgumentParser {
public:
    ArgumentParser() {};

public:
    auto init(int argc, char **argv) -> bool {
        this->argc = argc;

        for (int arg_index { 1 }; arg_index < argc; ++arg_index) {
            std::string_view argument { argv[arg_index] };

            if (requestingHelp(argument)) {
                help_requested = true;
                return true;
            }

            if (isOption(argument)) {
                const int next_arg_index { arg_index + 1 };

                std::string_view option { argument.substr(1) };
                if (next_arg_index >= argc) {
                    return false;
                } else {
                    std::string_view operand { argv[next_arg_index] };
                    optional[option] = operand;
                }

                ++arg_index;
            } else {
                required.push_back(argument);
            }
        }

        return true;
    }

    template <typename T>
    auto get(int index) const noexcept -> auto {
        if (argc == 1 || index >= (argc - 1)) {
            return std::optional<T>{};
        }

        return extractArg<T>(required[index]);
    }

    template <typename T>
    auto getOptional(std::string_view option, const T& default_value) const noexcept -> auto {
        if (optional.contains(option)) {
            return extractArg<T>(optional.at(option));
        } else {
            return std::optional<T>{ default_value };
        }
    }

    auto helpRequested() const noexcept -> bool {
        return help_requested;
    }

private:
    template <typename T>
    auto extractArg(std::string_view original_arg) const noexcept -> auto {
        T arg {};
        std::istringstream iss { original_arg.data() };
        if(!(iss >> arg)) {
            return std::optional<T>{};
        }
        return std::optional<T>{ arg };
    }

    auto requestingHelp(std::string_view argument) const noexcept -> bool {
        return argument == "-h" ||
               argument == "-help" ||
               argument == "--h" ||
               argument == "--help";
    }

    auto isOption(std::string_view argument) const noexcept -> bool{
        return argument.starts_with('-');
    }

private:
    int argc { 0 };
    bool help_requested { false };
    std::vector<std::string_view> required;
    std::unordered_map<std::string_view, std::string_view> optional;
};
