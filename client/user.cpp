#include "user.h"
#include <QException>

User::User() {
    id = -1;
}

User::User(QString name_, QString login_, QString password_) : name(name_), login(login_), password(password_) {
    avatar = avatarCorrection(QPixmap("://resourses/icons/empty_avatar.jpg"), 250);
    avatar2 = avatarCorrection(avatar, 30);
    static int id_counter = 1;
    id = id_counter++;
}


User::User(int id_, QString name_, QString login_, QString password_) : id(id_), name(name_), login(login_), password(password_) {};

bool User::setId(int id_) {
    id = id_;
    return true;
}

bool User::setName(QString name_) {
    name = name_;
    return true;
}

bool User::setLogin(QString login_) {
    login = login_;
    return true;
}

bool User::setPassword(QString password_) {
    password = password_;
    return true;
}

bool User::setAvatar(QPixmap avatar_) {
    avatar = avatarCorrection(avatar_, 250);
    avatar2 = avatarCorrection(avatar, 30);
    return true;
}

bool User::setFavourites(QVector<QString> favourites_) {
    favourites = favourites_;
    return true;
}

bool User::setNotifications(QVector<QString> notifications_) {
    notifications = notifications_;
    return true;
}

bool User::setRights(QMap<QString, bool> rights_) {
    rights = rights_;
    return true;
}

QPixmap User::avatarCorrection(QPixmap avatar_, int size) {
    if (avatar_.isNull()) return avatar_;
    if (avatar_.size() == QSize(size, size)) return avatar_;

    int squareSize = std::min(avatar_.height(), avatar_.width());
    int x = (avatar_.width() - squareSize) / 2;
    int y = (avatar_.height() - squareSize) / 2;
    QPixmap squarePixmap = avatar_.copy(x, y, squareSize, squareSize);
    QPixmap roundedPixmap(squarePixmap.size());
    roundedPixmap.fill(Qt::transparent);
    QPainter painter(&roundedPixmap);
    painter.setBrush(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawEllipse(roundedPixmap.rect());
    squarePixmap.setMask(roundedPixmap.mask());
    return squarePixmap.scaled(QSize(size, size), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
};



// class CurUser
CurUser& CurUser::getInstance() {
    static CurUser instance;
    return instance;
}

CurUser::CurUser() {
    default_avatar = QPixmap("://resourses/icons/empty_avatar.jpg");
    id = -1;
}

bool CurUser::init() {
    try {
        if (USE_DATABASE) {
            con = &DBAccountHandler::getInstance();
        } else {
            con = &LocalAccountHandler::getInstance();
        }
    } catch (QException e) {
        return false;
    }
    return true;
}

bool CurUser::tryRegister(QString name_, QString login_, QString password_) {
    std::optional<User> registered = con->tryRegister(name_, login_, password_);
    if (registered) {
        qDebug() << "SUCCESS REGISTER";
        setCurUser(*registered);
        return true;
    }
    return false;
};

bool CurUser::tryLogin(QString login_, QString password_) {
    std::optional<User> logged = con->tryLogin(login_, password_);
    if (logged) {
        qDebug() << "SUCCESSFULLY LOGGED IN WITH " << login_ << password_;
        setCurUser(*logged);
        return true;
    }
    return false;
};

bool CurUser::tryDeleteAccount() {
    bool deleted = con->tryDeleteAccount(login);
    if (deleted) {
        qDebug() << "SUCCESSFULLY DELETED USER " << login;
        unsetCurUser();
        return true;
    }
    return false;
}

bool CurUser::tryEditAvatar(QPixmap avatar_) {
    bool deleted = con->tryEditAvatar(login, avatar_);
    if (deleted) {
        qDebug() << "SUCCESSFULLY EDIT AVATAR " << login;
        setAvatar(avatar_);
        return true;
    }
    return false;
}

bool CurUser::tryDeleteAvatar() {
    bool deleted = con->tryDeleteAvatar(login);
    if (deleted) {
        qDebug() << "SUCCESSFULLY DELETED AVATAR " << login;
        setAvatar(default_avatar);
        return true;
    }
    return false;

};

bool CurUser::tryExit() {
    return unsetCurUser();
};

bool CurUser::setCurUser(User user_) {
    getInstance().setAvatar(user_.getAvatar());
    getInstance().setFavourites(user_.getFavourites());
    getInstance().setNotifications(user_.getNotifications());
    getInstance().setId(user_.getId());
    getInstance().setName(user_.getName());
    getInstance().setLogin(user_.getLogin());
    getInstance().setPassword(user_.getPassword());
    getInstance().setRights(user_.getRights());
    return true;
}

bool CurUser::unsetCurUser() {
    getInstance().setId();
    getInstance().setAvatar();
    getInstance().setFavourites();
    getInstance().setNotifications();
    getInstance().setName();
    getInstance().setLogin();
    getInstance().setPassword();
    getInstance().setRights();
    return true;
}


// class LocalAccountHandler
LocalAccountHandler::LocalAccountHandler() {
    default_avatar = QPixmap("://resourses/icons/empty_avatar.jpg");

    users.append(User("Artem", "ibartiom", "1234"));
    users.last().setAvatar(QPixmap(":/resourses/icons/photo_2024-03-06.jpeg"));
}

LocalAccountHandler& LocalAccountHandler::getInstance() {
    static LocalAccountHandler instance;
    return instance;
}

std::optional<User> LocalAccountHandler::tryRegister(QString name_, QString login_, QString password_) {
    for (User u : users) {
        if (login_ == u.getLogin()) {
            return std::nullopt;
        }
    }
    users.append(User(name_, login_, password_));
    return users.last();
}

std::optional<User> LocalAccountHandler::tryLogin(QString login_, QString password_) {
    for (User u : users) {
        if (login_ == u.getLogin() && password_ == u.getPassword()) {
            return u;
        }
    }
    return std::nullopt;
};

bool LocalAccountHandler::tryEditAvatar(QString login_, QPixmap avatar_) {
    for (User u : users) {
        if (login_ == u.getLogin()) {
            u.setAvatar(avatar_);
            return true;
        }
    }
    return false;
};

bool LocalAccountHandler::tryDeleteAvatar(QString login_) {
    for (User u : users) {
        if (login_ == u.getLogin()) {
            u.setAvatar(default_avatar);
            return true;
        }
    }
    return false;
};

bool LocalAccountHandler::tryDeleteAccount(QString login_) {
    for (int i = 0; i < users.size(); i++) {
        if (login_ == users[i].getLogin()) {
            users.removeAt(i);
            return true;
        }
    }
    return false;
};




// class DBAccountHandler
DBAccountHandler::DBAccountHandler() {
    default_avatar = QPixmap("://resourses/icons/empty_avatar.jpg");

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(DATABASE_IP);
    db.setPort(DATABASE_PORT);

    db.setDatabaseName(DATABASE_DATANAME);
    db.setUserName(DATABASE_USERNAME);
    db.setPassword(DATABASE_USERPASS);

    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных:" << db.lastError().text();
        throw QException();
    }

    qDebug() << "Успешное подключение к базе данных.";
}

DBAccountHandler& DBAccountHandler::getInstance() {
    static DBAccountHandler instance;
    return instance;
}

std::optional<User> DBAccountHandler::tryRegister(QString name_, QString login_, QString password_) {
    QSqlQuery query(db);
    query.prepare("SELECT id FROM users WHERE login = :login;");
    query.bindValue(":login", login_);
    query.exec();

    if (query.next()) {
        qDebug() << "User with the given login already exists";
        return std::nullopt;
    }

    query.prepare("INSERT INTO users (name, login, password) VALUES (:name, :login, :password);");
    query.bindValue(":name", name_);
    query.bindValue(":login", login_);
    query.bindValue(":password", password_);
    if (!query.exec()) {
        qDebug() << "Failed to register user";
        db.rollback();
        return std::nullopt;
    }

    query.prepare("SELECT LAST_INSERT_ID();");
    if (!query.exec() || !query.next()) {
        qDebug() << "Failed to retrieve last inserted ID";
        db.rollback();
        return std::nullopt;
    }
    db.commit();

    User tmp(query.value(0).toInt(), name_, login_, password_);
    tmp.setAvatar(default_avatar);
    return tmp;
}


std::optional<User> DBAccountHandler::tryLogin(QString login_, QString password_) {
    QSqlQuery query(db);
    query.prepare("SELECT id, name, password, avatar FROM users WHERE login = :login;");
    query.bindValue(":login", login_);
    query.bindValue(":password", password_);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return std::nullopt;
    }
    if (!query.next()) {
        qDebug() << "No user found with the given login";
        return std::nullopt;
    }
    if (query.value("password").toString() != password_) {
        qDebug() << "Incorrect password";
        return std::nullopt;
    }

    User tmp(query.value(0).toInt(), query.value(1).toString(), login_, password_);
    QByteArray avatarData = query.value("avatar").toByteArray();
    if (!avatarData.isEmpty()) {
        QPixmap pixmap;
        pixmap.loadFromData(avatarData);
        tmp.setAvatar(pixmap);
    } else {
        tmp.setAvatar(default_avatar);
    }
    return tmp;
};


bool DBAccountHandler::tryEditAvatar(QString login_, QPixmap avatar_) {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    avatar_.save(&buffer, "PNG");
    qDebug() << buffer.size();

    QSqlQuery query;
    query.prepare("UPDATE users SET avatar = :avatar WHERE login = :login;");
    query.bindValue(":avatar", byteArray);
    query.bindValue(":login", CurUser::getInstance().getLogin());

    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return false;
    }
    qDebug() << "Изображение успешно добавлено в базу данных.";
    return true;
};

bool DBAccountHandler::tryDeleteAvatar(QString login_) {
    QByteArray byteArray;
    QSqlQuery query;
    query.prepare("UPDATE users SET avatar = :avatar WHERE login = :login;");
    query.bindValue(":avatar", byteArray);
    query.bindValue(":login", CurUser::getInstance().getLogin());

    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return false;
    }
    qDebug() << "Изображение успешно добавлено в базу данных.";
    return true;
};


bool DBAccountHandler::tryDeleteAccount(QString login_) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM users WHERE login = :login");
    query.bindValue(":login", login_);
    if (!query.exec()) {
        qDebug() << "Failed to delete user";
        return false; // обработайте ошибку
    }
    db.commit();
    return true;
};
