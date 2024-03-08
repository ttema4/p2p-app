#ifndef FAVOURITEPAGE_H
#define FAVOURITEPAGE_H

#include <QMainWindow>
#include "headermenu.h"

namespace Ui {
class FavouritePage;
}

class FavouritePage : public QMainWindow
{
    Q_OBJECT

public:
    explicit FavouritePage(QWidget *parent = nullptr);
    ~FavouritePage();
signals:
    void homePage();
    void myPage();
    void registerPage();
    void loginPage();
    void notifyPage();
    void settingsPage();

private:
    HeaderMenu *menu;
    Ui::FavouritePage *ui;
};

#endif // FAVOURITEPAGE_H
