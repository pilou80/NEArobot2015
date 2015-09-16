#ifndef RBTSERVERSOCKET_H
#define RBTSERVERSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QUdpSocket>
#include <QTimer>

class rbtserversocket : public QObject
{
    Q_OBJECT
public:
    explicit rbtserversocket(QObject *parent = 0, QString name = "rbtserversocket", uint port = 1234, bool automaticMode = true);

    typedef struct{
        QTcpSocket *socket;
        QByteArray *buffer;
        int clientId;
    } structClientInfo;
signals:
    void dataReceived(QString dataName, QByteArray data, int id);
    void clientConnected(int id);
    void clientDisconnected(int id);

public slots:
    void sendData(QString dataName, QByteArray data, QList<int> ids = QList<int>());

private slots:
    void handleUdpTimerTimeout();
    void handleNewConnection();
    void handleClientDisconnect();
    void handleReadyRead();

private:
    int clientId(QTcpSocket *socket);
    int clientIndex(QTcpSocket *socket);
    QString serverName;
    uint serverPort;
    QTcpServer *tcpServer;
    QUdpSocket *udpSocket;
    QTimer *udpTimer;
    QList<structClientInfo> clientList;
    QByteArray dataBuffer;
    int m_clientIndex;
};

#endif // RBTSERVERSOCKET_H
