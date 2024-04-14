#include "action.h"

jsoncons::json Action::jsonSchema_ = json::parse(R"(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "TestWhatToSaySchema",
    "description": "A basic schema for testing",
    "type": "object",
    "properties": {
        "whatToSay": {
            "description": "The dialog that should be said",
            "type": "string",
            "maxLength": 5
        },
        "facialExpression": {
            "description": "The facial expression the character should have",
            "enum": [ "smiling", "thinking", "serious" ]
        }
    },
    "required": [ "whatToSay", "facialExpression" ]
}
)");
