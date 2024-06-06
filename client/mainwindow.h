#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QDebug>
#include <QFocusEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QPair>
#include <QParallelAnimationGroup>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QResizeEvent>
#include <QSet>
#include <QStyle>
#include <QStyleOptionButton>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QValidator>
#include <QVector>
#include <QWidget>
#include <limits>
#include "chainmonitor.h"
#include "chaintableview.h"
#include "datareciever.h"
#include "favouritepage.h"
#include "headermenu.h"
#include "logic_fwd.h"
#include "loginpage.h"
#include "mypage.h"
#include "notifypage.h"
#include "registerpage.h"
#include "settingspage.h"
#include "user.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void resizeTable();
    ~MainWindow();

signals:
    void myPage();
    void registerPage();
    void loginPage();
    void notifyPage();
    void favouritePage();
    void settingsPage();

protected:
    void resizeEvent(QResizeEvent *e);
    bool eventFilter(QObject *target, QEvent *event);

private:
    void updateTable(QVector<Chain> chains);

    void windowChanger(QMainWindow *toOpen);

    void open_mypage();
    void open_homepage();
    void open_loginpage();
    void open_registerpage();
    void open_notifypage();
    void open_favouritepage();
    void open_settingspage();

    void showFilters();
    void filterHidden();
    void applyFilters();
    void resetFilters();

    void onCellClicked(Chain &chain);
    void chainMonitorHide();

    HeaderMenu *menu;

    MyPage *mypage;
    LoginPage *loginpage;
    RegisterPage *registerpage;
    NotifyPage *notifypage;
    FavouritePage *favouritepage;
    SettingsPage *settingspage;
    ChainMonitor *chainmonitor;
    QMainWindow *currentpage;

    QParallelAnimationGroup *sizeAnim;

    QTimer *timer;
    QTime *time;

    ChainTableView *chainTable;
    bool chainMonitorOpen;
    Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
