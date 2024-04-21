#ifndef LMI_CLIENT_H
#define LMI_CLIENT_H

#include "lmi/api.h"
#include <cpr/cpr.h>
#include <string>

#include <jsoncons/json.hpp>
#include <iostream>

namespace lmi {

    inline jsoncons::json openAISchemaTransform(const jsoncons::json& validationSchema) {
        // Create new JSON object for transformed schema
        jsoncons::json transformedSchema = jsoncons::json::object();

        // set the basic properties of the function object
        transformedSchema["type"] = "function";
        transformedSchema["function"] = jsoncons::json::object();
        transformedSchema["function"]["name"] = validationSchema["title"].as<std::string>();
        transformedSchema["function"]["description"] = validationSchema["description"].as<std::string>();

        // set parameters for the function
        transformedSchema["function"]["parameters"] = jsoncons::json::object({
            {"type", "object"},
            {"properties", validationSchema["properties"]},
            {"required", validationSchema["required"]}
        });

        return transformedSchema;
    }

}


#endif
