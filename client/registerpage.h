#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QMainWindow>
#include <QMessageBox>
#include <QShowEvent>
#include "headermenu.h"

namespace Ui {
class RegisterPage;
}

class RegisterPage : public QMainWindow {
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr);
    ~RegisterPage();

protected:
    void showEvent(QShowEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void myPage();
    void homePage();
    void loginPage();
    void notifyPage();
    void favouritePage();
    void settingsPage();

private:
    void tryRegister();
    void changeFocus();

    HeaderMenu *menu;

    Ui::RegisterPage *ui;
};

#endif  // REGISTERPAGE_H
