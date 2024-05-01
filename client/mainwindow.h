#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mypage.h"
#include "headermenu.h"
#include "loginpage.h"
#include "registerpage.h"
#include "notifypage.h"
#include "favouritepage.h"
#include "settingspage.h"
#include "datareciever.h"
#include "logic_fwd.h"
#include "chainmonitor.h"

#include <QMainWindow>
#include <QFrame>
#include <QPropertyAnimation>
#include <QVector>
#include <QFocusEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionButton>
#include <QTimer>
#include <QTime>


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

    void onCellClicked(int row, int column);
    void chainMonitorHide();


    HeaderMenu* menu;

    MyPage *mypage;
    LoginPage *loginpage;
    RegisterPage *registerpage;
    NotifyPage *notifypage;
    FavouritePage *favouritepage;
    SettingsPage *settingspage;
    ChainMonitor *chainmonitor;
    QMainWindow *currentpage;

    QParallelAnimationGroup *sizeAnim;

    QVector<Chain> chains;
    QTimer *timer;
    QTime *time;

    bool chainMonitorOpen;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
