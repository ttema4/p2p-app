#ifndef USER_H
#define USER_H

#include "config.h"

#include <QString>
#include <QPixmap>
#include <QBitmap>
#include <QByteArray>
#include <QBuffer>
#include <QPainter>

#include <QVector>
#include <QMap>
#include <QtSql>
#include <QtPlugin>
#include <QSqlDriverPlugin>

#include <QDebug>
#include <optional>

class User {
public:
    User();
    User(QString name_, QString login_, QString password_); // for LocalAccountHendler
    User(int id, QString name_, QString login_, QString password_); // for DBAccountHandler

    int getId() { return id; };
    QString getName() { return name; };
    QString getLogin() { return login; };
    QString getPassword() { return password; };
    QPixmap getAvatar() { return avatar; };
    QPixmap getAvatar2() { return avatar2; };

    QVector<QString> getFavourites() { return favourites; };
    QVector<QString> getNotifications() { return notifications; };

    QMap<QString, bool> getRights() { return rights; };

    friend class LocalAccountHandler;
    friend class DBAccountHandler;

protected:
    QPixmap avatarCorrection(QPixmap avatar_, int size);

    bool setId(int id_ = -1);
    bool setName(QString name_ = QString());
    bool setLogin(QString login_ = QString());
    bool setPassword(QString password_ = QString());
    bool setAvatar(QPixmap avatar_ = QPixmap());

    bool setFavourites(QVector<QString> favourites_ = QVector<QString>());
    bool setNotifications(QVector<QString> notifications_ = QVector<QString>());

    bool setRights(QMap<QString, bool> rights_ = QMap<QString, bool>());

    int id;
    QString name;
    QString login;
    QString password;
    QPixmap avatar; // bigger (250x250)
    QPixmap avatar2; // small (30x30)

    QVector<QString> favourites;
    QVector<QString> notifications;

    QMap<QString, bool> rights;
};


class CurUser : public User {
public:
    static CurUser& getInstance();

    bool tryRegister(QString name_, QString login_, QString password_);
    bool tryLogin(QString login_, QString password_);
    bool tryDeleteAccount();
    bool tryExit();

    bool tryEditName(QString name_);
    bool tryEditLogin(QString login_);
    bool tryEditPassword(QString password_);
    bool tryEditAvatar(QPixmap avatar_ = QPixmap());

    bool tryDeleteAvatar();

    friend class LocalAccountHandler;
    friend class DBAccountHandler;

private:
    QPixmap default_avatar;

    bool setCurUser(User user_);
    bool unsetCurUser();

    CurUser();
    CurUser(const CurUser&) = delete;
    CurUser& operator=(const CurUser&) = delete;
};


class AccountHandler {
public:
    virtual std::optional<User> tryRegister(QString name_, QString login_, QString password_) = 0;
    virtual std::optional<User> tryLogin(QString login_, QString password_) = 0;

    virtual bool tryEditAvatar(QString login_, QPixmap avatar_ = QPixmap()) = 0;
    virtual bool tryDeleteAvatar(QString login_) = 0;
    virtual bool tryDeleteAccount(QString login_) = 0;
};

class LocalAccountHandler : public AccountHandler {
public:
    static LocalAccountHandler& getInstance();

    std::optional<User> tryRegister(QString name_, QString login_, QString password_) override;
    std::optional<User> tryLogin(QString login_, QString password_) override;

    bool tryEditAvatar(QString login_, QPixmap avatar_ = QPixmap()) override;
    bool tryDeleteAvatar(QString login_) override;
    bool tryDeleteAccount(QString login_) override;

private:
    QPixmap default_avatar;
    QVector<User> users;

    LocalAccountHandler();
    LocalAccountHandler(const LocalAccountHandler&) = delete;
    LocalAccountHandler& operator=(const LocalAccountHandler&) = delete;
};


class DBAccountHandler : public AccountHandler {
public:
    static DBAccountHandler& getInstance();

    std::optional<User> tryRegister(QString name_, QString login_, QString password_) override;
    std::optional<User> tryLogin(QString login_, QString password_) override;

    bool tryEditAvatar(QString login_, QPixmap avatar_ = QPixmap()) override;
    bool tryDeleteAvatar(QString login_) override;
    bool tryDeleteAccount(QString login_) override;


private:
    QPixmap default_avatar;
    QSqlDatabase db;

    DBAccountHandler();
    DBAccountHandler(const DBAccountHandler&) = delete;
    DBAccountHandler& operator=(const DBAccountHandler&) = delete;
};

#endif // USER_H
