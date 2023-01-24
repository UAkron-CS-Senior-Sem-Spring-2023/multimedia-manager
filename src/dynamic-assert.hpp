#pragma once

#ifdef NDEBUG
#define dynamic_assert()
#else

#include <stdexcept>

#define dynamic_assert(expression, message) {\
    if (!(expression)) {\
        dynamic_assert_fail("Condition: (" #expression ") failed, " #message, __FILE__, __LINE__);\
    }\
}

void dynamic_assert_fail(std::string message, std::string file, int line);
#endif