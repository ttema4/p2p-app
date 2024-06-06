#ifndef TEXTPIXMAPBUTTON_H
#define TEXTPIXMAPBUTTON_H

#include <QBitmap>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QShowEvent>
#include <QStackedLayout>
#include <QVBoxLayout>
#include "user.h"

class TextPixmapButton : public QPushButton {
public:
    TextPixmapButton(const QString &text, QPixmap pixmap, QWidget *parent = nullptr);
    TextPixmapButton(QWidget *parent = nullptr);
    void initialize();

    bool updateLayout();

private:
    QLabel *text1_btn;
    QLabel *text2_btn;
    QLabel *pixmapLabel;

    QHBoxLayout *layout;
};

#endif  // TEXTPIXMAPBUTTON_H
