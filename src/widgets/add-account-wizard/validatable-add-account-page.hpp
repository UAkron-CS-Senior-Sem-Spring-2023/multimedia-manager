#pragma once

#include <QWidget>

class ValidatableAddAccountPage : public QWidget {
    Q_OBJECT

    public:
        ValidatableAddAccountPage(QWidget* parent = nullptr);

        virtual bool isComplete() const;
    signals:
        void completeChanged();
};