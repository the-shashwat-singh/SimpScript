#include "Lexer.h"
#include <cctype>
#include <stdexcept>

namespace SimpScript {

// Initialize static lookup tables
std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"function", TokenType::FUNCTION},
    {"return", TokenType::RETURN},
    {"show", TokenType::SHOW},
    {"shownl", TokenType::SHOWNL},
    {"nextl", TokenType::NEXTL},
    {"ask", TokenType::ASK},
    {"endif", TokenType::ENDIF},
    {"endwhile", TokenType::ENDWHILE},
    {"endfor", TokenType::ENDFOR},
    {"endfunction", TokenType::ENDFUNCTION},
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"not", TokenType::NOT}
};

std::unordered_map<std::string, TokenType> Lexer::naturalOperators = {
    {"equals", TokenType::EQUALS},
    {"isnt", TokenType::ISNT},
    {"greater than", TokenType::GREATER_THAN},
    {"less than", TokenType::LESS_THAN},
    {"at least", TokenType::AT_LEAST},
    {"at most", TokenType::AT_MOST}
};

// Constructor
Lexer::Lexer(const std::string& source) : source(source) {
    if (!source.empty()) {
        currentChar = source[0];
    }
}

// Helper methods
void Lexer::advance() {
    position++;
    if (position >= source.length()) {
        currentChar = '\0'; // End of file
        return;
    }
    
    currentChar = source[position];
    column++;
    
    if (currentChar == '\n') {
        line++;
        column = 1;
    }
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && std::isspace(currentChar)) {
        advance();
    }
}

void Lexer::skipComment() {
    // Skip until end of line or end of file
    while (currentChar != '\n' && !isAtEnd()) {
        advance();
    }
    
    // Skip the newline character
    if (currentChar == '\n') {
        advance();
    }
}

char Lexer::peek() const {
    if (position + 1 >= source.length()) {
        return '\0'; // End of file
    }
    return source[position + 1];
}

bool Lexer::isDigit(char c) const {
    return std::isdigit(c);
}

bool Lexer::isAlpha(char c) const {
    return std::isalpha(c) || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool Lexer::isAtEnd() const {
    return position >= source.length();
}

// Token creation helpers
Token Lexer::makeToken(TokenType type) const {
    return Token(type, line, column);
}

Token Lexer::makeToken(TokenType type, int value) const {
    return Token(type, value, line, column);
}

Token Lexer::makeToken(TokenType type, double value) const {
    return Token(type, value, line, column);
}

Token Lexer::makeToken(TokenType type, const std::string& value) const {
    return Token(type, value, line, column);
}

// Handle specific token types
Token Lexer::handleNumber() {
    int startPos = position;
    int startCol = column;
    bool isFloat = false;
    
    // Process integer part
    while (isDigit(currentChar) && !isAtEnd()) {
        advance();
    }
    
    // Process decimal part
    if (currentChar == '.' && isDigit(peek())) {
        isFloat = true;
        advance(); // Skip the decimal point
        
        while (isDigit(currentChar) && !isAtEnd()) {
            advance();
        }
    }
    
    // Get the lexeme (number as string)
    std::string numberStr = source.substr(startPos, position - startPos);
    
    if (isFloat) {
        return Token(TokenType::FLOAT, std::stod(numberStr), line, startCol);
    } else {
        return Token(TokenType::INTEGER, std::stoi(numberStr), line, startCol);
    }
}

Token Lexer::handleIdentifier() {
    int startPos = position;
    int startCol = column;
    
    while (isAlphaNumeric(currentChar) && !isAtEnd()) {
        advance();
    }
    
    // Extract the identifier
    std::string identifier = source.substr(startPos, position - startPos);
    
    // Check if it's a keyword
    auto it = keywords.find(identifier);
    if (it != keywords.end()) {
        return makeToken(it->second);
    }
    
    // It's a regular identifier
    return makeToken(TokenType::IDENTIFIER, identifier);
}

Token Lexer::handleString() {
    int startCol = column;
    advance(); // Skip the opening quote
    
    int startPos = position;
    while (currentChar != '"' && !isAtEnd()) {
        advance();
    }
    
    if (isAtEnd()) {
        return makeToken(TokenType::ERROR, "Unterminated string");
    }
    
    // Extract the string without the quotes
    std::string str = source.substr(startPos, position - startPos);
    
    advance(); // Skip the closing quote
    
    return makeToken(TokenType::STRING, str);
}

Token Lexer::handleWord() {
    int startPos = position;
    int startCol = column;
    
    // Collect the entire word
    while (isAlpha(currentChar) && !isAtEnd()) {
        advance();
    }
    
    // Skip whitespace
    while (!isAtEnd() && std::isspace(currentChar) && currentChar != '\n') {
        advance();
    }
    
    // If there's another word following, we might have a multi-word operator
    if (isAlpha(currentChar)) {
        // Save the position to restore if it's not a multi-word operator
        int tempPos = position;
        int tempLine = line;
        int tempCol = column;
        std::string firstWord = source.substr(startPos, tempPos - startPos - 1); // -1 to exclude the space
        
        // Get the second word
        int secondWordStart = position;
        while (isAlpha(currentChar) && !isAtEnd()) {
            advance();
        }
        
        // Try the two words together
        std::string twoWords = firstWord + " " + source.substr(secondWordStart, position - secondWordStart);
        twoWords = twoWords.substr(0, twoWords.find_last_not_of(" \t") + 1); // Trim right
        
        auto it = naturalOperators.find(twoWords);
        if (it != naturalOperators.end()) {
            return makeToken(it->second);
        }
        
        // Not a multi-word operator, restore position
        position = tempPos;
        line = tempLine;
        column = tempCol;
        currentChar = source[position];
    }
    
    // Process as regular identifier or keyword
    position = startPos;
    column = startCol;
    currentChar = source[position];
    return handleIdentifier();
}

Token Lexer::handleOperator() {
    switch (currentChar) {
        case '+': {
            advance();
            if (currentChar == '=') {
                advance();
                return makeToken(TokenType::PLUS_ASSIGN);
            }
            return makeToken(TokenType::PLUS);
        }
        case '-': {
            advance();
            if (currentChar == '=') {
                advance();
                return makeToken(TokenType::MINUS_ASSIGN);
            }
            return makeToken(TokenType::MINUS);
        }
        case '*': {
            advance();
            if (currentChar == '=') {
                advance();
                return makeToken(TokenType::MULT_ASSIGN);
            }
            return makeToken(TokenType::MULTIPLY);
        }
        case '/': {
            advance();
            if (currentChar == '=') {
                advance();
                return makeToken(TokenType::DIV_ASSIGN);
            }
            return makeToken(TokenType::DIVIDE);
        }
        case '%': {
            advance();
            return makeToken(TokenType::MODULO);
        }
        case '=': {
            advance();
            if (currentChar == '=') {
                advance();
                return makeToken(TokenType::EQUAL);
            }
            return makeToken(TokenType::ASSIGN);
        }
        case '!': {
            advance();
            if (currentChar == '=') {
                advance();
                return makeToken(TokenType::NOT_EQUAL);
            }
            // Error, standalone ! is not supported
            return makeToken(TokenType::ERROR, "Unexpected character '!'");
        }
        case '>': {
            advance();
            if (currentChar == '=') {
                advance();
                return makeToken(TokenType::GREATER_EQUAL);
            }
            return makeToken(TokenType::GREATER);
        }
        case '<': {
            advance();
            if (currentChar == '=') {
                advance();
                return makeToken(TokenType::LESS_EQUAL);
            }
            return makeToken(TokenType::LESS);
        }
        case '(': {
            advance();
            return makeToken(TokenType::LEFT_PAREN);
        }
        case ')': {
            advance();
            return makeToken(TokenType::RIGHT_PAREN);
        }
        case '[': {
            advance();
            return makeToken(TokenType::LEFT_BRACKET);
        }
        case ']': {
            advance();
            return makeToken(TokenType::RIGHT_BRACKET);
        }
        case '{': {
            advance();
            return makeToken(TokenType::LEFT_BRACE);
        }
        case '}': {
            advance();
            return makeToken(TokenType::RIGHT_BRACE);
        }
        case ',': {
            advance();
            return makeToken(TokenType::COMMA);
        }
        case ':': {
            advance();
            return makeToken(TokenType::COLON);
        }
        case ';': {
            advance();
            return makeToken(TokenType::SEMICOLON);
        }
        default:
            // Error, unexpected character
            std::string errorMsg = "Unexpected character '";
            errorMsg += currentChar;
            errorMsg += "'";
            advance();
            return makeToken(TokenType::ERROR, errorMsg);
    }
}

// Public methods
Token Lexer::nextToken() {
    // Skip whitespace and comments
    skipWhitespace();
    
    if (isAtEnd()) {
        return makeToken(TokenType::END_OF_FILE);
    }
    
    // Check for comments
    if (currentChar == '#') {
        skipComment();
        return nextToken(); // Recursively get the next token
    }
    
    // Check for numbers
    if (isDigit(currentChar)) {
        return handleNumber();
    }
    
    // Check for identifiers and keywords
    if (isAlpha(currentChar)) {
        return handleWord(); // This will handle both single keywords and multi-word operators
    }
    
    // Check for strings
    if (currentChar == '"') {
        return handleString();
    }
    
    // Handle operators and other symbols
    return handleOperator();
}

Token Lexer::peekToken() {
    // Save current state
    int oldPosition = position;
    int oldLine = line;
    int oldColumn = column;
    char oldChar = currentChar;
    
    // Get the next token
    Token token = nextToken();
    
    // Restore state
    position = oldPosition;
    line = oldLine;
    column = oldColumn;
    currentChar = oldChar;
    
    return token;
}

} // namespace SimpScript 