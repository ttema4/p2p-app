#include "chainmonitor.h"
#include "ui_chainmonitor.h"
#include <QPushButton>

ChainMonitor::ChainMonitor(QWidget *parent) : QWidget(parent), ui(new Ui::ChainMonitor) {
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &ChainMonitor::close);
    hide();
}

ChainMonitor::ChainMonitor(QWidget *parent, Chain c) : QWidget(parent), ui(new Ui::ChainMonitor) {
    ui->setupUi(this);
    ui->label->setText(QString("Coin1: %1\nCoin2: %2\nExchange rate: %3\nBank: %4\nMin-max: %5-%6\nSeller rating: %7").arg(QString::fromStdString(c.buy.coin1), QString::fromStdString(c.buy.coin2), QString::number(c.buy.exchange_rate, 'f', 2), QString::fromStdString(c.buy.bank), QString::number(c.buy.min_max.first), QString::number(c.buy.min_max.second), QString::number(c.buy.seller_rating, 'f', 2)));
    ui->label_6->setText(QString("id: <a href=\"%8\">Order link</a>").arg(QString::fromStdString(c.buy.id)));
    ui->label_2->setText(QString("Coin1: %1\nCoin2: %2\nExchange rate: %3\nBank: %4\nMin-max: %5-%6\nSeller rating: %7").arg(QString::fromStdString(c.sell.coin1), QString::fromStdString(c.sell.coin2), QString::number(c.sell.exchange_rate, 'f', 2), QString::fromStdString(c.sell.bank), QString::number(c.sell.min_max.first), QString::number(c.sell.min_max.second), QString::number(c.sell.seller_rating, 'f', 2)));
    ui->label_7->setText(QString("id: <a href=\"%8\">Order link</a>").arg(QString::fromStdString(c.sell.id)));
    ui->label_3->setText(QString("Coin-Pair: %1 -> %2\nSpread: %3").arg(QString::fromStdString(c.change.first), QString::fromStdString(c.change.second), QString::number(c.spread, 'f', 2)));
    connect(ui->pushButton, &QPushButton::clicked, this, &ChainMonitor::close);
    setStyleSheet(
        "#widget_3 { background-color: #F5F5F5; border-radius: 10px; border: 2px solid #82a1c9; }"
        "QFrame#frame, QFrame#frame_2, QFrame#frame_3 { background-color: #F5F5F5; border-radius: 10px; border: 1px solid #82a1c9; }"
        "QPushButton { background-color: none; border-style: none; font: 25px; }" );
    hide();
}

void ChainMonitor::remove() {
    move(QPoint(parentWidget()->size().width() / 2 - size().width() / 2, parentWidget()->size().height() / 2 - size().height() / 2));

}

void ChainMonitor::mousePressEvent(QMouseEvent* event) {
    if (!rect().contains(event->pos())) {
        this->close(); // Закрыть при клике за пределами
    }
}

void ChainMonitor::closeEvent(QCloseEvent *event)  {
    emit monitorSlosed();
    QWidget::closeEvent(event);
}


ChainMonitor::~ChainMonitor() {
    delete ui;
}
