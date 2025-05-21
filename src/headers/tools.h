//
// Created by David Yang on 2025-05-21.
//

#pragma once
#include <string>
#include <vector>

namespace asmfmt {
    /** formatting tools.
     * @brief Provides a set of tools for formatting strings and numbers.
     */

    [[nodiscard]] std::string rformat(std::string str, const std::vector<std::string>& args);
}