#include "Parser.h"
#include <sstream>
#include <iostream>  // Added for std::cerr

namespace SimpScript {

// ParseError implementation
ParseError::ParseError(const std::string& message)
    : std::runtime_error(message) {}

// Parser implementation
Parser::Parser(Lexer& lexer) 
    : lexer(lexer), currentToken(TokenType::ERROR, 0, 0) {  // Initialize currentToken with a dummy token
    advance(); // Initialize currentToken with the first real token
}

void Parser::advance() {
    currentToken = lexer.nextToken();
}

void Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        advance();
    } else {
        throw error(message);
    }
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) const {
    return currentToken.getType() == type;
}

ParseError Parser::error(const std::string& message) {
    std::stringstream errorMsg;
    errorMsg << "Error at line " << currentToken.getLine() 
             << ", column " << currentToken.getColumn() 
             << ": " << message;
    return ParseError(errorMsg.str());
}

// Parse the entire program
std::unique_ptr<ASTNode> Parser::parse() {
    try {
        return program();
    } catch (const ParseError& e) {
        // Print error and try to recover
        std::cerr << e.what() << std::endl;
        synchronize();
        
        // Return a dummy program node
        std::vector<std::unique_ptr<ASTNode>> emptyStatements;
        return std::make_unique<ProgramNode>(std::move(emptyStatements));
    }
}

// Skip tokens until we find a statement boundary
void Parser::synchronize() {
    advance();
    
    while (currentToken.getType() != TokenType::END_OF_FILE) {
        // Skip until we find a semicolon or a statement keyword
        if (currentToken.getType() == TokenType::SEMICOLON) {
            advance();
            return;
        }
        
        switch (currentToken.getType()) {
            case TokenType::FUNCTION:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::FOR:
            case TokenType::RETURN:
            case TokenType::SHOW:
            case TokenType::SHOWNL:
            case TokenType::ASK:
                return;
            default:
                break;
        }
        
        advance();
    }
}

// Grammar rules
std::unique_ptr<ASTNode> Parser::program() {
    std::vector<std::unique_ptr<ASTNode>> statements;
    
    while (currentToken.getType() != TokenType::END_OF_FILE) {
        statements.push_back(statement());
    }
    
    return std::make_unique<ProgramNode>(std::move(statements));
}

std::unique_ptr<ASTNode> Parser::statement() {
    if (match(TokenType::IF)) {
        return ifStatement();
    }
    if (match(TokenType::WHILE)) {
        return whileStatement();
    }
    if (match(TokenType::FOR)) {
        return forStatement();
    }
    if (match(TokenType::FUNCTION)) {
        return functionDeclaration();
    }
    if (match(TokenType::RETURN)) {
        return returnStatement();
    }
    if (match(TokenType::SHOW)) {
        return printStatement(false);
    }
    if (match(TokenType::SHOWNL)) {
        return printStatement(true);
    }
    
    // Default to expression statement
    return expressionStatement();
}

std::unique_ptr<ASTNode> Parser::ifStatement() {
    auto condition = expression();
    
    auto thenBranch = statement();
    
    std::unique_ptr<ASTNode> elseBranch = nullptr;
    if (match(TokenType::ELSE)) {
        elseBranch = statement();
    }
    
    consume(TokenType::ENDIF, "Expect 'endif' after if statement");
    
    return std::make_unique<IfNode>(std::move(condition), 
                                    std::move(thenBranch), 
                                    std::move(elseBranch));
}

std::unique_ptr<ASTNode> Parser::whileStatement() {
    auto condition = expression();
    auto body = block();
    
    consume(TokenType::ENDWHILE, "Expect 'endwhile' after while loop");
    
    return std::make_unique<WhileNode>(std::move(condition), std::move(body));
}

std::unique_ptr<ASTNode> Parser::forStatement() {
    // For loop syntax: for init; condition; increment
    
    // Parse initialization
    auto init = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after for loop initialization");
    
    // Parse condition
    auto condition = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after for loop condition");
    
    // Parse increment
    auto increment = expression();
    
    // Parse body
    auto body = statement();
    
    consume(TokenType::ENDFOR, "Expect 'endfor' after for loop");
    
    return std::make_unique<ForNode>(std::move(init), 
                                     std::move(condition), 
                                     std::move(increment), 
                                     std::move(body));
}

std::unique_ptr<ASTNode> Parser::block() {
    std::vector<std::unique_ptr<ASTNode>> statements;
    
    while (!check(TokenType::END_OF_FILE) && 
           !check(TokenType::ENDIF) && 
           !check(TokenType::ENDWHILE) && 
           !check(TokenType::ENDFOR) && 
           !check(TokenType::ENDFUNCTION)) {
        statements.push_back(statement());
    }
    
    return std::make_unique<BlockNode>(std::move(statements));
}

std::unique_ptr<ASTNode> Parser::expressionStatement() {
    auto expr = expression();
    return expr;
}

std::unique_ptr<ASTNode> Parser::printStatement(bool newline) {
    auto expr = expression();
    return std::make_unique<PrintNode>(std::move(expr), newline);
}

std::unique_ptr<ASTNode> Parser::functionDeclaration() {
    // Parse function name
    consume(TokenType::IDENTIFIER, "Expect function name");
    std::string name = currentToken.getStringValue();
    advance();
    
    // Parse parameters
    consume(TokenType::LEFT_PAREN, "Expect '(' after function name");
    std::vector<std::string> parameters;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            consume(TokenType::IDENTIFIER, "Expect parameter name");
            parameters.push_back(currentToken.getStringValue());
            advance();
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters");
    
    // Parse function body
    auto body = block();
    
    consume(TokenType::ENDFUNCTION, "Expect 'endfunction' after function body");
    
    return std::make_unique<FunctionDefNode>(name, parameters, std::move(body));
}

std::unique_ptr<ASTNode> Parser::returnStatement() {
    auto expr = expression();
    return std::make_unique<ReturnNode>(std::move(expr));
}

std::unique_ptr<ASTNode> Parser::expression() {
    return assignment();
}

std::unique_ptr<ASTNode> Parser::assignment() {
    auto expr = logicalOr();
    
    if (match(TokenType::ASSIGN)) {
        auto value = assignment();
        
        // Check if the left side is a valid assignment target
        if (auto* varExpr = dynamic_cast<VariableNode*>(expr.get())) {
            std::string name = varExpr->getName();
            return std::make_unique<AssignmentNode>(name, std::move(value));
        } else if (auto* arrayExpr = dynamic_cast<ArrayAccessNode*>(expr.get())) {
            return std::make_unique<ArrayAssignmentNode>(
                arrayExpr->getArray(),
                arrayExpr->getIndex(),
                std::move(value));
        }
        
        throw error("Invalid assignment target");
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::logicalOr() {
    auto expr = logicalAnd();
    
    while (true) {
        // Check for OR operator without advancing
        if (!check(TokenType::OR)) break;
        
        // Now advance past the operator
        advance();
        
        // Parse the right operand
        auto right = logicalAnd();
        
        // Create the binary operation node
        expr = std::make_unique<BinaryOpNode>(
            BinaryOpNode::OpType::OR, 
            std::move(expr), 
            std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::logicalAnd() {
    auto expr = equality();
    
    while (true) {
        // Check for AND operator without advancing
        if (!check(TokenType::AND)) break;
        
        // Now advance past the operator
        advance();
        
        // Parse the right operand
        auto right = equality();
        
        // Create the binary operation node
        expr = std::make_unique<BinaryOpNode>(
            BinaryOpNode::OpType::AND, 
            std::move(expr), 
            std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::equality() {
    auto expr = comparison();
    
    while (true) {
        // Check for operators without advancing
        TokenType opType;
        bool isBinaryOp = false;
        
        if (check(TokenType::EQUAL)) {
            opType = TokenType::EQUAL;
            isBinaryOp = true;
        } else if (check(TokenType::NOT_EQUAL)) {
            opType = TokenType::NOT_EQUAL;
            isBinaryOp = true;
        } else if (check(TokenType::EQUALS)) {
            opType = TokenType::EQUALS;
            isBinaryOp = true;
        } else if (check(TokenType::ISNT)) {
            opType = TokenType::ISNT;
            isBinaryOp = true;
        }
        
        if (!isBinaryOp) break;
        
        // Now advance past the operator
        advance();
        
        // Parse the right operand
        auto right = comparison();
        
        // Create the binary operation node
        BinaryOpNode::OpType nodeOpType;
        if (opType == TokenType::EQUAL || opType == TokenType::EQUALS) {
            nodeOpType = BinaryOpNode::OpType::EQ;
        } else {
            nodeOpType = BinaryOpNode::OpType::NEQ;
        }
        
        expr = std::make_unique<BinaryOpNode>(nodeOpType, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::comparison() {
    auto expr = term();
    
    while (true) {
        // Check for operators without advancing
        TokenType opType;
        bool isBinaryOp = false;
        
        if (check(TokenType::GREATER)) {
            opType = TokenType::GREATER;
            isBinaryOp = true;
        } else if (check(TokenType::LESS)) {
            opType = TokenType::LESS;
            isBinaryOp = true;
        } else if (check(TokenType::GREATER_EQUAL)) {
            opType = TokenType::GREATER_EQUAL;
            isBinaryOp = true;
        } else if (check(TokenType::LESS_EQUAL)) {
            opType = TokenType::LESS_EQUAL;
            isBinaryOp = true;
        } else if (check(TokenType::GREATER_THAN)) {
            opType = TokenType::GREATER_THAN;
            isBinaryOp = true;
        } else if (check(TokenType::LESS_THAN)) {
            opType = TokenType::LESS_THAN;
            isBinaryOp = true;
        } else if (check(TokenType::AT_LEAST)) {
            opType = TokenType::AT_LEAST;
            isBinaryOp = true;
        } else if (check(TokenType::AT_MOST)) {
            opType = TokenType::AT_MOST;
            isBinaryOp = true;
        }
        
        if (!isBinaryOp) break;
        
        // Now advance past the operator
        advance();
        
        // Parse the right operand
        auto right = term();
        
        // Create the binary operation node
        BinaryOpNode::OpType nodeOpType;
        switch (opType) {
            case TokenType::GREATER:
            case TokenType::GREATER_THAN:
                nodeOpType = BinaryOpNode::OpType::GT;
                break;
            case TokenType::LESS:
            case TokenType::LESS_THAN:
                nodeOpType = BinaryOpNode::OpType::LT;
                break;
            case TokenType::GREATER_EQUAL:
            case TokenType::AT_LEAST:
                nodeOpType = BinaryOpNode::OpType::GTE;
                break;
            case TokenType::LESS_EQUAL:
            case TokenType::AT_MOST:
                nodeOpType = BinaryOpNode::OpType::LTE;
                break;
            default:
                throw error("Unknown comparison operator");
        }
        
        expr = std::make_unique<BinaryOpNode>(nodeOpType, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::term() {
    auto expr = factor();
    
    while (true) {
        // Check for operators without advancing
        TokenType opType;
        bool isBinaryOp = false;
        
        if (check(TokenType::PLUS)) {
            opType = TokenType::PLUS;
            isBinaryOp = true;
        } else if (check(TokenType::MINUS)) {
            opType = TokenType::MINUS;
            isBinaryOp = true;
        }
        
        if (!isBinaryOp) break;
        
        // Now advance past the operator
        advance();
        
        // Parse the right operand
        auto right = factor();
        
        // Create the binary operation node
        BinaryOpNode::OpType nodeOpType;
        if (opType == TokenType::PLUS) {
            nodeOpType = BinaryOpNode::OpType::ADD;
        } else {
            nodeOpType = BinaryOpNode::OpType::SUB;
        }
        
        expr = std::make_unique<BinaryOpNode>(nodeOpType, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::factor() {
    auto expr = unary();
    
    while (true) {
        // Check for operators without advancing
        TokenType opType;
        bool isBinaryOp = false;
        
        if (check(TokenType::MULTIPLY)) {
            opType = TokenType::MULTIPLY;
            isBinaryOp = true;
        } else if (check(TokenType::DIVIDE)) {
            opType = TokenType::DIVIDE;
            isBinaryOp = true;
        } else if (check(TokenType::MODULO)) {
            opType = TokenType::MODULO;
            isBinaryOp = true;
        }
        
        if (!isBinaryOp) break;
        
        // Now advance past the operator
        advance();
        
        // Parse the right operand
        auto right = unary();
        
        // Create the binary operation node
        BinaryOpNode::OpType nodeOpType;
        if (opType == TokenType::MULTIPLY) {
            nodeOpType = BinaryOpNode::OpType::MUL;
        } else if (opType == TokenType::DIVIDE) {
            nodeOpType = BinaryOpNode::OpType::DIV;
        } else {
            nodeOpType = BinaryOpNode::OpType::MOD;
        }
        
        expr = std::make_unique<BinaryOpNode>(nodeOpType, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::unary() {
    if (match(TokenType::MINUS) || match(TokenType::NOT)) {
        TokenType op = currentToken.getType();
        advance();
        auto right = unary();
        
        UnaryOpNode::OpType opType;
        if (op == TokenType::MINUS) {
            opType = UnaryOpNode::OpType::NEGATIVE;
        } else {
            opType = UnaryOpNode::OpType::NOT;
        }
        
        return std::make_unique<UnaryOpNode>(opType, std::move(right));
    }
    
    return call();
}

std::unique_ptr<ASTNode> Parser::call() {
    auto expr = primary();
    
    while (true) {
        if (match(TokenType::LEFT_PAREN)) {
            expr = finishCall(std::move(expr));
        } else if (match(TokenType::LEFT_BRACKET)) {
            auto index = expression();
            consume(TokenType::RIGHT_BRACKET, "Expect ']' after array index");
            expr = std::make_unique<ArrayAccessNode>(std::move(expr), std::move(index));
        } else {
            break;
        }
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::finishCall(std::unique_ptr<ASTNode> callee) {
    std::vector<std::unique_ptr<ASTNode>> arguments;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            arguments.push_back(expression());
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_PAREN, "Expect ')' after function arguments");
    
    // Extract function name from callee
    std::string name;
    if (auto* varExpr = dynamic_cast<VariableNode*>(callee.get())) {
        name = varExpr->getName();
    } else {
        throw error("Expected function name");
    }
    
    return std::make_unique<FunctionCallNode>(name, std::move(arguments));
}

std::unique_ptr<ASTNode> Parser::primary() {
    if (check(TokenType::INTEGER)) {
        int value = currentToken.getIntValue();
        advance(); // Now advance after getting the value
        return std::make_unique<LiteralNode>(value);
    }
    if (check(TokenType::FLOAT)) {
        double value = currentToken.getFloatValue();
        advance(); // Now advance after getting the value
        return std::make_unique<LiteralNode>(value);
    }
    if (check(TokenType::STRING)) {
        std::string value = currentToken.getStringValue();
        advance(); // Now advance after getting the value
        return std::make_unique<LiteralNode>(value);
    }
    if (check(TokenType::IDENTIFIER)) {
        std::string name = currentToken.getStringValue();
        advance(); // Now advance after getting the value
        return std::make_unique<VariableNode>(name);
    }
    if (match(TokenType::ASK)) {
        return std::make_unique<InputNode>();
    }
    if (match(TokenType::LEFT_PAREN)) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return expr;
    }
    if (match(TokenType::LEFT_BRACKET)) {
        std::vector<std::unique_ptr<ASTNode>> elements;
        
        if (!check(TokenType::RIGHT_BRACKET)) {
            do {
                elements.push_back(expression());
            } while (match(TokenType::COMMA));
        }
        
        consume(TokenType::RIGHT_BRACKET, "Expect ']' after array elements");
        return std::make_unique<ArrayLiteralNode>(std::move(elements));
    }
    
    // Add more context to the error
    std::string errorMsg = "Expect expression, got ";
    errorMsg += "token type " + std::to_string(static_cast<int>(currentToken.getType()));
    if (currentToken.hasStringValue()) {
        errorMsg += " with value '" + currentToken.getStringValue() + "'";
    }
    throw error(errorMsg);
}

} // namespace SimpScript 