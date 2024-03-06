#include "mainwindow.h"
#include "headermenu.h"
#include "mypage.h"
#include "ui_mainwindow.h"
#include "data_reciever.cpp"
#include "mainwindow_fwd.hpp"
#include <QWidget>
#include <QResizeEvent>
#include <QDebug>
#include <QPixmap>
#include <QIcon>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QPropertyAnimation>
#include <QPixmap>
#include <QMouseEvent>
#include <QVBoxLayout>

void MainWindow::windowChanger(QMainWindow *toOpen) {
    toOpen->show();
    toOpen->setGeometry(currentpage->geometry());
    currentpage->hide();
    if (toOpen == this) this->resizeTable();
    currentpage = toOpen;
}

void MainWindow::open_homepage() {
    windowChanger(this);
}

void MainWindow::open_mypage() {
    windowChanger(mypage);
}

void MainWindow::open_registerpage() {
    windowChanger(registerpage);
}

void MainWindow::open_loginpage() {
    windowChanger(loginpage);
}

void MainWindow::resizeTable() {
    int width = ui->tableWidget->width();
    if (width == 100) width = 635;
    ui->tableWidget->setColumnWidth(0, std::round(width * 0.17));
    ui->tableWidget->setColumnWidth(1, std::round(width * 0.04));
    ui->tableWidget->setColumnWidth(2, std::round(width * 0.15));
    ui->tableWidget->setColumnWidth(3, std::round(width * 0.19));
    ui->tableWidget->setColumnWidth(4, std::round(width * 0.17));
    ui->tableWidget->setColumnWidth(5, std::round(width * 0.04));
    ui->tableWidget->setColumnWidth(6, std::round(width * 0.15));
    ui->tableWidget->setColumnWidth(7, std::round(width * 0.09));
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    if (!menu->isMenuVisible()) return false;

    if (target->objectName() == "tableWidget") {
        if(event->type() == QTabletEvent::InputMethodQuery) {
            menu->showMenu();
            return true;
        }
    } else {
        if(event->type() == QMouseEvent::MouseButtonPress) {
            menu->showMenu();
            return true;
        }
    }
    return false;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    currentpage = this;
    mypage = new MyPage();
    registerpage = new RegisterPage();
    loginpage = new LoginPage();
    int n = 30;

    ui->setupUi(this);
    this->setFocus();

    // menu = new HeaderMenu("BTC/USDT: 43000$; ETH/USDT: 8000$", ui->widget_3->parentWidget());
    menu = new HeaderMenu("Актуальные P2P-связки", ui->widget_3->parentWidget());
    ui->widget_3->parentWidget()->layout()->replaceWidget(ui->widget_3, menu);

    menu->installEventFilter(this);
    ui->pushButton->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->centralwidget->installEventFilter(this);
    ui->checkBox->installEventFilter(this);
    ui->checkBox_2->installEventFilter(this);
    ui->checkBox_3->installEventFilter(this);
    ui->checkBox_4->installEventFilter(this);
    ui->checkBox_5->installEventFilter(this);
    ui->checkBox_6->installEventFilter(this);
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);

    // connect(menu, &HeaderMenu::homePage, this, &MainWindow::open_homepage);
    connect(menu, &HeaderMenu::myPage, this, &MainWindow::open_mypage);

    connect(mypage, &MyPage::homePage, this, &MainWindow::open_homepage);
    connect(mypage, &MyPage::loginPage, this, &MainWindow::open_loginpage);
    connect(mypage, &MyPage::registerPage, this, &MainWindow::open_registerpage);

    connect(loginpage, &LoginPage::homePage, this, &MainWindow::open_homepage);
    connect(loginpage, &LoginPage::myPage, this, &MainWindow::open_mypage);
    connect(loginpage, &LoginPage::registerPage, this, &MainWindow::open_registerpage);

    connect(registerpage, &RegisterPage::homePage, this, &MainWindow::open_homepage);
    connect(registerpage, &RegisterPage::myPage, this, &MainWindow::open_mypage);
    connect(registerpage, &RegisterPage::loginPage, this, &MainWindow::open_loginpage);



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
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    resizeTable();
    menu->resizeMenu();

    QMainWindow::resizeEvent(e);
}


MainWindow::~MainWindow()
{
    delete ui;
}
