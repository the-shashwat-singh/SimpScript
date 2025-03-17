#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "AST.h"
#include "Value.h"
#include "Environment.h"
#include <memory>
#include <stdexcept>
#include <string>

namespace SimpScript {

// Custom exception for runtime errors
class RuntimeError : public std::runtime_error {
public:
    explicit RuntimeError(const std::string& message);
};

// Custom exception for return statements
class ReturnValue : public std::runtime_error {
private:
    Value value;

public:
    explicit ReturnValue(const Value& value);
    Value getValue() const;
};

class Interpreter {
private:
    std::shared_ptr<Environment> environment;
    std::shared_ptr<Environment> globals;

    // Setup global environment with native functions
    void setupGlobals();

public:
    Interpreter();
    
    // Evaluate an AST node and return its value
    Value evaluate(ASTNode* node);
    
    // Execute a program
    Value execute(const std::unique_ptr<ASTNode>& program);
    
    // Environment access for functions
    std::shared_ptr<Environment> getEnvironment();
    std::shared_ptr<Environment> getGlobals();
    void setEnvironment(std::shared_ptr<Environment> env);
    
    // Helper methods for the REPL
    void defineVariable(const std::string& name, const Value& value);
    Value getVariable(const std::string& name);
};

} // namespace SimpScript

#endif // INTERPRETER_H 