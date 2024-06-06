#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::filterHidden() {
    bool checked = ui->hideButton->isChecked();
    ui->hideButton->setStyleSheet(
        checked ? "border-image: url(:/resourses/icons/left-arrow.png);"
                : "border-image: url(:/resourses/icons/right-arrow.png);"
    );
}

void MainWindow::showFilters() {
    bool checked = ui->hideButton->isChecked();
    ui->hideButton->setStyleSheet("border-image: url(://resourses/icons/no-arrow.png);");

    sizeAnim->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    connect(sizeAnim, &QParallelAnimationGroup::finished, this, &MainWindow::filterHidden);
    sizeAnim->start();
}

void MainWindow::applyFilters() {
    QMap<QString, QString> banks{{"Сбербанк", "Sber"},   {"Тинькофф", "Tinkoff"}, {"Альфа-Банк", "Alpha"},
                                 {"Райффайзен", "Raif"}, {"Газпром", "Gasprom"},  {"ВТБ", "VTB"},
                                 {"СБП", "SBP"}};

    QSet<QString> selectedBanks, selectedMarkets;
    QPair<double, double> selectedMinMax;

    foreach (QCheckBox *checkBox, ui->groupBox_2->findChildren<QCheckBox *>()) {
        if (checkBox->isChecked()) {
            selectedBanks.insert(banks[checkBox->text()]);
        }
    }

    foreach (QCheckBox *checkBox, ui->groupBox_3->findChildren<QCheckBox *>()) {
        if (checkBox->isChecked()) {
            selectedMarkets.insert(checkBox->text());
        }
    }

    selectedMinMax.first = (ui->lineEdit->text() != "") ? ui->lineEdit->text().toDouble() : 0;
    selectedMinMax.second =
        (ui->lineEdit_2->text() != "") ? ui->lineEdit_2->text().toDouble() : std::numeric_limits<double>::max();

    chainTable->setFilters(selectedBanks, selectedMarkets, selectedMinMax);
}

void MainWindow::resetFilters() {
    foreach (QCheckBox *checkBox, ui->groupBox_2->findChildren<QCheckBox *>()) {
        checkBox->setChecked(false);
    }

    foreach (QCheckBox *checkBox, ui->groupBox_3->findChildren<QCheckBox *>()) {
        checkBox->setChecked(false);
    }

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();

    applyFilters();
}

void MainWindow::chainMonitorHide() {
    foreach (QWidget *widget, this->findChildren<QWidget *>()) {
        widget->setEnabled(true);
    }
    chainMonitorOpen = false;
}

void MainWindow::onCellClicked(Chain &chain) {
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
    connect(chainmonitor, &ChainMonitor::monitorClosed, this, &MainWindow::chainMonitorHide);
    chainmonitor->show();
}

void MainWindow::updateTable(QVector<Chain> new_chains) {
    timer->stop();
    time->setHMS(0, 0, 0);
    ui->label->setText(QString("Last update 0s ago"));
    timer->start(1000);

    CurUser::getInstance().setCurrentChains(new_chains);
    chainTable->setData(std::move(new_chains));
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    chainMonitorOpen = false;

    ui->setupUi(this);
    this->setFocus();

    menu = new HeaderMenu("Актуальные P2P-связки", ui->widget_3->parentWidget());
    ui->widget_3->parentWidget()->layout()->replaceWidget(ui->widget_3, menu);

    chainTable = new ChainTableView(ui->widget_4->parentWidget());
    ui->widget_4->parentWidget()->layout()->replaceWidget(ui->widget_4, chainTable);

    menu->installEventFilter(this);
    chainTable->installEventFilter(this);
    ui->pushButton->installEventFilter(this);
    ui->pushButton_2->installEventFilter(this);
    ui->centralwidget->installEventFilter(this);
    ui->checkBox->installEventFilter(this);
    ui->checkBox_2->installEventFilter(this);
    ui->checkBox_3->installEventFilter(this);
    ui->checkBox_4->installEventFilter(this);
    ui->checkBox_5->installEventFilter(this);
    ui->checkBox_7->installEventFilter(this);
    ui->checkBox_8->installEventFilter(this);
    ui->checkBox_9->installEventFilter(this);
    ui->checkBox_10->installEventFilter(this);
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->hideButton->installEventFilter(this);

    connect(menu, &HeaderMenu::myPage, this, &MainWindow::myPage);
    connect(menu, &HeaderMenu::loginPage, this, &MainWindow::loginPage);
    connect(menu, &HeaderMenu::registerPage, this, &MainWindow::registerPage);
    connect(menu, &HeaderMenu::notifyPage, this, &MainWindow::notifyPage);
    connect(menu, &HeaderMenu::favouritePage, this, &MainWindow::favouritePage);
    connect(menu, &HeaderMenu::settingsPage, this, &MainWindow::settingsPage);

    // Настройка бокового меню
    connect(ui->hideButton, &QToolButton::toggled, this, &MainWindow::showFilters);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::applyFilters);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::resetFilters);

    sizeAnim = new QParallelAnimationGroup(ui->widget_2);
    sizeAnim->addAnimation(new QPropertyAnimation(ui->widget_2, "minimumWidth"));
    sizeAnim->addAnimation(new QPropertyAnimation(ui->widget_2, "maximumWidth"));

    for (int i = 0; i < sizeAnim->animationCount(); ++i) {
        QPropertyAnimation *animation = qobject_cast<QPropertyAnimation *>(sizeAnim->animationAt(i));
        animation->setDuration(500);
        animation->setStartValue(0);
        animation->setEndValue(ui->widget_2->maximumWidth());
    }

    ui->pushButton->setStyleSheet(
        "background-color: #FCFCFC; border-radius: 6px; border: 1px solid #bebebe; font: 12px; padding: 4px;"
    );
    ui->lineEdit->setStyleSheet("background-color: #FCFCFC; border-radius: 4px; border: 1px solid #bebebe");
    ui->lineEdit_2->setStyleSheet("background-color: #FCFCFC; border-radius: 4px; border: 1px solid #bebebe");

    connect(chainTable, &ChainTableView::cellClicked, this, &MainWindow::onCellClicked);

    timer = new QTimer(this);
    time = new QTime(0, 0, 0);
    connect(timer, &QTimer::timeout, this, [&]() {
        time = new QTime(time->addSecs(1));
        int minutes = time->minute();
        int seconds = time->second();

        QString timeString;
        if (minutes > 0) {
            timeString = QString("Last update %1m %2s ago").arg(minutes, seconds);
        } else {
            timeString = QString("Last update %1s ago").arg(seconds);
        }
        ui->label->setText(timeString);
    });
    timer->start(1000);

    connect(&DataReciever::getInstance(), &DataReciever::dataParsed, this, &MainWindow::updateTable);
    DataReciever::getInstance().start();
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    menu->resizeMenu();

    QMainWindow::resizeEvent(e);
}

MainWindow::~MainWindow() {
    delete ui;
}
