#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QFile styleF;
    styleF.setFileName(":/resourses/css/style.css");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();

    QApplication a(argc, argv);
    a.setStyleSheet(qssStr);
    MainWindow w;
    w.show();
    return a.exec();
}
