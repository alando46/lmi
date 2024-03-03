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





struct Person : public Reflective {
    std::string name;
    int age;
    double height;

    Person(const std::string& inputName, int inputAge, double inputHeight) {
        auto validateName = [](const std::string& val) -> std::string {
            if (val.empty())
                throw std::invalid_argument("Name cannot be empty");
            return val;
        };

        auto validateAge = [](int val) -> int {
            if (val < 0 || val > 150)
                throw std::invalid_argument("Age must be between 0 and 150");
            return val;
        };

        auto validateHeight = [](double val) -> double {
            if (val < 0.0 || val > 3.0)
                throw std::invalid_argument("Height must be between 0.0 and 3.0 meters");
            return val;
        };

        // Validate and assign
        this->name = validateName(inputName);
        this->age = validateAge(inputAge);
        this->height = validateHeight(inputHeight);
    }
};


BOOST_HANA_ADAPT_STRUCT(Person, name, age, height);
