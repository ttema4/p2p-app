#include "mypage.h"
#include "ui_mypage.h"
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QPixmap>
#include <QFileDialog>


MyPage::MyPage(QWidget *parent) : QMainWindow(parent), ui(new Ui::MyPage) {
    ui->setupUi(this);
    menu = new HeaderMenu("Мой профиль", ui->widget_2->parentWidget());
    ui->widget_2->parentWidget()->layout()->replaceWidget(ui->widget_2, menu);

    connect(menu, &HeaderMenu::homePage, this, &MyPage::homePage);
    connect(menu, &HeaderMenu::loginPage, this, &MyPage::loginPage);

    connect(ui->pushButton, &QPushButton::clicked, this, &MyPage::exitAccount);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MyPage::loginPage);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MyPage::registerPage);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MyPage::openFileAndSetPixmap);
}

void MyPage::openFileAndSetPixmap() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выберите фотографию"), QDir::homePath(), tr("Изображения (*.png *.jpg *.jpeg)"));

    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) {
            CurUser::getInstance().setAvatar(pixmap);
            this->showEvent(new QShowEvent());
        } else {
            qDebug() << "Не удалось загрузить изображение" << fileName;
        }
    }
}


void MyPage::resizeEvent(QResizeEvent *e) {
    menu->resizeMenu();
    QMainWindow::resizeEvent(e);
}

void MyPage::showEvent(QShowEvent *event) {
    ui->label->setText(CurUser::getInstance().getName() + "\n" + CurUser::getInstance().getLogin() + "\n" + CurUser::getInstance().getPassword());
    ui->label_2->setPixmap(CurUser::getInstance().getAvatar2());
    menu->showEvent(event);
    QMainWindow::showEvent(event);
}

void MyPage::exitAccount() {
    CurUser::getInstance().unsetCurUser();
    menu->show();
    emit homePage();
}

MyPage::~MyPage()
{
    delete ui;
}
