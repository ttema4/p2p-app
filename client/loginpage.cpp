#include "loginpage.h"
#include "ui_loginpage.h"

LoginPage::LoginPage(QWidget *parent) : QMainWindow(parent), ui(new Ui::LoginPage) {
    ui->setupUi(this);
    menu = new HeaderMenu("Вход", ui->widget->parentWidget());
    ui->widget->parentWidget()->layout()->replaceWidget(ui->widget, menu);
    connect(menu, &HeaderMenu::homePage, this, &LoginPage::homePage);
    connect(menu, &HeaderMenu::myPage, this, &LoginPage::myPage);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &LoginPage::registerPage);
}

LoginPage::~LoginPage()
{
    delete ui;
}
