#include <QCoreApplication>
#include <QDir>
#include <QSettings>

#include "tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationName("test");
    QCoreApplication::setOrganizationDomain("1@1.com");
    QCoreApplication::setApplicationName("DataServer");
    QCoreApplication::setApplicationVersion("1.0.0");

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QDir::currentPath() + "/settings");

    TcpServer server;

    return a.exec();
}
