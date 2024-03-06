#ifndef TEXTPIXMAPBUTTON_H
#define TEXTPIXMAPBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QBitmap>

class TextPixmapButton : public QPushButton {
public:
    TextPixmapButton(const QString& text, QPixmap& pixmap, QWidget *parent = nullptr);

private:
    QString m_text;
    QPixmap m_pixmap;
};

#endif // TEXTPIXMAPBUTTON_H
