#ifndef LMI_API_H
#define LMI_API_H

#include <memory>
#include <iostream>
#include <string>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonschema/jsonschema.hpp>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <type_traits>

using jsoncons::json;
namespace jsonschema = jsoncons::jsonschema;

namespace lmi {

    jsoncons::json loadSchema(std::string& schemaPath) {
        std::ifstream file(schemaPath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open schema file: " + schemaPath);
        }
        jsoncons::json rawData = jsoncons::json::parse(file);
        // what does make_schema do and is it needed?
        // return jsoncons::jsonschema::make_schema(rawData);
        return rawData;
    }

    class SchemaBaseInterface {
    public:
        virtual ~SchemaBaseInterface() = default;
    };

    template <typename T>
    class SchemaBase: public SchemaBaseInterface {};


    /**
     * @brief LMI Function is a parent class designed to:
     * - hold data in its model attribute
     * - validate that data
     * - provide the schema that will be used to validate the data
     * - (optionally) return a json string of the validated data
     */
    class LMIFunction {
    protected:
        // schema in json form
        jsoncons::json jsonSchema_;
        jsoncons::json rawJson_;
        virtual void setValues() = 0;

    public:
        LMIFunction(
            jsoncons::json rawJson) :
                rawJson_(rawJson) {
        }
        virtual ~LMIFunction() = default;

        void validateAndSet() {
            // need to load json schema into a json_schema object
            // Throws schema_error if JSON Schema loading fails
            auto sch = jsonschema::make_schema(jsonSchema_);
            // build error reporter lambda
            std::size_t error_count = 0;
            auto reporter = [&error_count](const jsonschema::validation_output& o)
            {
                ++error_count;
                std::cout << o.instance_location() << ": " << o.message() << "\n";
            };
            // initialize validator
            jsonschema::json_validator<json> validator(sch);
            // validate model for conformance with schema
            validator.validate(rawJson_, reporter);
            // TODO: add spdlog
            std::cout << "\nError count: " << error_count << "\n\n";
            // validation passed, set the values on this object
            setValues();
        }
    };

} // namespace lmi

#endif
