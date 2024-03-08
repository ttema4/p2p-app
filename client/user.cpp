#include "user.h"
#include "config.h"

#include <QPixmap>
#include <QPainter>
#include <QBitmap>

User::User() {
    id = -1;
}

User::User(QString name_, QString login_, QString password_) : name(name_), login(login_), password(password_) {
    avatar_mx = QPixmap("://resourses/icons/empty_avatar.jpg");
    avatar1 = avatarCorrection(QPixmap("://resourses/icons/empty_avatar.jpg"), 30);
    avatar2 = avatarCorrection(QPixmap("://resourses/icons/empty_avatar.jpg"), 250);
    static int id_counter = 1;
    id = id_counter++;
}

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
    avatar_mx = avatar_;
    avatar1 = avatarCorrection(avatar_, 30);
    avatar2 = avatarCorrection(avatar_, 250);
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

CurUser& CurUser::getInstance() {
    static CurUser instance;
    return instance;
}

CurUser::CurUser() {
    if (CUSTOM_NEW_USER) {
        id = 0;
        name = "artem";
        login = "ibartiom";
        password = "1234";
        avatar1 = QPixmap(":/resourses/icons/photo_2024-03-06.jpeg");
    } else {
        id = -1;
    }
}

bool CurUser::setCurUser(User user_) {
    getInstance().setAvatar(user_.getAvatarMx());
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
    getInstance().setId(-1);
    getInstance().setAvatar(QPixmap());
    getInstance().setFavourites(QVector<QString>());
    getInstance().setNotifications(QVector<QString>());
    getInstance().setName(QString());
    getInstance().setLogin(QString());
    getInstance().setPassword(QString());
    getInstance().setRights(QMap<QString, bool>());
    return true;
}
