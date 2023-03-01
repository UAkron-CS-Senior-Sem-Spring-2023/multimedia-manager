#include <QApplication>

#include "main-window.hpp"
#include "request.hpp"

#include <memory>

#include <iostream>

int main(int argc, char** argv) {
    std::optional<std::string> result = Request::send("https://www.google.com");
    if (result) {
        std::cout << *result << std::endl;
    } else {
        std::cout << "IT FAILED!!!!" << std::endl;
    }
    auto app = QApplication(argc, argv);

    // Unimplemented:
    // Gather user settings
    // Display their messaging sources

    MainWindow mw;
    mw.show();

    return app.exec();
}