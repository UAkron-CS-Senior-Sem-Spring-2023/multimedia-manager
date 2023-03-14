#pragma once

#include <QWizard>

#include "request.hpp"

class SendMessageWizard : public QWizard {
    Q_OBJECT

    public:
        SendMessageWizard(QWidget* parent = nullptr);
    signals:
        void gotMessage(std::string subject, Request::MimeData message);
};