#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include <QMainWindow>
#include "datareciever.h"
#include "favouritepage.h"
#include "loginpage.h"
#include "mainwindow.h"
#include "mypage.h"
#include "notifypage.h"
#include "registerpage.h"
#include "settingspage.h"
#include "user.h"

namespace Ui {
class WindowHandler;
}

class WindowHandler : public QMainWindow {
    Q_OBJECT

public:
    explicit WindowHandler(QWidget *parent = nullptr);

    ~WindowHandler();

private:
    void init();
    void init2();
    void windowChanger(QMainWindow *toOpen);

    void open_mainwindow();
    void open_mypage();
    void open_homepage();
    void open_loginpage();
    void open_registerpage();
    void open_notifypage();
    void open_favouritepage();
    void open_settingspage();

    MainWindow *homepage;
    MyPage *mypage;
    LoginPage *loginpage;
    RegisterPage *registerpage;
    NotifyPage *notifypage;
    FavouritePage *favouritepage;
    SettingsPage *settingspage;
    QMainWindow *currentpage;

    Ui::WindowHandler *ui;
};

#endif  // WINDOWHANDLER_H
