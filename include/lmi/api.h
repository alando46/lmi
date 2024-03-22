#ifndef LMI_API_H
#define LMI_API_H

#include <iostream>
#include <string>
#include <jsoncons/json.hpp>
#include <vector>

namespace lmi {

    namespace openAI
    {
        class Function {
        public:
            std::string name;
            std::string arguments;
        };

        class ToolCall {
        public:
            std::string id;
            std::string type;
            Function function;
        };

        class Message {
        public:
            std::string role;
            json content;
            std::vector<ToolCall> tool_calls;
        };

        class Choice {
        public:
            int index;
            Message message;
            json logprobs;
            std::string finish_reason;
        };

        class Usage {
        public:
            int prompt_tokens;
            int completion_tokens;
            int total_tokens;
        };

        class ChatCompletion {
        public:
            std::string id;
            std::string object;
            int64_t created;
            std::string model;
            std::vector<Choice> choices;
            Usage usage;
        };
    } // namespace openAI


} // namespace lmi

JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::ChatCompletion, id, object, created, model, choices, usage)
JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::Choice, index, message, logprobs, finish_reason)
JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::Message, role, content, tool_calls)
JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::ToolCall, id, type, function)
JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::Function, name, arguments)
JSONCONS_ALL_MEMBER_TRAITS(lmi::openAI::Usage, prompt_tokens, completion_tokens, total_tokens)

#endif
