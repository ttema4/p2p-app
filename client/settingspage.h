#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QMainWindow>
#include <QPushButton>
#include <QShowEvent>
#include "headermenu.h"
#include "user.h"

namespace Ui {
class SettingsPage;
}

class SettingsPage : public QMainWindow {
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
    ~SettingsPage();

protected:
    void showEvent(QShowEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void homePage();
    void myPage();
    void registerPage();
    void loginPage();
    void notifyPage();
    void favouritePage();

private:
    void editName();
    void editPassword();

    HeaderMenu *menu;
    Ui::SettingsPage *ui;
};

#endif  // SETTINGSPAGE_H
