#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QMainWindow>
#include "headermenu.h"

namespace Ui {
class SettingsPage;
}

class SettingsPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
    ~SettingsPage();

signals:
    void homePage();
    void myPage();
    void registerPage();
    void loginPage();
    void notifyPage();
    void favouritePage();

private:
    HeaderMenu *menu;
    Ui::SettingsPage *ui;
};

#endif // SETTINGSPAGE_H
