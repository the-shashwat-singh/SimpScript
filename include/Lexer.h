#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace SimpScript {

class Lexer {
private:
    std::string source;
    int position = 0;
    int line = 1;
    int column = 1;
    char currentChar = '\0';

    // Lookup table for keywords
    static std::unordered_map<std::string, TokenType> keywords;
    // Lookup table for natural language operators
    static std::unordered_map<std::string, TokenType> naturalOperators;

    // Methods for lexer operation
    void advance();
    void skipWhitespace();
    void skipComment();
    char peek() const;
    
    // Token recognition methods
    Token makeToken(TokenType type) const;
    Token makeToken(TokenType type, int value) const;
    Token makeToken(TokenType type, double value) const;
    Token makeToken(TokenType type, const std::string& value) const;
    
    Token handleNumber();
    Token handleIdentifier();
    Token handleString();
    Token handleOperator();
    Token handleWord(); // For multi-word operators like "greater than"

    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;

public:
    // Initialize the lexer with source code
    Lexer(const std::string& source);
    
    // Scan and return the next token
    Token nextToken();
    
    // Peek at the next token without advancing
    Token peekToken();
    
    // Check if we've reached the end of the source code
    bool isAtEnd() const;
};

} // namespace SimpScript

#endif // LEXER_H 