#include "validatable-add-account-page.hpp"

ValidatableAddAccountPage::ValidatableAddAccountPage(QWidget* parent)
    : QWidget(parent)
{}

bool ValidatableAddAccountPage::isComplete() const {
    return true;
}