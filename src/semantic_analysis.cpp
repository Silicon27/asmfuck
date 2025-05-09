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



#include "headers/ast.h"
#include "headers/lexer.h"
#include "headers/error.h"
#include "headers/semantic_analysis.h"

[[nodiscard]] char sem_analysis::binary_to_char(const std::string &binary) {
    return static_cast<char>(std::bitset<8>(binary).to_ulong());
}


sem_analysis::SemanticAnalyser::SemanticAnalyser(std::shared_ptr<ProgramNode> program, std::string filename) : program_(std::move(program)), filename(std::move(filename)) {}

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
            OutputNameGetterVisitor visitor;
            node->accept(&visitor);

            std::string name = visitor.getName();

            if (std::holds_alternative<Variable>(this->symbol_table[name])) {
                Variable var = std::get<Variable>(this->symbol_table[name]);

                std::cout << var.bitset.get_bits() << std::endl;

            } else if (std::holds_alternative<Array>(this->symbol_table[name])) {
                Array arr = std::get<Array>(this->symbol_table[name]);

                std::string constructed_string;
                // convert all bits to chars to print a string
                for (const auto &var : arr.variables) {
                    if (var.bitset.get_bits().length() != 8) {
                        this->error_pack.augment(tcomp::Error{
                            .filepath = this->filename,
                            .type = tcomp::ErrorType::SEMANTIC_ERROR,
                            .Xmessage = "Array variable is not 8 bits",
                            .line = 0,
                            .column = 0
                        });
                        continue;
                    }

                    constructed_string += binary_to_char(var.bitset.get_bits());
                }
                std::cout << constructed_string << std::endl;

            }
        }
    }
}