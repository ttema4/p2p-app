#include "textpixmapbutton.h"

TextPixmapButton::TextPixmapButton(const QString& text, const QPixmap& pixmap, QWidget *parent) : QPushButton(parent), m_text(text), m_pixmap(pixmap) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel(text));
    layout->addStretch(1);

    QLabel *pixmapLabel = new QLabel();
    pixmapLabel->setPixmap(pixmap.scaled(QSize(20, 20), Qt::KeepAspectRatio));
    layout->addWidget(pixmapLabel);

    layout->setContentsMargins(7, 3, 15, 3);
    setLayout(layout);
}
