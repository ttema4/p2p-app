#include "headermenu.h"

HeaderMenu::HeaderMenu(QString center_text_, QWidget *parent) : QWidget{parent} {
    const QPalette palet(QColor("#e6e6e6"));
    this->setPalette(palet);
    this->setAutoFillBackground(true);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 15);

    logo_button = new QPushButton(this);
    logo_button->setObjectName("logoButton");

    logo_button->setMinimumWidth(150);
    logo_button->setMinimumHeight(40);
    layout->addWidget(logo_button);
    layout->addStretch(7);

    center_text = new QLabel(center_text_);
    center_text->setObjectName("MenuCenterText");
    layout->addWidget(center_text);
    layout->addStretch(10);

    status_text = new QLabel("Ваш id: " + QString::number(CurUser::getInstance().getId()));
    status_text->hide();
    layout->addWidget(status_text);
    menu_button = new QPushButton(this);
    menu_button->setFixedSize(QSize(40, 30));
    menu_button->setObjectName("menuButton");
    layout->addWidget(menu_button);

    menu_frame = new QFrame(parent);
    menu_frame->setObjectName("MenuQFrame");
    menu_frame->setFrameShape(QFrame::NoFrame);
    menu_frame->setLineWidth(1);
    menu_frame->setMinimumWidth(0);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(80);              // Радиус размытия
    effect->setXOffset(-5);                 // Смещение тени по горизонтали
    effect->setYOffset(5);                  // Смещение тени по вертикали
    effect->setColor(QColor(0, 0, 0, 50));  // Цвет тени

    menu_frame->setGraphicsEffect(effect);

    QVBoxLayout *mvbox = new QVBoxLayout;
    button1 = new TextPixmapButton();

    QPushButton *button2 = new QPushButton("Уведомления");
    button2->setMinimumHeight(30);
    QPushButton *button3 = new QPushButton("Избранное");
    button3->setMinimumHeight(30);
    QPushButton *button4 = new QPushButton("Настройки");
    button4->setMinimumHeight(30);
    QPushButton *button5 = new QPushButton("Выйти");
    button5->setMinimumHeight(30);

    connect(menu_button, &QPushButton::clicked, this, &HeaderMenu::showMenu);
    connect(logo_button, &QPushButton::clicked, this, &HeaderMenu::open_homePage);

    connect(button1, &QPushButton::clicked, this, &HeaderMenu::open_myPage);
    connect(button2, &QPushButton::clicked, this, &HeaderMenu::open_notifyPage);
    connect(button3, &QPushButton::clicked, this, &HeaderMenu::open_favotitePage);
    connect(button4, &QPushButton::clicked, this, &HeaderMenu::open_settingsPage);
    connect(button5, &QPushButton::clicked, this, &HeaderMenu::open_exitPage);

    mvbox->addWidget(button1);
    mvbox->addWidget(button2);
    mvbox->addWidget(button3);
    mvbox->addWidget(button4);
    mvbox->addWidget(button5);
    mvbox->addStretch(1);
    menu_frame->setLayout(mvbox);

    menu_frame->hide();
}

void HeaderMenu::resizeMenu() {
    int button_pos_x = menu_button->x() + menu_button->width() - menuWidth;
    int button_pos_y = menu_button->y() + menu_button->height();
    menu_frame->move(button_pos_x, button_pos_y);
    menu_frame->resize(menuWidth, menuHeight);
}

void HeaderMenu::hideMenu() {
    menu_frame->hide();
}

void HeaderMenu::showMenu() {
    this->setFocus();
    anim = new QPropertyAnimation(menu_frame, "size");
    anim->setDuration(300);
    if (!menuVisible) {
        anim->setStartValue(QSize(menuWidth, 0));
        anim->setEndValue(QSize(menuWidth, menuHeight));
        menuVisible = true;
        menu_frame->show();
        resizeMenu();
    } else {
        anim->setStartValue(QSize(menuWidth, menuHeight));
        anim->setEndValue(QSize(menuWidth, 0));
        menuVisible = false;
        connect(anim, &QPropertyAnimation::finished, this, &HeaderMenu::hideMenu);
    }
    anim->start();
}

void HeaderMenu::showEvent(QShowEvent *event) {
    logo_button->style()->unpolish(logo_button);
    logo_button->style()->polish(logo_button);

    status_text->setText("Ваш id: " + QString::number(CurUser::getInstance().getId()));
    button1->updateLayout();
    QWidget::showEvent(event);
}

void HeaderMenu::hideEvent(QHideEvent *event) {
    QWidget::hideEvent(event);
}

void HeaderMenu::resizeEvent(QResizeEvent *event) {
    resizeMenu();
    QWidget::resizeEvent(event);
}

void HeaderMenu::open_homePage() {
    if (menuVisible) {
        showMenu();
    }
    emit homePage();
};

void HeaderMenu::open_myPage() {
    if (menuVisible) {
        showMenu();
    }
    if (CurUser::getInstance().getId() == -1) {
        emit loginPage();
    } else {
        emit myPage();
    }
};

void HeaderMenu::open_notifyPage() {
    if (menuVisible) {
        showMenu();
    }
    emit notifyPage();
};

void HeaderMenu::open_favotitePage() {
    if (menuVisible) {
        showMenu();
    }
    emit favouritePage();
};

void HeaderMenu::open_settingsPage() {
    if (menuVisible) {
        showMenu();
    }
    emit settingsPage();
};

void HeaderMenu::open_exitPage() {
    if (menuVisible) {
        showMenu();
    }
    CurUser::getInstance().userExit();
    this->showEvent(new QShowEvent());
    emit homePage();
};
