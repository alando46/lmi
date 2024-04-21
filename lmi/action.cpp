#include "action.h"


// Note: title must be the name of the class otherwise model will
// not know what schema is validating
    jsoncons::json Action::jsonSchema_ = json::parse(R"(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "Action",
    "description": "A function for taking an action",
    "type": "object",
    "properties": {
        "whatToSay": {
            "description": "The dialog that should be said",
            "type": "string",
            "maxLength": 20
        },
        "facialExpression": {
            "description": "The facial expression the character should have",
            "enum": [ "smiling", "thinking", "serious" ]
        }
    },
    "required": [ "whatToSay", "facialExpression" ]
}
    )");
