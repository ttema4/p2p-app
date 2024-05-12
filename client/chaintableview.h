#ifndef CHAINTABLEVIEW_H
#define CHAINTABLEVIEW_H


#include "logic_fwd.h"
#include <QObject>
#include <QWidget>
#include <QVector>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>

enum class SortType {
    none,
    up,
    down
};

class TableOrderWidget : public QWidget {
    Q_OBJECT
public:
    TableOrderWidget(Order& ord, QWidget *parent = nullptr);
};

class TableChangeWidget : public QLabel {
    Q_OBJECT
public:
    TableChangeWidget(std::pair<std::string, std::string>& change, QWidget *parent = nullptr);
};

class TableIndexWidget : public QLabel {
    Q_OBJECT
public:
    TableIndexWidget(int ind, QWidget *parent = nullptr);
};

class TableSpreadWidget : public QWidget {
    Q_OBJECT
public:
    TableSpreadWidget(double spread, QWidget *parent = nullptr);
};

class TableCellWidget : public QWidget {
    Q_OBJECT
public:
    TableCellWidget(Chain ch, int ind, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
};

class ChainTableView : public QWidget {
    Q_OBJECT
public:
    ChainTableView(QWidget *parent = nullptr);
    void setData(QVector<Chain> &data);
    void resizeTable();

signals:
    void cellClicked(Chain &chain);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void onTableCellClicked(int row, int col);
    void onHeaderCellClicked(int row, int col);

    QTableWidget *header;
    QTableWidget *table;
    QVector<Chain> chains;
    QVector<Chain> showedChains;

    SortType sortType;
};

#endif // CHAINTABLEVIEW_H
