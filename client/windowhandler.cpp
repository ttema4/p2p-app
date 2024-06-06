#include "windowhandler.h"
#include "ui_windowhandler.h"
#include "user.h"
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QException>


WindowHandler::WindowHandler(QWidget *parent) : QMainWindow(parent), ui(new Ui::WindowHandler) {
    ui->setupUi(this);
    ui->label->setStyleSheet("image: url(:/resourses/icons/app-logo-3.png);");
    QTimer::singleShot(100, this, &WindowHandler::init);
}

void WindowHandler::init() {
    QFile configFile;
    configFile.setFileName(":/config.json");

    if (!configFile.open(QIODevice::ReadOnly)) {
        ui->label_2->setText("Парсинг config.json...\tОшибка!");
        qWarning() << "Failed to open config file";
        return;
    }

    QTextStream in(&configFile);
    QString configData = in.readAll();
    configFile.close();

    try {
        nlohmann::json config = nlohmann::json::parse(configData.toStdString());
        GlobalCondition::getInstance().setFromJson(config);
    } catch (...) {
        ui->label_2->setText("Парсинг config.json...\tОшибка!");
        qWarning() << "Failed to parsing config file";
        return;
    }
    ui->label_2->setText("Парсинг config.json...\tУспех!\nПодключение к серверу...");
    qInfo() << "Successfully parsed config.json";
    QTimer::singleShot(100, this, &WindowHandler::init2);
}

void WindowHandler::init2() {
    if (!DataReciever::getInstance().init()) {
        ui->label_2->setText("Парсинг config.json...\tУспех!\nПодключение к серверу...\tОшибка!");
        qWarning() << "Error while connecting to server";
        return;
    }
    ui->label_2->setText("Парсинг config.json...\tУспех!\nПодключение к серверу...\tУспех!\nПодключение к БД...");
    QTimer::singleShot(100, this, &WindowHandler::init3);
}

void WindowHandler::init3() {
    if (!CurUser::getInstance().init()) {
        ui->label_2->setText("Парсинг config.json...\tУспех!\nПодключение к серверу...\tУспех!\nПодключение к БД...\tОшибка!");
        qWarning() << "Error while connecting to dataBase";
        return;
    }
    ui->label_2->setText("Парсинг config.json...\tУспех!\nПодключение к серверу...\tУспех!\nПодключение к БД...\tУспех!");
    QTimer::singleShot(100, this, &WindowHandler::init4);
}

void WindowHandler::init4() {
    currentpage = this;
    homepage = new MainWindow();
    mypage = new MyPage();
    registerpage = new RegisterPage();
    loginpage = new LoginPage();
    notifypage = new NotifyPage();
    favouritepage = new FavouritePage();
    settingspage = new SettingsPage();

    connect(homepage, &MainWindow::myPage, this, &WindowHandler::open_mypage);
    connect(homepage, &MainWindow::loginPage, this, &WindowHandler::open_loginpage);
    connect(homepage, &MainWindow::registerPage, this, &WindowHandler::open_registerpage);
    connect(homepage, &MainWindow::notifyPage, this, &WindowHandler::open_notifypage);
    connect(homepage, &MainWindow::favouritePage, this, &WindowHandler::open_favouritepage);
    connect(homepage, &MainWindow::settingsPage, this, &WindowHandler::open_settingspage);

    connect(mypage, &MyPage::homePage, this, &WindowHandler::open_homepage);
    connect(mypage, &MyPage::notifyPage, this, &WindowHandler::open_notifypage);
    connect(mypage, &MyPage::favouritePage, this, &WindowHandler::open_favouritepage);
    connect(mypage, &MyPage::settingsPage, this, &WindowHandler::open_settingspage);

    connect(loginpage, &LoginPage::homePage, this, &WindowHandler::open_homepage);
    connect(loginpage, &LoginPage::myPage, this, &WindowHandler::open_mypage);
    connect(loginpage, &LoginPage::registerPage, this, &WindowHandler::open_registerpage);
    connect(loginpage, &LoginPage::notifyPage, this, &WindowHandler::open_notifypage);
    connect(loginpage, &LoginPage::favouritePage, this, &WindowHandler::open_favouritepage);
    connect(loginpage, &LoginPage::settingsPage, this, &WindowHandler::open_settingspage);

    connect(registerpage, &RegisterPage::homePage, this, &WindowHandler::open_homepage);
    connect(registerpage, &RegisterPage::myPage, this, &WindowHandler::open_mypage);
    connect(registerpage, &RegisterPage::loginPage, this, &WindowHandler::open_loginpage);
    connect(registerpage, &RegisterPage::notifyPage, this, &WindowHandler::open_notifypage);
    connect(registerpage, &RegisterPage::favouritePage, this, &WindowHandler::open_favouritepage);
    connect(registerpage, &RegisterPage::settingsPage, this, &WindowHandler::open_settingspage);

    connect(notifypage, &NotifyPage::homePage, this, &WindowHandler::open_homepage);
    connect(notifypage, &NotifyPage::myPage, this, &WindowHandler::open_mypage);
    connect(notifypage, &NotifyPage::loginPage, this, &WindowHandler::open_loginpage);
    connect(notifypage, &NotifyPage::registerPage, this, &WindowHandler::open_registerpage);
    connect(notifypage, &NotifyPage::favouritePage, this, &WindowHandler::open_favouritepage);
    connect(notifypage, &NotifyPage::settingsPage, this, &WindowHandler::open_settingspage);

    connect(favouritepage, &FavouritePage::homePage, this, &WindowHandler::open_homepage);
    connect(favouritepage, &FavouritePage::myPage, this, &WindowHandler::open_mypage);
    connect(favouritepage, &FavouritePage::loginPage, this, &WindowHandler::open_loginpage);
    connect(favouritepage, &FavouritePage::registerPage, this, &WindowHandler::open_registerpage);
    connect(favouritepage, &FavouritePage::notifyPage, this, &WindowHandler::open_notifypage);
    connect(favouritepage, &FavouritePage::settingsPage, this, &WindowHandler::open_settingspage);

    connect(settingspage, &SettingsPage::homePage, this, &WindowHandler::open_homepage);
    connect(settingspage, &SettingsPage::myPage, this, &WindowHandler::open_mypage);
    connect(settingspage, &SettingsPage::loginPage, this, &WindowHandler::open_loginpage);
    connect(settingspage, &SettingsPage::registerPage, this, &WindowHandler::open_registerpage);
    connect(settingspage, &SettingsPage::notifyPage, this, &WindowHandler::open_notifypage);
    connect(settingspage, &SettingsPage::favouritePage, this, &WindowHandler::open_favouritepage);

    QTimer::singleShot(1000, this, &WindowHandler::open_homepage);
}


void WindowHandler::windowChanger(QMainWindow *toOpen) {
    toOpen->show();
    toOpen->setGeometry(currentpage->geometry());
    currentpage->hide();
    currentpage = toOpen;
}

void WindowHandler::open_homepage() {
    windowChanger(homepage);
}

void WindowHandler::open_mypage() {
    windowChanger(mypage);
}

void WindowHandler::open_registerpage() {
    windowChanger(registerpage);
}

void WindowHandler::open_loginpage() {
    windowChanger(loginpage);
}

void WindowHandler::open_notifypage() {
    windowChanger(notifypage);
}

void WindowHandler::open_favouritepage() {
    windowChanger(favouritepage);
}

void WindowHandler::open_settingspage() {
    windowChanger(settingspage);
}

WindowHandler::~WindowHandler() {
    delete ui;
}
