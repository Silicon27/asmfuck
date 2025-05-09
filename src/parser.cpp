//
// Created by David Yang on 2025-04-21.
//


#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <variant>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <bitset>
#include <atomic>


#include "headers/lexer.h"
#include "headers/error.h"
#include "headers/parser.h"


// #[Parser::Parser<Impl>]
Parser::Parser(const std::string filename, std::vector<Token> tokens, std::vector<Token> unfilteredTokens, std::map<int, std::string> unfilteredLines, ErrorPack error_pack, int allowed_errors)
        : filename(filename), tokens(std::move(tokens)), unfilteredTokens(std::move(unfilteredTokens)), unfilteredLines(std::move(unfilteredLines)),  allowed_errors(allowed_errors), error_pack(error_pack)
{
    this->program = std::make_shared<ProgramNode>();
    this->currentNode = this->program;
}

// #[Getter<Impl>]
std::shared_ptr<ProgramNode> Parser::G_program() const {
    return this->program;
}

std::vector<Token> Parser::G_tokens() const {
    return this->tokens;
}

// #[Setters<Impl>]
void Parser::S_program(std::shared_ptr<ProgramNode> Pn) {
    this->program = std::move(Pn);
}





std::unique_ptr<Parser> Parser::createParser(const std::string filename, const std::vector<Token> &tokens,
    const std::vector<Token> &unfilteredTokens, const std::map<int, std::string> &unfilteredLines, ErrorPack &error_pack, int allowed_errors) {

    return std::make_unique<Parser>(filename, tokens, unfilteredTokens, unfilteredLines, error_pack, allowed_errors);
}

// #[Parser::Generic_pc<tToken>::Generic_pc]
template <char const* tToken>
Parser::Generic_pc<tToken>::Generic_pc(int &pos, const std::vector<Token> &tokens)
    : pos(pos), tokens(tokens), error() {
}

template <char const* tToken>
bool Parser::Generic_pc<tToken>::operator()() {
    if (pos < tokens.size() && tokens[pos].value == tToken) {
        ++pos;
        return true;
    }
    return false;
}

template <char const* tToken>
void Parser::Generic_pc<tToken>::operator|=(tcomp::Error &&error) {
    this->error = error;
}


template <char const* tToken>
void Parser::consume(Generic_pc<tToken> &parser) {
    if (!parser()) {
        error_pack.augment(parser.getError());
    }   
}

bool Parser::IsDigit::check(const std::string& value) {
    return !value.empty() && std::ranges::all_of(value, ::isdigit);
}

/// predicate function to check if the token is a digit
bool Parser::IsDigit::predicate(const Token& token) {
    return check(token.value);
}

bool Parser::more_than_allowed_errors() const {
    return error_pack.errors.size() > allowed_errors;
}


void Parser::parse_variable(int &pos) {
    Generic_pc<parser_constants::TOKEN_LEFT_BRACKET> left_bracket(pos, tokens);
    Generic_pc<parser_constants::TOKEN_RIGHT_BRACKET> right_bracket(pos, tokens);
    Generic_pc<parser_constants::TOKEN_PLUS> plus(pos, tokens);
    Generic_pc<parser_constants::TOKEN_MINUS> minus(pos, tokens);
    Generic_pc<parser_constants::TOKEN_EQ_ARROW> eq_arrow(pos, tokens);
    
    left_bracket |= tcomp::Error{
        .filepath = this->filename,
        .type = tcomp::ErrorType::SYNTAX_ERROR,
        .Xmessage = "Expected '['",
        .line = tokens[pos].line,
        .column = tokens[pos].column
    };
    right_bracket |= tcomp::Error{
        .filepath = this->filename,
        .type = tcomp::ErrorType::SYNTAX_ERROR,
        .Xmessage = "Expected ']'",
        .line = tokens[pos].line,
        .column = tokens[pos].column
    };
    plus |= tcomp::Error{
        .filepath = this->filename,
        .type = tcomp::ErrorType::SYNTAX_ERROR,
        .Xmessage = "Expected '+'",
        .line = tokens[pos].line,
        .column = tokens[pos].column
    };
    minus |= tcomp::Error{
        .filepath = this->filename,
        .type = tcomp::ErrorType::SYNTAX_ERROR,
        .Xmessage = "Expected '-'",
        .line = tokens[pos].line,
        .column = tokens[pos].column
    };
    eq_arrow |= tcomp::Error{
        .filepath = this->filename,
        .type = tcomp::ErrorType::SYNTAX_ERROR,
        .Xmessage = "Expected '=>'",
        .line = tokens[pos].line,
        .column = tokens[pos].column
    };

    this->consume(left_bracket);

    std::vector<bool> bits;

    while (pos < tokens.size()) {
        if (IsDigit::predicate(tokens[pos])) {
            int num = std::stoi(tokens[pos].value); pos++;

            if (plus()) {

                for (int i = 0; i < num; ++i) {
                    bits.push_back(true);
                }

            } else if (minus()) {

                for (int i = 0; i < num; ++i) {
                    bits.push_back(false);
                }

            } else {
                this->error_pack.augment(tcomp::Error{
                    .filepath = this->filename,
                    .type = tcomp::ErrorType::SYNTAX_ERROR,
                    .Xmessage = "Expected ']'",
                    .line = tokens[pos].line,
                    .column = tokens[pos].column
                });
                continue;
            }
        } if (plus()) {
            bits.push_back(true);
            continue;
        } if (minus()) {
            bits.push_back(false);
            continue;
        } if (right_bracket()) {
            break;
        }
    }

    // TODO make a critical_consume function that would instantly terminate on error if the consumed parser threw an error
    this->consume(eq_arrow);

    // #[Identifier]
    std::string name = tokens[pos].value; pos++;

    // TODO add better error recovery

    tc_Bitset bitset(bits);
    auto variable = std::make_shared<ExprVariableNode>();

    variable->bits = bitset;
    variable->name = name;

    // #[Program Node]
    this->currentNode->addChild(variable);
}

void Parser::parse_out(int &pos) {
    Generic_pc<parser_constants::TOKEN_LEFT_SHIFT> left_shift(pos, tokens);

    left_shift |= tcomp::Error{
        .filepath = this->filename,
        .type = tcomp::ErrorType::SYNTAX_ERROR,
        .Xmessage = "Expected ']'",
        .line = tokens[pos].line,
        .column = tokens[pos].column
    };

    auto outNode = std::make_shared<StmtOutputNode>();

    this->consume(left_shift);

    // #[Identifier]
    std::string name = tokens[pos].value; pos++;

    OutputNameAssignVisitor visitor(name);

    outNode->accept(&visitor);

    this->currentNode->addChild(outNode);
}

void Parser::parse_array(int &pos) {
    Generic_pc<parser_constants::TOKEN_LESS> less(pos, tokens);
    Generic_pc<parser_constants::TOKEN_BIGGER> bigger(pos, tokens);
    Generic_pc<parser_constants::TOKEN_COMMA> comma(pos, tokens);
    Generic_pc<parser_constants::TOKEN_EQ_ARROW> eq_arrow(pos, tokens);

    less |= tcomp::Error{
        .filepath = this->filename,
        .type = tcomp::ErrorType::SYNTAX_ERROR,
        .Xmessage = "Expected '<'",
        .line = tokens[pos].line,
        .column = tokens[pos].column
    };

    bigger |= tcomp::Error{
        .filepath = this->filename,
        .type = tcomp::ErrorType::SYNTAX_ERROR,
        .Xmessage = "Expected '>'",
        .line = tokens[pos].line,
        .column = tokens[pos].column
    };

    comma |= tcomp::Error{
        .filepath = this->filename,
        .type = tcomp::ErrorType::SYNTAX_ERROR,
        .Xmessage = "Expected ','",
        .line = tokens[pos].line,
        .column = tokens[pos].column
    };

    eq_arrow |= tcomp::Error{
        .filepath = this->filename,
        .type = tcomp::ErrorType::SYNTAX_ERROR,
        .Xmessage = "Expected '=>'",
        .line = tokens[pos].line,
        .column = tokens[pos].column
    };

    this->consume(less);

    auto arrayNode = std::make_shared<StmtArrayNode>();

    while (pos < tokens.size()) {
        if (tokens[pos].type != TokenType::IDENTIFIER) {
            this->error_pack.augment(tcomp::Error{
                .filepath = this->filename,
                .type = tcomp::ErrorType::SYNTAX_ERROR,
                .Xmessage = "Expected identifier",
                .line = tokens[pos].line,
                .column = tokens[pos].column
            });
        }

        IdentifierNameManagementVisitor visitor(tokens[pos].value);

        auto identifierNode = std::make_shared<ExprIdentifierNode>();

        identifierNode->accept(&visitor);

        arrayNode->addChild(identifierNode);

        pos++;


        if (comma()) {
            this->consume(comma);
            continue;
        } else if (bigger()) {
            break;
        }
    }

    this->consume(bigger);
    this->consume(eq_arrow);

    std::string name = tokens[pos].value; pos++;

    arrayNode->name = name;

    this->currentNode->addChild(arrayNode);

}


void Parser::parse() {
    for (int pos = 0; pos < tokens.size();) {
        const auto &token = tokens[pos];
        if (token.type == TokenType::SYMBOL) {
            if (token.value == "[")
                parse_variable(pos);
            else if (token.value == "<<")
                parse_out(pos);
            else if (token.value == "<")
                parse_array(pos);


        } else if (token.type == TokenType::KEYWORD) {
            // Handle identifier
        } else if (token.type == TokenType::eof) {
            break;
        }

        if (this->more_than_allowed_errors()) {
            std::cout << "Too many errors, stopping parsing." << std::endl;
            // TODO:  output collected errors
            break;
        }
    }

    ErrorHandler E_handler(this->error_pack, this->unfilteredTokens, this->unfilteredLines);
    E_handler.handle();
}