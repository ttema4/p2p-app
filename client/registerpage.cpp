#include "registerpage.h"
#include "ui_registerpage.h"

RegisterPage::RegisterPage(QWidget *parent) : QMainWindow(parent), ui(new Ui::RegisterPage) {
    ui->setupUi(this);
    menu = new HeaderMenu("Регистрация", ui->widget->parentWidget());
    ui->widget->parentWidget()->layout()->replaceWidget(ui->widget, menu);
    connect(menu, &HeaderMenu::homePage, this, &RegisterPage::homePage);
    connect(menu, &HeaderMenu::myPage, this, &RegisterPage::myPage);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &RegisterPage::loginPage);
}

RegisterPage::~RegisterPage()
{
    delete ui;
}
