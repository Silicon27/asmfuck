//
// Created by David Yang on 2025-05-06.
//

#include <unordered_map>
#include <map>
#include <string>
#include <iostream>
#include <utility>
#include <variant>
#include <unordered_set>
#include <vector>



#include "headers/exprtk.hpp"
#include "headers/ast.h"
#include "headers/lexer.h"
#include "headers/error.h"
#include "headers/semantic_analysis.h"
#include "headers/tools.h"


// NOTICE ================ MSB needs to be considered for all bit to int conversions

[[nodiscard]] char sem_analysis::binary_to_char(const std::string &binary) {
    return static_cast<char>(std::bitset<8>(binary).to_ulong());
}

#include <string>
#include <stdexcept>
#include <cstdint>

[[nodiscard]] int64_t sem_analysis::binary_to_int64_t(const std::string &binary, bool is_signed) {
    if (binary.empty() || binary.find_first_not_of("01") != std::string::npos) {
        throw std::invalid_argument("Input must be a non-empty binary string containing only '0' and '1'.");
    }

    const size_t n = binary.length();

    // Convert binary string to unsigned 64-bit integer
    uint64_t unsigned_value = 0;
    for (char bit : binary) {
        unsigned_value = (unsigned_value << 1) | (bit - '0');
    }

    // Handle signed interpretation using two's complement
    if (is_signed && binary[0] == '1') {
        int64_t signed_value = static_cast<int64_t>(unsigned_value) - (1LL << n);
        return signed_value;
    }

    return static_cast<int64_t>(unsigned_value);
}

[[nodiscard]] std::string sem_analysis::int_to_binary(int64_t number) {
    if (number == 0) {
        return "0";
    }

    const bool isNegative = (number < 0);
    auto absNumber = static_cast<unsigned int>(std::abs(number));

    std::string binary;
    while (absNumber > 0) {
        binary.insert(binary.begin(), (absNumber % 2) ? '1' : '0');
        absNumber /= 2;
    }

    if (isNegative) {
        binary.insert(binary.begin(), '1');
    } else {
        binary.insert(binary.begin(), '0');
    }

    return binary;
}


sem_analysis::SemanticAnalyser::SemanticAnalyser(std::shared_ptr<ProgramNode> program, std::string filename) : program_(std::move(program)), filename(std::move(filename)) {}
sem_analysis::SemanticAnalyser::SemanticAnalyser(std::shared_ptr<ProgramNode> program, std::unordered_map<std::string, SymbolInfo> symbol_table, std::string filename) : symbol_table(std::move(symbol_table)), program_(std::move(program)), filename(std::move(filename)) {}

sem_analysis::SemanticAnalyser::~SemanticAnalyser() = default;

void sem_analysis::SemanticAnalyser::analyze() {
    // perform semantic analysis on the constructed tree
    for (std::shared_ptr node : this->program_->getChildren()) {
        WhichVisitor which_visitor;
        node->accept(&which_visitor);
        if (which_visitor.visitor_type_name == "ExprVariableNode") {
            VariableValueGetterVisitor visitor("_DEF_VAL", tc_Bitset("0"));
            node->accept(&visitor);

            std::string name = visitor.getName();
            tc_Bitset value = visitor.getValue();

            Variable var = {name, value};
            this->symbol_table.emplace(name, SymbolInfo(var));
        } else if (which_visitor.visitor_type_name == "StmtOutputNode") {
            OutputGetterVisitor visitor;
            node->accept(&visitor);

            std::string name = visitor.getName();

            if (std::holds_alternative<Variable>(this->symbol_table[name])) {
                Variable var = std::get<Variable>(this->symbol_table[name]);

                if (visitor.getOutputAsNormal()) {
                    std::cout << binary_to_int64_t(var.bitset.get_bits(), true) << std::endl;
                } else {
                    std::cout << var.bitset.get_bits() << std::endl;
                }

            } else if (std::holds_alternative<Array>(this->symbol_table[name])) {
                Array arr = std::get<Array>(this->symbol_table[name]);

                std::string constructed_string;
                // convert all bits to chars to print a string
                if (visitor.getOutputAsNormal()) {
                    for (const auto &val : arr.variables) {
                        constructed_string += std::to_string(binary_to_int64_t(val.get_bits(), true)) + " ";
                    }
                } else {
                    for (const auto &val : arr.variables) {
                        if (val.get_bits().length() != 8) {
                            this->error_pack.augment(tcomp::Error{
                                .filepath = this->filename,
                                .type = tcomp::ErrorType::SEMANTIC_ERROR,
                                .Xmessage = "Array variable is not 8 bits",
                                .line = 0,
                                .column = 0
                            });
                            continue;
                        }

                        constructed_string += binary_to_char(val.get_bits());
                    }
                }
                std::cout << constructed_string << '\n';

            }
        } else if (which_visitor.visitor_type_name == "StmtArrayNode") {
            ArrayNameManagementVisitor visitor;
            node->accept(&visitor);

            std::string name = visitor.getName();

            Array arr;
            if (this->symbol_table.contains(name)) {
                arr = std::get<Array>(this->symbol_table[name]);
            } else {
            }

            for (const auto &child : node->getChildren()) {
                IdentifierNameGetterVisitor var_visitor;
                child->accept(&var_visitor);

                std::string var_name = var_visitor.getName();

                tc_Bitset value = std::get<Variable>(this->symbol_table[var_name]).bitset;
                arr.variables.push_back(value);
            }

            /*
             * Optimise later, this code is fucking disgusting,
             * and we could have just gotten the reference to
             * the existing array in the for loop above
             *
             * If we did have the reference, we would not need to overwrite the existing key-value pair
             */
            this->symbol_table[name] = SymbolInfo(arr);
        } else if (which_visitor.getVisitorTypeName() == "StmtLoopNode")  {
            LoopIterationCountGetterVisitor visitor;
            node->accept(&visitor);
            int64_t iteration_count = 0;
            if (auto it = this->symbol_table.find(visitor.getName()); it != this->symbol_table.end()) {
                iteration_count = binary_to_int64_t(std::get<Variable>(it->second).bitset.get_bits());
            } else {
                throw std::runtime_error("Variable not found in symbol table: " + visitor.getName());
            }

            if (iteration_count < 0) {
                this->error_pack.augment(tcomp::Error{
                    .filepath = this->filename,
                    .type = tcomp::ErrorType::SEMANTIC_ERROR,
                    .Xmessage = "Iteration count cannot be negative",
                    .line = 0,
                    .column = 0
                });
            }

            std::shared_ptr<ProgramNode> sub_program = std::make_shared<ProgramNode>();
            for (auto node_children : node->getChildren()) {
                sub_program->addChild(node_children);
            }

            SemanticAnalyser sub_analyser(sub_program, this->symbol_table, this->filename);
            for (; iteration_count > 0; --iteration_count) {
                sub_analyser.analyze();
            }
            this->symbol_table = sub_analyser.symbol_table;
            this->error_pack.merge(sub_analyser.error_pack);
        } else if (which_visitor.getVisitorTypeName() == "ExprEvaluateNode") {
            EvaluateNodeExpressionGetterVisitor visitor;
            node->accept(&visitor);
            std::string expression = visitor.getExpression();
            auto variables = visitor.getVariables();
            std::vector<std::string> variable_values;
            for (const auto &var : variables) {
                if (this->symbol_table.contains(var)) {
                    variable_values.push_back(std::to_string(binary_to_int64_t(std::get<Variable>(this->symbol_table[var]).bitset.get_bits(), true)));
                } else {
                    // TODO impl cutoff errors, which when triggered would terminate the program immediately
                    this->error_pack.augment(tcomp::Error{
                        .filepath = this->filename,
                        .type = tcomp::ErrorType::SEMANTIC_ERROR,
                        .Xmessage = "Variable not found in symbol table: " + var,
                        .line = 0,
                        .column = 0
                    });
                }
            }

            expression = asmfmt::rformat(expression, variable_values);

            exprtk::expression<double> expr;
            exprtk::parser<double> parser;

            parser.compile(expression, expr);

            std::shared_ptr<AST> assignToNode = node->getChildren()[0]; // TODO add multi assignment in the future

            IdentifierNameGetterVisitor assignToVisitor;
            assignToNode->accept(&assignToVisitor);
            std::string assignToName = assignToVisitor.getName();

            if (this->symbol_table.contains(assignToName)) {
                Variable var = std::get<Variable>(this->symbol_table[assignToName]);
                var.bitset = tc_Bitset(int_to_binary(static_cast<int64_t>(expr.value())));
                this->symbol_table[assignToName] = SymbolInfo(var);
            } else {
                Variable new_var(assignToName, tc_Bitset(int_to_binary(static_cast<int64_t>(expr.value()))));
                this->symbol_table[assignToName] = SymbolInfo(new_var);
            }
        }
    }
}