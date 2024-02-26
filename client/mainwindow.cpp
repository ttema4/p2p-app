#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "data_reciever.cpp"
#include "mainwindow_fwd.hpp"
#include <QResizeEvent>
#include <QDebug>
#include <QPixmap>
#include <QIcon>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    int n = 50;

    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->setRowCount(n + 1);
    ui->tableWidget->setColumnCount(8);

    QTableWidgetItem* header0 = new QTableWidgetItem(QString("Покупка на P2P"));
    header0->setTextAlignment(Qt::AlignCenter) ;
    ui->tableWidget->setItem(0, 0, header0);

    // ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QString("Покупка на P2P")));
    ui->tableWidget->setSpan(0, 0, 1, 3);

    QTableWidgetItem* header3 = new QTableWidgetItem(QString("Обмен на споте"));
    header3->setTextAlignment(Qt::AlignCenter) ;
    ui->tableWidget->setItem(0, 3, header3);

    // ui->tableWidget->setItem(0, 3, new QTableWidgetItem(QString("Обмен на споте")));


    QTableWidgetItem* header4 = new QTableWidgetItem(QString("Продажа на P2P"));
    header4->setTextAlignment(Qt::AlignCenter) ;
    ui->tableWidget->setItem(0, 4, header4);

    // ui->tableWidget->setItem(0, 4, new QTableWidgetItem(QString("Продажа на P2P")));
    ui->tableWidget->setSpan(0, 4, 1, 3);

    QTableWidgetItem* header7 = new QTableWidgetItem(QString("Спред"));
    header7->setTextAlignment(Qt::AlignCenter) ;
    ui->tableWidget->setItem(0, 7, header7);

    // ui->tableWidget->setItem(0, 7, new QTableWidgetItem(QString("Спред")));


    // ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    DataReciever dr;

    for (int i = 1; i <= n; i++) {
        Chain new_chain = dr.recieveNewChain();

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString((new_chain.buy.coin1 + "/" + new_chain.buy.coin2).c_str())));

        QTableWidgetItem* bank1 = new QTableWidgetItem;
        bank1->setIcon(QIcon(include_map[new_chain.buy.bank].c_str()));
        bank1->setToolTip(QString((new_chain.buy.bank).c_str()));;
        ui->tableWidget->setItem( i, 1, bank1);

        // ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString((new_chain.buy.bank).c_str())));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString(tr("%1").arg((double)new_chain.buy.exchange_rate))));

        QTableWidgetItem* change = new QTableWidgetItem(QString((new_chain.change.first + " -> " + new_chain.change.second).c_str()));
        change->setTextAlignment(Qt::AlignCenter) ;
        ui->tableWidget->setItem( i, 3, change);

        // ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString((new_chain.change.first + " -> " + new_chain.change.second).c_str())));

        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString((new_chain.sell.coin1 + "/" + new_chain.sell.coin2).c_str())));

        QTableWidgetItem* bank2 = new QTableWidgetItem;
        bank2->setIcon(QIcon(include_map[new_chain.sell.bank].c_str()));
        bank2->setToolTip(QString((new_chain.sell.bank).c_str()));
        ui->tableWidget->setItem( i, 5, bank2);

        // ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString((new_chain.sell.bank).c_str())));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString(tr("%1").arg((double)new_chain.sell.exchange_rate))));

        QTableWidgetItem* spread = new QTableWidgetItem(QString(tr("%1%").arg((double)new_chain.spread)));
        spread->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem( i, 7, spread);

        // ui->tableWidget->setItem(i, 7, new QTableWidgetItem(QString(tr("%1%").arg((double)new_chain.spread))));
    }
}

void MainWindow::resizeEvent(QResizeEvent *e) {
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

    // qDebug() << width;
    QMainWindow::resizeEvent(e);
}


MainWindow::~MainWindow()
{
    delete ui;
}
