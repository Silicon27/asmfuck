//
// Created by David Yang on 2025-04-24.
//

#pragma once


namespace tcomp {
    enum class ErrorType {
        RUNTIME_ERROR = 1,
        SYNTAX_ERROR = 2,
        SEMANTIC_ERROR = 3,
        PARSER_ERROR = 4,
    };

    struct Error {
        std::string filepath;
        ErrorType type;
        std::string Xmessage; // an extra message to be appended to the "tip" section of the error message
        int line;
        int column;
    };
}

struct ErrorPack {
    std::vector<tcomp::Error> errors;

    void augment(const tcomp::Error &error);

    void merge(const ErrorPack & error_pack);
};

std::string getErrorType(tcomp::ErrorType type, tcomp::Error error);

class ErrorHandler {
public:
    explicit ErrorHandler(ErrorPack &errors, std::vector<Token> unfiltered_tokens, std::map<int, std::string> unfiltered_lines);
    ~ErrorHandler() = default;

    void handle();

private:
    ErrorPack &errors_;
    std::vector<Token> unfiltered_tokens_;
    std::map<int, std::string> unfiltered_lines_;
};