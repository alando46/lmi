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

    // is there a way within the schema to search nested schema?
    // it seems a macro can automatically serialize nested things:
    //     1. how does this work? ie is there a hook or something that it uses to know it should serialize the (nested) next layer down?
    //     2. can i make a macro which calls the serialization macro to piggy-back on that functionality and also provides similar functionality for validation schemas?
    //     3. are there any built-in functionalities within the validation schema to allow for this nested-like functionality?

    //     the goal is to have each class define its own validation schema, and if a class is composed, it only has to define its own validation schema and not any nested
    //     schemas. the concern would be, if this is done in json directly, it could yield a poor developer experience because then a developer would need to have this schema
    //     linked in code and in the validation. that might be v0 though...

    // constraint: no nesting until we write a true reflection system.
    // WE validate the entire chat api json, and then we extract the tool calls out and make them c++ objects
    // those objects, which are not nested, can validate themselves.

    // new mvp -> write validation schema for chat completion object, extract just the tool calls.
    // tool call objects are like what is defined beflow, they hold schema and can validate themselves

    class LMIFunction {
    protected:
        jsoncons::json schema_;
        std::string& schemaPath_;
        std::unique_ptr<lmi::SchemaBase> model_;

    public:

        std::string& getSchemaPath() const {
            return schemaPath_;
        }

        LMIFunction(std::string& schemaPath, std::unique_ptr<lmi::SchemaBase> model) : schemaPath_(schemaPath), model_(std::move(model)) {
            validate();
        }
        virtual ~LMIFunction() = default;

        void validate() {
            // load schema
            auto modelSchema = loadSchema(schemaPath_);
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

    // namespace openAI
    // {
    //     class Function : public SchemaBase<Function> {
    //     public:
    //         std::string name;
    //         std::string arguments;
    //     };

    //     class ToolCall : public SchemaBase<ToolCall> {
    //     public:
    //         std::string id;
    //         std::string type;
    //         Function function;
    //     };

    //     class Message : public SchemaBase<Message> {
    //     public:
    //         std::string role;
    //         jsoncons::json content;
    //         std::vector<ToolCall> tool_calls;
    //     };

    //     class Choice : public SchemaBase<Choice> {
    //     public:
    //         int index;
    //         Message message;
    //         jsoncons::json logprobs;
    //         std::string finish_reason;
    //     };

    //     class Usage : public SchemaBase<Usage> {
    //     public:
    //         int prompt_tokens;
    //         int completion_tokens;
    //         int total_tokens;
    //     };

    //     class ChatCompletionModel : public SchemaBase<ChatCompletionModel> {
    //     public:
    //         std::string id;
    //         std::string object;
    //         int64_t created;
    //         std::string model;
    //         std::string system_fingerprint;
    //         std::vector<Choice> choices;
    //         Usage usage;
    //     };
    // } // namespace openAI


} // namespace lmi

// JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::ChatCompletionModel, id, object, created, model, choices, usage, system_fingerprint)
// JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::Choice, index, message, logprobs, finish_reason)
// JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::Message, role, content, tool_calls)
// JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::ToolCall, id, type, function)
// JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::Function, name, arguments)
// JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::Usage, prompt_tokens, completion_tokens, total_tokens)

#endif
