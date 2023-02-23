#include <QMainWindow>

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>

#include <memory>
#include <vector>
#include <unordered_map>

class MainWindow : public QMainWindow {
    public:
        MainWindow(QWidget* parent = nullptr);
    private:
        enum ManagingType {
            ACCOUNTS,
            ACCOUNT_GROUPS
        };
        ManagingType managingType = ACCOUNTS;

        std::unique_ptr<QWidget> windowWidget;
        std::unique_ptr<QGridLayout> windowGrid;

        // lefthand side of application
        // contains list of groups of accounts/accounts and buttons to switch management style
        void constructManageableWidget();
        std::unique_ptr<QWidget> manageableWidget;
        std::unique_ptr<QGridLayout> manageableGrid;

        std::unique_ptr<QStackedLayout> managingTypeStackedLayout;
        std::unique_ptr<QLabel> managingAccountsLabel;
        std::unique_ptr<QLabel> managingAccountGroupsLabel;

        std::unique_ptr<QWidget> manageableNamesWidget;
        std::unique_ptr<QVBoxLayout> manageableNamesLayout;
        std::vector<std::unique_ptr<QLabel>> manageableNames;

        std::unique_ptr<QStackedLayout> modifyManageablesButtonsStackedLayout;

        std::unique_ptr<QWidget> modifyManageableAccountWidget;
        std::unique_ptr<QHBoxLayout> modifyManageableAccountLayout;
        std::unique_ptr<QPushButton> addManageableAccountButton;
        std::unique_ptr<QPushButton> removeManageableAccountButton;

        std::unique_ptr<QWidget> modifyManageableAccountGroupWidget;
        std::unique_ptr<QHBoxLayout> modifyManageableAccountGroupLayout;
        std::unique_ptr<QPushButton> addManageableAccountGroupButton;
        std::unique_ptr<QPushButton> removeManageableAccountGroupButton;

        std::unique_ptr<QGroupBox> manageableTypeGroupBox;
        std::unique_ptr<QHBoxLayout> manageableTypeLayout;
        void manageableAccountsButtonClicked();
        std::unique_ptr<QPushButton> manageableAccountsButton;
        void manageableAccountGroupsButtonClicked();
        std::unique_ptr<QPushButton> manageableAccountGroupsButton;

        // righthand side of application
        // contains the selected group of accounts/account and buttons to perform actions on that group/account
        void constructManagingWidget();
        std::unique_ptr<QWidget> managingWidget;
        std::unique_ptr<QGridLayout> managingGrid;

        std::unique_ptr<QStackedLayout> managingTypeInfoStackedLayout;
        std::unique_ptr<QLabel> managingAccountsInfoLabel;
        std::unique_ptr<QLabel> managingAccountGroupsInfoLabel;

        std::unique_ptr<QWidget> managingInfoWidget;
        std::unique_ptr<QVBoxLayout> managingInfoLayout;
        std::vector<std::unique_ptr<QLabel>> managingInfoLabels;

        std::unique_ptr<QHBoxLayout> managingActionButtonsLayout;
        std::unique_ptr<QPushButton> managingSendMessageButton;
        std::unique_ptr<QPushButton> managingViewInboxButton;

        void updateManagingType(ManagingType type);
};