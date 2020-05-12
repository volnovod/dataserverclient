#include "tcpserver.h"
#include <QSettings>

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    loadSettings();

    startServer();
}

void TcpServer::startServer()
{

    m_hostAddress = QHostAddress::Any;

    if (!listen(m_hostAddress, m_port))
    {
        qDebug() << "Error when listen to IP-" << m_hostAddress << ":"<< m_port;
        return;
    }
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "New client";
    Socket* socket = new Socket(socketDescriptor, m_size);
    QThread* socketThread = new QThread;
    connect(socket, &Socket::finished, socket, &Socket::deleteLater);
    connect(socketThread, &QThread::started, socket, &Socket::init);
    connect(socket, &Socket::finished, socketThread, &QThread::quit);
    connect(socketThread, &QThread::finished, socketThread, &QThread::deleteLater);
    socket->moveToThread(socketThread);
    socketThread->start();
}

void TcpServer::loadSettings()
{
    QSettings settings;
    settings.beginGroup("DataServerSettings");
    if(!settings.contains("port") || !settings.contains("arraySize")){
        writeSettings();
    }
    m_port = settings.value("port", DEFAULT_SERVER_PORT).toInt();
    m_size = settings.value("arraySize", DEFAULT_ARRAY_SIZE).toInt();
    settings.endGroup();

}

void TcpServer::writeSettings()
{
    QSettings settings;
    settings.beginGroup("DataServerSettings");
    settings.setValue("port", DEFAULT_SERVER_PORT);
    settings.setValue("arraySize", DEFAULT_ARRAY_SIZE);
    settings.sync();
    settings.endGroup();
}
