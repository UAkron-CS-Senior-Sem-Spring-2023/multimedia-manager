#include <QApplication>

#include "main-window.hpp"
#include "q-request.hpp"

int main(int argc, char** argv) {

    auto app = QApplication(argc, argv);

    // Unimplemented:
    // Gather user settings
    // Display their messaging sources
    
    QRequest& qr = QRequest::singleton();

    auto headers = Request::SMTPHeaders("robertson.gregory.a@gmail.com", { "robertson.gregory.a@gmail.com"}, {}, "testing smtp");
    Request::MimeData md;
    md.addText("a lot of text!!!");
    qr.gmailSMTP(1, headers, md);

    MainWindow mw;
    mw.show();

    return app.exec();
}