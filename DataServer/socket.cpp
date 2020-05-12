#include "socket.h"

Socket::Socket(const int &socketDescretpor, const quint32 &size, QObject *parent) : QObject(parent), m_socket(this)
{
    m_socketDescriptor = socketDescretpor;
    m_size = size;
}

void Socket::readyRead()
{
    m_rxBuf = m_socket.read(m_socket.bytesAvailable());
    if(m_rxBuf.size() >= sizeof(double)){
        processInputData();
    }
}

void Socket::disconnected()
{
    m_socketDescriptor = 0;
    disconnect(&m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    disconnect(&m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    disconnect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    emit finished();
}

void Socket::error(QAbstractSocket::SocketError error)
{
    switch (error) {
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

void Socket::writeData(QByteArray data)
{
    if (m_socket.isOpen())
    {
        m_socket.write(data);
    }
}

void Socket::init()
{
    if(m_socketDescriptor != 0){
        if(!m_socket.setSocketDescriptor(m_socketDescriptor)){
            qDebug() << "Error in setting socket descriptor";
        }
        connect(&m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(&m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    }
}

void Socket::processInputData()
{
    double value = 0;
    quint32 size = m_size;
    memcpy(&value, m_rxBuf.constData(), sizeof(double));

    double* array = prepareResponceArray(value, size);

    QByteArray header;
    header.append(reinterpret_cast<const char*>(&size), sizeof(quint32));
    writeData(header);

    QByteArray data;
    data.append(reinterpret_cast<const char*>(array), sizeof(double) * size);
    writeData(data);

    delete [] array;
}

double *Socket::prepareResponceArray(const double &value, const quint32 &size) const
{
    double initialValue = value;
    double* result = new double[size];

    for(quint32 i = 0; i < size; i++){
        result[i] = initialValue;
        initialValue++;
    }

    return result;
}
