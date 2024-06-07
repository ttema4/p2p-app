#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "windowhandler.h"

QFile logFile;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (!logFile.isOpen()) {
        qWarning() << "Log file is not open!";
        return;
    }

    QTextStream out(&logFile);
    QString file = context.file ? context.file : "unknown";
    QString function = context.function ? context.function : "unknown";
    QString line = context.line >= 0 ? QString::number(context.line) : "unknown";

    switch (type) {
#ifdef DEBUG_MODE
        case QtDebugMsg:
            out << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ");
            out << QString("Debug: %1 (%2:%3, %4)\n").arg(msg, file, line, function);
            break;
#endif
        case QtInfoMsg:
            out << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ");
            out << QString("Info: %1 (%2:%3, %4)\n").arg(msg, file, line, function);
            break;
        case QtWarningMsg:
            out << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ");
            out << QString("Warning: %1 (%2:%3, %4)\n").arg(msg, file, line, function);
            break;
        case QtCriticalMsg:
            out << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ");
            out << QString("Critical: %1 (%2:%3, %4)\n").arg(msg, file, line, function);
            break;
        case QtFatalMsg:
            out << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ");
            out << QString("Fatal: %1 (%2:%3, %4)\n").arg(msg, file, line, function);
            abort();
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    QFile styleF;
    styleF.setFileName(":/resourses/css/style.css");
    if (!styleF.open(QFile::ReadOnly)) {
        qCritical() << "Cannot open style file for reading:";
        return -1;
    }

    logFile.setFileName(QString(LOG_DIR) + "/application.log");
    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        qCritical() << "Cannot open log file for writing:" << QString(LOG_DIR) + "/application.log";
        return -1;
    }

    {
        QTextStream out(&logFile);
        out << "\n\n === NEW SESSION at " << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ")
            << " === \n\n";
    }

    QApplication a(argc, argv);
    qInstallMessageHandler(myMessageOutput);

    QString qssStr = styleF.readAll();
    a.setStyleSheet(qssStr);

    WindowHandler w;
    w.show();

    int ret = a.exec();

    {
        QTextStream out(&logFile);
        out << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ");
        out << QString("Info: End session with exit code: %1\n").arg(ret);
    }

    logFile.close();
    styleF.close();

    return ret;
}
