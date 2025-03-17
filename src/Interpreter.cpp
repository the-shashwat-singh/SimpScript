#include "Interpreter.h"
#include "AST.h"
#include "Value.h"
#include "Environment.h"
#include <iostream>
#include <string>
#include <functional>

namespace SimpScript {

// RuntimeError implementation
RuntimeError::RuntimeError(const std::string& message)
    : std::runtime_error(message) {}

// ReturnValue implementation
ReturnValue::ReturnValue(const Value& value)
    : std::runtime_error("Return value"), value(value) {}

Value ReturnValue::getValue() const {
    return value;
}

// Interpreter implementation
Interpreter::Interpreter() {
    globals = std::make_shared<Environment>();
    environment = globals;
    
    setupGlobals();
}

void Interpreter::setupGlobals() {
    // Setup built-in functions and values here
    
    // Function to print text without a newline
    auto show = std::make_shared<NativeFunction>(1, [](std::vector<Value>& args) -> Value {
        std::cout << args[0].toString();
        return Value();
    });
    
    // Function to print text with a newline
    auto shownl = std::make_shared<NativeFunction>(1, [](std::vector<Value>& args) -> Value {
        std::cout << args[0].toString() << std::endl;
        return Value();
    });
    
    // Function to read a line from standard input
    auto ask = std::make_shared<NativeFunction>(0, [](std::vector<Value>& args) -> Value {
        std::string input;
        std::getline(std::cin, input);
        return Value(input);
    });
    
    // Newline constant for use in string concatenation
    
    // Add built-in functions to global environment
    globals->define("show", Value(show));
    globals->define("shownl", Value(shownl));
    globals->define("ask", Value(ask));
    globals->define("nextl", Value("\n"));
    
    // Array methods
    // size() method for arrays and strings
    auto size = std::make_shared<NativeFunction>(1, [](std::vector<Value>& args) -> Value {
        Value& target = args[0];
        return Value(target.size());
    });
    globals->define("size", Value(size));
}

// Evaluate an AST node
Value Interpreter::evaluate(ASTNode* node) {
    return node->evaluate(*this);
}

// Execute a program
Value Interpreter::execute(const std::unique_ptr<ASTNode>& program) {
    return program->evaluate(*this);
}

// Environment access for functions
std::shared_ptr<Environment> Interpreter::getEnvironment() {
    return environment;
}

std::shared_ptr<Environment> Interpreter::getGlobals() {
    return globals;
}

void Interpreter::setEnvironment(std::shared_ptr<Environment> env) {
    environment = env;
}

// Helper methods for the REPL
void Interpreter::defineVariable(const std::string& name, const Value& value) {
    globals->define(name, value);
}

Value Interpreter::getVariable(const std::string& name) {
    return globals->get(name);
}

} // namespace SimpScript 