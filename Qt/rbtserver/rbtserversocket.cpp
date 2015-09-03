#include "rbtserversocket.h"
#include <QDebug>

rbtserversocket::rbtserversocket(QObject *parent, QString name, uint port) : QObject(parent)
{
    serverName = name;
    serverPort = port;

    udpTimer = new QTimer(this);
    udpTimer->setSingleShot(false);

    connect(udpTimer, SIGNAL(timeout()), this, SLOT(handleUdpTimerTimeout()));



    udpSocket = new QUdpSocket(this);

    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
    tcpServer->listen(QHostAddress::Any, port);

    udpTimer->start(1000);

}

void rbtserversocket::handleUdpTimerTimeout()
{
    QByteArray data;

    data.append( (char)(((quint16)serverName.count() >> 8) & 0xFF) );
    data.append( (char)(((quint16)serverName.count() ) & 0xFF) );
    data.append( serverName.toLatin1());

    data.append( (char)(((quint16)serverPort >> 8) & 0xFF) );
    data.append( (char)(((quint16)serverPort ) & 0xFF) );

    udpSocket->writeDatagram(data, QHostAddress::Broadcast, 1357);
}

void rbtserversocket::handleNewConnection()
{
    Q_ASSERT_X(tcpServer->hasPendingConnections(), "rbtserversocket::handleNewConnection", "No pending Connections");
    QTcpSocket *newConn = tcpServer->nextPendingConnection();

    Q_ASSERT(newConn != NULL);
    clientList.append(newConn);
    qDebug() << "New client : " << newConn->localAddress().toString();
    connect(newConn, SIGNAL(disconnected()), this, SLOT(handleClientDisconnect()));
    connect(newConn, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));

    dataBuffer.clear();

}

void rbtserversocket::handleClientDisconnect()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    Q_ASSERT(client != NULL);
    Q_ASSERT( QString(client->metaObject()->className()) == "QTcpSocket");
    Q_ASSERT(client->state() == QAbstractSocket::UnconnectedState);

    clientList.removeAll(client);
    client->deleteLater();

}

void rbtserversocket::handleReadyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    Q_ASSERT(client != NULL);
    Q_ASSERT( QString(client->metaObject()->className()) == "QTcpSocket");


    dataBuffer.append(client->readAll());

    if(dataBuffer.count() < 4)
    {
        qWarning() << "rbtserversocket::handleReadyRead() Data length error (fulllength)";
        return;
    }
    uint fullLength = ((quint32)dataBuffer.at(0)<<24)+
            ((quint32)dataBuffer.at(1)<<16)+
            ((quint32)dataBuffer.at(2)<<8) +
             (quint32)dataBuffer.at(3);

    if( (dataBuffer.count()-4) < (int)fullLength) return;

    dataBuffer.remove(0, 4);

    uint stringlength = ((quint32)dataBuffer.at(0)<<8) + (quint32)dataBuffer.at(1);
    dataBuffer.remove(0, 2);

    if(dataBuffer.count() < (int)stringlength)
    {
        qWarning() << "rbtserversocket::handleReadyRead() Data length error (stringLength)";
        dataBuffer.clear();
        return;
    }

    QString dataName = QString::fromLatin1(dataBuffer.left(stringlength));
    dataBuffer.remove(0, stringlength);

    if(dataBuffer.count() < 4)
    {
        qWarning() << "rbtserversocket::handleReadyRead() Data length error (dataLength)";
        dataBuffer.clear();
        return;
    }

    uint dataLength = ((quint32)dataBuffer.at(0)<<24)+
            ((quint32)dataBuffer.at(1)<<16)+
            ((quint32)dataBuffer.at(2)<<8) +
            (quint32)dataBuffer.at(3);
    dataBuffer.remove(0, 4);
    if(dataBuffer.count() < (int)dataLength)
    {
        qWarning() << "rbtserversocket::handleReadyRead() Data length error (data)";
        dataBuffer.clear();
        return;
    }

    QByteArray data = dataBuffer.left(dataLength);

    dataBuffer.remove(0, dataLength);

    qDebug() << "dataReceived : " << dataName << " => " << data.length() << "bytes";
    emit(dataReceived(dataName, data));

}

void rbtserversocket::sendData(QString datatype, QByteArray data)
{

}
