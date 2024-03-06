#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QMainWindow>
#include "headermenu.h"

namespace Ui {
class RegisterPage;
}

class RegisterPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr);
    ~RegisterPage();

protected:
    // void resizeEvent(QResizeEvent *e);

signals:
    void myPage();
    void homePage();
    void loginPage();

private:
    HeaderMenu* menu;

    Ui::RegisterPage *ui;
};

#endif // REGISTERPAGE_H
