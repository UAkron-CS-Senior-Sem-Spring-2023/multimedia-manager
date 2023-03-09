#include "account-info-widget.hpp"

AccountInfoWidget::AccountInfoWidget(QWidget* parent)
    : QWidget(parent),
    managingGrid(QGridLayout(this)),
    managingAccountsInfoLabel(QLabel(tr("Account Information"))),
    managingAccountGroupsInfoLabel(QLabel(tr("Account Group Information"))),
    managingInfoLayout(QVBoxLayout(&managingInfoWidget)),
    managingSendMessageButton(QPushButton(tr("Send message"))),
    managingViewInboxButton(QPushButton(tr("View inbox")))
{
    int workingRow = 0;

    // set up managed accounts info labels
    managingTypeInfoStackedLayout.insertWidget(AccountManager::ACCOUNTS, &managingAccountsInfoLabel);
    managingTypeInfoStackedLayout.insertWidget(AccountManager::ACCOUNT_GROUPS, &managingAccountGroupsInfoLabel);
    managingTypeInfoStackedLayout.setCurrentIndex(AccountManager::INITIAL_MANAGING_TYPE);
    managingGrid.addLayout(&managingTypeInfoStackedLayout, workingRow, 0);
    ++workingRow;

    // set up managed accounts info
    managingInfoWidget.setBackgroundRole(QPalette::Base);
    managingInfoWidget.setAutoFillBackground(true);
    managingInfoLayout.setContentsMargins(0, 0, 0, 16);

    managingGrid.setRowStretch(workingRow, 1);
    managingGrid.addWidget(&managingInfoWidget, workingRow, 0);
    ++workingRow;

    // set up actionable buttons for that account

    managingActionButtonsLayout.addWidget(&managingSendMessageButton);
    managingActionButtonsLayout.addStretch(1);
    managingActionButtonsLayout.addWidget(&managingViewInboxButton);

    managingGrid.addLayout(&managingActionButtonsLayout, workingRow, 0);
    ++workingRow;
}

void AccountInfoWidget::updateManagingType(AccountManager::ManagingType type) {
    managingTypeInfoStackedLayout.setCurrentIndex(type);
}