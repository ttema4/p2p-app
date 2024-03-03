#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mypage.h"
#include <QMainWindow>
#include <QFrame>
#include <QPropertyAnimation>
#include <QFocusEvent>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *e);

private:
    void showMenu();
    void hideMenu();

    void resizeTable();
    void resizeMenu();

    void open_mypage();
    void open_homepage();

    int menuHeight;
    bool menuVisible;
    QPropertyAnimation *anim;
    QFrame *overlay_frame;
    Ui::MainWindow *ui;

    MyPage *mypage;
};
#endif // MAINWINDOW_H
