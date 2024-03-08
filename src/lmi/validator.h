#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <map>
#include <functional>
#include <regex>
#include <string>
#include <optional>
#include "exceptions.h"

/// @brief Base Validator Class
template<typename T>
class BaseFieldValidator {
    public:
        virtual T validate(const T& data) const = 0;
        virtual nlohmann::json getValidationSchema() const = 0;
        virtual std::string getName() const = 0;
        virtual ~BaseFieldValidator() {}
};

class LTValidator : public BaseFieldValidator {
    public:
        LTValidator(int validationParam): validationParam_(validationParam) {};
        std:: validate(const int& data) override {
            std::string argName = "bob";
            if (!(data < validationParam_)) {
                std::string errorMessage = "Validation for arg: `" + argName + "` failed. " + std::to_string(data) + " is not less than " + std::to_string(validationParam_) + "." << std::endl;
                throw IntValidationError(errorMessage, data, argName);
            }
            return data;
        }

        nlohmann::json getValidationSchema() {
            return
        }
    private:
        int validationParam_;
};


// struct LT {
//     int validate(int rawArg, std::string rawArgRep, int validatorParam) {
//         if !(rawArg < validatorParam) {
//             throw InputError "{rawArgRep} must be less than {validatorParam}";
//         return rawArg;
//         }
//     }
//     json validationSchema() {

//     }
// };

/// @brief Regex Validator Class
class RegexValidator : public BaseFieldValidator {
    std::regex pattern;
    public:
        RegexValidator(const std::string& pattern_str) : pattern(pattern_str) {}
        bool validate(const nlohmann::json& j) const override;
};

class LengthValidator : public BaseFieldValidator {
    size_t min_len, max_len;
    public:
        LengthValidator(size_t min_len, size_t max_len) : min_len(min_len), max_len(max_len) {}
        bool validate(const nlohmann::json& j) const override;
};

class JsonSchemaValidator {
    protected:
        nlohmann::json object;
        std::map<std::string, std::shared_ptr<BaseValidator>> validators;
        bool validate();

    public:
        JsonSchemaValidator(const nlohmann::json& j) : object(j) {}

        virtual void setupValidators() = 0;
        virtual bool isValid() {
            setupValidators();
            return validate();
        }
};
