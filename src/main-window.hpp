#include <QMainWindow>

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>

#include <memory>
#include <vector>

class MainWindow : public QMainWindow {
    public:
        MainWindow(QWidget* parent = nullptr);
    private:
        std::unique_ptr<QWidget> windowWidget;
        std::unique_ptr<QGridLayout> windowGrid;

        // lefthand side of application
        // contains list of groups of accounts/accounts and buttons to switch management style
        void constructManageableWidget();
        std::unique_ptr<QWidget> manageableWidget;
        std::unique_ptr<QGridLayout> manageableGrid;

        std::unique_ptr<QVBoxLayout> manageableNamesLayout;
        std::vector<std::unique_ptr<QLabel>> manageableNames;

        std::unique_ptr<QGroupBox> manageableTypeGroupBox;
        std::unique_ptr<QHBoxLayout> manageableTypeLayout;
        std::unique_ptr<QPushButton> manageableAccountsButton;
        std::unique_ptr<QPushButton> manageableAccountGroupsButton;

        // righthand side of application
        // contains the selected group of accounts/account and buttons to perform actions on that group/account
        std::unique_ptr<QWidget> activeManageableWidget;
};