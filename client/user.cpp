#include "user.h"

// checked

// class User
User::User() {
    id = -1;
}

User::User(const QString &name_, const QString &login_, const QString &password_)
    : name(name_), login(login_), password(password_) {
    avatar = avatarCorrection(default_avatar, 250);
    avatar2 = avatarCorrection(avatar, 30);
    static int id_counter = 0;
    id = ++id_counter;
}

User::User(const int id_, const QString &name_, const QString &login_, const QString &password_)
    : id(id_), name(name_), login(login_), password(password_){};

void User::setAvatar(const QPixmap &avatar_) {
    avatar = avatarCorrection(avatar_, 250);
    avatar2 = avatarCorrection(avatar, 30);
}

QPixmap User::avatarCorrection(const QPixmap &avatar_, int size) const {
    if (avatar_.isNull()) {
        return avatar_;
    }
    if (avatar_.size() == QSize(size, size)) {
        return avatar_;
    }

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
CurUser &CurUser::getInstance() {
    static CurUser instance;
    return instance;
}

CurUser::CurUser() {
    id = -1;
}

bool CurUser::init() {
    try {
        if (GlobalCondition::getInstance().use_database) {
            con = &DBAccountHandler::getInstance();
        } else {
            con = &LocalAccountHandler::getInstance();
        }
    } catch (...) {
        qDebug() << "Error while connecting to UserHandler";
        return false;
    }
    return true;
}

bool CurUser::tryRegister(const QString &name_, const QString &login_, const QString &password_) {
    std::optional<User> registered = con->tryRegister(name_, login_, password_);
    if (registered) {
        qDebug() << "Successfully registered via:" << name_ << login_ << password_;
        setCurUser(*registered);
        return true;
    }
    qDebug() << "Error while registering via:" << name_ << login_ << password_;
    return false;
};

bool CurUser::tryLogin(const QString &login_, const QString &password_) {
    std::optional<User> logged = con->tryLogin(login_, password_);
    if (logged) {
        qDebug() << "Successfully logged via:" << login_ << password_;
        setCurUser(*logged);
        return true;
    }
    qDebug() << "Error while logging via:" << login_ << password_;
    return false;
};

bool CurUser::tryDeleteAccount() {
    bool deleted = con->tryDeleteAccount();
    if (deleted) {
        qDebug() << "Successfully deleted user:" << login;
        unsetCurUser();
        return true;
    }
    qDebug() << "Error while deleting user:" << login;
    return false;
}

bool CurUser::tryEditAvatar(const QPixmap &avatar_) {
    bool deleted = con->tryEditAvatar(avatar_);
    if (deleted) {
        qDebug() << "Successfully edited user avatar:" << login;
        setAvatar(avatar_);
        return true;
    }
    qDebug() << "Error while editing user avatar:" << login;
    return false;
}

bool CurUser::tryDelAvatar() {
    bool deleted = con->tryDeleteAvatar();
    if (deleted) {
        qDebug() << "Successfully deleted user avatar:" << login;
        setAvatar(default_avatar);
        return true;
    }
    qDebug() << "Error while deleting user avatar:" << login;
    return false;
};

bool CurUser::tryAddFavorites(const QString &chainHash) {
    if (id == -1) {
        return false;
    }

    favourites.insert(chainHash);
    bool added = con->tryEditFavourites(favourites);
    if (added) {
        qDebug() << "Successfully edited favourites user:" << login << chainHash;
        return true;
    }
    qDebug() << "Error while editing favourites user:" << login << chainHash;
    favourites.remove(chainHash);
    return false;
};

bool CurUser::tryDelFavorites(const QString &chainHash) {
    if (id == -1) {
        return false;
    }
    if (!favourites.contains(chainHash)) {
        return false;
    }

    favourites.remove(chainHash);
    bool deleted = con->tryEditFavourites(favourites);
    if (deleted) {
        qDebug() << "Successfully delete favourites user:" << login << chainHash;
        return true;
    }
    qDebug() << "Error while deleting favourites user:" << login << chainHash;
    favourites.insert(chainHash);
    return false;
};

bool CurUser::tryClearFavourites() {
    if (id == -1) {
        return false;
    }

    bool deleted = con->tryEditFavourites({});
    if (deleted) {
        qDebug() << "Successfully cleaned favourites user:" << login;
        favourites.clear();
        return true;
    }
    qDebug() << "Error while cleaning favourites user:" << login;
    return false;
}

bool CurUser::isFavorites(const QString &chainHash) const {
    return favourites.contains(chainHash);
}

void CurUser::userExit() {
    unsetCurUser();
};

void CurUser::setCurrentChains(const QVector<Chain> &newChains) {
    currentChains = newChains;
}

QVector<Chain> CurUser::getCurrentChains() const {
    return currentChains;
}

void CurUser::setCurUser(const User &user_) {
    getInstance().setAvatar(user_.getAvatar());
    getInstance().setFavourites(user_.getFavourites());
    getInstance().setNotifications(user_.getNotifications());
    getInstance().setId(user_.getId());
    getInstance().setName(user_.getName());
    getInstance().setLogin(user_.getLogin());
    getInstance().setPassword(user_.getPassword());
    getInstance().setRights(user_.getRights());
    qDebug() << "Successfully set current user:" << user_.getLogin();
}

void CurUser::unsetCurUser() {
    getInstance().setId();
    getInstance().setAvatar();
    getInstance().setFavourites();
    getInstance().setNotifications();
    getInstance().setName();
    getInstance().setLogin();
    getInstance().setPassword();
    getInstance().setRights();
    qDebug() << "Successfully log out user";
}

// class LocalAccountHandler
LocalAccountHandler::LocalAccountHandler() {
    // for fast-tests
    users.append(User("Artem", "ibartiom", "1234"));
    users.last().setAvatar(QPixmap(":/resourses/icons/photo_2024-03-06.jpeg"));
}

LocalAccountHandler &LocalAccountHandler::getInstance() {
    static LocalAccountHandler instance;
    return instance;
}

std::optional<User>
LocalAccountHandler::tryRegister(const QString &name_, const QString &login_, const QString &password_) {
    for (const User &u : users) {
        if (login_ == u.getLogin()) {
            return std::nullopt;
        }
    }
    users.append(User(name_, login_, password_));
    return users.last();
}

std::optional<User> LocalAccountHandler::tryLogin(const QString &login_, const QString &password_) {
    for (const User &u : users) {
        if (login_ == u.getLogin() && password_ == u.getPassword()) {
            return u;
        }
    }
    return std::nullopt;
};

bool LocalAccountHandler::tryEditAvatar(const QPixmap &avatar_) {
    for (User &u : users) {
        if (CurUser::getInstance().getLogin() == u.getLogin()) {
            u.setAvatar(avatar_);
            return true;
        }
    }
    return false;
};

bool LocalAccountHandler::tryEditFavourites(const QSet<QString> &favourites_) {
    for (User &u : users) {
        if (CurUser::getInstance().getLogin() == u.getLogin()) {
            u.setFavourites(favourites_);
            return true;
        }
    }
    return false;
};

bool LocalAccountHandler::tryDeleteAvatar() {
    for (User &u : users) {
        if (CurUser::getInstance().getLogin() == u.getLogin()) {
            u.setAvatar(default_avatar);
            return true;
        }
    }
    return false;
};

bool LocalAccountHandler::tryDeleteAccount() {
    for (int i = 0; i < users.size(); i++) {
        if (CurUser::getInstance().getLogin() == users[i].getLogin()) {
            users.removeAt(i);
            return true;
        }
    }
    return false;
};

// class DBAccountHandler
DBAccountHandler::DBAccountHandler() {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(QString::fromStdString(GlobalCondition::getInstance().database_ip));
    db.setPort(GlobalCondition::getInstance().database_port);

    db.setDatabaseName(QString::fromStdString(GlobalCondition::getInstance().database_dataname));
    db.setUserName(QString::fromStdString(GlobalCondition::getInstance().database_username));
    db.setPassword(QString::fromStdString(GlobalCondition::getInstance().database_userpass));

    if (!db.open()) {
        qDebug() << "Error while connecting to DataBase:" << db.lastError().text();
        throw QException();
    }

    qDebug() << "Successfully connected to DataBase";
}

DBAccountHandler &DBAccountHandler::getInstance() {
    static DBAccountHandler instance;
    return instance;
}

std::optional<User>
DBAccountHandler::tryRegister(const QString &name_, const QString &login_, const QString &password_) {
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

std::optional<User> DBAccountHandler::tryLogin(const QString &login_, const QString &password_) {
    QSqlQuery query(db);
    query.prepare("SELECT id, name, password, avatar, favourites FROM users WHERE login = :login;");
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

    QSet<QString> favourites;
    QByteArray favouritesData = query.value("favourites").toByteArray();

    if (!favouritesData.isEmpty()) {
        QDataStream in(favouritesData);
        in >> favourites;
        tmp.setFavourites(favourites);
    }

    return tmp;
};

bool DBAccountHandler::tryEditAvatar(const QPixmap &avatar_) {
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
        qDebug() << "Error while editing avatar:" << query.lastError().text();
        return false;
    }
    qDebug() << "Avatar succesfully added to DataBase";
    return true;
};

bool DBAccountHandler::tryEditFavourites(const QSet<QString> &favourites_) {
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out << favourites_;

    QSqlQuery query;
    query.prepare("UPDATE users SET favourites = :favourites WHERE login = :login;");
    query.bindValue(":favourites", byteArray);
    query.bindValue(":login", CurUser::getInstance().getLogin());

    if (!query.exec()) {
        qDebug() << "Error while editing favourites:" << query.lastError().text();
        return false;
    }
    qDebug() << "Favourites successfully edited";
    return true;
};

bool DBAccountHandler::tryDeleteAvatar() {
    QByteArray byteArray;
    QSqlQuery query;
    query.prepare("UPDATE users SET avatar = :avatar WHERE login = :login;");
    query.bindValue(":avatar", byteArray);
    query.bindValue(":login", CurUser::getInstance().getLogin());

    if (!query.exec()) {
        qDebug() << "Error while deleting avatar:" << query.lastError().text();
        return false;
    }
    qDebug() << "Avatar successfully deleted";
    return true;
};

bool DBAccountHandler::tryDeleteAccount() {
    QSqlQuery query(db);
    query.prepare("DELETE FROM users WHERE login = :login");
    query.bindValue(":login", CurUser::getInstance().getLogin());
    if (!query.exec()) {
        qDebug() << "Error while deleting user:" << query.lastError().text();
        return false;
    }
    qDebug() << "User succesfully deleted";
    db.commit();
    return true;
};
