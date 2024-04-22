#include "mainwindow.h"
#include "ui_mainwindow.h"
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
#include <QParallelAnimationGroup>
#include <QPixmap>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QVector>

void MainWindow::filterHidden() {
    bool checked = ui->hideButton->isChecked();
    ui->hideButton->setStyleSheet(checked ? "border-image: url(:/resourses/icons/left-arrow.png);" : "border-image: url(:/resourses/icons/right-arrow.png);");
}

void MainWindow::showFilters() {
    bool checked = ui->hideButton->isChecked();
    ui->hideButton->setStyleSheet("border-image: url(://resourses/icons/no-arrow.png);");

    sizeAnim->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    connect(sizeAnim, &QParallelAnimationGroup::finished, this, &MainWindow::filterHidden);
    sizeAnim->start();
}

void MainWindow::chainMonitorHide() {
    chainMonitorOpen = false;
}

void MainWindow::onCellClicked(int row, int column) {
    if (chainMonitorOpen) { chainmonitor->close(); return; }
    if (row == 0) return;
    // qDebug() << "Cell clicked at row" << row << "and column" << column;
    chainmonitor = new ChainMonitor(this, chains[row - 1]);
    chainmonitor->move(QPoint(this->size().width() / 2 - chainmonitor->size().width() / 2, this->size().height() / 2 - chainmonitor->size().height() / 2));
    chainmonitor->setWindowModality(Qt::ApplicationModal);
    chainMonitorOpen = true;
    connect(chainmonitor, &ChainMonitor::monitorSlosed, this, &MainWindow::chainMonitorHide);
    chainmonitor->show();
}

void MainWindow::updateTable() {
    chains = dr.recieveNewChain();
    ui->tableWidget->setRowCount(chains.size() + 1);

    for (int i = 1; i <= chains.size(); i++) {
        Chain new_chain = chains[i - 1];

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString((new_chain.buy.coin1 + "/" + new_chain.buy.coin2).c_str())));

        QTableWidgetItem* bank1 = new QTableWidgetItem;
        bank1->setIcon(QIcon(include_map[new_chain.buy.bank].c_str()));
        bank1->setToolTip(QString((new_chain.buy.bank).c_str()));;
        ui->tableWidget->setItem(i, 1, bank1);

        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString(tr("%1").arg((double)new_chain.buy.exchange_rate))));

        QTableWidgetItem* change = new QTableWidgetItem(QString((new_chain.change.first + " -> " + new_chain.change.second).c_str()));
        change->setTextAlignment(Qt::AlignCenter) ;
        ui->tableWidget->setItem(i, 3, change);

        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString((new_chain.sell.coin1 + "/" + new_chain.sell.coin2).c_str())));

        QTableWidgetItem* bank2 = new QTableWidgetItem;
        bank2->setIcon(QIcon(include_map[new_chain.sell.bank].c_str()));
        bank2->setToolTip(QString((new_chain.sell.bank).c_str()));
        ui->tableWidget->setItem(i, 5, bank2);

        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString(tr("%1").arg((double)new_chain.sell.exchange_rate))));

        QTableWidgetItem* spread = new QTableWidgetItem(QString(tr("%1%").arg((double)new_chain.spread)));
        spread->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 7, spread);
    }
}

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

void MainWindow::open_notifypage() {
    windowChanger(notifypage);
}

void MainWindow::open_favouritepage() {
    windowChanger(favouritepage);
}

void MainWindow::open_settingspage() {
    windowChanger(settingspage);
}

void MainWindow::resizeTable() {
    int width = ui->tableWidget->width();
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
    if (menu->isMenuVisible()) {
        if (target->objectName() == "tableWidget") {
            if(event->type() == QTabletEvent::InputMethodQuery) {
                menu->showMenu();
                return true;
            } else if (event->type() == QTabletEvent::Resize) {
                resizeTable();
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
    if (chainMonitorOpen) {
        if(event->type() == QMouseEvent::MouseButtonPress) {
            chainmonitor->close();
            return true;
        } else if (event->type() == QTabletEvent::Resize) {
            resizeTable();
            chainmonitor->remove();
            return true;
        }
        return false;
    }
    if (target->objectName() == "tableWidget") {
        if(event->type() == QTabletEvent::Resize) {
            resizeTable();
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
    notifypage = new NotifyPage();
    favouritepage = new FavouritePage();
    settingspage = new SettingsPage();
    chainMonitorOpen = false;

    ui->setupUi(this);
    this->setFocus();

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
    ui->hideButton->installEventFilter(this);

    connect(menu, &HeaderMenu::myPage, this, &MainWindow::open_mypage);
    connect(menu, &HeaderMenu::loginPage, this, &MainWindow::open_loginpage);
    connect(menu, &HeaderMenu::registerPage, this, &MainWindow::open_registerpage);
    connect(menu, &HeaderMenu::notifyPage, this, &MainWindow::open_notifypage);
    connect(menu, &HeaderMenu::favouritePage, this, &MainWindow::open_favouritepage);
    connect(menu, &HeaderMenu::settingsPage, this, &MainWindow::open_settingspage);

    connect(mypage, &MyPage::homePage, this, &MainWindow::open_homepage);
    connect(mypage, &MyPage::loginPage, this, &MainWindow::open_loginpage);
    connect(mypage, &MyPage::registerPage, this, &MainWindow::open_registerpage);
    connect(mypage, &MyPage::notifyPage, this, &MainWindow::open_notifypage);
    connect(mypage, &MyPage::favouritePage, this, &MainWindow::open_favouritepage);
    connect(mypage, &MyPage::settingsPage, this, &MainWindow::open_settingspage);

    connect(loginpage, &LoginPage::homePage, this, &MainWindow::open_homepage);
    connect(loginpage, &LoginPage::myPage, this, &MainWindow::open_mypage);
    connect(loginpage, &LoginPage::registerPage, this, &MainWindow::open_registerpage);
    connect(loginpage, &LoginPage::notifyPage, this, &MainWindow::open_notifypage);
    connect(loginpage, &LoginPage::favouritePage, this, &MainWindow::open_favouritepage);
    connect(loginpage, &LoginPage::settingsPage, this, &MainWindow::open_settingspage);

    connect(registerpage, &RegisterPage::homePage, this, &MainWindow::open_homepage);
    connect(registerpage, &RegisterPage::myPage, this, &MainWindow::open_mypage);
    connect(registerpage, &RegisterPage::loginPage, this, &MainWindow::open_loginpage);
    connect(registerpage, &RegisterPage::notifyPage, this, &MainWindow::open_notifypage);
    connect(registerpage, &RegisterPage::favouritePage, this, &MainWindow::open_favouritepage);
    connect(registerpage, &RegisterPage::settingsPage, this, &MainWindow::open_settingspage);

    connect(notifypage, &NotifyPage::homePage, this, &MainWindow::open_homepage);
    connect(notifypage, &NotifyPage::myPage, this, &MainWindow::open_mypage);
    connect(notifypage, &NotifyPage::loginPage, this, &MainWindow::open_loginpage);
    connect(notifypage, &NotifyPage::registerPage, this, &MainWindow::open_registerpage);
    connect(notifypage, &NotifyPage::favouritePage, this, &MainWindow::open_favouritepage);
    connect(notifypage, &NotifyPage::settingsPage, this, &MainWindow::open_settingspage);

    connect(favouritepage, &FavouritePage::homePage, this, &MainWindow::open_homepage);
    connect(favouritepage, &FavouritePage::myPage, this, &MainWindow::open_mypage);
    connect(favouritepage, &FavouritePage::loginPage, this, &MainWindow::open_loginpage);
    connect(favouritepage, &FavouritePage::registerPage, this, &MainWindow::open_registerpage);
    connect(favouritepage, &FavouritePage::notifyPage, this, &MainWindow::open_notifypage);
    connect(favouritepage, &FavouritePage::settingsPage, this, &MainWindow::open_settingspage);

    connect(settingspage, &SettingsPage::homePage, this, &MainWindow::open_homepage);
    connect(settingspage, &SettingsPage::myPage, this, &MainWindow::open_mypage);
    connect(settingspage, &SettingsPage::loginPage, this, &MainWindow::open_loginpage);
    connect(settingspage, &SettingsPage::registerPage, this, &MainWindow::open_registerpage);
    connect(settingspage, &SettingsPage::notifyPage, this, &MainWindow::open_notifypage);
    connect(settingspage, &SettingsPage::favouritePage, this, &MainWindow::open_favouritepage);


    // Настройка бокового меню
    connect(ui->hideButton, &QToolButton::toggled, this, &MainWindow::showFilters);

    sizeAnim = new QParallelAnimationGroup(ui->widget_2);
    sizeAnim->addAnimation(new QPropertyAnimation(ui->widget_2, "minimumWidth"));
    sizeAnim->addAnimation(new QPropertyAnimation(ui->widget_2, "maximumWidth"));

    for (int i = 0; i < sizeAnim->animationCount(); ++i) {
        QPropertyAnimation *animation = qobject_cast<QPropertyAnimation*>(sizeAnim->animationAt(i));
        animation->setDuration(500);
        animation->setStartValue(0);
        animation->setEndValue(ui->widget_2->maximumWidth());
    }


    // Настройка таблицы
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->setRowCount(1);
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

    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &MainWindow::onCellClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::updateTable);

    updateTable();
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    menu->resizeMenu();
    QMainWindow::resizeEvent(e);
}


MainWindow::~MainWindow() {
    delete ui;
}
