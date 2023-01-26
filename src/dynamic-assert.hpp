#pragma once

#ifdef NDEBUG
#define dynamic_assert(expression, message) (void(0))
#else

#include <string_view>

#define dynamic_assert(expression, message) {\
    if (expression) {}\
    else {\
        dynamic_assert_fail("Condition: (" #expression ") failed; Error: " #message, __FILE__, __LINE__);\
    }\
}\
(void(0))

void dynamic_assert_fail(std::string_view message, std::string_view file, int line);

#endif