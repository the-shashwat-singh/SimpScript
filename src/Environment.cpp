#include "Environment.h"
#include <stdexcept>
#include <sstream>

namespace SimpScript {

// Constructor for global environment
Environment::Environment() : enclosing(nullptr) {}

// Constructor for local environment with an enclosing environment
Environment::Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}

// Define a variable in the current environment
void Environment::define(const std::string& name, const Value& value) {
    values[name] = value;
}

// Get a variable's value from the environment
Value Environment::get(const std::string& name) {
    auto it = values.find(name);
    if (it != values.end()) {
        return it->second;
    }
    
    // If not found in current environment, look in the enclosing one
    if (enclosing != nullptr) {
        return enclosing->get(name);
    }
    
    // Variable not found
    std::stringstream error;
    error << "Undefined variable '" << name << "'";
    throw std::runtime_error(error.str());
}

// Assign a new value to an existing variable
void Environment::assign(const std::string& name, const Value& value) {
    auto it = values.find(name);
    if (it != values.end()) {
        it->second = value;
        return;
    }
    
    // If not found in current environment, try to assign in the enclosing one
    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }
    
    // Variable not found
    std::stringstream error;
    error << "Undefined variable '" << name << "'";
    throw std::runtime_error(error.str());
}

// Check if a variable exists in the current environment
bool Environment::exists(const std::string& name) const {
    return values.find(name) != values.end();
}

// Get the enclosing environment
std::shared_ptr<Environment> Environment::getEnclosing() const {
    return enclosing;
}

} // namespace SimpScript 