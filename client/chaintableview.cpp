#include "chaintableview.h"
#include "QtWidgets/qheaderview.h"


TableNullWidget::TableNullWidget(QWidget *parent) : QWidget(parent) {
    setMinimumHeight(50);
    QLabel *label = new QLabel("Chains not found");
    label->setStyleSheet("color: grey; font-size: 15px;");
    label->setAlignment(Qt::AlignCenter);
    setLayout(new QVBoxLayout(this));
    layout()->addWidget(label);
}

TableChangeWidget::TableChangeWidget(std::pair<std::string, std::string> &change, QWidget *parent) : QLabel(parent) {
    if (change.first != change.second) {
        setText(QString::fromStdString(change.first) + " → " + QString::fromStdString(change.second));
    } else {
        setText("—");
    }
    setStyleSheet("font-size: 15px;");
    setAlignment(Qt::AlignCenter);
}

TableSpreadWidget::TableSpreadWidget(double spread, QWidget *parent) : QWidget(parent) {
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel *label = new QLabel();
    label->setText(QString::number(spread, 'f', 1) + "%");
    label->setFixedSize(50, 27);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    if (spread > 0) {
        label->setStyleSheet(
            "font-size: 15px;"
            "background-color: rgba(0, 255, 0, 30); "
            "border-radius: 4px; "
        );
    } else if (spread == 0) {
        label->setStyleSheet(
            "font-size: 15px;"
            "background-color: rgba(128, 128, 128, 30); "
            "border-radius: 4px; "
        );
    } else {
        label->setStyleSheet(
            "font-size: 15px;"
            "background-color: rgba(255, 0, 0, 30); "
            "border-radius: 4px; "
        );
    }
}

TableIndexWidget::TableIndexWidget(int ind, QWidget *parent) : QLabel(parent) {
    setText(QString::number(ind));
    setStyleSheet("font-size: 15px; ");
    setAlignment(Qt::AlignCenter);
}

TableOrderWidget::TableOrderWidget(Order &ord, QWidget *parent) : QWidget(parent) {
    static QMap<std::string, QString> includeMap{
        {"Tinkoff", "://resourses/bank_icons/Tink_bank.svg"}, {"Sber", "://resourses/bank_icons/Sber_bank.svg"},
        {"Alpha", "://resourses/bank_icons/Alpha_bank.svg"},  {"Gasprom", "://resourses/bank_icons/Gasprom_bank.svg"},
        {"SBP", "://resourses/bank_icons/SBP_bank.svg"},      {"Raif", "://resourses/bank_icons/Raf_bank.svg"},
        {"VTB", "://resourses/bank_icons/VTB_bank.svg"}
    };

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 2, 0, 2);
    layout->setVerticalSpacing(1);

    QLabel *label_1 = new QLabel();
    label_1->setFixedSize(27, 27);
    label_1->setAlignment(Qt::AlignCenter);

    QIcon icon(includeMap[ord.banks[0]]);
    QPixmap pixmap = icon.pixmap(QSize(18, 18));
    label_1->setPixmap(pixmap);

    label_1->setStyleSheet("background-color: rgba(128, 128, 128, 30); border-radius: 4px; ");
    layout->addWidget(label_1, 0, 0, 2, 1);

    QLabel *label_2 = new QLabel(QString::fromStdString(ord.coin2));
    label_2->setMinimumSize(0, 23);
    label_2->setStyleSheet("font-size: 17px;");
    label_2->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    layout->addWidget(label_2, 0, 1);

    QLabel *label_3 = new QLabel(
        QString::fromStdString(ord.market) + " • " + QString::fromStdString(ord.banks[0]) +
        (ord.banks.size() > 1 ? QString("(+%1)").arg(ord.banks.size() - 1) : "")
    );
    label_3->setMinimumSize(0, 15);
    label_3->setStyleSheet("font-size: 10px; font-weight: light; color: grey; ");
    label_3->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(label_3, 1, 1);

    QSpacerItem *horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Preferred);
    layout->addItem(horizontalSpacer, 0, 2, 2, 1);

    QLabel *label_4 = new QLabel(QString::number(ord.exchange_rate, 'f', 2));
    label_4->setMinimumSize(0, 25);
    label_4->setStyleSheet("font-size: 17px;");
    label_4->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    layout->addWidget(label_4, 0, 3);

    QLabel *label_5 = new QLabel(QString::fromStdString(ord.coin1) + "/1 " + QString::fromStdString(ord.coin2));
    label_5->setMinimumSize(0, 15);
    label_5->setStyleSheet("font-size: 8px; font-weight: 500; color: grey; ");
    label_5->setAlignment(Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(label_5, 1, 3);
};

TableCellWidget::TableCellWidget(Chain ch, int ind, QWidget *parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 4, 0, 4);
    layout->setSpacing(8);

    QFrame *verticalLine1 = new QFrame();
    verticalLine1->setFrameStyle(QFrame::VLine);
    verticalLine1->setStyleSheet("QFrame { color: lightgrey; }");

    QFrame *verticalLine2 = new QFrame();
    verticalLine2->setFrameStyle(QFrame::VLine);
    verticalLine2->setStyleSheet("QFrame { color: lightgrey; }");

    QFrame *verticalLine3 = new QFrame();
    verticalLine3->setFrameStyle(QFrame::VLine);
    verticalLine3->setStyleSheet("QFrame { color: lightgrey; }");

    QFrame *verticalLine4 = new QFrame();
    verticalLine4->setFrameStyle(QFrame::VLine);
    verticalLine4->setStyleSheet("QFrame { color: lightgrey; }");

    layout->addWidget(new TableIndexWidget(ind + 1));
    layout->addWidget(verticalLine1);
    layout->addWidget(new TableOrderWidget(ch.buy));
    layout->addWidget(verticalLine2);
    layout->addWidget(new TableChangeWidget(ch.change));
    layout->addWidget(verticalLine3);
    layout->addWidget(new TableOrderWidget(ch.sell));
    layout->addWidget(verticalLine4);
    layout->addWidget(new TableSpreadWidget(ch.spread));
};

void TableCellWidget::resizeEvent(QResizeEvent *event) {
    double width = this->width();
    this->layout()->itemAt(0)->widget()->setFixedWidth(std::round(width * 0.04));
    this->layout()->itemAt(2)->widget()->setFixedWidth(std::round(width * 0.29));
    this->layout()->itemAt(4)->widget()->setFixedWidth(std::round(width * 0.16));
    this->layout()->itemAt(6)->widget()->setFixedWidth(std::round(width * 0.29));
    this->layout()->itemAt(8)->widget()->setFixedWidth(std::round(width * 0.1));
    QWidget::resizeEvent(event);
}

ChainTableView::ChainTableView(QWidget *parent) : QWidget{parent} {
    filterMinMax = {0, std::numeric_limits<double>::max()};
    sortType = SortType::none;

    QFont font;
    font.setPointSize(12);
    font.setWeight(QFont::DemiBold);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 0, 5, 5);
    layout->setSpacing(5);

    header = new QTableWidget();
    header->verticalHeader()->hide();
    header->horizontalHeader()->hide();
    header->setRowCount(1);
    header->setColumnCount(5);

    QTableWidgetItem *header0 = new QTableWidgetItem();
    header0->setIcon(QIcon("://resourses/icons/sort.png"));
    header0->setTextAlignment(Qt::AlignCenter);
    header->setItem(0, 0, header0);

    QTableWidgetItem *header1 = new QTableWidgetItem(QString("Покупка на P2P"));
    header1->setTextAlignment(Qt::AlignCenter);
    header1->setFont(font);
    header->setItem(0, 1, header1);

    QTableWidgetItem *header2 = new QTableWidgetItem(QString("Обмен на споте"));
    header2->setTextAlignment(Qt::AlignCenter);
    header2->setFont(font);
    header->setItem(0, 2, header2);

    QTableWidgetItem *header3 = new QTableWidgetItem(QString("Продажа на P2P"));
    header3->setTextAlignment(Qt::AlignCenter);
    header3->setFont(font);
    header->setItem(0, 3, header3);

    QTableWidgetItem *header4 = new QTableWidgetItem(QString("Спред"));
    header4->setTextAlignment(Qt::AlignCenter);
    header4->setFont(font);
    header->setItem(0, 4, header4);

    header->setFixedHeight(header->rowHeight(0));
    header->verticalScrollBar()->setDisabled(true);
    header->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    header->horizontalScrollBar()->setDisabled(true);
    header->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    header->verticalScrollBar()->setDisabled(true);
    header->setSelectionMode(QAbstractItemView::NoSelection);
    header->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(header);

    table = new QTableWidget();
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    table->setRowCount(1);
    table->setColumnCount(1);

    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->horizontalScrollBar()->setDisabled(true);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(table);

    header->setShowGrid(false);
    header->setStyleSheet(
        "QTableWidget{background-color: #e4e4e4; border: none; border-radius: 10px; } QTableWidget::item { "
        "background-color: #e4e4e4; border-radius: 10px; }"
    );
    table->setShowGrid(false);
    table->setStyleSheet(
        "QTableWidget{background-color: #ececec; border: none; } QTableWidget::item { background-color: #fbfbfb; "
        "margin-top: 5px; border-radius: 10px; }"
    );

    connect(header, &QTableWidget::cellClicked, this, &ChainTableView::onHeaderCellClicked);
    connect(table, &QTableWidget::cellClicked, this, &ChainTableView::onTableCellClicked);

    setData();
}

void ChainTableView::onTableCellClicked(int row, int col) {
    emit cellClicked(showedChains[row]);
}

void ChainTableView::updateTable() {
    setData(chains);
}

void ChainTableView::onHeaderCellClicked(int row, int col) {
    switch (sortType) {
        case SortType::none:
            header->itemAt(0, 0)->setIcon(QIcon("://resourses/icons/sort-up.png"));
            sortType = SortType::up;
            break;
        case SortType::up:
            header->itemAt(0, 0)->setIcon(QIcon("://resourses/icons/sort-down.png"));
            sortType = SortType::down;
            break;
        case SortType::down:
            header->itemAt(0, 0)->setIcon(QIcon("://resourses/icons/sort.png"));
            sortType = SortType::none;
            break;
    }
    updateTable();
}

void ChainTableView::setFilters(
    QSet<QString> &selectedBanks,
    QSet<QString> &selectedMarkets,
    QPair<double, double> &selectedMinMax
) {
    if (filterBanks == selectedBanks && filterMarkets == selectedMarkets && filterMinMax == selectedMinMax) {
        qDebug() << "reset filters";
        return;
    }

    filterBanks = std::move(selectedBanks);
    filterMarkets = std::move(selectedMarkets);
    filterMinMax = std::move(selectedMinMax);

    updateTable();
}

void ChainTableView::setData(QVector<Chain> data) {
    if (data.empty()) {
        table->setRowCount(1);
        table->setCellWidget(0, 0, new TableNullWidget());
        table->setRowHeight(0, 60);
        return;
    }

    if (&chains != &data) {
        chains = std::move(data);
    }

    if (filterBanks.size() || filterMarkets.size() || filterMinMax.first ||
        filterMinMax.second < std::numeric_limits<double>::max()) {
        showedChains.clear();
        showedChains.reserve(chains.size());
        foreach (Chain chain, chains) {
            bool bankContains1 =
                filterBanks.empty() ||
                std::find_if(chain.buy.banks.begin(), chain.buy.banks.end(), [&](const std::string &str) {
                    return filterBanks.contains(QString::fromStdString(str));
                }) != chain.buy.banks.end();
            bool bankContains2 =
                filterBanks.empty() ||
                std::find_if(chain.sell.banks.begin(), chain.sell.banks.end(), [&](const std::string &str) {
                    return filterBanks.contains(QString::fromStdString(str));
                }) != chain.sell.banks.end();
            bool marketContains =
                filterMarkets.empty() || filterMarkets.contains(QString::fromStdString(chain.buy.market)) &&
                                             filterMarkets.contains(QString::fromStdString(chain.sell.market));
            bool minMaxMatches =
                (filterMinMax.first == 0 && filterMinMax.second == std::numeric_limits<double>::max()) ||
                (filterMinMax.first >= chain.buy.min_max.first && filterMinMax.second <= chain.buy.min_max.second) &&
                    (filterMinMax.first >= chain.sell.min_max.first && filterMinMax.second <= chain.sell.min_max.second
                    );
            if (bankContains1 && bankContains2 && marketContains && minMaxMatches) {
                showedChains.push_back(chain);
            }
        }
    } else {
        showedChains = chains;
    }

    if (showedChains.empty()) {
        table->setRowCount(1);
        table->setCellWidget(0, 0, new TableNullWidget());
        table->setRowHeight(0, 60);
        return;
    }

    switch (sortType) {
        case SortType::up:
            std::sort(showedChains.begin(), showedChains.end(), [](Chain &a, Chain &b) { return a.spread > b.spread; });
            break;
        case SortType::down:
            std::sort(showedChains.begin(), showedChains.end(), [](Chain &a, Chain &b) { return a.spread < b.spread; });
            break;
        case SortType::none:
            break;
    }

    table->setRowCount(showedChains.count());
    for (int i = 0; i < int(showedChains.size()); i++) {
        table->setCellWidget(i, 0, new TableCellWidget(showedChains[i], i));
        table->setRowHeight(i, 60);
    }
};

void ChainTableView::resizeTable() {
    double width = header->width();
    header->setColumnWidth(0, std::round(width * 0.04));
    header->setColumnWidth(1, std::round(width * 0.34));
    header->setColumnWidth(2, std::round(width * 0.18));
    header->setColumnWidth(3, std::round(width * 0.34));
    header->setColumnWidth(4, std::round(width * 0.075));
    table->setColumnWidth(0, table->width());
}

void ChainTableView::resizeEvent(QResizeEvent *event) {
    resizeTable();
    QWidget::resizeEvent(event);
}
