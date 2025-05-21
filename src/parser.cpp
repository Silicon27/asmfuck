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

std::vector<Token> Parser::G_unfilteredTokens() const {
    return this->unfilteredTokens;
}

std::map<int, std::string> Parser::G_unfilteredLines() const {
    return this->unfilteredLines;
}

int Parser::G_allowed_errors() const {
    return this->allowed_errors;
}

bool Parser::G_handle_errors() const {
    return this->handle_errors;
}

ErrorPack Parser::G_error_pack() const {
    return this->error_pack;
}

std::shared_ptr<AST> Parser::G_currentNode() const {
    return this->currentNode;
}

// #[Setters<Impl>]
void Parser::S_program(std::shared_ptr<ProgramNode> Pn) {
    this->program = std::move(Pn);
}

void Parser::S_handle_errors(bool handle_errors) {
    this->handle_errors = handle_errors;
}


std::vector<Token> Parser::subarray_creator_from_scope(int &pos) {
        int amount = 0;
        const int initialPos = pos;
        do {
            if (this->tokens[pos].value == "{") {
                ++amount;
            } else if (this->tokens[pos].value == "}") {
                --amount;
            }
            ++pos;
        } while (amount != 0 && pos < static_cast<int>(this->tokens.size()));
        return {this->tokens.begin() + (initialPos+1), this->tokens.begin() + (pos-1)};
}



std::unique_ptr<Parser> Parser::createParser(const std::string filename, const std::vector<Token> &tokens,
    const std::vector<Token> &unfilteredTokens, const std::map<int, std::string> &unfilteredLines, ErrorPack &error_pack, int allowed_errors) {

    return std::make_unique<Parser>(filename, tokens, unfilteredTokens, unfilteredLines, error_pack, allowed_errors);
}

// #[Parser::Generic_pc<tToken>::Generic_pc]
template <char const* tToken>
Parser::Generic_pc<tToken>::Generic_pc(int &pos, const std::vector<Token> &tokens)
    : pos(pos), tokens(tokens), error() {
    this->operator|=(tcomp::Error{
        .filepath = "",
        .type = tcomp::ErrorType::SYNTAX_ERROR,
        .Xmessage = "Expected '" + std::string(tToken) + "'",
        .line = tokens[pos].line,
        .column = tokens[pos].column
    });
}

template <char const* tToken>
bool Parser::Generic_pc<tToken>::operator()() {
    if (pos < static_cast<int>(tokens.size()) && tokens[pos].value == tToken) {
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


template<char const *... tToken>
void Parser::consume_choice(std::vector<std::function<void()>> &choice_functions, Generic_pc<tToken>... parsers) {
    int i = 0;
    bool found = false;
    auto do_parser_stuff = [&i, &choice_functions, &found](const auto& parser) {
        if (!found && parser()) {
            choice_functions[i]();
            found = true;
        }
    };
    (do_parser_stuff(parsers), ...);
}


bool Parser::IsDigit::check(const std::string& value) {
    return !value.empty() && std::ranges::all_of(value, ::isdigit);
}

/// predicate function to check if the token is a digit
bool Parser::IsDigit::predicate(const Token& token) {
    return check(token.value);
}

bool Parser::more_than_allowed_errors() const {
    return static_cast<int>(error_pack.errors.size()) > allowed_errors;
}

std::string Parser::identifier_parser(int &pos) {
    if (tokens[pos].type == TokenType::IDENTIFIER) {
        ++pos;
        return tokens[pos-1].value;
    }
    this->error_pack.augment(tcomp::Error{
        .filepath = this->filename,
        .type = tcomp::ErrorType::SYNTAX_ERROR,
        .Xmessage = "Expected identifier",
        .line = tokens[pos].line,
        .column = tokens[pos].column
    });
    return "";
}



void Parser::parse_variable(int &pos) {
    Generic_pc<parser_constants::TOKEN_LEFT_BRACKET> left_bracket(pos, tokens);
    Generic_pc<parser_constants::TOKEN_RIGHT_BRACKET> right_bracket(pos, tokens);
    Generic_pc<parser_constants::TOKEN_PLUS> plus(pos, tokens);
    Generic_pc<parser_constants::TOKEN_MINUS> minus(pos, tokens);
    Generic_pc<parser_constants::TOKEN_EQ_ARROW> eq_arrow(pos, tokens);

    this->consume(left_bracket);

    std::vector<bool> bits;

    while (pos < static_cast<int>(tokens.size())) {
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
    std::string name = this->identifier_parser(pos);

    // TODO add better error recovery

    tc_Bitset bitset(bits);
    auto variable = std::make_shared<ExprVariableNode>();

    variable->bits = bitset;
    variable->name = name;

    // #[Program Node]
    this->currentNode->addChild(variable);
}

void Parser::parse_out(int &pos, bool output_as_normal) {
    Generic_pc<parser_constants::TOKEN_LEFT_SHIFT> left_shift(pos, tokens);
    Generic_pc<parser_constants::TOKEN_LEFT_SHIFT_AT> left_shift_at(pos, tokens);


    auto outNode = std::make_shared<StmtOutputNode>();

    if (output_as_normal) {
        this->consume(left_shift_at);
    } else {
        this->consume(left_shift);
    }

    // #[Identifier]
    std::string name = this->identifier_parser(pos);

    OutputAssignVisitor visitor(name, output_as_normal);

    outNode->accept(&visitor);

    this->currentNode->addChild(outNode);
}

void Parser::parse_array(int &pos) {
    Generic_pc<parser_constants::TOKEN_LESS> less(pos, tokens);
    Generic_pc<parser_constants::TOKEN_BIGGER> bigger(pos, tokens);
    Generic_pc<parser_constants::TOKEN_COMMA> comma(pos, tokens);
    Generic_pc<parser_constants::TOKEN_EQ_ARROW> eq_arrow(pos, tokens);

    this->consume(less);

    auto arrayNode = std::make_shared<StmtArrayNode>();

    IdentifierNameAssignVisitor visitor(tokens[pos].value); pos++;

    auto identifierNode = std::make_shared<ExprIdentifierNode>();

    identifierNode->accept(&visitor);

    arrayNode->addChild(identifierNode);

    while (tokens[pos].value != ">") {
        if (tokens[pos].value == ",") {
            this->consume(comma);

            visitor.setName(tokens[pos].value); pos++;

            auto identifierNode2 = std::make_shared<ExprIdentifierNode>();
            identifierNode2->accept(&visitor);

            arrayNode->addChild(identifierNode2);

            continue;
        }
    }

    this->consume(bigger);
    this->consume(eq_arrow);

    std::string name = this->identifier_parser(pos);

    arrayNode->name = name;

    this->currentNode->addChild(arrayNode);

}

void Parser::parse_loop(int &pos) {
    Generic_pc<parser_constants::TOKEN_LEFT_PAREN> left_paren(pos, tokens);
    Generic_pc<parser_constants::TOKEN_RIGHT_PAREN> right_paren(pos, tokens);
    Generic_pc<parser_constants::TOKEN_LEFT_BRACE> left_brace(pos, tokens);
    Generic_pc<parser_constants::TOKEN_RIGHT_BRACE> right_brace(pos, tokens);
    Generic_pc<parser_constants::TOKEN_DOLLAR> dollar(pos, tokens);
    Generic_pc<parser_constants::TOKEN_COLON> colon(pos, tokens);


    this->consume(left_paren);
    this->consume(colon);

    std::string iteration_count_variable = this->identifier_parser(pos);

    this->consume(dollar);

    // TODO add all operations inside the loop as a child of the loop node

    auto loopNode = std::make_shared<StmtLoopNode>();

    std::vector<Token> sub_tokens = this->subarray_creator_from_scope(pos);

    Parser sub_parser(this->filename, sub_tokens, this->unfilteredTokens, this->unfilteredLines, this->error_pack, false);
    sub_parser.parse();
    std::shared_ptr<ProgramNode> sub_program = sub_parser.G_program();
    this->error_pack.merge(sub_parser.G_error_pack());

    for (auto &child : sub_program->getChildren()) {
        loopNode->addChild(child);
    }

    this->consume(right_paren);

    LoopIterationCountIdentifierAssignAndGetterVisitor visitor(iteration_count_variable);
    loopNode->accept(&visitor);

    this->currentNode->addChild(loopNode);
}

void Parser::parse_expression(int &pos) {
    Generic_pc<parser_constants::TOKEN_NOT> exclamation(pos, tokens);
    Generic_pc<parser_constants::TOKEN_LEFT_BRACE> left_brace(pos, tokens);
    Generic_pc<parser_constants::TOKEN_RIGHT_BRACE> right_brace(pos, tokens);
    Generic_pc<parser_constants::TOKEN_EQ_ARROW> eq_arrow(pos, tokens);

    // TODO impl parse-time expression evaluation

    auto evalNode = std::make_shared<ExprEvaluateNode>("");
    auto assignedToIdentifierNode = std::make_shared<ExprIdentifierNode>();

    this->consume(exclamation);
    this->consume(left_brace);
    std::string expression;
    std::vector<std::string> variables;

    while (tokens[pos].value != "}") {
        if (tokens[pos].type == TokenType::IDENTIFIER) {
            expression += "{}";
            variables.push_back(tokens[pos].value);
            ++pos;
            continue;
        }
        expression += tokens[pos].value;
        ++pos;
    }
    this->consume(right_brace);

    this->consume(eq_arrow);

    std::string identifier = this->identifier_parser(pos);

    evalNode->expression = expression;
    evalNode->variables = variables;
    
    assignedToIdentifierNode->name = identifier;

    evalNode->addChild(assignedToIdentifierNode);

    this->currentNode->addChild(evalNode);
}


void Parser::parse() {
    for (int pos = 0; pos < static_cast<int>(tokens.size());) {
        const auto &token = tokens[pos];
        if (token.type == TokenType::SYMBOL) {
            if (token.value == "[")
                parse_variable(pos);
            else if (token.value == "<<")
                parse_out(pos, false);
            else if (token.value == "<<@")
                parse_out(pos, true);
            else if (token.value == "<")
                parse_array(pos);
            else if (token.value == "(")
                parse_loop(pos);
            else if (token.value == "!")
                parse_expression(pos);

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

    if (this->handle_errors) {
        ErrorHandler E_handler(this->error_pack, this->unfilteredTokens, this->unfilteredLines);
        E_handler.handle();
    }
}