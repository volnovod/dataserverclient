#pragma once
#include <QThread>

class DataWriter : public QThread
{
    Q_OBJECT
public:
    explicit DataWriter(const QByteArray& data, QObject *parent = nullptr);

signals:


    // QThread interface
protected:
    void run();

private:
    QByteArray m_buffer;
};
