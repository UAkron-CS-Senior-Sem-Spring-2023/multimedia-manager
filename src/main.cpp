#include "window-manager.hpp"

#include <memory>
#include <array>

int main() {
    std::array<std::unique_ptr<Window>, 1> windows;

    auto windowManager = WindowManager(
        std::move(windows)
    );

    windowManager.display();
    // Unimplemented:
    // Gather user settings
    // Display their messaging sources

    return 0;
}