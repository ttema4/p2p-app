#include "chainmonitor.h"
#include "ui_chainmonitor.h"

ChainMonitor::ChainMonitor(QWidget *parent) : QWidget(parent), ui(new Ui::ChainMonitor) {
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &ChainMonitor::close);
    hide();
}

ChainMonitor::ChainMonitor(QWidget *parent, Chain c) : QWidget(parent), ui(new Ui::ChainMonitor) {
    ui->setupUi(this);
    chainHash =
        QString::fromStdString(c.buy.coin1 + c.buy.coin2 + c.buy.market + c.sell.coin1 + c.sell.coin2 + c.sell.market);
    isFavorite = CurUser::getInstance().isFavorites(chainHash);
    if (isFavorite) {
        ui->pushButton_2->setText("★");
    }

    if (c.change.first == c.change.second) {
        ui->label_8->setText(QString("Связка RUB - %1 - RUB со спредом %2\%")
                                 .arg(QString::fromStdString(c.change.first), QString::number(c.spread, 'f', 2)));
    } else {
        ui->label_8->setText(QString("Связка RUB - %1 - %2 - RUB со спредом %3\%")
                                 .arg(
                                     QString::fromStdString(c.change.first), QString::fromStdString(c.change.second),
                                     QString::number(c.spread, 'f', 2)
                                 ));
    }

    ui->label_3->setText(QString("<b>Coin Pair:</b> %1 → %2<br><b>Spread:</b> %3%")
                             .arg(
                                 QString::fromStdString(c.change.first), QString::fromStdString(c.change.second),
                                 QString::number(c.spread, 'f', 2)
                             ));

    QString lbl1 = QString(
                       "<p style='line-height: 150%;'><b>Coin 1:</b> %1<br><b>Coin 2:</b> %2<br><b>Market:</b> "
                       "%3<br><b>Exchange rate:</b> %4 RUB<br><b>Banks:</b> %5"
    )
                       .arg(
                           QString::fromStdString(c.buy.coin1), QString::fromStdString(c.buy.coin2),
                           QString::fromStdString(c.buy.market), QString::number(c.buy.exchange_rate, 'f', 2),
                           QString::fromStdString(c.buy.banks[0])
                       );
    for (int i = 1; i < c.buy.banks.size(); i++) {
        lbl1 += (", " + c.buy.banks[i]);
    }
    lbl1 += QString("<br><b>Min Volume:</b> %1 RUB<br><b>Max Volume:</b> %2 RUB<br><b>Seller rating:</b> %3</p>")
                .arg(
                    QString::number(c.buy.min_max.first), QString::number(c.buy.min_max.second),
                    QString::number(c.buy.seller_rating, 'f', 2)
                );
    ui->label->setText(lbl1);
    ui->label_6->setText(QString("<a href=\"%8\">Order link</a>").arg(QString::fromStdString(c.buy.id)));

    QString lbl2 = QString(
                       "<p style='line-height: 150%;'><b>Coin 1:</b> %1<br><b>Coin 2:</b> %2<br><b>Market:</b> "
                       "%3<br><b>Exchange rate:</b> %4 RUB<br><b>Banks:</b> %5"
    )
                       .arg(
                           QString::fromStdString(c.sell.coin2), QString::fromStdString(c.sell.coin1),
                           QString::fromStdString(c.sell.market), QString::number(c.sell.exchange_rate, 'f', 2),
                           QString::fromStdString(c.sell.banks[0])
                       );
    for (int i = 1; i < c.sell.banks.size(); i++) {
        lbl2 += (", " + c.sell.banks[i]);
    }
    lbl2 += QString("<br><b>Min Volume:</b> %1 RUB<br><b>Max Volume:</b> %2 RUB<br><b>Seller rating:</b> %3</p>")
                .arg(
                    QString::number(c.sell.min_max.first), QString::number(c.sell.min_max.second),
                    QString::number(c.sell.seller_rating, 'f', 2)
                );

    ui->label_2->setText(lbl2);
    ui->label_7->setText(QString("<a href=\"%8\">Order link</a>").arg(QString::fromStdString(c.sell.id)));
    connect(ui->pushButton, &QPushButton::clicked, this, &ChainMonitor::close);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &ChainMonitor::favouriteSwap);

    setStyleSheet(
        "#widget_3 { background-color: #F5F5F5; border-radius: 10px; }"
        "QFrame#frame, QFrame#frame_2, QFrame#frame_3 { background-color: #F5F5F5; border-radius: 5px; border: 1px "
        "solid #bebebe; }"
        "QPushButton { background-color: none; border-style: none; }"
        "QLabel#label_3, QLabel#label_4, QLabel#label_5 { font-size: 14px; }"
    );
    hide();

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(50);
    effect->setXOffset(0);
    effect->setYOffset(0);
    effect->setColor(QColor(0, 0, 0, 80));
    ui->widget_3->setGraphicsEffect(effect);
}

void ChainMonitor::favouriteSwap() {
    if (isFavorite) {
        if (CurUser::getInstance().tryDelFavorites(chainHash)) {
            ui->pushButton_2->setText("☆");
            isFavorite = false;
        }
    } else {
        if (CurUser::getInstance().tryAddFavorites(chainHash)) {
            ui->pushButton_2->setText("★");
            isFavorite = true;
        }
    }
}

void ChainMonitor::remove() {
    move(QPoint(
        parentWidget()->size().width() / 2 - size().width() / 2,
        parentWidget()->size().height() / 2 - size().height() / 2
    ));
}

void ChainMonitor::resizeEvent(QResizeEvent *e) {
    move(QPoint(
        parentWidget()->size().width() / 2 - size().width() / 2,
        parentWidget()->size().height() / 2 - size().height() / 2
    ));
    QWidget::resizeEvent(e);
}

void ChainMonitor::closeEvent(QCloseEvent *event) {
    emit monitorClosed();
    QWidget::closeEvent(event);
}

ChainMonitor::~ChainMonitor() {
    delete ui;
}
