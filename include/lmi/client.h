#ifndef LMI_CLIENT_H
#define LMI_CLIENT_H

#include "lmi/api.h"
#include <cpr/cpr.h>
#include <string>

namespace lmi {

    struct LMIMessage {
        std::string role = "user";
        std::string content = "";
    };

    class BackendBase {
        std::string& endPoint_;
        std::string& apiKey_;
        public:
            BackendBase(std::string& endPoint, std::string& apiKey) : endPoint_(endPoint), apiKey_(apiKey) {}

            std::unique_ptr<lmi::LMIFunction> create(std::string model, LMIMessage messages, const std::vector<std::unique_ptr<LMIFunction>>& responseModel, int retries=3);

    };

}


#endif
