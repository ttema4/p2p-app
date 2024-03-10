#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QMainWindow>
#include <QLineEdit>
#include <QShowEvent>
#include "headermenu.h"
#include "accounthandler.h"

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
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *e) override;
    // void hideEvent(QHideEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void myPage();
    void homePage();
    void registerPage();
    void notifyPage();
    void favouritePage();
    void settingsPage();

private:
    void tryLogin();

    HeaderMenu* menu;

    Ui::LoginPage *ui;
};

#endif // LOGINPAGE_H
