//
// Created by David Yang on 2025-05-21.
//

#include "headers/tools.h"

std::string asmfmt::rformat(std::string str, const std::vector<std::string>& args) {
    for (const auto &arg : args) {
        size_t pos = str.find("{}");
        if (pos != std::string::npos) {
            str.replace(pos, 2, arg);
        }
    }
    return str;
}