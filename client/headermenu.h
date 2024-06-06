#ifndef HEADERMENU_H
#define HEADERMENU_H

#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QShowEvent>
#include <QWidget>
#include "textpixmapbutton.h"
#include "user.h"

class HeaderMenu : public QWidget {
    Q_OBJECT
public:
    explicit HeaderMenu(QString center_text_, QWidget *parent = nullptr);

    void showMenu();
    void hideMenu();

    void resizeMenu();

    bool isMenuVisible() {
        return menuVisible;
    }

    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void homePage();
    void myPage();
    void registerPage();
    void loginPage();
    void notifyPage();
    void favouritePage();
    void settingsPage();

private:
    void open_homePage();
    void open_myPage();
    void open_notifyPage();
    void open_favotitePage();
    void open_settingsPage();
    void open_exitPage();

    bool menuVisible = false;
    int menuHeight = 185;
    int menuWidth = 165;

    QLabel *center_text;
    QLabel *status_text;
    QPushButton *logo_button;
    QPushButton *menu_button;
    QFrame *menu_frame;
    TextPixmapButton *button1;

    QPropertyAnimation *anim;
};

#endif  // HEADERMENU_H
