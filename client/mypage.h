#ifndef MYPAGE_H
#define MYPAGE_H

#include <QMainWindow>
#include <QFrame>
#include <QPropertyAnimation>

namespace Ui {
class MyPage;
}

class MyPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyPage(QWidget *parent = nullptr);
    ~MyPage();

    void resizeMenu();

protected:
    void resizeEvent(QResizeEvent *e);

signals:
    void firstWindow(); // Сигнал для первого окна на открытие

private:
    void logo_clicked();

    void showMenu();
    void hideMenu();

    int menuHeight;
    bool menuVisible;
    QPropertyAnimation *anim;
    QFrame *overlay_frame;
    Ui::MyPage *ui;
};

#endif // MYPAGE_H
