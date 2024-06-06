#include "settingspage.h"
#include "ui_settingspage.h"

SettingsPage::SettingsPage(QWidget *parent) : QMainWindow(parent), ui(new Ui::SettingsPage) {
    ui->setupUi(this);
    menu = new HeaderMenu("Настройки", ui->widget->parentWidget());
    ui->widget->parentWidget()->layout()->replaceWidget(ui->widget, menu);

    ui->label->setText("Log file directory: " + QString(LOG_DIR) + "/application.log");

    connect(menu, &HeaderMenu::homePage, this, &SettingsPage::homePage);
    connect(menu, &HeaderMenu::loginPage, this, &SettingsPage::loginPage);
    connect(menu, &HeaderMenu::myPage, this, &SettingsPage::myPage);
    connect(menu, &HeaderMenu::notifyPage, this, &SettingsPage::notifyPage);
    connect(menu, &HeaderMenu::favouritePage, this, &SettingsPage::favouritePage);

    connect(ui->pushButton_2, &QPushButton::clicked, this, &SettingsPage::editName);
    connect(ui->pushButton, &QPushButton::clicked, this, &SettingsPage::editPassword);
}

void SettingsPage::editName() {
    bool edited = CurUser::getInstance().tryEditName(ui->lineEdit_2->text());
    if (edited) {
        ui->lineEdit_2->setText("");
        menu->showEvent(new QShowEvent);
    } else {
        ui->lineEdit_2->setFocus();
        ui->lineEdit_2->selectAll();
    }
};

void SettingsPage::editPassword() {
    bool edited = CurUser::getInstance().tryEditPassword(ui->lineEdit->text());
    if (edited) {
        ui->lineEdit->setText("");
    } else {
        ui->lineEdit->setFocus();
        ui->lineEdit->selectAll();
    }
};

void SettingsPage::showEvent(QShowEvent *event) {
    if (CurUser::getInstance().getId() != -1) {
        ui->label_2->setText("Вы вошли под аккаунтом: " + CurUser::getInstance().getLogin());
        ui->groupBox->setEnabled(true);
        ui->groupBox_2->setEnabled(true);
    } else {
        ui->label_2->setText("Войдите или зарегистрируйтесь!");
        ui->groupBox->setEnabled(false);
        ui->groupBox_2->setEnabled(false);
    }

    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
}

void SettingsPage::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (focusWidget() == ui->lineEdit) {
            editPassword();
        } else if (focusWidget() == ui->lineEdit_2) {
            editName();
        }
    }
    QMainWindow::keyPressEvent(event);
}

SettingsPage::~SettingsPage() {
    delete ui;
}
