//
// Created by David Yang on 2025-05-06.
//

#pragma once

struct Variable {
    std::string name;
    tc_Bitset bitset;

    Variable() : bitset("0") {}
    Variable(std::string name, tc_Bitset bitset) : name(std::move(name)), bitset(std::move(bitset)) {}
};

struct Array {
    std::vector<tc_Bitset> variables;

    Array() = default;
};

struct Collection {
    std::shared_ptr<AST> collection_head;

    Collection() = default;
};

using SymbolInfo = std::variant<Variable, Array, Collection>;

namespace sem_analysis {
    [[nodiscard]] char binary_to_char(const std::string &binary);
    [[nodiscard]] int64_t binary_to_int64_t(const std::string &binary, bool is_signed = false);
    [[nodiscard]] std::string int_to_binary(int64_t value);

    class SemanticAnalyser {
    public:
        explicit SemanticAnalyser(std::shared_ptr<ProgramNode> program, std::string filename = "");
        SemanticAnalyser(std::shared_ptr<ProgramNode> program, std::unordered_map<std::string, SymbolInfo> symbol_table, std::string filename = "");

        ~SemanticAnalyser();

        void analyze();

    protected:
        std::unordered_map<std::string, SymbolInfo> symbol_table;

    private:
        std::shared_ptr<ProgramNode> program_;
        ErrorPack error_pack;
        std::string filename;
    };
}
