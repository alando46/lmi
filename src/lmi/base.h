#include <concepts>
#include "exceptions.h"
#include <string>
#include <memory>
#include <boost/hana.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <unordered_map>
#include <functional>

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

struct Reflective {
    // BOOST_HANA_DEFINE_STRUCT();
    // Map of validators: attribute name -> validation function that returns a modified json
    // std::unordered_map<std::string, std::function<std::any(const json&)>> validators;

    // Update from a JSON object
    // void update(const json& j) {
    //     auto members = hana::members(*this);
    //     hana::for_each(members, [&](auto& member) {
    //         auto name = hana::to<char const*>(hana::first(member));
    //         if (j.contains(name) && validators.find(name) != validators.end()) {
    //             json validatedVal = validators[name](j[name]); // Validate and possibly modify
    //             hana::second(member) = validatedVal.get<std::decay_t<decltype(hana::second(member))>>();
    //         }
    //     });
    // }

    // Serialize to a JSON object
    // json to_json() const {
    //     json j;
    //     auto members = hana::members(*this);
    //     std::cout << "to jsonss" << std::endl;
    //     // std::cout << members << std::endl;
    //     hana::for_each(members, [&](const auto& member) {
    //         auto name = hana::to<char const*>(hana::first(member));
    //         std::cout << name << std::endl;
    //         j[name] = hana::second(member);
    //     });
    //     std::cout << "byebye" << std::endl;
    //     return j;
    // }
};


#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

struct Person : public Reflective {
    std::string name;
    int age;
    double height;

    // Constructor with direct values
    Person(const std::string& inputName, int inputAge, double inputHeight) {
        name = validateName(inputName);
        age = validateAge(inputAge);
        height = validateHeight(inputHeight);
    }

    // Constructor from nlohmann::json object
    Person(const nlohmann::json& json) {
        name = validateName(json.at("name").get<std::string>());
        age = validateAge(json.at("age").get<int>());
        height = validateHeight(json.at("height").get<double>());
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


BOOST_HANA_ADAPT_STRUCT(Person, name, age, height);
