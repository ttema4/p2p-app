#include "textpixmapbutton.h"

void TextPixmapButton::initialize() {
    text1_btn = new QLabel();
    text2_btn = new QLabel();
    pixmapLabel = new QLabel();
    text1_btn->setStyleSheet("font: 13px;");
    text2_btn->setStyleSheet("font: bold 9px;");
    pixmapLabel->setMaximumSize(QSize(30, 30));

    QWidget *texts = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(text1_btn);
    vLayout->addWidget(text2_btn);
    vLayout->setAlignment(Qt::AlignCenter | Qt::AlignLeft);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);
    texts->setLayout(vLayout);

    layout = new QHBoxLayout();
    layout->addWidget(texts);
    layout->addStretch(1);
    layout->addWidget(pixmapLabel);
    layout->setContentsMargins(7, 3, 5, 3);

    this->setMinimumHeight(40);
    this->setLayout(layout);
}

TextPixmapButton::TextPixmapButton(const QString& text, QPixmap pixmap, QWidget *parent) : QPushButton(parent) {
    initialize();

    text1_btn->setText(text);
    text2_btn->setText("Мой профиль");
    pixmapLabel->setPixmap(pixmap);
}

TextPixmapButton::TextPixmapButton(QWidget *parent) : QPushButton(parent){
    initialize();

    text1_btn->setText("Войти");
    text2_btn->setText("Зарегистрироваться");
    pixmapLabel->hide();
};


bool TextPixmapButton::updateLayout() {
    if (CurUser::getInstance().getId() == -1) {
        text1_btn->setText("Войти");
        text2_btn->setText("Зарегистрироваться");
        pixmapLabel->hide();
    } else {
        text1_btn->setText(CurUser::getInstance().getName());
        text2_btn->setText("Мой профиль");
        pixmapLabel->show();
        pixmapLabel->setPixmap(CurUser::getInstance().getAvatar1());
    }
    return true;
};
