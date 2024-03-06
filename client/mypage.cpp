#include "mypage.h"
#include "ui_mypage.h"
#include <QLabel>
#include <QFrame>
#include <QPushButton>


MyPage::MyPage(QWidget *parent) : QMainWindow(parent), ui(new Ui::MyPage) {
    ui->setupUi(this);
    menu = new HeaderMenu("Мой профиль", ui->widget_2->parentWidget());
    ui->widget_2->parentWidget()->layout()->replaceWidget(ui->widget_2, menu);
    connect(menu, &HeaderMenu::homePage, this, &MyPage::homePage);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MyPage::loginPage);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MyPage::registerPage);
}


void MyPage::resizeEvent(QResizeEvent *e) {
    menu->resizeMenu();
    QMainWindow::resizeEvent(e);
}

MyPage::~MyPage()
{
    delete ui;
}
