#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QShowEvent>
#include <QVBoxLayout>
#include "headermenu.h"

namespace Ui {
class LoginPage;
}

class LoginPage : public QMainWindow {
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *e) override;
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

    HeaderMenu *menu;

    Ui::LoginPage *ui;
};

#endif  // LOGINPAGE_H
