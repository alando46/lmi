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
#include <validationError.h>

using jsoncons::json;
namespace jsonschema = jsoncons::jsonschema;

namespace lmi {

    inline jsoncons::json loadSchema(std::string& schemaPath) {
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
    // protected:
        // schema in json form
        // static jsoncons::json jsonSchema_;
        // jsoncons::json rawJson_;
        // virtual void setValues() = 0;

    public:
        virtual ~LMIFunction() = default;

        virtual jsoncons::json getSchema() = 0;
    };

    inline void validate(jsoncons::json schema, jsoncons::json rawJson) {
        std::string output;
        jsoncons::encode_json(schema, output, jsoncons::indenting::indent);
        std::cout << output << std::endl;
        // need to load json schema into a json_schema object
        // Throws schema_error if JSON Schema loading fails
        auto sch = jsonschema::make_schema(schema);
        // build error reporter lambda
        std::size_t errorCount = 0;
        std::string errorMessage = "";
        auto reporter = [&errorCount, &errorMessage](const jsonschema::validation_output& o)
        {
            ++errorCount;
            errorMessage += "\n";
            errorMessage += o.instance_location();
            errorMessage += ": ";
            errorMessage += o.message();
        };
        // initialize validator
        jsonschema::json_validator<json> validator(sch);
        // validate model for conformance with schema
        validator.validate(rawJson, reporter);
        if (errorCount > 0) {
            throw ValidationException(errorMessage);
        }

        // TODO: add spdlog
        // std::cout << "\nError count: " << error_count << "\n\n";
    }

} // namespace lmi

#endif
