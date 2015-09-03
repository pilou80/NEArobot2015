#include "iaa_picture_client.h"
#include <qdebug.h>
#include <QByteArray>

iaa_picture_client::iaa_picture_client(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, SIGNAL(readyRead()),this, SLOT(handleUdpDatagramReady()));
    udpSocket->bind(1357);

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(handleClientConnected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(handleClientDisconnected()));

}

void iaa_picture_client::handleUdpDatagramReady()
{
    if(tcpSocket->state() != QAbstractSocket::UnconnectedState) return;
    qDebug() << "iaa_picture_client::handleUdpDatagramReady()";
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        if(datagram.count() < 4)
        {
            qDebug() << "iaa_picture_client::handleUdpDatagramReady() Datagram length error";
            return;
        }

        uint stringlength = ((quint16)datagram.at(0)<<8) + (quint16)datagram.at(1);
        datagram.remove(0, 2);

        if(datagram.count() < stringlength)
        {
            qDebug() << "iaa_picture_client::handleUdpDatagramReady() Datagram length error";
            return;
        }

        QString serverName = QString::fromLatin1(datagram.left(stringlength));
        datagram.remove(0, stringlength);


        if(datagram.count() < 2)
        {
            qDebug() << "iaa_picture_client::handleUdpDatagramReady() Datagram length error";
            return;
        }

        quint16 port = (((quint16)datagram.at(0))<<8) + (quint16)datagram.at(1);

        qDebug() << "Connect to :" << sender.toString() << ":" << port;
        tcpSocket->connectToHost(sender, port);
    }
}

void iaa_picture_client::handleClientConnected()
{
    qDebug() << "iaa_picture_client::handleClientConnected()";
    emit connectionStatusChange(true);
}

void iaa_picture_client::handleClientDisconnected()
{
    qDebug() << "iaa_picture_client::handleClientDisconnected()";
    emit connectionStatusChange(false);
}

void iaa_picture_client::dataSend(QString datatype, QByteArray data)
{
    QByteArray dataOut;
    if(tcpSocket->state() != QAbstractSocket::ConnectedState) return;

    qDebug() << "datatype.count() :" << datatype.count();
    dataOut.append( (char)(((quint16)datatype.count() >> 8) & 0xFF) );
    dataOut.append( (char)(((quint16)datatype.count() ) & 0xFF) );

    qDebug() << "datatype :" << datatype;
    dataOut.append( datatype.toLatin1());

    dataOut.append( (char)(((quint32)data.count() >> 24) & 0xFF) );
    dataOut.append( (char)(((quint32)data.count() >> 16) & 0xFF) );
    dataOut.append( (char)(((quint32)data.count() >> 8) & 0xFF) );
    dataOut.append( (char)(((quint32)data.count() ) & 0xFF) );

    qDebug() << "data.count() :" << data.count();
    dataOut.append(data);

    QByteArray fullsize;
    fullsize.append( (char)(((quint32)dataOut.count() >> 24) & 0xFF) );
    fullsize.append( (char)(((quint32)dataOut.count() >> 16) & 0xFF) );
    fullsize.append( (char)(((quint32)dataOut.count() >> 8) & 0xFF) );
    fullsize.append( (char)(((quint32)dataOut.count() ) & 0xFF) );

    qDebug() << "dataOut.count() (fullsize) :" << dataOut.count();
    fullsize.append(dataOut);
    tcpSocket->write(fullsize);
}
