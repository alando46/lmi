#ifndef VALIDATION_EXCEPTION_H
#define VALIDATION_EXCEPTION_H

#include <stdexcept>
#include <string>
using namespace std;

class ValidationException : public runtime_error {
public:
	ValidationException() : runtime_error("Validation error(s)") {}
	ValidationException(const string& message) : runtime_error(message) {}
};

#endif
