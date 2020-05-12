#include <QCoreApplication>
#include <QSettings>
#include <QDir>
#include "TCPLink.h"
#include <QDataStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationName("test");
    QCoreApplication::setOrganizationDomain("1@1.com");
    QCoreApplication::setApplicationName("DataClient");
    QCoreApplication::setApplicationVersion("1.0.0");

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QDir::currentPath() + "/settings");

    return a.exec();
}
