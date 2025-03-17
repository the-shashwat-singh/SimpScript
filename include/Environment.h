#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Value.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace SimpScript {

class Environment {
private:
    std::unordered_map<std::string, Value> values;
    std::shared_ptr<Environment> enclosing;

public:
    // Create a global environment
    Environment();
    
    // Create a local environment with the given enclosing environment
    explicit Environment(std::shared_ptr<Environment> enclosing);
    
    // Define a new variable in the current environment
    void define(const std::string& name, const Value& value);
    
    // Get the value of a variable by name
    Value get(const std::string& name);
    
    // Assign a new value to an existing variable
    void assign(const std::string& name, const Value& value);
    
    // Check if a variable exists in the current environment
    bool exists(const std::string& name) const;
    
    // Get the enclosing environment
    std::shared_ptr<Environment> getEnclosing() const;
};

} // namespace SimpScript

#endif // ENVIRONMENT_H 