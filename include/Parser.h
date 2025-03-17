#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "AST.h"
#include "Lexer.h"
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

namespace SimpScript {

// Custom exception for parse errors
class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& message);
};

class Parser {
private:
    Lexer& lexer;
    Token currentToken;
    
    // Helper methods for parsing
    void advance();
    void consume(TokenType type, const std::string& message);
    bool match(TokenType type);
    bool check(TokenType type) const;
    
    // Parsing methods for different grammar rules
    std::unique_ptr<ASTNode> program();
    std::unique_ptr<ASTNode> statement();
    std::unique_ptr<ASTNode> ifStatement();
    std::unique_ptr<ASTNode> whileStatement();
    std::unique_ptr<ASTNode> forStatement();
    std::unique_ptr<ASTNode> block();
    std::unique_ptr<ASTNode> expressionStatement();
    std::unique_ptr<ASTNode> printStatement(bool newline);
    std::unique_ptr<ASTNode> functionDeclaration();
    std::unique_ptr<ASTNode> returnStatement();
    
    std::unique_ptr<ASTNode> expression();
    std::unique_ptr<ASTNode> assignment();
    std::unique_ptr<ASTNode> logicalOr();
    std::unique_ptr<ASTNode> logicalAnd();
    std::unique_ptr<ASTNode> equality();
    std::unique_ptr<ASTNode> comparison();
    std::unique_ptr<ASTNode> term();
    std::unique_ptr<ASTNode> factor();
    std::unique_ptr<ASTNode> unary();
    std::unique_ptr<ASTNode> primary();
    std::unique_ptr<ASTNode> call();
    std::unique_ptr<ASTNode> arrayAccess(std::unique_ptr<ASTNode> array);
    
    std::unique_ptr<ASTNode> finishCall(std::unique_ptr<ASTNode> callee);
    
    // Error handling
    void synchronize();
    ParseError error(const std::string& message);

public:
    explicit Parser(Lexer& lexer);
    
    // Parse the input and build an AST
    std::unique_ptr<ASTNode> parse();
};

} // namespace SimpScript

#endif // PARSER_H 