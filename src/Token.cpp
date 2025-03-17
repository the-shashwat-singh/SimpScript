#include "Token.h"
#include <stdexcept>
#include <sstream>

namespace SimpScript {

// Constructors
Token::Token(TokenType type, int line, int column)
    : type(type), value(std::monostate()), line(line), column(column) {}

Token::Token(TokenType type, int value, int line, int column)
    : type(type), value(value), line(line), column(column) {}

Token::Token(TokenType type, double value, int line, int column)
    : type(type), value(value), line(line), column(column) {}

Token::Token(TokenType type, const std::string& value, int line, int column)
    : type(type), value(value), line(line), column(column) {}

// Accessor methods
TokenType Token::getType() const {
    return type;
}

bool Token::hasIntValue() const {
    return std::holds_alternative<int>(value);
}

bool Token::hasFloatValue() const {
    return std::holds_alternative<double>(value);
}

bool Token::hasStringValue() const {
    return std::holds_alternative<std::string>(value);
}

int Token::getIntValue() const {
    if (!hasIntValue()) {
        throw std::runtime_error("Token does not contain an integer value");
    }
    return std::get<int>(value);
}

double Token::getFloatValue() const {
    if (!hasFloatValue()) {
        throw std::runtime_error("Token does not contain a float value");
    }
    return std::get<double>(value);
}

std::string Token::getStringValue() const {
    if (!hasStringValue()) {
        throw std::runtime_error("Token does not contain a string value");
    }
    return std::get<std::string>(value);
}

int Token::getLine() const {
    return line;
}

int Token::getColumn() const {
    return column;
}

// Convert token to string for debugging
std::string Token::toString() const {
    std::stringstream ss;
    
    ss << "Token(";
    
    // Print token type
    switch (type) {
        case TokenType::END_OF_FILE: ss << "END_OF_FILE"; break;
        case TokenType::ERROR: ss << "ERROR"; break;
        case TokenType::INTEGER: ss << "INTEGER"; break;
        case TokenType::FLOAT: ss << "FLOAT"; break;
        case TokenType::STRING: ss << "STRING"; break;
        case TokenType::IDENTIFIER: ss << "IDENTIFIER"; break;
        case TokenType::PLUS: ss << "PLUS"; break;
        case TokenType::MINUS: ss << "MINUS"; break;
        case TokenType::MULTIPLY: ss << "MULTIPLY"; break;
        case TokenType::DIVIDE: ss << "DIVIDE"; break;
        case TokenType::MODULO: ss << "MODULO"; break;
        case TokenType::ASSIGN: ss << "ASSIGN"; break;
        case TokenType::PLUS_ASSIGN: ss << "PLUS_ASSIGN"; break;
        case TokenType::MINUS_ASSIGN: ss << "MINUS_ASSIGN"; break;
        case TokenType::MULT_ASSIGN: ss << "MULT_ASSIGN"; break;
        case TokenType::DIV_ASSIGN: ss << "DIV_ASSIGN"; break;
        case TokenType::EQUAL: ss << "EQUAL"; break;
        case TokenType::NOT_EQUAL: ss << "NOT_EQUAL"; break;
        case TokenType::GREATER: ss << "GREATER"; break;
        case TokenType::LESS: ss << "LESS"; break;
        case TokenType::GREATER_EQUAL: ss << "GREATER_EQUAL"; break;
        case TokenType::LESS_EQUAL: ss << "LESS_EQUAL"; break;
        case TokenType::AND: ss << "AND"; break;
        case TokenType::OR: ss << "OR"; break;
        case TokenType::NOT: ss << "NOT"; break;
        case TokenType::EQUALS: ss << "EQUALS"; break;
        case TokenType::ISNT: ss << "ISNT"; break;
        case TokenType::GREATER_THAN: ss << "GREATER_THAN"; break;
        case TokenType::LESS_THAN: ss << "LESS_THAN"; break;
        case TokenType::AT_LEAST: ss << "AT_LEAST"; break;
        case TokenType::AT_MOST: ss << "AT_MOST"; break;
        case TokenType::LEFT_PAREN: ss << "LEFT_PAREN"; break;
        case TokenType::RIGHT_PAREN: ss << "RIGHT_PAREN"; break;
        case TokenType::LEFT_BRACKET: ss << "LEFT_BRACKET"; break;
        case TokenType::RIGHT_BRACKET: ss << "RIGHT_BRACKET"; break;
        case TokenType::LEFT_BRACE: ss << "LEFT_BRACE"; break;
        case TokenType::RIGHT_BRACE: ss << "RIGHT_BRACE"; break;
        case TokenType::COMMA: ss << "COMMA"; break;
        case TokenType::COLON: ss << "COLON"; break;
        case TokenType::SEMICOLON: ss << "SEMICOLON"; break;
        case TokenType::IF: ss << "IF"; break;
        case TokenType::ELSE: ss << "ELSE"; break;
        case TokenType::WHILE: ss << "WHILE"; break;
        case TokenType::FOR: ss << "FOR"; break;
        case TokenType::FUNCTION: ss << "FUNCTION"; break;
        case TokenType::RETURN: ss << "RETURN"; break;
        case TokenType::SHOW: ss << "SHOW"; break;
        case TokenType::SHOWNL: ss << "SHOWNL"; break;
        case TokenType::NEXTL: ss << "NEXTL"; break;
        case TokenType::ASK: ss << "ASK"; break;
        case TokenType::ENDIF: ss << "ENDIF"; break;
        case TokenType::ENDWHILE: ss << "ENDWHILE"; break;
        case TokenType::ENDFOR: ss << "ENDFOR"; break;
        case TokenType::ENDFUNCTION: ss << "ENDFUNCTION"; break;
        default: ss << "UNKNOWN"; break;
    }
    
    // Print value if present
    if (hasIntValue()) {
        ss << ", " << getIntValue();
    } else if (hasFloatValue()) {
        ss << ", " << getFloatValue();
    } else if (hasStringValue()) {
        ss << ", \"" << getStringValue() << "\"";
    }
    
    ss << ", line=" << line << ", col=" << column << ")";
    
    return ss.str();
}

} // namespace SimpScript 