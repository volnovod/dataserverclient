#include <QDebug>
#include <QTimer>
#include <QSettings>

#include "TCPLink.h"
#include "datawriter.h"

TCPLink::TCPLink(QObject* perent)
    : QObject(perent)
{
    loadSettings();
    connectSocket( m_address.toString(), m_port);
}

// Connect the connection.
void TCPLink::connectSocket(const QString& address, quint16 port)
{
    m_socket.abort();
    m_address.setAddress(address);
    m_port = port;

    if (m_address.isNull()) {
        emit communicationError(tr("%1. IP Address is invalid").arg(this->objectName()));
        return;
    }

    connect(&m_socket, &QTcpSocket::connected, this, &TCPLink::connected);
    m_socket.connectToHost(m_address, m_port);

}

void TCPLink::disconnectSocket()
{
    if (m_socket.isOpen()) {
        disconnect(&m_socket, &QTcpSocket::readyRead, this, &TCPLink::readData);
        disconnect(&m_socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
                   this, &TCPLink::_socketError);

        m_socket.disconnectFromHost();

        if (m_socket.state() != QAbstractSocket::UnconnectedState)
            m_socket.waitForDisconnected(100);

        m_rxBuf.clear();
    }
}

void TCPLink::writeData(const QByteArray &data)
{
    if (m_socket.isOpen())
        m_socket.write(data);
}

void TCPLink::readData()
{
    if (m_socket.isOpen()) {
        QByteArray inData = m_socket.readAll();
        if(m_bufferSize){
            m_rxBuf += inData;
        }else{
            memcpy((void*)(&m_bufferSize), inData.constData(), sizeof(quint32));
            return;
        }


        if (m_rxBuf.size() >= m_bufferSize * sizeof(double))
            processInputData();
    }
}

void TCPLink::processInputData()
{
    DataWriter* writer = new DataWriter(m_rxBuf);
    writer->setObjectName(objectName().append("data"));
    connect(writer, &QThread::finished, writer, &QThread::deleteLater);
    connect(writer, &QThread::finished, this, &TCPLink::writingFinished);
    writer->start();
}

void TCPLink::connected()
{
    qDebug() << "Connected";
    connect(&m_socket, &QTcpSocket::readyRead, this, &TCPLink::readData);
    connect(&m_socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
            this, &TCPLink::_socketError);

    QTimer::singleShot(3000, this, [=](){
        writeData(prepareRequest());
    });

}

void TCPLink::writingFinished()
{
    disconnectSocket();
    emit finished();
}

void TCPLink::loadSettings()
{
    QSettings settings;
    settings.beginGroup("DataClientSettings");
    if(!settings.contains("initialValue") || !settings.contains("serverAddress") || !settings.contains("serverPort")){
        writeSettings();
    }
    m_initialValue = settings.value("initialValue", DEFAULT_INITIAL_VALUE).toDouble();
    m_address = QHostAddress(settings.value("serverAddress", DEFAULT_SERVER_ADDRESS).toString());
    m_port = settings.value("serverPort", DEFAULT_SERVER_PORT).toInt();
    settings.endGroup();
}

void TCPLink::writeSettings()
{
    QSettings settings;
    settings.beginGroup("DataClientSettings");
    settings.setValue("initialValue", DEFAULT_INITIAL_VALUE);
    settings.setValue("serverAddress", DEFAULT_SERVER_ADDRESS);
    settings.setValue("serverPort", DEFAULT_SERVER_PORT);
    settings.sync();
    settings.endGroup();
}

QByteArray TCPLink::prepareRequest() const
{
    QByteArray request;
    request.append(reinterpret_cast<const char*>(&m_initialValue), sizeof(double));
    return request;
}

void TCPLink::_socketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);

    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << m_socket.errorString();
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << m_socket.errorString();
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << m_socket.errorString();
        break;
    default:
        qDebug() << m_socket.errorString();
        break;
    }
}
