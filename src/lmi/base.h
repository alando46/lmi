#include <concepts>
#include "exceptions.h"
#include <memory>
#include <boost/hana.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <string>


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

struct BaseFunction {
    public:
        virtual ~BaseFunction();
        virtual validate() const = 0;
    protected:
        std::vector<BaseValidationError> errors;
        void validate_() {
            json raw_input = to_json(this);
        }
};



// int main() {
//     MyClass obj = MyClass::create(); // Factory method handles everything.
// }


hana::for_each(john, [](auto pair) {
  std::cout << hana::to<char const*>(hana::first(pair)) << ": "
            << hana::second(pair) << std::endl;
});


class Person {
    public:
    BOOST_HANA_DEFINE_STRUCT(Persion,
       (std::string, name),
        (int, age),
        (double, height)
    );
    private:
        std::vector<BaseValidationError> validationErrors_;
        template<typename T>
        void validateField(std::string attribName, T value, callable validators) {
            // TODO: DEFINE SOME MAP IMPLEMENTATINO WHERE WE CHECK EACH VALIDATOR IN THE LIST
        }
        void validate() {
            hana::for_each(this, [](auto pair)){
                std::string attribName = hana::to<char const*>(hana::first(pair));
                //TODO ADD TYPE FOR FIELDVALIDATORS
                fieldValidators = validators_[attribName];
                validateField(attribName, hana::second(pair), fieldValidators);
            }
        }
        // Constructor with direct values
        Person(const std::string& name, int age, double height) {
            this->name=name;
            this->age=age;
            this->height=height;

            // name = validateName(inputName);
            // age = validateAge(inputAge);
            // height = validateHeight(inputHeight);

            // if we validate post init we can store validation functions in map
            // and use string from names to pass to validator for arg.
            //
            // alternatively if we validate on init then we will need to manually call these functions
            // and pass in a string for the argument name, as well as its value
        }


    // Constructor from nlohmann::json object
    // Person(const nlohmann::json& json) {
    //     name = validateName(json.at("name").get<std::string>());
    //     age = validateAge(json.at("age").get<int>());
    //     height = validateHeight(json.at("height").get<double>());
    // }

    static Person create(const std::string& name, int age, double height) {
        Person obj(const std::string& name, int age, double height);
        obj.validate();
        if (!obj.errors.empty()) {
            throw ModelValidatrionErrors(obj.errors);
        }
    }
private:
    // Validation functions
    static std::string validateName(const std::string& val) {
        if (val.empty())
            throw std::invalid_argument("Name cannot be empty");
        return val;
    }

    static int validateAge(int val) {
        if (val < 0 || val > 150)
            throw std::invalid_argument("Age must be between 0 and 150");
        return val;
    }

    static double validateHeight(double val) {
        if (val < 0.0 || val > 3.0)
            throw std::invalid_argument("Height must be between 0.0 and 3.0 meters");
        return val;
    }
};


// BOOST_HANA_ADAPT_STRUCT(Person, name, age, height);
