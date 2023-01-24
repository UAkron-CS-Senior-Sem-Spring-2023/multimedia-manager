#pragma once

#include "dynamic-assert.hpp"

#include <array>
#include <cassert>
#include <memory>
#include <type_traits>

class Window {
    public:
        virtual ~Window() = default;

        virtual void display();
};

template <class WindowContainer>
class WindowManager {
    public:
        WindowManager(WindowContainer windows)
            : windows(std::move(windows))
        {
            static_assert(std::is_same<typename WindowContainer::reference, std::unique_ptr<Window>&>::value,
                "WindowContainer does not contain unique ptr to windows"
            );
            static_assert(std::tuple_size<WindowContainer>::value >= 1,
                "WindowContainer does not contain at least one window"
                ", or does not implement std::tuple_size<WindowContainer>"
            );
            for (const auto& window : windows) {
                // There must exist a window pointed to
                dynamic_assert(window, "There must exist a window pointed to");
            }
        }

        void display() {
            windows.at(activeWindow)->display();
        }
    private:
        WindowContainer windows;
        std::size_t activeWindow = 0;
};
