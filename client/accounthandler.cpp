#include "accounthandler.h"
#include <QDebug>

AccountHandler::AccountHandler() {
    users.append(User("Artem", "ibartion", "1234"));
    users.last().setAvatar(QPixmap(":/resourses/icons/photo_2024-03-06.jpeg"));
}

AccountHandler& AccountHandler::getInstance() {
    static AccountHandler instance;
    return instance;
}

bool AccountHandler::tryRegister(QString name_, QString login_, QString password_) {
    users.append(User(name_, login_, password_));
    return CurUser::getInstance().setCurUser(users.last());
}

bool AccountHandler::tryLogin(QString login_, QString password_) {
    for (User u : users) {
        if (login_ == u.getLogin() && password_ == u.getPassword()) {
            return CurUser::getInstance().setCurUser(u);
        }
    }
    return false;
};
