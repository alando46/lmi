#pragma once

#include <stdexcept>
#include <string>

class ValidationError : public std::runtime_error {
public:
    ValidationError(const std::string& repr, const std::string& message)
    : std::runtime_error(message+repr), repr_(repr) {}

    const std::string& repr() const noexcept {
        return repr_;
    }
private:
    std::string repr_;

};
