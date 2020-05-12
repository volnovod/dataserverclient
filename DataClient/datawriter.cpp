#include "datawriter.h"
#include <QFile>
#include <QDataStream>
#include <QTime>
#include <QDebug>

DataWriter::DataWriter(const QByteArray &data, QObject *parent) : QThread(parent),
    m_buffer(data)
{

}

void DataWriter::run()
{
    QString name(objectName());
    name.append(".bin");
    QFile file(name);
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);
        stream. setVersion(QDataStream::Qt_4_2);
        stream << m_buffer;
        if(stream.status() != QDataStream::Ok)
        {
            qDebug() << "Writing error";
        }
    }
    file.close();
}
