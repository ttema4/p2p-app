#ifndef CHAINMONITOR_H
#define CHAINMONITOR_H

#include "logic_fwd.h"
#include "user.h"

#include <QWidget>
#include <QDialog>
#include <QMouseEvent>
#include <QString>

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
    void monitorClosed();

protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void favouriteSwap();

    bool isFavorite;
    QString chainHash;

    Ui::ChainMonitor *ui;
};

#endif // CHAINMONITOR_H
