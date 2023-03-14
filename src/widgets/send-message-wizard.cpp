#include "send-message-wizard.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

SendMessageWizard::SendMessageWizard(QWidget* parent) {
    QGridLayout* sendMessageWizardPageLayout = new QGridLayout;
    int workingRow = 0;

    sendMessageWizardPageLayout->addWidget(
        new QLabel(tr("Enter the subject for your message below, in non-email accounts this will be included as 'Subject: '.")),
        workingRow++, 0
    );
    QLineEdit* subjectLine = new QLineEdit;
    sendMessageWizardPageLayout->addWidget(subjectLine, workingRow++, 0);

    sendMessageWizardPageLayout->addWidget(
        new QLabel(tr("Enter the body for your message below.")),
        workingRow++, 0
    );
    QTextEdit* bodyTextArea = new QTextEdit;
    sendMessageWizardPageLayout->addWidget(bodyTextArea, workingRow++, 0);

    QWizardPage* sendMessageWizardPage = new QWizardPage;
    sendMessageWizardPage->setLayout(sendMessageWizardPageLayout);

    addPage(sendMessageWizardPage);

    QList<QWizard::WizardButton> layout;
    layout << QWizard::Stretch << QWizard::CancelButton << QWizard::FinishButton;
    setButtonLayout(layout);
    setButtonText(QWizard::FinishButton, "Send message");

    connect(this, &SendMessageWizard::finished, [this, subjectLine, bodyTextArea]() {
        auto mimeData = Request::MimeData();
        mimeData.addText(bodyTextArea->toPlainText().toStdString());
        emit gotMessage(subjectLine->text().toStdString(), std::move(mimeData));

        subjectLine->clear();
        bodyTextArea->clear();
    });
}