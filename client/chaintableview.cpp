#include "chaintableview.h"
#include "QtWidgets/qheaderview.h"
#include <QLabel>
#include <QSpacerItem>
#include <QScrollBar>
#include <QSizePolicy>
#include <QPixmap>
#include <QIcon>
#include <QMap>
#include <QFont>

#include <algorithm>


TableChangeWidget::TableChangeWidget(std::pair<std::string, std::string>& change, QWidget *parent) : QLabel(parent) {
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
    label->setText(QString::number(spread) + "%");
    label->setFixedSize(50, 27);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    if (spread > 0) {
        label->setStyleSheet("font-size: 15px;"
                             "background-color: rgba(0, 255, 0, 30); "
                             "border-radius: 4px; ");
    } else if (spread == 0) {
        label->setStyleSheet("font-size: 15px;"
                             "background-color: rgba(128, 128, 128, 30); "
                             "border-radius: 4px; ");
    } else {
        label->setStyleSheet("font-size: 15px;"
                             "background-color: rgba(255, 0, 0, 30); "
                             "border-radius: 4px; ");
    }
}

TableIndexWidget::TableIndexWidget(int ind, QWidget *parent) : QLabel(parent) {
    setText(QString::number(ind));
    setStyleSheet("font-size: 15px; ");
    setAlignment(Qt::AlignCenter);
}


TableOrderWidget::TableOrderWidget(Order& ord, QWidget *parent) : QWidget(parent) {
    static QMap<std::string, QString> includeMap {
        {"Tinkoff", "://resourses/bank_icons/Tink_bank.svg"},
        {"Sber", "://resourses/bank_icons/Sber_bank.svg"},
        {"Alpha", "://resourses/bank_icons/Alpha_bank.svg"},
        {"Qiwi", "://resourses/bank_icons/QIWI_bank.svg"},
        {"SBP", "://resourses/bank_icons/SBP_bank.svg"}
    };

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 2, 0, 2);
    layout->setVerticalSpacing(2);


    QFrame *verticalLine = new QFrame();
    verticalLine->setFrameStyle(QFrame::VLine);
    verticalLine->setStyleSheet("QFrame { color: darkgrey; }");
    layout->addWidget(verticalLine, 0, 0, 2, 1);

    QLabel *label_1 = new QLabel();
    label_1->setFixedSize(25, 25);
    label_1->setAlignment(Qt::AlignCenter);

    QIcon icon(includeMap[ord.bank]);
    QPixmap pixmap = icon.pixmap(QSize(17, 17));
    label_1->setPixmap(pixmap);

    label_1->setStyleSheet(
        "background-color: rgba(128, 128, 128, 30); "
        "border-radius: 4px; "
        );
    layout->addWidget(label_1, 0, 1, 2, 1);

    QLabel *label_2 = new QLabel(QString::fromStdString(ord.coin2));
    label_2->setMinimumSize(0, 25);
    label_2->setStyleSheet("font-size: 18px;");
    label_2->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
    layout->addWidget(label_2, 0, 2);

    QLabel *label_3 = new QLabel(QString::fromStdString(ord.market) + ", " + QString::fromStdString(ord.bank));
    label_3->setMinimumSize(0, 25);
    label_3->setStyleSheet("font-size: 10px; font-weight: light;");
    label_3->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    layout->addWidget(label_3, 1, 2);

    QSpacerItem *horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Preferred);
    layout->addItem(horizontalSpacer, 0, 3, 2, 1);

    QLabel *label_4 = new QLabel(QString::number(ord.exchange_rate, 'f', 2));
    label_4->setMinimumSize(0, 25);
    label_4->setStyleSheet("font-size: 18px;");
    label_4->setAlignment(Qt::AlignRight|Qt::AlignBottom);
    layout->addWidget(label_4, 0, 4);

    QLabel *label_5 = new QLabel(QString::fromStdString(ord.coin1) + "/1 " + QString::fromStdString(ord.coin2));
    label_5->setMinimumSize(0, 25);
    label_5->setStyleSheet("font-size: 10px; font-weight: light;");
    label_5->setAlignment(Qt::AlignRight|Qt::AlignTop);
    layout->addWidget(label_5, 1, 4);

    QFrame *verticalLine2 = new QFrame();
    verticalLine2->setFrameStyle(QFrame::VLine);
    verticalLine2->setStyleSheet("QFrame { color: darkgrey; }");
    layout->addWidget(verticalLine2, 0, 5, 2, 1);

};

ChainTableView::ChainTableView(QWidget *parent) : QWidget{parent} {
    sortType = SortType::none;

    QFont font;
    font.setPointSize(12);
    font.setBold(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 0, 5, 5);
    layout->setSpacing(0);

    header = new QTableWidget();
    header->verticalHeader()->hide();
    header->horizontalHeader()->hide();
    header->setRowCount(1);
    header->setColumnCount(5);

    QTableWidgetItem* header0 = new QTableWidgetItem();
    header0->setIcon(QIcon("://resourses/icons/sort.png"));
    header0->setTextAlignment(Qt::AlignCenter);
    header->setItem(0, 0, header0);

    QTableWidgetItem* header1 = new QTableWidgetItem(QString("Покупка на P2P"));
    header1->setTextAlignment(Qt::AlignCenter);
    header1->setFont(font);
    header->setItem(0, 1, header1);

    QTableWidgetItem* header2 = new QTableWidgetItem(QString("Обмен на споте"));
    header2->setTextAlignment(Qt::AlignCenter);
    header2->setFont(font);
    header->setItem(0, 2, header2);

    QTableWidgetItem* header3 = new QTableWidgetItem(QString("Продажа на P2P"));
    header3->setTextAlignment(Qt::AlignCenter);
    header3->setFont(font);
    header->setItem(0, 3, header3);

    QTableWidgetItem* header4 = new QTableWidgetItem(QString("Спред"));
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
    table->setRowCount(30);
    table->setColumnCount(5);

    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->horizontalScrollBar()->setDisabled(true);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(table);

    header->setShowGrid(false);
    header->setStyleSheet("QTableWidget{background-color: #ececec; border: none;} QTableWidget::item { background-color: #ececec; }");
    table->setShowGrid(false);
    table->setStyleSheet("QTableWidget{background-color: #ececec; border: none;} QTableWidget::item { background-color: #fefefe; margin-top: 5px;}");

    connect(header, &QTableWidget::cellClicked, this, &ChainTableView::onHeaderCellClicked);
    connect(table, &QTableWidget::cellClicked, this, &ChainTableView::onTableCellClicked);


}

void ChainTableView::onTableCellClicked(int row, int col) {
    emit cellClicked(chains[row]);
}

void ChainTableView::onHeaderCellClicked(int row, int col) {
    if (row == 0 && col == 0) {
        switch(sortType) {
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
        setData(chains);
    }
}

void ChainTableView::setData(QVector<Chain> &data) {
    if (&chains != &data) chains = std::move(data);

    switch(sortType) {
    case SortType::up:
        std::sort(chains.begin(), chains.end(), [](Chain a, Chain b) {
            return a.spread > b.spread;
        });
        break;
    case SortType::down:
        std::sort(chains.begin(), chains.end(), [](Chain a, Chain b) {
            return a.spread < b.spread;
        });
        break;
    case SortType::none:
        break;
    }

    table->setRowCount(chains.count());
    for (int i = 0; i < int(chains.size()); i++) {
        table->setCellWidget(i, 0, new TableIndexWidget(i + 1));
        table->setCellWidget(i, 1, new TableOrderWidget(chains[i].buy));
        table->setCellWidget(i, 2, new TableChangeWidget(chains[i].change));
        table->setCellWidget(i, 3, new TableOrderWidget(chains[i].sell));
        table->setCellWidget(i, 4, new TableSpreadWidget(chains[i].spread));
        table->setRowHeight(i, 65);
    }
};

void ChainTableView::resizeTable(QTableWidget *table) {
    double width = table->width();
    table->setColumnWidth(0, std::round(width * 0.04));
    table->setColumnWidth(1, std::round(width * 0.34));
    table->setColumnWidth(2, std::round(width * 0.18));
    table->setColumnWidth(3, std::round(width * 0.34));
    table->setColumnWidth(4, std::round(width * 0.10));
}

void ChainTableView::resizeTable() {
    resizeTable(header);
    resizeTable(table);
}

void ChainTableView::resizeEvent(QResizeEvent *event) {
    resizeTable();
    QWidget::resizeEvent(event);
}
