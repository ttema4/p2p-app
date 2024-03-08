#include "settingspage.h"
#include "ui_settingspage.h"

SettingsPage::SettingsPage(QWidget *parent) : QMainWindow(parent), ui(new Ui::SettingsPage) {
    ui->setupUi(this);
    menu = new HeaderMenu("Настройки", ui->widget->parentWidget());
    ui->widget->parentWidget()->layout()->replaceWidget(ui->widget, menu);

    connect(menu, &HeaderMenu::homePage, this, &SettingsPage::homePage);
    connect(menu, &HeaderMenu::loginPage, this, &SettingsPage::loginPage);
    connect(menu, &HeaderMenu::myPage, this, &SettingsPage::myPage);
    connect(menu, &HeaderMenu::notifyPage, this, &SettingsPage::notifyPage);
    connect(menu, &HeaderMenu::favouritePage, this, &SettingsPage::favouritePage);
}

SettingsPage::~SettingsPage()
{
    delete ui;
}
