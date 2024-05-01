#ifndef CHAINMONITOR_H
#define CHAINMONITOR_H

#include "logic_fwd.hpp"

#include <QWidget>
#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class ChainMonitor;
}

class ChainMonitor : public QWidget {
    Q_OBJECT

public:
    explicit ChainMonitor(QWidget *parent);
    explicit ChainMonitor(QWidget *parent, Chain c);
    void remove();
    ~ChainMonitor();
signals:
    void monitorSlosed();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void mousePressEvent(QMouseEvent* event) override;
    Ui::ChainMonitor *ui;
};

#endif // CHAINMONITOR_H
