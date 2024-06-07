#include "loginpage.h"
#include "ui_loginpage.h"

LoginPage::LoginPage(QWidget *parent) : QMainWindow(parent), ui(new Ui::LoginPage) {
    ui->setupUi(this);
    menu = new HeaderMenu("Вход", ui->widget->parentWidget());
    ui->widget->parentWidget()->layout()->replaceWidget(ui->widget, menu);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->pushButton_3->setIcon(QIcon("://resourses/icons/eye-icon-open.png"));
    ui->pushButton_3->setCursor(Qt::PointingHandCursor);
    ui->pushButton_3->setStyleSheet("border:none;padding:0px;");
    ui->pushButton_3->setFixedSize(20, 20);

    ui->groupBox->layout()->setContentsMargins(20, 15, 20, 15);
    ui->groupBox_2->layout()->setContentsMargins(20, 15, 0, 15);
    ui->groupBox_2->layout()->setSpacing(0);

    connect(ui->pushButton_3, &QPushButton::clicked, this, [this]() {
        if (ui->lineEdit_2->echoMode() == QLineEdit::Password) {
            ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
            ui->pushButton_3->setIcon(QIcon("://resourses/icons/eye-icon-close.png"));
        } else {
            ui->lineEdit_2->setEchoMode(QLineEdit::Password);
            ui->pushButton_3->setIcon(QIcon("://resourses/icons/eye-icon-open.png"));
        }
    });

    ui->frame->layout()->setSpacing(0);

    connect(menu, &HeaderMenu::homePage, this, &LoginPage::homePage);
    connect(menu, &HeaderMenu::myPage, this, &LoginPage::myPage);
    connect(menu, &HeaderMenu::notifyPage, this, &LoginPage::notifyPage);
    connect(menu, &HeaderMenu::favouritePage, this, &LoginPage::favouritePage);
    connect(menu, &HeaderMenu::settingsPage, this, &LoginPage::settingsPage);

    connect(ui->pushButton_2, &QPushButton::clicked, this, &LoginPage::registerPage);
    connect(ui->pushButton, &QPushButton::clicked, this, &LoginPage::tryLogin);

    installEventFilter(this);
    menu->installEventFilter(this);
    ui->pushButton->installEventFilter(this);
    ui->pushButton_2->installEventFilter(this);
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
}

void LoginPage::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (focusWidget() == ui->lineEdit) {
            ui->lineEdit_2->setFocus();
        } else if (focusWidget() == ui->lineEdit_2) {
            tryLogin();
        } else {
            QMainWindow::keyPressEvent(event);
        }
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

bool LoginPage::eventFilter(QObject *obj, QEvent *event) {
    if (!menu->isMenuVisible()) {
        return false;
    }

    if (event->type() == QEvent::MouseButtonPress) {
        menu->showMenu();
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void LoginPage::tryLogin() {
    if (CurUser::getInstance().tryLogin(ui->lineEdit->text(), ui->lineEdit_2->text())) {
        ui->lineEdit->setText("");
        ui->lineEdit_2->setText("");
        emit LoginPage::homePage();
    } else {
        ui->lineEdit->setFocus();
        ui->lineEdit->selectAll();
        ui->lineEdit_2->setText("");

        QMessageBox *m =
            new QMessageBox(QMessageBox::NoIcon, "", "", QMessageBox::Ok | QMessageBox::Default, this, Qt::Sheet);
        m->setText("Ошибка");
        m->setInformativeText("Пользователь не найден");
        QPixmap exportSuccess("://resourses/icons/pepe.png");
        exportSuccess = exportSuccess.scaled(QSize(60, 60), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m->setIconPixmap(exportSuccess);
        m->exec();
    }
};

void LoginPage::showEvent(QShowEvent *event) {
    ui->lineEdit->setFocus();
    QMainWindow::showEvent(event);
}

LoginPage::~LoginPage() {
    delete ui;
}
