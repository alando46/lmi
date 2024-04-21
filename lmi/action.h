#ifndef LMI_ACTION_H
#define LMI_ACTION_H

#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonschema/jsonschema.hpp>
#include <string>
#include "lmi/lmi.h"

class Action : public lmi::LMIFunction {
    private:
        // define data attribs
        std::string whatToSay_;
        std::string facialExpression_;
        jsoncons::json rawJson_;
        // must define
        static jsoncons::json jsonSchema_;
    protected:
        // must implement
        void setValues() {
            whatToSay_ = rawJson_["whatToSay"].as<std::string>();
            facialExpression_ = rawJson_["facialExpression"].as<std::string>();
        }
    public:
        Action(jsoncons::json rawJson) : rawJson_(rawJson) {
            // must call this here, because if we validate and set in the parent
            // constructor, jsonSchema_ will still be null because this object is not initialized yet
            // alternatively we could define the validation schema within this construtor block,
            // but that seems worse
            // validateAndSet();
            setValues();
        }

        // static Action create(jsoncons::json rawJson) {
        //     lmi::validate(jsonSchema_, rawJson);
        //     return Action(rawJson);
        // };

        static std::unique_ptr<Action> create(jsoncons::json rawJson) {
            lmi::validate(jsonSchema_, rawJson);
            return std::make_unique<Action>(rawJson);
        };


        static jsoncons::json getSchema() {
            return jsonSchema_;
        }

        std::string getWhatToSay() {
            return whatToSay_;
        }

        std::string getFacialExpression() {
            return facialExpression_;
        }
    };

#endif
