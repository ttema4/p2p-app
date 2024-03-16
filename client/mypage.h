#ifndef MYPAGE_H
#define MYPAGE_H

#include <QMainWindow>
#include "headermenu.h"
#include "user.h"

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
    void showEvent(QShowEvent *e);

signals:
    void homePage();
    void registerPage();
    void loginPage();
    void notifyPage();
    void favouritePage();
    void settingsPage();

private:
    void openFileAndSetPixmap();
    void exitAccount();

    void deleteAvatar();
    void deleteAccount();

    HeaderMenu* menu;
    Ui::MyPage *ui;
};

#endif // MYPAGE_H
