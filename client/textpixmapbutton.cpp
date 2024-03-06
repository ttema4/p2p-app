#include "textpixmapbutton.h"

TextPixmapButton::TextPixmapButton(const QString& text, QPixmap& pixmap, QWidget *parent) : QPushButton(parent), m_text(text), m_pixmap(pixmap) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *text_btn =  new QLabel(text);
    // text_btn->setStyleSheet("font: 10px;");
    layout->addWidget(text_btn);
    layout->addStretch(1);

    QLabel *pixmapLabel = new QLabel();
    QPixmap roundedPixmap(pixmap.size());
    roundedPixmap.fill(Qt::transparent);
    QPainter painter(&roundedPixmap);
    painter.setBrush(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawEllipse(roundedPixmap.rect());
    pixmap.setMask(roundedPixmap.mask());
    pixmapLabel->setPixmap(pixmap.scaled(QSize(25, 25), Qt::KeepAspectRatio));
    layout->addWidget(pixmapLabel);

    layout->setContentsMargins(7, 3, 12, 3);
    setLayout(layout);
}
