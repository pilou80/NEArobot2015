#include "rbtclientsocket.h"
#include <qdebug.h>
#include <QByteArray>
#include <rbtcommonserializer.h>

rbtClientSocket::rbtClientSocket(QObject *parent, bool automatic) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, SIGNAL(readyRead()),this, SLOT(handleUdpDatagramReady()));
    if(automatic) udpSocket->bind(1357);

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(handleClientConnected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(handleClientDisconnected()));

}

void rbtClientSocket::handleUdpDatagramReady()
{
    if(tcpSocket->state() != QAbstractSocket::UnconnectedState) return;
    qDebug() << "rbtclientsocket::handleUdpDatagramReady()";
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        int port;
        QString serverName;

        if(rbtCommonSerializer::deSerializeDatagram(&port, &serverName, &datagram))
        {
            qDebug() << "rbtClientSocket::handleUdpDatagramReady Connect to :" << sender.toString() << ":" << port;
            tcpSocket->connectToHost(sender, port);
        }
        else
        {
            qWarning() << "rbtClientSocket::handleUdpDatagramReady unable to compute datagram !";
        }

/*
        if(datagram.count() < 4)
        {
            qDebug() << "rbtclientsocket::handleUdpDatagramReady() Datagram length error";
            return;
        }

        uint stringlength = ((quint16)datagram.at(0)<<8) + (quint16)datagram.at(1);
        datagram.remove(0, 2);

        if(datagram.count() < (int)stringlength)
        {
            qDebug() << "rbtclientsocket::handleUdpDatagramReady() Datagram length error";
            return;
        }

        QString serverName = QString::fromLatin1(datagram.left(stringlength));
        datagram.remove(0, stringlength);


        if(datagram.count() < 2)
        {
            qDebug() << "rbtclientsocket::handleUdpDatagramReady() Datagram length error";
            return;
        }

        quint16 port = (((quint16)datagram.at(0))<<8) + (quint16)datagram.at(1);
*/

    }
}

void rbtClientSocket::handleClientConnected()
{
    qDebug() << "rbtclientsocket::handleClientConnected()";
    emit connectionStatusChange(true);
}

void rbtClientSocket::handleClientDisconnected()
{
    qDebug() << "rbtclientsocket::handleClientDisconnected()";
    emit connectionStatusChange(false);
}

void rbtClientSocket::dataSend(QString dataName, QByteArray data)
{
    QByteArray dataOut;
    if(tcpSocket->state() != QAbstractSocket::ConnectedState) return;

    qDebug() << "dataName.count() :" << dataName.count();
    dataOut.append( (char)(((quint16)dataName.count() >> 8) & 0xFF) );
    dataOut.append( (char)(((quint16)dataName.count() ) & 0xFF) );

    qDebug() << "dataName :" << dataName;
    dataOut.append( dataName.toLatin1());

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
