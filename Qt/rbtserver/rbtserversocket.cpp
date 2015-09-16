#include "rbtserversocket.h"
#include "rbtcommonserializer.h"
#include <QDebug>

rbtserversocket::rbtserversocket(QObject *parent, QString name, uint port, bool automaticMode) : QObject(parent)
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

    m_clientIndex = 1;

    if(automaticMode)
    {
        qDebug() << "rbtserversocket::rbtserversocket starting server in Automatic mode (udp brodcast port 1357)";
        handleUdpTimerTimeout();
        udpTimer->start(1000);
    }
    else qDebug() << "rbtserversocket::rbtserversocket starting server in manual Mode (tcp server port" << port << ")";
}

int rbtserversocket::clientId(QTcpSocket *socket)
{
    for(int i; i<clientList.count(); i++)
        if(clientList.at(i).socket == socket) return clientList.at(i).clientId;
    return -1;
}

int rbtserversocket::clientIndex(QTcpSocket *socket)
{
    for(int i; i<clientList.count(); i++)
        if(clientList.at(i).socket == socket) return i;
    return -1;
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

    structClientInfo client;
    client.socket = newConn;
    client.clientId = m_clientIndex++;
    client.buffer = new QByteArray();
    clientList.append(client);

    qDebug() << "rbtserversocket::handleNewConnection New client id:" << client.clientId <<" | ip: " << newConn->localAddress().toString();
    connect(newConn, SIGNAL(disconnected()), this, SLOT(handleClientDisconnect()));
    connect(newConn, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
}

void rbtserversocket::handleClientDisconnect()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    Q_ASSERT(client != NULL);
    Q_ASSERT( QString(client->metaObject()->className()) == "QTcpSocket");
    Q_ASSERT(client->state() == QAbstractSocket::UnconnectedState);


    int index = clientIndex(client);

    qDebug() << "rbtserversocket::handleClientDisconnect client id:" << clientList.at(index).clientId <<" | ip: " << client->localAddress().toString();

    clientList.at(index).socket->deleteLater();
    delete clientList.at(index).buffer;
    clientList.removeAt(index);
}

void rbtserversocket::handleReadyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    Q_ASSERT(client != NULL);
    Q_ASSERT( QString(client->metaObject()->className()) == "QTcpSocket");

    QByteArray *buffer = clientList.at(clientIndex(client)).buffer;
    buffer->append(client->readAll());

    QString dataName;
    QByteArray data;

    if(rbtCommonSerializer::deSerialize(&dataName, &data, buffer))
    {
        qDebug() << "rbtserversocket::handleReadyRead dataReceived : " << dataName << " => " << data.length() << "bytes";
        emit(dataReceived(dataName, data, clientList.at(clientIndex(client)).clientId));
    }
}

void rbtserversocket::sendData(QString dataName, QByteArray data, QList<int> ids)
{
    if(clientList.count() == 0) return;
    QByteArray dataSerial;
    rbtCommonSerializer::serialize(dataName, &data, &dataSerial);

    //No id list mean broadcast to all Client
    if(ids.count() == 0)
    {
        foreach(structClientInfo client, clientList)
        {
            if(client.socket->isWritable()) client.socket->write(dataSerial);
        }
    }
    else
    {
        foreach(structClientInfo client, clientList)
        {
            if(ids.contains(client.clientId) &&
               (client.socket->isWritable()))
                    client.socket->write(dataSerial);
        }
    }
}
