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

        std::unique_ptr<QVBoxLayout> managedLayout;
        std::vector<std::unique_ptr<QLabel>> managedNames;

        std::unique_ptr<QGroupBox> managedTypeGroupBox;
        std::unique_ptr<QHBoxLayout> managedTypeLayout;
        std::unique_ptr<QPushButton> managedAccountsButton;
        std::unique_ptr<QPushButton> managedAccountGroupsButton;
};