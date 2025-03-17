#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <variant>

namespace SimpScript {

// TokenType enum represents all possible token types in SimpScript
enum class TokenType {
    // General tokens
    END_OF_FILE,
    ERROR,
    
    // Literals
    INTEGER,
    FLOAT,
    STRING,
    IDENTIFIER,
    
    // Arithmetic operators
    PLUS,       // +
    MINUS,      // -
    MULTIPLY,   // *
    DIVIDE,     // /
    MODULO,     // %
    
    // Assignment operators
    ASSIGN,     // =
    PLUS_ASSIGN,    // +=
    MINUS_ASSIGN,   // -=
    MULT_ASSIGN,    // *=
    DIV_ASSIGN,     // /=
    
    // Comparison operators
    EQUAL,          // ==
    NOT_EQUAL,      // !=
    GREATER,        // >
    LESS,           // <
    GREATER_EQUAL,  // >=
    LESS_EQUAL,     // <=
    
    // Logical operators
    AND,            // and
    OR,             // or
    NOT,            // not
    
    // Natural language tokens (synonyms for comparison/logical operators)
    EQUALS,         // equals
    ISNT,           // isn't
    GREATER_THAN,   // greater than
    LESS_THAN,      // less than
    AT_LEAST,       // at least
    AT_MOST,        // at most
    
    // Delimiters
    LEFT_PAREN,     // (
    RIGHT_PAREN,    // )
    LEFT_BRACKET,   // [
    RIGHT_BRACKET,  // ]
    LEFT_BRACE,     // {
    RIGHT_BRACE,    // }
    COMMA,          // ,
    COLON,          // :
    SEMICOLON,      // ;
    
    // Keywords
    IF,
    ELSE,
    WHILE,
    FOR,
    FUNCTION,
    RETURN,
    SHOW,
    SHOWNL,
    NEXTL,
    ASK,
    ENDIF,
    ENDWHILE,
    ENDFOR,
    ENDFUNCTION
};

// Token class to store token type and its value
class Token {
private:
    TokenType type;
    std::variant<std::monostate, int, double, std::string> value;
    int line;
    int column;

public:
    Token(TokenType type, int line, int column);
    Token(TokenType type, int value, int line, int column);
    Token(TokenType type, double value, int line, int column);
    Token(TokenType type, const std::string& value, int line, int column);

    TokenType getType() const;
    bool hasIntValue() const;
    bool hasFloatValue() const;
    bool hasStringValue() const;
    int getIntValue() const;
    double getFloatValue() const;
    std::string getStringValue() const;
    int getLine() const;
    int getColumn() const;
    
    // Convert token to string for debugging
    std::string toString() const;
};

} // namespace SimpScript

#endif // TOKEN_H 