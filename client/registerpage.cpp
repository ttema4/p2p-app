#include "registerpage.h"
#include "ui_registerpage.h"

RegisterPage::RegisterPage(QWidget *parent) : QMainWindow(parent), ui(new Ui::RegisterPage) {
    ui->setupUi(this);
    menu = new HeaderMenu("Регистрация", ui->widget->parentWidget());
    ui->widget->parentWidget()->layout()->replaceWidget(ui->widget, menu);

    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->pushButton_3->setIcon(QIcon("://resourses/icons/eye-icon-open.png"));
    ui->pushButton_3->setCursor(Qt::PointingHandCursor);
    ui->pushButton_3->setStyleSheet("border:none;padding:0px;");
    ui->pushButton_3->setFixedSize(20, 20);

    ui->groupBox->layout()->setContentsMargins(20, 15, 20, 15);
    ui->groupBox_2->layout()->setContentsMargins(20, 15, 20, 15);
    ui->groupBox_3->layout()->setContentsMargins(20, 15, 0, 15);
    ui->groupBox_3->layout()->setSpacing(0);

    connect(ui->pushButton_3, &QPushButton::clicked, this, [this]() {
        if (ui->lineEdit_3->echoMode() == QLineEdit::Password) {
            ui->lineEdit_3->setEchoMode(QLineEdit::Normal);
            ui->pushButton_3->setIcon(QIcon("://resourses/icons/eye-icon-close.png"));
        } else {
            ui->lineEdit_3->setEchoMode(QLineEdit::Password);
            ui->pushButton_3->setIcon(QIcon("://resourses/icons/eye-icon-open.png"));
        }
    });

    ui->frame->layout()->setSpacing(0);

    connect(menu, &HeaderMenu::homePage, this, &RegisterPage::homePage);
    connect(menu, &HeaderMenu::myPage, this, &RegisterPage::myPage);
    connect(menu, &HeaderMenu::loginPage, this, &RegisterPage::loginPage);
    connect(menu, &HeaderMenu::notifyPage, this, &RegisterPage::notifyPage);
    connect(menu, &HeaderMenu::favouritePage, this, &RegisterPage::favouritePage);
    connect(menu, &HeaderMenu::settingsPage, this, &RegisterPage::settingsPage);


    connect(ui->pushButton_2, &QPushButton::clicked, this, &RegisterPage::loginPage);
    connect(ui->pushButton, &QPushButton::clicked, this, &RegisterPage::tryRegister);

    installEventFilter(this);
    menu->installEventFilter(this);
    ui->pushButton->installEventFilter(this);
    ui->pushButton_2->installEventFilter(this);
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit_3->installEventFilter(this);
}


void RegisterPage::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (focusWidget() == ui->lineEdit) {
            ui->lineEdit_2->setFocus();
        } else if (focusWidget() == ui->lineEdit_2) {
            ui->lineEdit_3->setFocus();
        } else if (focusWidget() == ui->lineEdit_3) {
            tryRegister();
        } else {
            QMainWindow::keyPressEvent(event);
        }
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

bool RegisterPage::eventFilter(QObject *obj, QEvent *event) {
    if (!menu->isMenuVisible()) return false;

    if (event->type() == QEvent::MouseButtonPress) {
        menu->showMenu();
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void RegisterPage::tryRegister() {
    if (AccountHandler::getInstance().tryRegister(ui->lineEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text())) {
        ui->lineEdit->setText("");
        ui->lineEdit_2->setText("");
        ui->lineEdit_3->setText("");
        emit RegisterPage::homePage();
    }
};

void RegisterPage::showEvent(QShowEvent *event) {
    ui->lineEdit->setFocus();
    QMainWindow::showEvent(event);
}

RegisterPage::~RegisterPage() {
    delete ui;
}
