#ifndef MYPAGE_H
#define MYPAGE_H

#include <QMainWindow>
#include "headermenu.h"

namespace Ui {
class MyPage;
}

class MyPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyPage(QWidget *parent = nullptr);
    ~MyPage();

protected:
    void resizeEvent(QResizeEvent *e);

signals:
    void homePage();
    void registerPage();
    void loginPage();

private:
    HeaderMenu* menu;
    Ui::MyPage *ui;
};

#endif // MYPAGE_H
