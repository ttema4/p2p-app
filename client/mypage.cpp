#include "mypage.h"
#include "./ui_mypage.h"
#include <QLabel>
#include <QFrame>

void MyPage::logo_clicked() {
    this->close();        // Закрываем окно
    emit firstWindow(); // И вызываем сигнал на открытие главного окна
}

void MyPage::resizeMenu() {
    int button_pos_x = ui->pushButton->x() - 88;
    int button_pos_y = ui->pushButton->y() + ui->pushButton->height() + 17;
    overlay_frame->move(button_pos_x, button_pos_y);
    overlay_frame->resize(88 + ui->pushButton->width(), menuHeight);
}


void MyPage::hideMenu() {
    overlay_frame->hide();
}

void MyPage::showMenu() {
    anim = new QPropertyAnimation(overlay_frame, "size");
    anim->setDuration(300);
    if (!menuVisible) {
        anim->setStartValue(QSize(100 + ui->pushButton->width(), 0));
        anim->setEndValue(QSize(100 + ui->pushButton->width(), menuHeight));
        menuVisible = true;
        overlay_frame->show();
        resizeMenu();
    } else {
        anim->setStartValue(QSize(overlay_frame->width(), menuHeight));
        anim->setEndValue(QSize(overlay_frame->width(), 0));
        menuVisible = false;
        connect(anim, &QPropertyAnimation::finished, this, &MyPage::hideMenu);
    }
    anim->start();
}

MyPage::MyPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyPage)
{
    ui->setupUi(this);
    menuVisible = false;

    connect(ui->pushButton_2, &QPushButton::clicked, this, &MyPage::logo_clicked);

    connect(ui->pushButton, &QPushButton::clicked, this, &MyPage::showMenu);

    overlay_frame = new QFrame(this);
    overlay_frame->setObjectName("MenuQFrame");
    overlay_frame->setFrameShape(QFrame::NoFrame);
    overlay_frame->setLineWidth(1);
    overlay_frame->setMinimumWidth(0);

    QVBoxLayout *mvbox = new QVBoxLayout;
    QPushButton *button1 = new QPushButton("Мой профиль");
    QPushButton *button2 = new QPushButton("Уведомления");
    QPushButton *button3 = new QPushButton("Избранное");
    QPushButton *button4 = new QPushButton("Настройки");
    QPushButton *button5 = new QPushButton("Выйти");

    mvbox->addWidget(button1);
    mvbox->addWidget(button2);
    mvbox->addWidget(button3);
    mvbox->addWidget(button4);
    mvbox->addWidget(button5);
    mvbox->addStretch(1);
    overlay_frame->setLayout(mvbox);

    menuHeight = 170;
    overlay_frame->hide();
}


void MyPage::resizeEvent(QResizeEvent *e) {
    resizeMenu();

    QMainWindow::resizeEvent(e);
}

MyPage::~MyPage()
{
    delete ui;
}
