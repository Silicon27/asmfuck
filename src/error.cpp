//
// Created by David Yang on 2025-04-24.
//
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <map>
#include <format>
#include <sstream>

#include "headers/lexer.h"
#include "headers/error.h"


void ErrorPack::augment(const tcomp::Error &error) {
    this->errors.push_back(error);
}

void ErrorPack::merge(const ErrorPack &error_pack) {
    this->errors.insert(this->errors.end(), error_pack.errors.begin(), error_pack.errors.end());
}


std::unordered_map<tcomp::ErrorType, std::string> errorTypes = {
    {tcomp::ErrorType::RUNTIME_ERROR, "Runtime Error Occured At {}:{}, in file {}",},
    {tcomp::ErrorType::SYNTAX_ERROR, "Syntax Error Occured At {}:{}, in file {}",},
};


std::string getErrorType(const tcomp::ErrorType type, const tcomp::Error error) {
    return std::vformat(errorTypes.at(type), std::make_format_args(
        error.line,
        error.column,
        error.filepath
    ));
}

ErrorHandler::ErrorHandler(ErrorPack &errors, std::vector<Token> unfiltered_tokens, std::map<int, std::string> unfiltered_lines) : errors_(errors), unfiltered_tokens_(std::move(unfiltered_tokens)), unfiltered_lines_(std::move(unfiltered_lines)) {}

void ErrorHandler::handle() {
    for (auto &err : errors_.errors) {
        std::cout << getErrorType(err.type, err) << std::endl;
    }
    if (errors_.errors.size() > 0) {
        std::terminate();
    }
}
