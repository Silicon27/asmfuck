#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <csignal>
#include <thread>
#include <future>
#include <variant>
#include <optional>
#include <filesystem>
#include <unordered_map>
#include <cstdlib>
#include <ranges>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <memory>
#include <sstream>
#include <cctype>
#include <unordered_set>

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <unistd.h>
    #include <csignal>
#endif

#define TURING_COMPLETE_VER "1.0.0"

#define INTERPRETER_NAME "TComplete"

#define PARGS (pos, *tokens);


#include "src/headers/lexer.h"
#include "src/headers/error.h"
#include "src/headers/parser.h"
#include "src/headers/semantic_analysis.h"

int main(int argc, char *argv[]) {

    std::string input;
    int max_error_count = 20;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help")
            std::cout << INTERPRETER_NAME << std::endl;


        if (arg == "-v" || arg == "--version")
            std::cout << TURING_COMPLETE_VER << std::endl;

        if (arg == "-fmax_error_count")
            max_error_count = std::stoi(argv[++i]);


        else
            input = arg;
    }

    std::ifstream file(input);
    if (!file.is_open()) {
        std::cout << "File not found" << std::endl;
        return 1;
    }

    Lexer lexer(file);

    auto [tokens, unfilteredTokens, unfilteredLines] = lexer.tokenize();

    ErrorPack error_pack;

    Parser parser(input, tokens, unfilteredTokens, unfilteredLines, error_pack, max_error_count);

    parser.parse();

    std::shared_ptr<ProgramNode> Pn = parser.G_program();

    sem_analysis::SemanticAnalyser semantic_analyser(Pn, input);
    semantic_analyser.analyze();

    return 0;
}