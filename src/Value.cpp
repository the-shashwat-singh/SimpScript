#include "Value.h"
#include "Environment.h"
#include "AST.h"
#include "Interpreter.h"
#include <sstream>
#include <stdexcept>

namespace SimpScript {

// NativeFunction implementation
NativeFunction::NativeFunction(int arity, std::function<Value(std::vector<Value>&)> function)
    : _arity(arity), function(function) {}

int NativeFunction::arity() const {
    return _arity;
}

Value NativeFunction::call(std::vector<Value>& arguments) {
    return function(arguments);
}

// UserFunction implementation
UserFunction::UserFunction(const std::vector<std::string>& parameters, 
                           std::unique_ptr<ASTNode> body,
                           std::shared_ptr<Environment> closure)
    : parameters(parameters), body(std::move(body)), closure(closure) {}

int UserFunction::arity() const {
    return parameters.size();
}

Value UserFunction::call(std::vector<Value>& arguments) {
    // Create a new environment using the closure as the enclosing environment
    auto environment = std::make_shared<Environment>(closure);
    
    // Bind arguments to parameters
    for (size_t i = 0; i < parameters.size(); i++) {
        // If there are more parameters than arguments, the rest are nil
        if (i < arguments.size()) {
            environment->define(parameters[i], arguments[i]);
        } else {
            environment->define(parameters[i], Value());
        }
    }
    
    try {
        // Execute the function body in the new environment
        return body->evaluate(*reinterpret_cast<class Interpreter*>(environment.get()));
    } catch (const ReturnValue& returnValue) {
        // Catch return statements from within the function
        return returnValue.getValue();
    }
}

// Value implementation
Value::Value() : data(std::monostate()), type(Type::NIL) {}
Value::Value(bool value) : data(value), type(Type::BOOLEAN) {}
Value::Value(int value) : data(value), type(Type::INTEGER) {}
Value::Value(double value) : data(value), type(Type::FLOAT) {}
Value::Value(const std::string& value) : data(value), type(Type::STRING) {}
Value::Value(const ArrayType& array) : data(array), type(Type::ARRAY) {}
Value::Value(const FunctionType& function) : data(function), type(Type::FUNCTION) {}

bool Value::isNil() const { return type == Type::NIL; }
bool Value::isBoolean() const { return type == Type::BOOLEAN; }
bool Value::isInteger() const { return type == Type::INTEGER; }
bool Value::isFloat() const { return type == Type::FLOAT; }
bool Value::isNumber() const { return isInteger() || isFloat(); }
bool Value::isString() const { return type == Type::STRING; }
bool Value::isArray() const { return type == Type::ARRAY; }
bool Value::isFunction() const { return type == Type::FUNCTION; }

Value::Type Value::getType() const { return type; }

bool Value::asBoolean() const {
    if (!isBoolean()) {
        throw std::runtime_error("Value is not a boolean");
    }
    return std::get<bool>(data);
}

int Value::asInteger() const {
    if (isInteger()) {
        return std::get<int>(data);
    } else if (isFloat()) {
        return static_cast<int>(std::get<double>(data));
    }
    throw std::runtime_error("Value is not an integer");
}

double Value::asFloat() const {
    if (isFloat()) {
        return std::get<double>(data);
    } else if (isInteger()) {
        return static_cast<double>(std::get<int>(data));
    }
    throw std::runtime_error("Value is not a number");
}

std::string Value::asString() const {
    if (isString()) {
        return std::get<std::string>(data);
    }
    return toString();
}

Value::ArrayType& Value::asArray() {
    if (!isArray()) {
        throw std::runtime_error("Value is not an array");
    }
    return std::get<ArrayType>(data);
}

const Value::ArrayType& Value::asArray() const {
    if (!isArray()) {
        throw std::runtime_error("Value is not an array");
    }
    return std::get<ArrayType>(data);
}

Value::FunctionType Value::asFunction() const {
    if (!isFunction()) {
        throw std::runtime_error("Value is not a function");
    }
    return std::get<FunctionType>(data);
}

Value& Value::at(int index) {
    if (!isArray()) {
        throw std::runtime_error("Value is not an array");
    }
    ArrayType& array = std::get<ArrayType>(data);
    if (index < 0 || static_cast<size_t>(index) >= array.size()) {
        throw std::runtime_error("Array index out of bounds");
    }
    return array[index];
}

const Value& Value::at(int index) const {
    if (!isArray()) {
        throw std::runtime_error("Value is not an array");
    }
    const ArrayType& array = std::get<ArrayType>(data);
    if (index < 0 || static_cast<size_t>(index) >= array.size()) {
        throw std::runtime_error("Array index out of bounds");
    }
    return array[index];
}

void Value::set(int index, const Value& value) {
    if (!isArray()) {
        throw std::runtime_error("Value is not an array");
    }
    ArrayType& array = std::get<ArrayType>(data);
    if (index < 0 || static_cast<size_t>(index) >= array.size()) {
        throw std::runtime_error("Array index out of bounds");
    }
    array[index] = value;
}

int Value::size() const {
    if (isArray()) {
        return static_cast<int>(std::get<ArrayType>(data).size());
    } else if (isString()) {
        return static_cast<int>(std::get<std::string>(data).size());
    }
    throw std::runtime_error("Value does not have a size");
}

Value Value::call(std::vector<Value>& args) {
    if (!isFunction()) {
        throw std::runtime_error("Value is not callable");
    }
    
    FunctionType func = std::get<FunctionType>(data);
    
    // Check if number of arguments matches the function's arity
    if (static_cast<int>(args.size()) != func->arity()) {
        std::stringstream ss;
        ss << "Expected " << func->arity() << " arguments but got " << args.size();
        throw std::runtime_error(ss.str());
    }
    
    return func->call(args);
}

std::string Value::toString() const {
    std::stringstream ss;
    
    switch (type) {
        case Type::NIL:
            return "nil";
        case Type::BOOLEAN:
            return std::get<bool>(data) ? "true" : "false";
        case Type::INTEGER:
            ss << std::get<int>(data);
            return ss.str();
        case Type::FLOAT:
            ss << std::get<double>(data);
            return ss.str();
        case Type::STRING:
            return std::get<std::string>(data);
        case Type::ARRAY: {
            const ArrayType& array = std::get<ArrayType>(data);
            ss << "[";
            for (size_t i = 0; i < array.size(); i++) {
                if (i > 0) ss << ", ";
                ss << array[i].toString();
            }
            ss << "]";
            return ss.str();
        }
        case Type::FUNCTION:
            return "<function>";
        default:
            return "<unknown>";
    }
}

bool Value::isTruthy() const {
    if (isNil()) return false;
    if (isBoolean()) return asBoolean();
    if (isInteger()) return asInteger() != 0;
    if (isFloat()) return asFloat() != 0.0;
    if (isString()) return !asString().empty();
    if (isArray()) return !asArray().empty();
    return true;
}

// Arithmetic operators
Value Value::operator+(const Value& rhs) const {
    if (isString() || rhs.isString()) {
        // String concatenation
        return Value(this->toString() + rhs.toString());
    } else if (isNumber() && rhs.isNumber()) {
        // Numeric addition
        if (isFloat() || rhs.isFloat()) {
            return Value(asFloat() + rhs.asFloat());
        } else {
            return Value(asInteger() + rhs.asInteger());
        }
    }
    throw std::runtime_error("Cannot add these types");
}

Value Value::operator-(const Value& rhs) const {
    if (isNumber() && rhs.isNumber()) {
        if (isFloat() || rhs.isFloat()) {
            return Value(asFloat() - rhs.asFloat());
        } else {
            return Value(asInteger() - rhs.asInteger());
        }
    }
    throw std::runtime_error("Cannot subtract these types");
}

Value Value::operator*(const Value& rhs) const {
    if (isNumber() && rhs.isNumber()) {
        if (isFloat() || rhs.isFloat()) {
            return Value(asFloat() * rhs.asFloat());
        } else {
            return Value(asInteger() * rhs.asInteger());
        }
    }
    throw std::runtime_error("Cannot multiply these types");
}

Value Value::operator/(const Value& rhs) const {
    if (isNumber() && rhs.isNumber()) {
        if (rhs.isInteger() && rhs.asInteger() == 0) {
            throw std::runtime_error("Division by zero");
        }
        if (rhs.isFloat() && rhs.asFloat() == 0.0) {
            throw std::runtime_error("Division by zero");
        }
        
        if (isFloat() || rhs.isFloat()) {
            return Value(asFloat() / rhs.asFloat());
        } else {
            return Value(asInteger() / rhs.asInteger());
        }
    }
    throw std::runtime_error("Cannot divide these types");
}

Value Value::operator%(const Value& rhs) const {
    if (isInteger() && rhs.isInteger()) {
        if (rhs.asInteger() == 0) {
            throw std::runtime_error("Modulo by zero");
        }
        return Value(asInteger() % rhs.asInteger());
    }
    throw std::runtime_error("Modulo requires integer operands");
}

// Comparison operators
bool Value::operator==(const Value& rhs) const {
    if (type != rhs.type) {
        // Different types are always unequal
        // except for numeric types
        if (isNumber() && rhs.isNumber()) {
            return asFloat() == rhs.asFloat();
        }
        return false;
    }
    
    switch (type) {
        case Type::NIL:
            return true; // nil == nil
        case Type::BOOLEAN:
            return asBoolean() == rhs.asBoolean();
        case Type::INTEGER:
            return asInteger() == rhs.asInteger();
        case Type::FLOAT:
            return asFloat() == rhs.asFloat();
        case Type::STRING:
            return asString() == rhs.asString();
        case Type::ARRAY: {
            const ArrayType& a = asArray();
            const ArrayType& b = rhs.asArray();
            if (a.size() != b.size()) return false;
            for (size_t i = 0; i < a.size(); i++) {
                if (!(a[i] == b[i])) return false;
            }
            return true;
        }
        case Type::FUNCTION:
            // Functions are only equal if they're the same object
            return asFunction() == rhs.asFunction();
        default:
            return false;
    }
}

bool Value::operator!=(const Value& rhs) const {
    return !(*this == rhs);
}

bool Value::operator<(const Value& rhs) const {
    if (isNumber() && rhs.isNumber()) {
        return asFloat() < rhs.asFloat();
    } else if (isString() && rhs.isString()) {
        return asString() < rhs.asString();
    }
    throw std::runtime_error("Cannot compare these types with <");
}

bool Value::operator<=(const Value& rhs) const {
    return *this < rhs || *this == rhs;
}

bool Value::operator>(const Value& rhs) const {
    return !(*this <= rhs);
}

bool Value::operator>=(const Value& rhs) const {
    return !(*this < rhs);
}

} // namespace SimpScript 