#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mypage.h"
#include "headermenu.h"
#include "loginpage.h"
#include "registerpage.h"
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

    void windowChanger(QMainWindow *toClose, QMainWindow *toOpen);

    void open_mypage();
    void mypage_homepage();
    void mypage_loginpage();
    void mypage_registerpage();
    void loginpage_homepage();
    void registerpage_homepage();
    void loginpage_registerpage();
    void loginpage_mypage();
    void registerpage_loginpage();
    void registerpage_mypage();

    HeaderMenu* menu;
    MyPage *mypage;
    LoginPage *loginpage;
    RegisterPage *registerpage;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
