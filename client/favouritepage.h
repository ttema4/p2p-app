#ifndef FAVOURITEPAGE_H
#define FAVOURITEPAGE_H

#include <QMainWindow>
#include "headermenu.h"
#include "chaintableview.h"
#include "chainmonitor.h"
#include <QEvent>

namespace Ui {
class FavouritePage;
}

class FavouritePage : public QMainWindow
{
    Q_OBJECT

public:
    explicit FavouritePage(QWidget *parent = nullptr);
    ~FavouritePage();

signals:
    void homePage();
    void myPage();
    void registerPage();
    void loginPage();
    void notifyPage();
    void settingsPage();

protected:
    bool eventFilter(QObject *target, QEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    bool chainMonitorOpen;

    void onCellClicked(Chain &chain);
    void chainMonitorHide();
    void updateTable();
    void clearFavourites();

    ChainTableView *chainTable;
    ChainMonitor *chainmonitor;

    HeaderMenu *menu;
    Ui::FavouritePage *ui;
};

#endif // FAVOURITEPAGE_H
