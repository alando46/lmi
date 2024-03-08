#pragma once

#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>

class BaseValidationError : public std::runtime_error {
public:
    // Constructor: Only initializes the base class with the error message.
    explicit BaseValidationError(const std::string& message)
        : std::runtime_error(message) {}
};

class IntValidationError : public BaseValidationError {
public:
    // Constructor that takes an error message and the invalid integer value
    IntValidationError(const std::string& message, int invalidValue, std::string& argName)
        : BaseValidationError(message), invalidValue_(invalidValue), argName_(argName) {}

    // Getter for the invalid integer value
    int getInvalidValue() const {
        return invalidValue_;
    }

private:
    int invalidValue_; // Store the invalid integer value that caused the error
    std::string argName_;
};


class ModelValidatrionErrors : public std::runtime_error {
public:
    ModelValidatrionErrors(const std::vector<BaseValidationError>& errors)
        : std::runtime_error("One or more validation errors occurred"), errors_(errors) {}

    // Adds an error to the collection
    void addError(const BaseValidationError& error) {
        errors_.push_back(error);
    }

    // Returns a detailed error message including all validation errors
    virtual const char* what() const noexcept override {
        std::ostringstream oss;
        oss << std::runtime_error::what() << "\n";
        for (const auto& error : errors_) {
            oss << error.what() << "\n"; // Assuming BaseValidationError inherits from std::exception
        }
        detailed_message_ = oss.str(); // Cache the message
        return detailed_message_.c_str();
    }

    // Optionally, provide a way to iterate over or retrieve the errors
    const std::vector<BaseValidationError>& getErrors() const { return errors_; }

private:
    mutable std::string detailed_message_; // mutable for lazy evaluation in what()
    std::vector<BaseValidationError> errors_;
};
