#include "favouritepage.h"
#include "ui_favouritepage.h"

FavouritePage::FavouritePage(QWidget *parent) : QMainWindow(parent), ui(new Ui::FavouritePage) {
    ui->setupUi(this);
    chainMonitorOpen = false;

    menu = new HeaderMenu("Избранное", this);
    ui->widget->parentWidget()->layout()->replaceWidget(ui->widget, menu);

    chainTable = new ChainTableView(ui->widget_2->parentWidget());
    ui->widget_2->parentWidget()->layout()->replaceWidget(ui->widget_2, chainTable);

    connect(menu, &HeaderMenu::homePage, this, &FavouritePage::homePage);
    connect(menu, &HeaderMenu::loginPage, this, &FavouritePage::loginPage);
    connect(menu, &HeaderMenu::myPage, this, &FavouritePage::myPage);
    connect(menu, &HeaderMenu::notifyPage, this, &FavouritePage::notifyPage);
    connect(menu, &HeaderMenu::settingsPage, this, &FavouritePage::settingsPage);

    connect(chainTable, &ChainTableView::cellClicked, this, &FavouritePage::onCellClicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &FavouritePage::clearFavourites);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &FavouritePage::updateTable);

    menu->installEventFilter(this);
    ui->pushButton->installEventFilter(this);
    ui->pushButton_2->installEventFilter(this);
    ui->centralwidget->installEventFilter(this);

    updateTable();
}

void FavouritePage::clearFavourites() {
    CurUser::getInstance().tryClearFavourites();
    updateTable();
}

void FavouritePage::chainMonitorHide() {
    foreach (QWidget *widget, this->findChildren<QWidget *>()) {
        widget->setEnabled(true);
    }
    chainMonitorOpen = false;
}

void FavouritePage::onCellClicked(Chain &chain) {
    if (menu->isMenuVisible()) {
        menu->showMenu();
        return;
    }
    if (chainMonitorOpen) {
        chainmonitor->close();
        return;
    }
    foreach (QWidget *widget, this->findChildren<QWidget *>()) {
        widget->setEnabled(false);
    }
    chainmonitor = new ChainMonitor(this, chain);
    chainmonitor->move(QPoint(
        this->size().width() / 2 - chainmonitor->size().width() / 2,
        this->size().height() / 2 - chainmonitor->size().height() / 2
    ));
    chainmonitor->setWindowModality(Qt::ApplicationModal);
    chainMonitorOpen = true;
    connect(chainmonitor, &ChainMonitor::monitorClosed, this, &FavouritePage::chainMonitorHide);
    chainmonitor->show();
}

void FavouritePage::updateTable() {
    QVector<Chain> favouriteChains;
    foreach (Chain c, CurUser::getInstance().getCurrentChains()) {
        QString chainHash = QString::fromStdString(
            c.buy.coin1 + c.buy.coin2 + c.buy.market + c.sell.coin1 + c.sell.coin2 + c.sell.market
        );
        if (CurUser::getInstance().getFavourites().contains(chainHash)) {
            favouriteChains.push_back(c);
        }
    }

    chainTable->setData(favouriteChains);

    if (CurUser::getInstance().getId() != -1) {
        ui->label->setText(
            "Актуальны " + QString::number(favouriteChains.size()) + " из " +
            QString::number(CurUser::getInstance().getFavourites().size()) + " связок!"
        );
    } else {
        ui->label->setText("Для просмотра избранного войдите/зарегистрируйтесь!");
    }
}

void FavouritePage::showEvent(QShowEvent *event) {
    updateTable();
}

bool FavouritePage::eventFilter(QObject *target, QEvent *event) {
    if (menu->isMenuVisible() && event->type() == QMouseEvent::MouseButtonPress) {
        menu->showMenu();
        return true;
    }

    if (chainMonitorOpen && event->type() == QInputEvent::Resize) {
        chainmonitor->remove();
        return true;
    }

    return false;
}

FavouritePage::~FavouritePage() {
    delete ui;
}
