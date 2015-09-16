#ifndef RBTCLIENTSOCKET_H
#define RBTCLIENTSOCKET_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>

class rbtClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit rbtClientSocket(QObject *parent = 0, bool automatic = true);

signals:
    void connectionStatusChange(bool state);
    void dataReceived(QString dataName, QByteArray data);

public slots:
    void dataSend(QString dataName, QByteArray data);

private slots:
    void handleClientConnected();
    void handleClientDisconnected();
    void handleUdpDatagramReady();

private:
    QUdpSocket *udpSocket;
    QTcpSocket *tcpSocket;
};

#endif // RBTCLIENTSOCKET_H
