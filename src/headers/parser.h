#pragma once
#include "ast.h"

namespace parser_constants {
    inline constexpr char TOKEN_LEFT_BRACKET[] = "[";
    inline constexpr char TOKEN_RIGHT_BRACKET[] = "]";
    inline constexpr char TOKEN_PLUS[] = "+";
    inline constexpr char TOKEN_MINUS[] = "-";
    inline constexpr char TOKEN_EQ_ARROW[] = "=>";
    inline constexpr char TOKEN_LEFT_SHIFT[] = "<<";
    inline constexpr char TOKEN_RIGHT_SHIFT[] = ">>";
    inline constexpr char TOKEN_BIGGER[] = ">";
    inline constexpr char TOKEN_LESS[] = "<";
    inline constexpr char TOKEN_COMMA[] = ",";
    inline constexpr char TOKEN_ADD[] = "+";
    inline constexpr char TOKEN_SUB[] = "-";
    inline constexpr char TOKEN_MUL[] = "*";
    inline constexpr char TOKEN_DIV[] = "/";
    inline constexpr char TOKEN_MOD[] = "%";
    inline constexpr char TOKEN_AND[] = "&";
    inline constexpr char TOKEN_OR[] = "|";
    inline constexpr char TOKEN_XOR[] = "^";
    inline constexpr char TOKEN_NOT[] = "!";
    inline constexpr char TOKEN_EQ[] = "=";
    inline constexpr char TOKEN_NEQ[] = "!=";
    inline constexpr char TOKEN_LEQ[] = "<=";
    inline constexpr char TOKEN_GEQ[] = ">=";
    inline constexpr char TOKEN_DOT[] = ".";
    inline constexpr char TOKEN_COLON[] = ":";
    inline constexpr char TOKEN_SEMICOLON[] = ";";
    inline constexpr char TOKEN_LEFT_PAREN[] = "(";
    inline constexpr char TOKEN_RIGHT_PAREN[] = ")";
    inline constexpr char TOKEN_LEFT_BRACE[] = "{";
    inline constexpr char TOKEN_RIGHT_BRACE[] = "}";
}

class Parser {
public:
    Parser(const std::string filename, std::vector<Token> tokens, std::vector<Token> unfilteredTokens, std::map<int, std::string> unfilteredLines, ErrorPack error_pack, int allowed_errors = 20);
    ~Parser() = default;

    static std::unique_ptr<Parser> createParser(const std::string filename, const std::vector<Token> &tokens, const std::vector<Token> &unfilteredTokens, const std::map<int, std::string> &
                                         unfilteredLines, ErrorPack &error_pack, int allowed_errors = 20);

    template <const char* tToken>
    class Generic_pc {
    public:
        Generic_pc(int &pos, const std::vector<Token> &tokens);

        bool operator()();
        void operator|=(tcomp::Error &&error);

        [[nodiscard]] tcomp::Error getError() const {
            return error;
        }

    private:
        int &pos;
        const std::vector<Token> &tokens;
        tcomp::Error error;
    };

    class IsDigit {
    public:

        /// Check if the token is a digit
        static bool check(const std::string& value);

        /// predicate function to check if the token is a digit
        static bool predicate(const Token& token);

        /// Constant that can be used to identify this type
        static constexpr auto type = TokenType::NUMBER;

    };

    template <char const* tToken>
    void consume(Generic_pc<tToken> &parser);

    [[nodiscard]] bool more_than_allowed_errors() const;

    // #[Getters<Def>]
    [[nodiscard]] std::shared_ptr<ProgramNode> G_program() const;
    [[nodiscard]] std::vector<Token> G_tokens() const;

    // #[Setters<Def>]
    void S_program(std::shared_ptr<ProgramNode> Pn);

    void parse_variable(int &pos);
    void parse_array(int &pos);
    void parse_collection(int &pos);
    void parse_out(int &pos);

    void parse();

private:
    std::string filename;
    std::vector<Token> tokens;
    std::vector<Token> unfilteredTokens;
    std::map<int, std::string> unfilteredLines;
    int allowed_errors;
    ErrorPack error_pack;
    std::shared_ptr<ProgramNode> program;
    std::shared_ptr<AST> currentNode;
};
