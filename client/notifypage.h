#ifndef NOTIFYPAGE_H
#define NOTIFYPAGE_H

#include <QMainWindow>
#include "headermenu.h"

namespace Ui {
class NotifyPage;
}

class NotifyPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit NotifyPage(QWidget *parent = nullptr);
    ~NotifyPage();
signals:
    void homePage();
    void myPage();
    void registerPage();
    void loginPage();
    void favouritePage();
    void settingsPage();

private:
    HeaderMenu *menu;
    Ui::NotifyPage *ui;
};

#endif // NOTIFYPAGE_H
