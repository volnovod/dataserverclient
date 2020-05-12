#pragma once

#include <QObject>
#include <QTcpSocket>

class Socket : public QObject
{
    Q_OBJECT
public:
    explicit Socket(const int& socketDescretpor, const quint32& size, QObject *parent = nullptr);

signals:
    void finished();

public slots:
    void readyRead();
    void disconnected();
    void error(QAbstractSocket::SocketError);
    void writeData(QByteArray data);
    void init();

private:

    void processInputData();

    double* prepareResponceArray(const double &value, const quint32 &size) const;

    QTcpSocket      m_socket;
    QByteArray      m_rxBuf;
    int             m_socketDescriptor;
    quint32         m_size = 1000000;
};
