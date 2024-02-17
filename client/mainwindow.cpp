#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setColumnWidth(1, 20);
    ui->tableWidget->setColumnWidth(0, 70);
    ui->tableWidget->setColumnWidth(4, 70);
    ui->tableWidget->setColumnWidth(3, 120);
    ui->tableWidget->setColumnWidth(5, 20);

    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QString("Покупка на P2P")));
    ui->tableWidget->setSpan(0,0,1,3);

    ui->tableWidget->setItem(0, 3, new QTableWidgetItem(QString("Обмен на споте")));

    ui->tableWidget->setItem(0, 4, new QTableWidgetItem(QString("Продажа на P2P")));
    ui->tableWidget->setSpan(0,4,1,3);

    ui->tableWidget->setItem(0, 7, new QTableWidgetItem(QString("Спред")));

}

MainWindow::~MainWindow()
{
    delete ui;
}
