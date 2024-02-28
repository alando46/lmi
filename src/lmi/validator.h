#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <map>
#include <functional>
#include <regex>

/// @brief Base Validator Class
class BaseValidator {
    public:
        virtual bool validate(const nlohmann::json& j) const = 0;
};

/// @brief Regex Validator Class
class RegexValidator : public BaseValidator {
    std::regex pattern;
    public:
        RegexValidator(const std::string& pattern_str) : pattern(pattern_str) {}
        bool validate(const nlohmann::json& j) const override;
};

class LengthValidator : public BaseValidator {
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
