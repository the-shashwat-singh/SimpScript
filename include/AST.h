#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>

namespace SimpScript {

// Forward declarations
class Interpreter;
class Value;

// Base class for all AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual Value evaluate(Interpreter& interpreter) = 0;
    virtual std::unique_ptr<ASTNode> clone() const = 0;
};

// Expression nodes

// Literal (constant) values
class LiteralNode : public ASTNode {
private:
    std::variant<int, double, std::string, bool> value;

public:
    explicit LiteralNode(int value);
    explicit LiteralNode(double value);
    explicit LiteralNode(const std::string& value);
    explicit LiteralNode(bool value);

    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// Variable reference
class VariableNode : public ASTNode {
private:
    std::string name;

public:
    explicit VariableNode(const std::string& name);
    Value evaluate(Interpreter& interpreter) override;
    std::string getName() const;
    std::unique_ptr<ASTNode> clone() const override;
};

// Binary operations (arithmetic, logical, comparison)
class BinaryOpNode : public ASTNode {
public:
    enum class OpType {
        // Arithmetic
        ADD, SUB, MUL, DIV, MOD,
        // Comparison
        EQ, NEQ, GT, LT, GTE, LTE,
        // Logical
        AND, OR
    };

private:
    OpType opType;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

public:
    BinaryOpNode(OpType opType, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// Unary operations (not, negative)
class UnaryOpNode : public ASTNode {
public:
    enum class OpType {
        NOT, NEGATIVE
    };

private:
    OpType opType;
    std::unique_ptr<ASTNode> operand;

public:
    UnaryOpNode(OpType opType, std::unique_ptr<ASTNode> operand);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// Array literal [1, 2, 3]
class ArrayLiteralNode : public ASTNode {
private:
    std::vector<std::unique_ptr<ASTNode>> elements;

public:
    explicit ArrayLiteralNode(std::vector<std::unique_ptr<ASTNode>> elements);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// Array access a[index]
class ArrayAccessNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> array;
    std::unique_ptr<ASTNode> index;

public:
    ArrayAccessNode(std::unique_ptr<ASTNode> array, std::unique_ptr<ASTNode> index);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> getArray();
    std::unique_ptr<ASTNode> getIndex();
    std::unique_ptr<ASTNode> clone() const override;
};

// Function call node
class FunctionCallNode : public ASTNode {
private:
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> arguments;

public:
    FunctionCallNode(const std::string& name, std::vector<std::unique_ptr<ASTNode>> arguments);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// Statement nodes

// Block of statements
class BlockNode : public ASTNode {
private:
    std::vector<std::unique_ptr<ASTNode>> statements;

public:
    explicit BlockNode(std::vector<std::unique_ptr<ASTNode>> statements);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// Variable assignment
class AssignmentNode : public ASTNode {
private:
    std::string name;
    std::unique_ptr<ASTNode> expression;

public:
    AssignmentNode(const std::string& name, std::unique_ptr<ASTNode> expression);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// Array element assignment (a[index] = value)
class ArrayAssignmentNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> array;
    std::unique_ptr<ASTNode> index;
    std::unique_ptr<ASTNode> value;

public:
    ArrayAssignmentNode(std::unique_ptr<ASTNode> array, std::unique_ptr<ASTNode> index, std::unique_ptr<ASTNode> value);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// If statement
class IfNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBranch;
    std::unique_ptr<ASTNode> elseBranch; // Optional

public:
    IfNode(std::unique_ptr<ASTNode> condition, 
           std::unique_ptr<ASTNode> thenBranch,
           std::unique_ptr<ASTNode> elseBranch = nullptr);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// While loop
class WhileNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> body;

public:
    WhileNode(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> body);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// For loop
class ForNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> initialization;
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> increment;
    std::unique_ptr<ASTNode> body;

public:
    ForNode(std::unique_ptr<ASTNode> initialization,
            std::unique_ptr<ASTNode> condition,
            std::unique_ptr<ASTNode> increment,
            std::unique_ptr<ASTNode> body);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// Function definition
class FunctionDefNode : public ASTNode {
private:
    std::string name;
    std::vector<std::string> parameters;
    std::unique_ptr<ASTNode> body;

public:
    FunctionDefNode(const std::string& name,
                    const std::vector<std::string>& parameters,
                    std::unique_ptr<ASTNode> body);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// Return statement
class ReturnNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> expression;

public:
    explicit ReturnNode(std::unique_ptr<ASTNode> expression);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// Print statement (show)
class PrintNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> expression;
    bool newline;

public:
    PrintNode(std::unique_ptr<ASTNode> expression, bool newline);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// Input statement (ask)
class InputNode : public ASTNode {
public:
    InputNode() = default;
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

// Program node (root of AST)
class ProgramNode : public ASTNode {
private:
    std::vector<std::unique_ptr<ASTNode>> statements;

public:
    explicit ProgramNode(std::vector<std::unique_ptr<ASTNode>> statements);
    Value evaluate(Interpreter& interpreter) override;
    std::unique_ptr<ASTNode> clone() const override;
};

} // namespace SimpScript

#endif // AST_H 