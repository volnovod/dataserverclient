#pragma once

#include <QThread>
#include <QHostAddress>
#include <QTcpSocket>

#define DEFAULT_INITIAL_VALUE       0.0
#define DEFAULT_SERVER_ADDRESS      "127.0.0.1"
#define DEFAULT_SERVER_PORT         50000

class TCPLink : public QObject {
    Q_OBJECT

public:
    TCPLink(QObject* perent = nullptr);

public slots:
    void writeData(const QByteArray &data);
    void readData();

    void connectSocket(const QString& address, quint16 port);
    void disconnectSocket();
    void processInputData();
    void connected();

    void writingFinished();

signals:
    void TCPconnected() const;
    void communicationError(const QString&) const;
    void finished();

private:

    void loadSettings();
    void writeSettings();
    QByteArray prepareRequest() const;
    void _socketError(QAbstractSocket::SocketError socketError);

    QTcpSocket m_socket;
    QHostAddress m_address = QHostAddress::Any;
    quint16 m_port = 5760;
    QByteArray m_rxBuf;
    quint32 m_bufferSize=0;
    double m_initialValue=0.0;
};
