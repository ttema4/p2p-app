#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mypage.h"
#include "headermenu.h"
#include "loginpage.h"
#include "registerpage.h"
#include "notifypage.h"
#include "favouritepage.h"
#include "settingspage.h"
#include <QMainWindow>
#include <QFrame>
#include <QPropertyAnimation>
#include <QFocusEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionButton>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *e);
    bool eventFilter(QObject *target, QEvent *event);

private:
    void resizeTable();

    void windowChanger(QMainWindow *toOpen);

    void open_mypage();
    void open_homepage();
    void open_loginpage();
    void open_registerpage();
    void open_notifypage();
    void open_favouritepage();
    void open_settingspage();

    void hideFilters();
    void showFilters();

    HeaderMenu* menu;

    MyPage *mypage;
    LoginPage *loginpage;
    RegisterPage *registerpage;
    NotifyPage *notifypage;
    FavouritePage *favouritepage;
    SettingsPage *settingspage;

    bool filtersVisible = true;
    QParallelAnimationGroup *sizeAnim;
    QMainWindow *currentpage;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
