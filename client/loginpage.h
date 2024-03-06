#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QMainWindow>
#include "headermenu.h"

namespace Ui {
class LoginPage;
}

class LoginPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

protected:
    // void resizeEvent(QResizeEvent *e);

signals:
    void myPage();
    void homePage();
    void registerPage();

private:
    HeaderMenu* menu;

    Ui::LoginPage *ui;
};

#endif // LOGINPAGE_H
