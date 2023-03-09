#include <QApplication>

#include "widgets/main-window.hpp"
#include "q-request.hpp"

int main(int argc, char** argv) {

    auto app = QApplication(argc, argv);

    // Unimplemented:
    // Gather user settings
    // Display their messaging sources

    MainWindow mw;
    mw.show();

    return app.exec();
}