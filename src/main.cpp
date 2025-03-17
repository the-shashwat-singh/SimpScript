#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace SimpScript;

// Function to run a SimpScript file
void runFile(const std::string& path, bool debug = false, bool traceDebug = false) {
    // Read the file contents
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << path << "'" << std::endl;
        exit(1);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    
    // Debug mode: print tokens
    if (debug) {
        std::cout << "Tokens:" << std::endl;
        Lexer debugLexer(source);
        while (true) {
            Token token = debugLexer.nextToken();
            std::cout << token.toString() << std::endl;
            if (token.getType() == TokenType::END_OF_FILE) {
                break;
            }
        }
        std::cout << "End of tokens" << std::endl;
    }
    
    // Parse and execute
    try {
        Lexer lexer(source);
        Parser parser(lexer);
        
        // Enable trace debugging when requested
        if (traceDebug) {
            auto program = parser.parse();
            std::cout << "Parsing succeeded, executing program..." << std::endl;
            Interpreter interpreter;
            interpreter.execute(program);
        } else {
            auto program = parser.parse();
            Interpreter interpreter;
            interpreter.execute(program);
        }
    } catch (const ParseError& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        exit(1);
    } catch (const RuntimeError& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        exit(1);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    }
}

// Function to run the REPL (Read-Eval-Print Loop)
void runRepl() {
    std::cout << "SimpScript v1.0 - Interactive Mode" << std::endl;
    std::cout << "Type 'exit' to quit" << std::endl;
    
    Interpreter interpreter;
    std::string line;
    
    while (true) {
        std::cout << ">> ";
        if (!std::getline(std::cin, line) || line == "exit") {
            break;
        }
        
        try {
            Lexer lexer(line);
            Parser parser(lexer);
            auto program = parser.parse();
            
            Value result = interpreter.execute(program);
            if (!result.isNil()) {
                std::cout << result.toString() << std::endl;
            }
        } catch (const ParseError& e) {
            std::cerr << "Parse error: " << e.what() << std::endl;
        } catch (const RuntimeError& e) {
            std::cerr << "Runtime error: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 4) {
        std::cout << "Usage: simpscript [script] [--debug] [--trace]" << std::endl;
        return 1;
    } else if (argc >= 2) {
        bool debug = false;
        bool traceDebug = false;
        std::string scriptPath = argv[1];
        
        // Check for debug flags
        for (int i = 2; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "--debug") {
                debug = true;
            } else if (arg == "--trace") {
                traceDebug = true;
            }
        }
        
        // Run the provided script file
        runFile(scriptPath, debug, traceDebug);
    } else {
        // Run the REPL
        runRepl();
    }
    
    return 0;
} 