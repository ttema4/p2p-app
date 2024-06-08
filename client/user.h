#ifndef USER_H
#define USER_H

#include <QBitmap>
#include <QBuffer>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QException>
#include <QMap>
#include <QPainter>
#include <QPixmap>
#include <QSet>
#include <QSqlDriverPlugin>
#include <QSqlError>
#include <QString>
#include <QUuid>
#include <QVector>
#include <QtPlugin>
#include <QtSql>
#include <optional>
#include "globalcondition.h"
#include "logic_fwd.h"

struct AccountHandler;

class User {
public:
    User();
    explicit User(const QString &name_, const QString &login_, const QString &password_);  // for LocalAccountHendler
    explicit User(
        const int id,
        const QString &name_,
        const QString &login_,
        const QString &password_
    );  // for DBAccountHandler

    [[nodiscard]] int getId() const {
        return id;
    };

    [[nodiscard]] QString getName() const {
        return name;
    };

    [[nodiscard]] QString getLogin() const {
        return login;
    };

    [[nodiscard]] QString getPassword() const {
        return password;
    };

    [[nodiscard]] QPixmap getAvatar() const {
        return avatar;
    };

    [[nodiscard]] QPixmap getAvatar2() const {
        return avatar2;
    };

    [[nodiscard]] QSet<QString> getFavourites() const {
        return favourites;
    };

    [[nodiscard]] QSet<QString> getNotifications() const {
        return notifications;
    };

    [[nodiscard]] QMap<QString, bool> getRights() const {
        return rights;
    };

    friend class LocalAccountHandler;
    friend class DBAccountHandler;

protected:
    QPixmap avatarCorrection(const QPixmap &avatar_, int size) const;

    void setId(int id_ = -1) {
        id = id_;
    };

    void setName(QString name_ = QString()) {
        name = std::move(name_);
    };

    void setLogin(QString login_ = QString()) {
        login = std::move(login_);
    };

    void setPassword(QString password_ = QString()) {
        password = std::move(password_);
    };

    void setFavourites(QSet<QString> favourites_ = QSet<QString>()) {
        favourites = std::move(favourites_);
    };

    void setNotifications(QSet<QString> notifications_ = QSet<QString>()) {
        notifications = std::move(notifications_);
    };

    void setRights(QMap<QString, bool> rights_ = QMap<QString, bool>()) {
        rights = std::move(rights_);
    };

    void setAvatar(const QPixmap &avatar_ = QPixmap());

    int id;
    QString name;
    QString login;
    QString password;
    QPixmap avatar;   // bigger (250x250)
    QPixmap avatar2;  // small (30x30)

    QSet<QString> favourites;
    QSet<QString> notifications;
    QMap<QString, bool> rights;

    QPixmap default_avatar = QPixmap("://resourses/icons/empty_avatar.jpg");
};

class CurUser : public User {
public:
    static CurUser &getInstance();
    bool init();

    bool tryRegister(const QString &name_, const QString &login_, const QString &password_);
    bool tryLogin(const QString &login_, const QString &password_);
    bool tryDeleteAccount();
    void userExit();

    bool tryEditName(const QString &name_);
    bool tryEditLogin(const QString &login_);
    bool tryEditPassword(const QString &password_);
    bool tryEditAvatar(const QPixmap &avatar_ = QPixmap());
    bool tryAddFavorites(const QString &chainHash);

    bool tryDelFavorites(const QString &chainHash);
    bool tryDelAvatar();
    bool tryClearFavourites();

    bool isFavorites(const QString &chainHash) const;

    void setCurrentChains(const QVector<Chain> &newChains);
    [[nodiscard]] QVector<Chain> getCurrentChains() const;

    friend class LocalAccountHandler;
    friend class DBAccountHandler;

private:
    AccountHandler *con;
    QVector<Chain> currentChains;

    void setCurUser(const User &user_);
    void unsetCurUser();

    CurUser();
    CurUser(const CurUser &) = delete;
    CurUser &operator=(const CurUser &) = delete;
};

class AccountHandler {
public:
    virtual std::optional<User> tryRegister(const QString &name_, const QString &login_, const QString &password_) = 0;
    virtual std::optional<User> tryLogin(const QString &login_, const QString &password_) = 0;

    virtual bool tryEditName(const QString &newName) = 0;
    virtual bool tryEditPassword(const QString &newPassword) = 0;
    virtual bool tryEditAvatar(const QPixmap &avatar_ = QPixmap()) = 0;
    virtual bool tryEditFavourites(const QSet<QString> &favourites_) = 0;

    virtual bool tryDeleteAvatar() = 0;
    virtual bool tryDeleteAccount() = 0;

protected:
    QPixmap default_avatar = QPixmap("://resourses/icons/empty_avatar.jpg");
};

class LocalAccountHandler : public AccountHandler {
public:
    static LocalAccountHandler &getInstance();

    std::optional<User> tryRegister(const QString &name_, const QString &login_, const QString &password_) final;
    std::optional<User> tryLogin(const QString &login_, const QString &password_) final;

    bool tryEditName(const QString &newName) final;
    bool tryEditPassword(const QString &newPassword) final;
    bool tryEditAvatar(const QPixmap &avatar_ = QPixmap()) final;
    bool tryEditFavourites(const QSet<QString> &favourites_) final;

    bool tryDeleteAvatar() final;
    bool tryDeleteAccount() final;

private:
    QVector<User> users;

    LocalAccountHandler();
    LocalAccountHandler(const LocalAccountHandler &) = delete;
    LocalAccountHandler &operator=(const LocalAccountHandler &) = delete;
};

class DBAccountHandler : public AccountHandler {
public:
    static DBAccountHandler &getInstance();

    std::optional<User> tryRegister(const QString &name_, const QString &login_, const QString &password_) final;
    std::optional<User> tryLogin(const QString &login_, const QString &password_) final;

    bool tryEditName(const QString &newName) final;
    bool tryEditPassword(const QString &newPassword) final;
    bool tryEditAvatar(const QPixmap &avatar_ = QPixmap()) final;
    bool tryEditFavourites(const QSet<QString> &favourites_) final;

    bool tryDeleteAvatar() final;
    bool tryDeleteAccount() final;

private:
    QSqlDatabase db;

    DBAccountHandler();
    DBAccountHandler(const DBAccountHandler &) = delete;
    DBAccountHandler &operator=(const DBAccountHandler &) = delete;
};

#endif  // USER_H
