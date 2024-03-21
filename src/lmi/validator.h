#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <map>
#include <functional>
#include <regex>
#include <string>
#include <optional>
#include "exceptions.h"

#include <typeinfo>

#include <any>
#include <optional>



#include <typeinfo>
#include <string>
#include <memory>

#if defined(__GNUC__) || defined(__clang__)
#include <cxxabi.h>
#include <cstdlib>

std::string demangle(const char* name) {
    int status = -1;
    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, nullptr, nullptr, &status),
        std::free
    };
    return (status == 0) ? res.get() : name;
}

#elif defined(_MSC_VER)
#include <windows.h>
#include <dbghelp.h>

std::string demangle(const char* name) {
    char undecoratedName[4096];
    DWORD undecNameLength = UnDecorateSymbolName(
        name,
        undecoratedName,
        sizeof(undecoratedName),
        UNDNAME_COMPLETE
    );
    return (undecNameLength == 0) ? name : std::string(undecoratedName, undecNameLength);
}

#else
std::string demangle(const char* name) {
    // Fallback for other compilers: just return the name
    return name;
}
#endif

std::string getDemangledTypeName(const std::type_info& ti) {
    return demangle(ti.name());
}


class IBaseFieldValidator {
public:
    virtual ~IBaseFieldValidator() = default;
    virtual std::optional<BaseValidationError> validate(const std::any& data) const = 0;
};


template<typename T>
class BaseFieldValidator : public IBaseFieldValidator {
public:
    // Assuming we have an attribute name to include in the error message
    std::string attribName;

    BaseFieldValidator(const std::string& attrName) : attribName(attrName) {}

    virtual std::optional<BaseValidationError> validateTyped(const T& data) const = 0;

    std::optional<BaseValidationError> validate(const std::any& data) const override {
        try {
            const T& castedData = std::any_cast<const T&>(data);
            return validateTyped(castedData);
        } catch (const std::bad_any_cast& e) {
            std::string expectedTypeName = getDemangledTypeName(typeid(T));
            std::string actualTypeName = getDemangledTypeName(data.type());
            std::string errorMessage = "Invalid type for validation of '" + attribName +
                                       "'. Expected: " + expectedTypeName +
                                       ", but got: " + actualTypeName;

            // If you know the type can be converted to string, you can attempt to add the value
            // This part is highly type-specific and might not be applicable in a general context
            // errorMessage += ". Value: " + std::to_string(data);

            return std::optional<BaseValidationError>(BaseValidationError(errorMessage));
        }
    }

    virtual ~BaseFieldValidator() {}
};

class LTIntValidator : public BaseFieldValidator<int> {
public:
    LTIntValidator(const int& validationParam, const std::string& attribName): BaseFieldValidator<int>(attribName), validationParam_(validationParam) {};

    std::optional<BaseValidationError> validateTyped(const int& data) const override {
        if (!(data < validationParam_)) {
            std::string errorMessage = "Validation for arg: `" + attribName + "` failed. " + std::to_string(data) + " is not less than " + std::to_string(validationParam_) + ".\n";
            return IntValidationError(errorMessage);
        }
        return std::nullopt;
    }

    nlohmann::json getValidationSchema() const {
        return {
            {"type", "int"},
            {"validationParam", validationParam_},
            {"attribName", attribName}
        };
    }
private:
    int validationParam_;
};

class GTIntValidator : public BaseFieldValidator<int> {
public:
    GTIntValidator(const int& validationParam, const std::string& attribName): BaseFieldValidator<int>(attribName), validationParam_(validationParam) {};

    std::optional<BaseValidationError> validateTyped(const int& data) const override {
        if (!(data > validationParam_)) {
            std::string errorMessage = "Validation for arg: `" + attribName + "` failed. " + std::to_string(data) + " is not greater than " + std::to_string(validationParam_) + ".\n";
            return IntValidationError(errorMessage);
        }
        return std::nullopt;
    }

    nlohmann::json getValidationSchema() const {
        return {
            {"type", "GTIntValidator"},
            {"validationParam", validationParam_},
            {"attribName", attribName}
        };
    }
private:
    int validationParam_;
};



class LTFloatValidator : public BaseFieldValidator<float> {
public:
    LTFloatValidator(const float& validationParam, const std::string& attribName): BaseFieldValidator<float>(attribName), validationParam_(validationParam) {};

    std::optional<BaseValidationError> validateTyped(const float& data) const override {
        if (!(data < validationParam_)) {
            std::string errorMessage = "Validation for arg: `" + attribName + "` failed. " + std::to_string(data) + " is not less than " + std::to_string(validationParam_) + ".\n";
            return FloatValidationError(errorMessage);
        }
        return std::nullopt;
    }

    nlohmann::json getValidationSchema() const {
        return {
            {"type", "LTFloatValidator"},
            {"validationParam", validationParam_},
            {"attribName", attribName}
        };
    }
private:
    float validationParam_;
};


class GTFloatValidator : public BaseFieldValidator<float> {
public:
    GTFloatValidator(const float& validationParam, const std::string& attribName): BaseFieldValidator<float>(attribName), validationParam_(validationParam) {};

    std::optional<BaseValidationError> validateTyped(const float& data) const override {
        if (!(data > validationParam_)) {
            std::string errorMessage = "Validation for arg: `" + attribName + "` failed. " + std::to_string(data) + " is not greater than " + std::to_string(validationParam_) + ".\n";
            return FloatValidationError(errorMessage);
        }
        return std::nullopt;
    }

    nlohmann::json getValidationSchema() const {
        return {
            {"type", "GTFloatValidator"},
            {"validationParam", validationParam_},
            {"attribName", attribName}
        };
    }
private:
    float validationParam_;
};



// DEBUG:instructor:Instructor Request: mode.value='tool_call', response_model=<class '__main__.UserDetail'>, new_kwargs={'model': 'gpt-3.5-turbo', 'messages': [{'role': 'user', 'content': 'Extract Jason is 25 years old'}], 'tools': [{'type': 'function', 'function': {'name': 'UserDetail', 'description': 'Correctly extracted `UserDetail` with all the required parameters with correct types', 'parameters': {'properties': {'name': {'title': 'Name', 'type': 'string'}, 'age': {'title': 'Age', 'type': 'integer'}}, 'required': ['age', 'name'], 'type': 'object'}}}], 'tool_choice': {'type': 'function', 'function': {'name': 'UserDetail'}}}




// /// @brief Base Validator Class
// template<typename T>
// class BaseFieldValidator {
//     public:
//         virtual std::optional<BaseValidationError> validate(const T& data) const = 0;
//         virtual nlohmann::json getValidationSchema() const = 0;
//         virtual std::string getName() const = 0;
//         virtual ~BaseFieldValidator() {}
// };

// class LTValidator : public BaseFieldValidator<int> {
//     public:
//         LTValidator(const int& validationParam, const std::string& attribName): validationParam_(validationParam), attribName(attribName) {};
//         std::optional<BaseValidationError> validate(const int& data) const override {
//             if (!(data < validationParam_)) {
//                 std::string errorMessage = "Validation for arg: `" + attribName + "` failed. " + std::to_string(data) + " is not less than " + std::to_string(validationParam_) + ".\n";
//                 return IntValidationError(errorMessage);
//             }
//             return std::nullopt;
//         }

//         nlohmann::json getValidationSchema() const override {
//             return {
//                 {"type", "LTValidator"},
//                 {"validationParam", validationParam_},
//                 {"attribName", attribName}
//             };
//         }

//     private:
//         int validationParam_;
//         std::string attribName;
// };


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
// class RegexValidator : public BaseFieldValidator {
//     std::regex pattern;
//     public:
//         RegexValidator(const std::string& pattern_str) : pattern(pattern_str) {}
//         bool validate(const nlohmann::json& j) const override;
// };

// class LengthValidator : public BaseFieldValidator {
//     size_t min_len, max_len;
//     public:
//         LengthValidator(size_t min_len, size_t max_len) : min_len(min_len), max_len(max_len) {}
//         bool validate(const nlohmann::json& j) const override;
// };

// class JsonSchemaValidator {
//     protected:
//         nlohmann::json object;
//         std::map<std::string, std::shared_ptr<BaseValidator>> validators;
//         bool validate();

//     public:
//         JsonSchemaValidator(const nlohmann::json& j) : object(j) {}

//         virtual void setupValidators() = 0;
//         virtual bool isValid() {
//             setupValidators();
//             return validate();
//         }
// };
