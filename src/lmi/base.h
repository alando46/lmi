#include <concepts>
#include "exceptions.h"
#include "validator.h"
#include <memory>
#include <boost/hana.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm> // For std::transform
#include <iostream>
#include <iterator> // For std::back_inserter

namespace hana = boost::hana;
using json = nlohmann::json;


template <typename Xs>
std::string join(Xs&& xs, std::string sep) {
  return hana::fold(hana::intersperse(std::forward<Xs>(xs), sep), "", hana::_ + hana::_);
}
std::string quote(std::string s) { return "\"" + s + "\""; }
template <typename T>
auto to_json(T const& x) -> decltype(std::to_string(x)) {
  return std::to_string(x);
}
std::string to_json(char c) { return quote({c}); }
std::string to_json(std::string s) { return quote(s); }

template <typename Xs>
  std::enable_if_t<hana::Sequence<Xs>::value,
std::string> to_json(Xs const& xs) {
  auto json = hana::transform(xs, [](auto const& x) {
    return to_json(x);
  });
  return "[" + join(std::move(json), ", ") + "]";
}

template <typename T>
  std::enable_if_t<hana::Struct<T>::value,
std::string> to_json(T const& x) {
  auto json = hana::transform(hana::keys(x), [&](auto name) {
    auto const& member = hana::at_key(x, name);
    return quote(hana::to<char const*>(name)) + " : " + to_json(member);
  });
  return "{" + join(std::move(json), ", ") + "}";
}


#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>

// TODO:
// ADD LT VALIDATOR TO VALIDATOR FUNCTION AND GET PERSON TO VALIDATE

class Person {
    public:
        BOOST_HANA_DEFINE_STRUCT(Person,
        (std::string, name),
            (int, age),
            (float, height)
        );
        // Constructor with direct values
        Person(const std::string& name, int age, float height) {
            this->name=name;
            this->age=age;
            this->height=height;

            validators_["age"].push_back(std::make_unique<LTIntValidator>(100, "age"));
            validators_["age"].push_back(std::make_unique<GTIntValidator>(1, "age"));
            validators_["height"].push_back(std::make_unique<LTFloatValidator>(3.0, "height"));
            validators_["height"].push_back(std::make_unique<GTFloatValidator>(1.0, "height"));
        }

        void validate() {
            auto keys = hana::keys(*this);
            hana::for_each(keys, [this](const auto& key) {
                auto& member = hana::at_key(*this, key);
                std::string attribName = hana::to<char const*>(key);
                validateField(attribName, member);
            });
        }

        // json getValidSchema() {
        //     auto keys = hana::keys(*this);

        //     hana::for_each(keys, [this](const auto& key) {
        //         auto& member = hana::at_key(*this, key);
        //         std::string attribName = hana::to<char const*>(key);
        //     }

        //     json validSchema;

        // }


        static Person create(const std::string& name, int age, double height) {
            Person obj(name, age, height); // Corrected object instantiation
            obj.validate(); // Uncomment after implementing validate correctly
            if (!obj.validationErrors_.empty()) {
                throw ModelValidatrionErrors(obj.validationErrors_);
            }
            return obj;
        }
    private:
        // Define a type for a generic validator function.
        // Note: This uses std::any for input, requiring runtime type checking.
        std::unordered_map<std::string, std::vector<std::unique_ptr<IBaseFieldValidator>>> validators_;
        std::vector<BaseValidationError> validationErrors_;

        template<typename T>
        void validateField(const std::string& attribName, const T& value) {
            auto it = validators_.find(attribName);
            if (it == validators_.end()) return;

            for (const auto& validator : it->second) {
                auto validationResult = validator->validate(value);
                if (validationResult) {
                    validationErrors_.push_back(*validationResult);
                }
            }
        }
};
