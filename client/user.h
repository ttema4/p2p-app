#ifndef USER_H
#define USER_H

#include <QString>
#include <QPixmap>
#include <QVector>
#include <QMap>

class User {
public:
    User();
    User(QString name_, QString login_, QString password_);

    int getId() { return id; };
    QString getName() { return name; };
    QString getLogin() { return login; };
    QString getPassword() { return password; };
    QPixmap getAvatar1() { return avatar1; };
    QPixmap getAvatar2() { return avatar2; };
    QPixmap getAvatarMx() { return avatar_mx; };

    QVector<QString> getFavourites() { return favourites; };
    QVector<QString> getNotifications() { return notifications; };

    QMap<QString, bool> getRights() { return rights; };


    bool setId(int id_);
    bool setName(QString name_);
    bool setLogin(QString login_);
    bool setPassword(QString password_);
    bool setAvatar(QPixmap avatar_);

    bool setFavourites(QVector<QString> favourites_);
    bool setNotifications(QVector<QString> notifications_);

    bool setRights(QMap<QString, bool> rights_);

protected:
    QPixmap avatarCorrection(QPixmap avatar_, int size);

    int id;
    QString name;
    QString login;
    QString password;
    QPixmap avatar1; // small (30x30)
    QPixmap avatar2; // bigger (200x200)
    QPixmap avatar_mx; // original

    QVector<QString> favourites;
    QVector<QString> notifications;

    QMap<QString, bool> rights;
};

class CurUser : public User {
public:
    static CurUser& getInstance();

    bool setCurUser(User user_);
    bool unsetCurUser();

private:
    CurUser();
    CurUser(const CurUser&) = delete;
    CurUser& operator=(const CurUser&) = delete;
};

#endif // USER_H
