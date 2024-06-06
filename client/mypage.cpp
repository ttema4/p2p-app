#include "mypage.h"
#include "ui_mypage.h"

MyPage::MyPage(QWidget *parent) : QMainWindow(parent), ui(new Ui::MyPage) {
    ui->setupUi(this);
    menu = new HeaderMenu("Мой профиль", ui->widget_2->parentWidget());
    ui->widget_2->parentWidget()->layout()->replaceWidget(ui->widget_2, menu);

    connect(menu, &HeaderMenu::homePage, this, &MyPage::homePage);
    connect(menu, &HeaderMenu::notifyPage, this, &MyPage::notifyPage);
    connect(menu, &HeaderMenu::favouritePage, this, &MyPage::favouritePage);
    connect(menu, &HeaderMenu::settingsPage, this, &MyPage::settingsPage);

    connect(ui->pushButton, &QPushButton::clicked, this, &MyPage::exitAccount);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MyPage::openFileAndSetPixmap);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MyPage::deleteAvatar);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &MyPage::deleteAccount);
}

void MyPage::openFileAndSetPixmap() {
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Выберите фотографию"), QDir::homePath(), tr("Изображения (*.png *.jpg *.jpeg)")
    );

    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) {
            CurUser::getInstance().tryEditAvatar(pixmap);
            this->showEvent(new QShowEvent());
        } else {
            qDebug() << "Не удалось загрузить изображение:" << fileName;
        }
    }
}

void MyPage::resizeEvent(QResizeEvent *e) {
    menu->resizeMenu();
    QMainWindow::resizeEvent(e);
}

void MyPage::showEvent(QShowEvent *event) {
    ui->label->setText(
        "Id: " + QString::number(CurUser::getInstance().getId()) + "\nName: " + CurUser::getInstance().getName() +
        "\nLogin: " + CurUser::getInstance().getLogin() +
        "\nIn Favourite: " + QString::number(CurUser::getInstance().getFavourites().size())
    );
    ui->label_2->setPixmap(CurUser::getInstance().getAvatar());
    menu->showEvent(event);
    QMainWindow::showEvent(event);
}

void MyPage::deleteAvatar() {
    CurUser::getInstance().tryDelAvatar();
    this->showEvent(new QShowEvent());
}

void MyPage::deleteAccount() {
    CurUser::getInstance().tryDeleteAccount();
    emit homePage();
}

void MyPage::exitAccount() {
    CurUser::getInstance().userExit();
    emit homePage();
}

MyPage::~MyPage() {
    delete ui;
}
