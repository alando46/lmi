#include "validator.h"

bool LengthValidator::validate(const nlohmann::json& j) const {
    if (!j.is_string()) return false;
    size_t len = j.get<std::string>().length();
    return len >= min_len && len <= max_len;
};


bool RegexValidator::validate(const nlohmann::json& j) const {
    if (!j.is_string()) return false;
    return std::regex_match(j.get<std::string>(), pattern);
}

bool JsonSchemaValidator::validate() {
    for (const auto& [key, validator] : validators) {
        if (!object.contains(key) || !validator->validate(object[key])) {
            std::cout << "Validation failed for key: " << key << std::endl;
            return false;
        }
    }
    return true;
}

void User::setupValidators() {
            validators["name"] = std::make_shared<LengthValidator>(3, 15);
            validators["email"] = std::make_shared<RegexValidator>("^(\\w+)(\\.\\w+)*@(\\w+)(\\.(\\w+))+");
}
