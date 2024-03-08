#ifndef ACCOUNTHANDLER_H
#define ACCOUNTHANDLER_H

#include "user.h"
#include <QVector>
#include <QString>
#include <QPixmap>

class AccountHandler {
public:
    static AccountHandler& getInstance();
    bool tryRegister(QString name_, QString login_, QString password_);
    bool tryLogin(QString login_, QString password_);

private:
    AccountHandler();
    AccountHandler(const AccountHandler&) = delete;
    AccountHandler& operator=(const AccountHandler&) = delete;

    QVector<User> users;
};

#endif // ACCOUNTHANDLER_H
