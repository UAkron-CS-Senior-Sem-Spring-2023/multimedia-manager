#include "dynamic-assert.hpp"

#ifndef NDEBUG

#include <stdexcept>

void dynamic_assert_fail(std::string_view message, std::string_view file, int line) {
    std::string fullError = std::string(message);
    fullError += ".\n(\"";
    fullError.append(file);
    fullError += "\" L";
    fullError += std::to_string(line);
    fullError += ')';
    throw std::runtime_error(fullError);
}

#endif