#include "mainwindow.h"
#include "mypage.h"
#include "ui_mainwindow.h"
#include "data_reciever.cpp"
#include "mainwindow_fwd.hpp"
#include <QResizeEvent>
#include <QDebug>
#include <QPixmap>
#include <QIcon>
#include <string>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QPropertyAnimation>

void MainWindow::open_mypage() {
    mypage->show();
    mypage->resize(this->size());
    mypage->move(this->pos());
    mypage->resizeMenu();
    this->close();
}

void MainWindow::open_homepage() {
    this->resize(mypage->size());
    this->move(mypage->pos());
    this->show();
    resizeMenu();
    resizeTable();
}

void MainWindow::resizeTable() {
    int width = ui->tableWidget->width();
    if (width == 100) width = 600;
    ui->tableWidget->setColumnWidth(0, std::round(width * 0.17));
    ui->tableWidget->setColumnWidth(1, std::round(width * 0.04));
    ui->tableWidget->setColumnWidth(2, std::round(width * 0.15));
    ui->tableWidget->setColumnWidth(3, std::round(width * 0.19));
    ui->tableWidget->setColumnWidth(4, std::round(width * 0.17));
    ui->tableWidget->setColumnWidth(5, std::round(width * 0.04));
    ui->tableWidget->setColumnWidth(6, std::round(width * 0.15));
    ui->tableWidget->setColumnWidth(7, std::round(width * 0.09));
}

void MainWindow::resizeMenu() {
    int button_pos_x = ui->pushButton_2->x() - 100;
    int button_pos_y = ui->pushButton_2->y() + ui->pushButton_2->height() + 5;
    overlay_frame->move(button_pos_x, button_pos_y);
    overlay_frame->resize(100 + ui->pushButton_2->width(), menuHeight);
}

void MainWindow::hideMenu() {
    overlay_frame->hide();
}

void MainWindow::showMenu() {
    anim = new QPropertyAnimation(overlay_frame, "size");
    anim->setDuration(300);
    if (!menuVisible) {
        anim->setStartValue(QSize(100 + ui->pushButton_2->width(), 0));
        anim->setEndValue(QSize(100 + ui->pushButton_2->width(), menuHeight));
        menuVisible = true;
        overlay_frame->show();
        resizeMenu();
    } else {
        anim->setStartValue(QSize(overlay_frame->width(), menuHeight));
        anim->setEndValue(QSize(overlay_frame->width(), 0));
        menuVisible = false;
        connect(anim, &QPropertyAnimation::finished, this, &MainWindow::hideMenu);
    }
    anim->start();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    int n = 30;
    menuVisible = false;

    ui->setupUi(this);

    mypage = new MyPage();
    // подключаем к слоту запуска главного окна по кнопке во втором окне
    connect(mypage, &MyPage::firstWindow, this, &MainWindow::open_homepage);

    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->setRowCount(n + 1);
    ui->tableWidget->setColumnCount(8);

    QTableWidgetItem* header0 = new QTableWidgetItem(QString("Покупка на P2P"));
    header0->setTextAlignment(Qt::AlignCenter) ;
    ui->tableWidget->setItem(0, 0, header0);

    ui->tableWidget->setSpan(0, 0, 1, 3);

    QTableWidgetItem* header3 = new QTableWidgetItem(QString("Обмен на споте"));
    header3->setTextAlignment(Qt::AlignCenter) ;
    ui->tableWidget->setItem(0, 3, header3);

    QTableWidgetItem* header4 = new QTableWidgetItem(QString("Продажа на P2P"));
    header4->setTextAlignment(Qt::AlignCenter) ;
    ui->tableWidget->setItem(0, 4, header4);

    ui->tableWidget->setSpan(0, 4, 1, 3);

    QTableWidgetItem* header7 = new QTableWidgetItem(QString("Спред"));
    header7->setTextAlignment(Qt::AlignCenter) ;
    ui->tableWidget->setItem(0, 7, header7);

    DataReciever dr;

    for (int i = 1; i <= n; i++) {
        Chain new_chain = dr.recieveNewChain();

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString((new_chain.buy.coin1 + "/" + new_chain.buy.coin2).c_str())));

        QTableWidgetItem* bank1 = new QTableWidgetItem;
        bank1->setIcon(QIcon(include_map[new_chain.buy.bank].c_str()));
        bank1->setToolTip(QString((new_chain.buy.bank).c_str()));;
        ui->tableWidget->setItem( i, 1, bank1);

        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString(tr("%1").arg((double)new_chain.buy.exchange_rate))));

        QTableWidgetItem* change = new QTableWidgetItem(QString((new_chain.change.first + " -> " + new_chain.change.second).c_str()));
        change->setTextAlignment(Qt::AlignCenter) ;
        ui->tableWidget->setItem( i, 3, change);

        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString((new_chain.sell.coin1 + "/" + new_chain.sell.coin2).c_str())));

        QTableWidgetItem* bank2 = new QTableWidgetItem;
        bank2->setIcon(QIcon(include_map[new_chain.sell.bank].c_str()));
        bank2->setToolTip(QString((new_chain.sell.bank).c_str()));
        ui->tableWidget->setItem( i, 5, bank2);

        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString(tr("%1").arg((double)new_chain.sell.exchange_rate))));

        QTableWidgetItem* spread = new QTableWidgetItem(QString(tr("%1%").arg((double)new_chain.spread)));
        spread->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem( i, 7, spread);
    }


    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::showMenu);

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

    connect(button1, &QPushButton::clicked, this, &MainWindow::open_mypage);

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

void MainWindow::resizeEvent(QResizeEvent *e) {
    resizeTable();
    resizeMenu();

    QMainWindow::resizeEvent(e);
}


MainWindow::~MainWindow()
{
    delete ui;
}
