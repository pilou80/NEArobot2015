#include "rbtcommonserializer.h"
#include <QDebug>

rbtCommonSerializer::rbtCommonSerializer(QObject *parent) : QObject(parent)
{

}

void rbtCommonSerializer::serialize(QString dataName, QByteArray *data, QByteArray *dataSerial)
{
    Q_ASSERT(data != NULL);
    Q_ASSERT(dataSerial != NULL);

    dataSerial->append( (char)(((quint16)dataName.count() >> 8) & 0xFF) );
    dataSerial->append( (char)(((quint16)dataName.count() ) & 0xFF) );

    dataSerial->append( dataName.toLatin1());

    dataSerial->append( (char)(((quint32)data->count() >> 24) & 0xFF) );
    dataSerial->append( (char)(((quint32)data->count() >> 16) & 0xFF) );
    dataSerial->append( (char)(((quint32)data->count() >> 8) & 0xFF) );
    dataSerial->append( (char)(((quint32)data->count() ) & 0xFF) );

    dataSerial->append(*data);

    QByteArray fullsize;
    fullsize.append( (char)(((quint32)dataSerial->count() >> 24) & 0xFF) );
    fullsize.append( (char)(((quint32)dataSerial->count() >> 16) & 0xFF) );
    fullsize.append( (char)(((quint32)dataSerial->count() >> 8) & 0xFF) );
    fullsize.append( (char)(((quint32)dataSerial->count() ) & 0xFF) );

    dataSerial->prepend(fullsize);
}

bool rbtCommonSerializer::deSerialize(QString *dataName, QByteArray *data, QByteArray *dataSerial)
{
    Q_ASSERT(dataName != NULL);
    Q_ASSERT(data != NULL);
    Q_ASSERT(dataSerial != NULL);

    uint index = 0;
    if(dataSerial->count() < 4)
    {
        qWarning() << "rbtCommonSerializer::deSerialize Data length error (fulllength)";
        return false;
    }
    uint fullLength = ((quint32)(quint8)dataSerial->at(index++)<<24);
         fullLength += ((quint32)(quint8)dataSerial->at(index++)<<16);
         fullLength += ((quint32)(quint8)dataSerial->at(index++)<<8);
         fullLength += (quint32)(quint8)dataSerial->at(index++);

    if((dataSerial->count()) < (int)(fullLength + index))
    {
        qWarning() << "rbtCommonSerializer::deSerialize Data length error (fulllength size)";
        return false;
    }

    uint stringlength = ((quint32)(quint8)dataSerial->at(index++)<<8);
         stringlength += (quint32)(quint8)dataSerial->at(index++);

    if(dataSerial->count() < (int)(stringlength + index))
    {
        qWarning() << "rbtCommonSerializer::deSerialize Data length error (stringLength)";
        return false;
    }

    *dataName = QString::fromLatin1(dataSerial->mid(index, stringlength));
    index += stringlength;

    if(dataSerial->count() < (int)(4 + index))
    {
        qWarning() << "rbtCommonSerializer::deSerialize Data length error (dataLength)";
        return false;
    }

    uint dataLength = ((quint32)(quint8)dataSerial->at(index++)<<24);
         dataLength += ((quint32)(quint8)dataSerial->at(index++)<<16);
         dataLength += ((quint32)(quint8)dataSerial->at(index++)<<8);
         dataLength += (quint32)(quint8)dataSerial->at(index++);

    if(dataSerial->count() < (int)(dataLength + index))
    {
        qWarning() << "rbtCommonSerializer::deSerialize Data length error (data)";
        return false;
    }

    *data = dataSerial->mid(index, dataLength);
    dataSerial->remove(0, index + dataLength);
    return true;
}

void rbtCommonSerializer::serializeDatagram(int serverPort, QString serverName, QByteArray *data)
{
    Q_ASSERT(data != NULL);
    data->append( (char)(((quint16)serverName.count() >> 8) & 0xFF) );
    data->append( (char)(((quint16)serverName.count() ) & 0xFF) );
    data->append( serverName.toLatin1());

    data->append( (char)(((quint16)serverPort >> 8) & 0xFF) );
    data->append( (char)(((quint16)serverPort ) & 0xFF) );
}

bool rbtCommonSerializer::deSerializeDatagram(int *port, QString *serverName, QByteArray *data)
{
    Q_ASSERT(port != NULL);
    Q_ASSERT(serverName != NULL);
    Q_ASSERT(data != NULL);
    if(data->count() < 4)
    {
        qDebug() << "rbtclientsocket::handleUdpDatagramReady() Datagram length error";
        return false;
    }

    uint stringlength = ((quint16)data->at(0)<<8) + (quint16)data->at(1);
    data->remove(0, 2);

    if(data->count() < (int)stringlength)
    {
        qDebug() << "rbtclientsocket::handleUdpDatagramReady() Datagram length error";
        return false;
    }

    *serverName = QString::fromLatin1(data->left(stringlength));
    data->remove(0, stringlength);


    if(data->count() < 2)
    {
        qDebug() << "rbtclientsocket::handleUdpDatagramReady() Datagram length error";
        return false;
    }

    *port = (((quint16)data->at(0))<<8) + (quint16)data->at(1);
    return true;
}

