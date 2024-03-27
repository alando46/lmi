#ifndef LMI_API_H
#define LMI_API_H

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
        return jsoncons::json::parse(file);
    }

    template <typename T>
    class SchemaBase {};


    // todo: write unit tests for this class
    // lookup to see if jsoncons::encode_json(model_, encodedJsonStr); is necessary (can we validate jsoncons::json directly?)
    // create workflow where we sublass this and add 1 new method (for calling something in unreal api)
    // 1. load json into model
    // 2. instantiate LMI base -> fails if model doesn't meet schema
    // question: are the schema classes even needed for anything? answer: probably helpful for accessing specific data attributes
    // todo: write getters for each of your schema classes
    // alternatively, create a factory pattern where we take a response and try to validate for each of the schemas we have
    // imagine a tools workflow where we define multiple schema objects when we make a request
    // when making request we ship the schema for each of those objects
    
    template <typename T>
    class LMIBase {
    protected:
        jsoncons::json schema_;
        std::string& schemaPath_;
        lmi::SchemaBase<T> model_;

    public:

        std::string& getSchemaPath() const {
            return schemaPath_;
        }

        LMIBase(const std::string& schemaPath, lmi::SchemaBase<T> model) : schemaPath_(schemaPath), model_(model) {
            validate();
        }

        void validate() {
            // load schema
            jsoncons::json modelSchema = loadSchema(schemaPath_);
            // create validator
            jsoncons::jsonschema::json_validator<json> validator(modelSchema);
            // make string buffer
            std::string encodedJsonStr;
            // encode model to json string
            jsoncons::encode_json(model_, encodedJsonStr);
            // validate model for conformance with schema
            validator.is_valid(encodedJsonStr);
        }
    };



    namespace openAI
    {
        class Function : public SchemaBase<Function> {
        public:
            std::string name;
            std::string arguments;
        };

        class ToolCall : public SchemaBase<ToolCall> {
        public:
            std::string id;
            std::string type;
            Function function;
        };

        class Message : public SchemaBase<Message> {
        public:
            std::string role;
            jsoncons::json content;
            std::vector<ToolCall> tool_calls;
        };

        class Choice : public SchemaBase<Choice> {
        public:
            int index;
            Message message;
            jsoncons::json logprobs;
            std::string finish_reason;
        };

        class Usage : public SchemaBase<Usage> {
        public:
            int prompt_tokens;
            int completion_tokens;
            int total_tokens;
        };

        class ChatCompletionModel : public SchemaBase<ChatCompletionModel> {
        public:
            std::string id;
            std::string object;
            int64_t created;
            std::string model;
            std::string system_fingerprint;
            std::vector<Choice> choices;
            Usage usage;
        };
    } // namespace openAI


} // namespace lmi

JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::ChatCompletionModel, id, object, created, model, choices, usage, system_fingerprint)
JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::Choice, index, message, logprobs, finish_reason)
JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::Message, role, content, tool_calls)
JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::ToolCall, id, type, function)
JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::Function, name, arguments)
JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::Usage, prompt_tokens, completion_tokens, total_tokens)

#endif
