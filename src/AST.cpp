#include "AST.h"
#include "Interpreter.h"
#include "Environment.h"
#include "Value.h"
#include <stdexcept>
#include <iostream>

namespace SimpScript {

// Helper methods for assignment
std::string VariableNode::getName() const {
    return name;
}

std::unique_ptr<ASTNode> ArrayAccessNode::getArray() {
    return std::move(array);
}

std::unique_ptr<ASTNode> ArrayAccessNode::getIndex() {
    return std::move(index);
}

// LiteralNode implementation
LiteralNode::LiteralNode(int value) : value(value) {}
LiteralNode::LiteralNode(double value) : value(value) {}
LiteralNode::LiteralNode(const std::string& value) : value(value) {}
LiteralNode::LiteralNode(bool value) : value(value) {}

Value LiteralNode::evaluate(Interpreter& interpreter) {
    if (std::holds_alternative<int>(value)) {
        return Value(std::get<int>(value));
    } else if (std::holds_alternative<double>(value)) {
        return Value(std::get<double>(value));
    } else if (std::holds_alternative<std::string>(value)) {
        return Value(std::get<std::string>(value));
    } else if (std::holds_alternative<bool>(value)) {
        return Value(std::get<bool>(value));
    }
    
    return Value(); // nil
}

// VariableNode implementation
VariableNode::VariableNode(const std::string& name) : name(name) {}

Value VariableNode::evaluate(Interpreter& interpreter) {
    return interpreter.getEnvironment()->get(name);
}

// BinaryOpNode implementation
BinaryOpNode::BinaryOpNode(OpType opType, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
    : opType(opType), left(std::move(left)), right(std::move(right)) {}

Value BinaryOpNode::evaluate(Interpreter& interpreter) {
    Value leftVal = left->evaluate(interpreter);
    Value rightVal = right->evaluate(interpreter);
    
    switch (opType) {
        case OpType::ADD:
            return leftVal + rightVal;
        case OpType::SUB:
            return leftVal - rightVal;
        case OpType::MUL:
            return leftVal * rightVal;
        case OpType::DIV:
            return leftVal / rightVal;
        case OpType::MOD:
            return leftVal % rightVal;
        case OpType::EQ:
            return Value(leftVal == rightVal);
        case OpType::NEQ:
            return Value(leftVal != rightVal);
        case OpType::GT:
            return Value(leftVal > rightVal);
        case OpType::LT:
            return Value(leftVal < rightVal);
        case OpType::GTE:
            return Value(leftVal >= rightVal);
        case OpType::LTE:
            return Value(leftVal <= rightVal);
        case OpType::AND:
            return Value(leftVal.isTruthy() && rightVal.isTruthy());
        case OpType::OR:
            return Value(leftVal.isTruthy() || rightVal.isTruthy());
    }
    
    throw std::runtime_error("Unknown binary operator");
}

// UnaryOpNode implementation
UnaryOpNode::UnaryOpNode(OpType opType, std::unique_ptr<ASTNode> operand)
    : opType(opType), operand(std::move(operand)) {}

Value UnaryOpNode::evaluate(Interpreter& interpreter) {
    Value val = operand->evaluate(interpreter);
    
    switch (opType) {
        case OpType::NOT:
            return Value(!val.isTruthy());
        case OpType::NEGATIVE:
            if (val.isInteger()) {
                return Value(-val.asInteger());
            } else if (val.isFloat()) {
                return Value(-val.asFloat());
            }
            throw std::runtime_error("Cannot negate non-numeric value");
    }
    
    throw std::runtime_error("Unknown unary operator");
}

// ArrayLiteralNode implementation
ArrayLiteralNode::ArrayLiteralNode(std::vector<std::unique_ptr<ASTNode>> elements)
    : elements(std::move(elements)) {}

Value ArrayLiteralNode::evaluate(Interpreter& interpreter) {
    std::vector<Value> values;
    
    for (const auto& element : elements) {
        values.push_back(element->evaluate(interpreter));
    }
    
    return Value(values);
}

// ArrayAccessNode implementation
ArrayAccessNode::ArrayAccessNode(std::unique_ptr<ASTNode> array, std::unique_ptr<ASTNode> index)
    : array(std::move(array)), index(std::move(index)) {}

Value ArrayAccessNode::evaluate(Interpreter& interpreter) {
    Value arrayVal = array->evaluate(interpreter);
    Value indexVal = index->evaluate(interpreter);
    
    if (!arrayVal.isArray()) {
        throw std::runtime_error("Cannot index non-array value");
    }
    
    if (!indexVal.isInteger()) {
        throw std::runtime_error("Array index must be an integer");
    }
    
    return arrayVal.at(indexVal.asInteger());
}

// FunctionCallNode implementation
FunctionCallNode::FunctionCallNode(const std::string& name, std::vector<std::unique_ptr<ASTNode>> arguments)
    : name(name), arguments(std::move(arguments)) {}

Value FunctionCallNode::evaluate(Interpreter& interpreter) {
    // Evaluate function value
    Value function = interpreter.getEnvironment()->get(name);
    
    // Evaluate arguments
    std::vector<Value> args;
    for (const auto& arg : arguments) {
        args.push_back(arg->evaluate(interpreter));
    }
    
    // Call function
    return function.call(args);
}

// BlockNode implementation
BlockNode::BlockNode(std::vector<std::unique_ptr<ASTNode>> statements)
    : statements(std::move(statements)) {}

Value BlockNode::evaluate(Interpreter& interpreter) {
    Value result;
    
    for (const auto& statement : statements) {
        result = statement->evaluate(interpreter);
    }
    
    return result;
}

// AssignmentNode implementation
AssignmentNode::AssignmentNode(const std::string& name, std::unique_ptr<ASTNode> expression)
    : name(name), expression(std::move(expression)) {}

Value AssignmentNode::evaluate(Interpreter& interpreter) {
    Value value = expression->evaluate(interpreter);
    
    try {
        // Try to assign to existing variable
        interpreter.getEnvironment()->assign(name, value);
    } catch (const std::runtime_error&) {
        // Variable doesn't exist, define it
        interpreter.getEnvironment()->define(name, value);
    }
    
    return value;
}

// ArrayAssignmentNode implementation
ArrayAssignmentNode::ArrayAssignmentNode(std::unique_ptr<ASTNode> array, std::unique_ptr<ASTNode> index, std::unique_ptr<ASTNode> value)
    : array(std::move(array)), index(std::move(index)), value(std::move(value)) {}

Value ArrayAssignmentNode::evaluate(Interpreter& interpreter) {
    Value arrayVal = array->evaluate(interpreter);
    Value indexVal = index->evaluate(interpreter);
    Value val = value->evaluate(interpreter);
    
    if (!arrayVal.isArray()) {
        throw std::runtime_error("Cannot index non-array value");
    }
    
    if (!indexVal.isInteger()) {
        throw std::runtime_error("Array index must be an integer");
    }
    
    arrayVal.set(indexVal.asInteger(), val);
    return val;
}

// IfNode implementation
IfNode::IfNode(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> thenBranch, std::unique_ptr<ASTNode> elseBranch)
    : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

Value IfNode::evaluate(Interpreter& interpreter) {
    if (condition->evaluate(interpreter).isTruthy()) {
        return thenBranch->evaluate(interpreter);
    } else if (elseBranch) {
        return elseBranch->evaluate(interpreter);
    }
    
    return Value(); // nil
}

// WhileNode implementation
WhileNode::WhileNode(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> body)
    : condition(std::move(condition)), body(std::move(body)) {}

Value WhileNode::evaluate(Interpreter& interpreter) {
    Value result;
    
    while (condition->evaluate(interpreter).isTruthy()) {
        result = body->evaluate(interpreter);
    }
    
    return result;
}

// ForNode implementation
ForNode::ForNode(std::unique_ptr<ASTNode> initialization, std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> increment, std::unique_ptr<ASTNode> body)
    : initialization(std::move(initialization)), condition(std::move(condition)), increment(std::move(increment)), body(std::move(body)) {}

Value ForNode::evaluate(Interpreter& interpreter) {
    // Create a new environment for the loop
    auto enclosing = interpreter.getEnvironment();
    auto loopEnv = std::make_shared<Environment>(enclosing);
    interpreter.setEnvironment(loopEnv);
    
    Value result;
    
    try {
        // Initialize
        initialization->evaluate(interpreter);
        
        // Loop
        while (condition->evaluate(interpreter).isTruthy()) {
            result = body->evaluate(interpreter);
            increment->evaluate(interpreter);
        }
    } catch (...) {
        // Restore environment on error
        interpreter.setEnvironment(enclosing);
        throw;
    }
    
    // Restore environment
    interpreter.setEnvironment(enclosing);
    
    return result;
}

// FunctionDefNode implementation
FunctionDefNode::FunctionDefNode(const std::string& name, const std::vector<std::string>& parameters, std::unique_ptr<ASTNode> body)
    : name(name), parameters(parameters), body(std::move(body)) {}

Value FunctionDefNode::evaluate(Interpreter& interpreter) {
    // Create the function object
    auto function = std::make_shared<UserFunction>(
        parameters,
        body->clone(),
        interpreter.getEnvironment()
    );
    
    // Define the function in the current environment
    interpreter.getEnvironment()->define(name, Value(function));
    
    return Value(); // nil
}

// ReturnNode implementation
ReturnNode::ReturnNode(std::unique_ptr<ASTNode> expression)
    : expression(std::move(expression)) {}

Value ReturnNode::evaluate(Interpreter& interpreter) {
    Value value = expression->evaluate(interpreter);
    throw ReturnValue(value);
}

// PrintNode implementation
PrintNode::PrintNode(std::unique_ptr<ASTNode> expression, bool newline)
    : expression(std::move(expression)), newline(newline) {}

Value PrintNode::evaluate(Interpreter& interpreter) {
    Value value = expression->evaluate(interpreter);
    
    if (newline) {
        std::cout << value.toString() << std::endl;
    } else {
        std::cout << value.toString();
    }
    
    return value;
}

// InputNode implementation
Value InputNode::evaluate(Interpreter&) {
    std::string input;
    std::getline(std::cin, input);
    return Value(input);
}

// ProgramNode implementation
ProgramNode::ProgramNode(std::vector<std::unique_ptr<ASTNode>> statements)
    : statements(std::move(statements)) {}

Value ProgramNode::evaluate(Interpreter& interpreter) {
    Value result;
    
    for (const auto& statement : statements) {
        result = statement->evaluate(interpreter);
    }
    
    return result;
}

// Clone methods for AST nodes
std::unique_ptr<ASTNode> LiteralNode::clone() const {
    if (std::holds_alternative<int>(value)) {
        return std::make_unique<LiteralNode>(std::get<int>(value));
    } else if (std::holds_alternative<double>(value)) {
        return std::make_unique<LiteralNode>(std::get<double>(value));
    } else if (std::holds_alternative<std::string>(value)) {
        return std::make_unique<LiteralNode>(std::get<std::string>(value));
    } else if (std::holds_alternative<bool>(value)) {
        return std::make_unique<LiteralNode>(std::get<bool>(value));
    }
    return std::make_unique<LiteralNode>(0);
}

std::unique_ptr<ASTNode> VariableNode::clone() const {
    return std::make_unique<VariableNode>(name);
}

std::unique_ptr<ASTNode> BinaryOpNode::clone() const {
    return std::make_unique<BinaryOpNode>(
        opType,
        left->clone(),
        right->clone()
    );
}

std::unique_ptr<ASTNode> UnaryOpNode::clone() const {
    return std::make_unique<UnaryOpNode>(
        opType,
        operand->clone()
    );
}

std::unique_ptr<ASTNode> ArrayLiteralNode::clone() const {
    std::vector<std::unique_ptr<ASTNode>> clonedElements;
    for (const auto& element : elements) {
        clonedElements.push_back(element->clone());
    }
    return std::make_unique<ArrayLiteralNode>(std::move(clonedElements));
}

std::unique_ptr<ASTNode> ArrayAccessNode::clone() const {
    return std::make_unique<ArrayAccessNode>(
        array->clone(),
        index->clone()
    );
}

std::unique_ptr<ASTNode> FunctionCallNode::clone() const {
    std::vector<std::unique_ptr<ASTNode>> clonedArgs;
    for (const auto& arg : arguments) {
        clonedArgs.push_back(arg->clone());
    }
    return std::make_unique<FunctionCallNode>(name, std::move(clonedArgs));
}

std::unique_ptr<ASTNode> BlockNode::clone() const {
    std::vector<std::unique_ptr<ASTNode>> clonedStatements;
    for (const auto& statement : statements) {
        clonedStatements.push_back(statement->clone());
    }
    return std::make_unique<BlockNode>(std::move(clonedStatements));
}

std::unique_ptr<ASTNode> AssignmentNode::clone() const {
    return std::make_unique<AssignmentNode>(
        name,
        expression->clone()
    );
}

std::unique_ptr<ASTNode> ArrayAssignmentNode::clone() const {
    return std::make_unique<ArrayAssignmentNode>(
        array->clone(),
        index->clone(),
        value->clone()
    );
}

std::unique_ptr<ASTNode> IfNode::clone() const {
    std::unique_ptr<ASTNode> clonedElse = nullptr;
    if (elseBranch) {
        clonedElse = elseBranch->clone();
    }
    return std::make_unique<IfNode>(
        condition->clone(),
        thenBranch->clone(),
        std::move(clonedElse)
    );
}

std::unique_ptr<ASTNode> WhileNode::clone() const {
    return std::make_unique<WhileNode>(
        condition->clone(),
        body->clone()
    );
}

std::unique_ptr<ASTNode> ForNode::clone() const {
    return std::make_unique<ForNode>(
        initialization->clone(),
        condition->clone(),
        increment->clone(),
        body->clone()
    );
}

std::unique_ptr<ASTNode> FunctionDefNode::clone() const {
    return std::make_unique<FunctionDefNode>(
        name,
        parameters,
        body->clone()
    );
}

std::unique_ptr<ASTNode> ReturnNode::clone() const {
    return std::make_unique<ReturnNode>(expression->clone());
}

std::unique_ptr<ASTNode> PrintNode::clone() const {
    return std::make_unique<PrintNode>(expression->clone(), newline);
}

std::unique_ptr<ASTNode> InputNode::clone() const {
    return std::make_unique<InputNode>();
}

std::unique_ptr<ASTNode> ProgramNode::clone() const {
    std::vector<std::unique_ptr<ASTNode>> clonedStatements;
    for (const auto& statement : statements) {
        clonedStatements.push_back(statement->clone());
    }
    return std::make_unique<ProgramNode>(std::move(clonedStatements));
}

} // namespace SimpScript 