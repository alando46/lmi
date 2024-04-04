#ifndef LMI_DOSOMETHING_H
#define LMI_DOSOMETHING_H

#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonschema/jsonschema.hpp>
#include <string>
#include "lmi/lmi.h"

class DoSomething : public lmi::LMIFunction {
    private:
        // define data attribs
        std::string whatToSay_;
        std::string facialExpression_;
        // must define
        static jsoncons::json jsonSchema_;
    protected:
        // must implement
        void setValues() override {
            whatToSay_ = rawJson_["whatToSay"].as<std::string>();
            facialExpression_ = rawJson_["facialExpression"].as<std::string>();
        }
    public:
        DoSomething(jsoncons::json rawJson) : lmi::LMIFunction(rawJson) {
            // must call this here, because if we validate and set in the parent
            // constructor, jsonSchema_ will still be null because this object is not initialized yet
            // alternatively we could define the validation schema within this construtor block,
            // but that seems worse
            // validateAndSet();
            setValues();
        }

        static DoSomething create(jsoncons::json rawJson) {
            lmi::validate(jsonSchema_, rawJson);
            return DoSomething(rawJson);
        };

        std::string getWhatToSay() {
            return whatToSay_;
        }

        std::string getFacialExpression() {
            return facialExpression_;
        }
    };

#endif
