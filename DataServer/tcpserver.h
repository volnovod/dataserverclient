#pragma once

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

#include "socket.h"

#define DEFAULT_SERVER_PORT "50000"
#define DEFAULT_ARRAY_SIZE 1000000


class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    TcpServer(QObject *parent = nullptr);

private slots:
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:

    void loadSettings();
    void writeSettings();

    QHostAddress        m_hostAddress = QHostAddress::Any;
    quint16             m_port = 0;
    quint32             m_size = 1000000;

};
