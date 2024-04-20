#include "action.h"

jsoncons::json Action::jsonSchema_ = json::parse(R"(
{
    "type": "function",
    "function": {
        "name": "TestWhatToSaySchema",
        "description": "A basic schema for testing",
        "parameters" : {
            "type": "object",
            "properties": {
                "whatToSay": {
                    "description": "The dialog that should be said",
                    "type": "string",
                    "maxLength": 40
                },
                "facialExpression": {
                    "description": "The facial expression the character should have",
                    "enum": [ "smiling", "thinking", "serious" ]
                }
            },
            "required": [ "whatToSay", "facialExpression" ]
        }
    }
}
)");


