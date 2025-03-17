#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <variant>
#include <memory>

namespace SimpScript {

// Forward declarations
class ASTNode;
class Environment;

// Represents callable functions (both native and user-defined)
class Callable {
public:
    virtual ~Callable() = default;
    virtual int arity() const = 0;
    virtual class Value call(std::vector<class Value>& arguments) = 0;
};

// Native function (C++ implemented)
class NativeFunction : public Callable {
private:
    int _arity;
    std::function<class Value(std::vector<class Value>&)> function;

public:
    NativeFunction(int arity, std::function<class Value(std::vector<class Value>&)> function);
    int arity() const override;
    class Value call(std::vector<class Value>& arguments) override;
};

// User-defined function
class UserFunction : public Callable {
private:
    std::vector<std::string> parameters;
    std::unique_ptr<ASTNode> body;
    std::shared_ptr<Environment> closure;

public:
    UserFunction(const std::vector<std::string>& parameters, 
                 std::unique_ptr<ASTNode> body,
                 std::shared_ptr<Environment> closure);
    int arity() const override;
    class Value call(std::vector<class Value>& arguments) override;
};

// Represents a runtime value in SimpScript
class Value {
public:
    // Value types
    enum class Type {
        NIL,
        BOOLEAN,
        INTEGER,
        FLOAT,
        STRING,
        ARRAY,
        FUNCTION,
        NATIVE_FUNCTION
    };

private:
    // Internal value storage using std::variant
    using ArrayType = std::vector<Value>;
    using FunctionType = std::shared_ptr<Callable>;
    
    std::variant<
        std::monostate,       // NIL
        bool,                 // BOOLEAN
        int,                  // INTEGER
        double,               // FLOAT
        std::string,          // STRING
        ArrayType,            // ARRAY
        FunctionType          // FUNCTION
    > data;
    
    Type type;

public:
    // Constructors
    Value(); // NIL value
    explicit Value(bool value);
    explicit Value(int value);
    explicit Value(double value);
    explicit Value(const std::string& value);
    explicit Value(const ArrayType& array);
    explicit Value(const FunctionType& function);

    // Type checking
    bool isNil() const;
    bool isBoolean() const;
    bool isInteger() const;
    bool isFloat() const;
    bool isNumber() const;
    bool isString() const;
    bool isArray() const;
    bool isFunction() const;
    
    Type getType() const;

    // Value extraction
    bool asBoolean() const;
    int asInteger() const;
    double asFloat() const;
    std::string asString() const;
    ArrayType& asArray();
    const ArrayType& asArray() const;
    FunctionType asFunction() const;

    // Array operations
    Value& at(int index);
    const Value& at(int index) const;
    void set(int index, const Value& value);
    int size() const;
    
    // Function operations
    Value call(std::vector<Value>& args);
    
    // Utility methods
    std::string toString() const;
    bool isTruthy() const;
    
    // Operators
    Value operator+(const Value& rhs) const;
    Value operator-(const Value& rhs) const;
    Value operator*(const Value& rhs) const;
    Value operator/(const Value& rhs) const;
    Value operator%(const Value& rhs) const;
    
    bool operator==(const Value& rhs) const;
    bool operator!=(const Value& rhs) const;
    bool operator<(const Value& rhs) const;
    bool operator<=(const Value& rhs) const;
    bool operator>(const Value& rhs) const;
    bool operator>=(const Value& rhs) const;
};

} // namespace SimpScript

#endif // VALUE_H 