#include "notifypage.h"
#include "ui_notifypage.h"

NotifyPage::NotifyPage(QWidget *parent) : QMainWindow(parent), ui(new Ui::NotifyPage) {
    ui->setupUi(this);
    menu = new HeaderMenu("Уведомления", ui->widget->parentWidget());
    ui->widget->parentWidget()->layout()->replaceWidget(ui->widget, menu);

    connect(menu, &HeaderMenu::homePage, this, &NotifyPage::homePage);
    connect(menu, &HeaderMenu::loginPage, this, &NotifyPage::loginPage);
    connect(menu, &HeaderMenu::myPage, this, &NotifyPage::myPage);
    connect(menu, &HeaderMenu::favouritePage, this, &NotifyPage::favouritePage);
    connect(menu, &HeaderMenu::settingsPage, this, &NotifyPage::settingsPage);
}

NotifyPage::~NotifyPage() {
    delete ui;
}
