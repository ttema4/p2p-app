#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "data_reciever.cpp"
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    int n = 30;

    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->setRowCount(n + 1);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setColumnWidth(1, 20);
    ui->tableWidget->setColumnWidth(0, 70);
    ui->tableWidget->setColumnWidth(4, 70);
    ui->tableWidget->setColumnWidth(3, 120);
    ui->tableWidget->setColumnWidth(5, 20);

    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QString("Покупка на P2P")));
    ui->tableWidget->setSpan(0, 0, 1, 3);

    ui->tableWidget->setItem(0, 3, new QTableWidgetItem(QString("Обмен на споте")));

    ui->tableWidget->setItem(0, 4, new QTableWidgetItem(QString("Продажа на P2P")));
    ui->tableWidget->setSpan(0, 4, 1, 3);

    ui->tableWidget->setItem(0, 7, new QTableWidgetItem(QString("Спред")));

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    DataReciever dr;

    for (int i = 1; i <= n; i++) {
        Chain new_chain = dr.recieveNewChain();
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString((new_chain.buy.coin1 + "/" + new_chain.buy.coin2).c_str())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString((new_chain.buy.bank).c_str())));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString(tr("%1").arg((double)new_chain.buy.exchange_rate))));

        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString((new_chain.change.first + " -> " + new_chain.change.second).c_str())));

        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString((new_chain.sell.coin1 + "/" + new_chain.sell.coin2).c_str())));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString((new_chain.sell.bank).c_str())));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString(tr("%1").arg((double)new_chain.sell.exchange_rate))));

        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(QString(tr("%1 %").arg((double)new_chain.spread))));
    }
    
}

MainWindow::~MainWindow()
{
    delete ui;
}
