#include "favouritepage.h"
#include "ui_favouritepage.h"

FavouritePage::FavouritePage(QWidget *parent) : QMainWindow(parent), ui(new Ui::FavouritePage) {
    ui->setupUi(this);
    menu = new HeaderMenu("Избранное", ui->widget->parentWidget());
    ui->widget->parentWidget()->layout()->replaceWidget(ui->widget, menu);

    connect(menu, &HeaderMenu::homePage, this, &FavouritePage::homePage);
    connect(menu, &HeaderMenu::loginPage, this, &FavouritePage::loginPage);
    connect(menu, &HeaderMenu::myPage, this, &FavouritePage::myPage);
    connect(menu, &HeaderMenu::notifyPage, this, &FavouritePage::notifyPage);
    connect(menu, &HeaderMenu::settingsPage, this, &FavouritePage::settingsPage);
}

FavouritePage::~FavouritePage()
{
    delete ui;
}
