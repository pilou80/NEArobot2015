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
    explicit rbtserversocket(QObject *parent = 0, QString name = "rbtserversocket", uint port = 1234);

signals:
    void dataReceived(QString datatype, QByteArray data);

public slots:

private slots:
    void handleUdpTimerTimeout();
    void handleNewConnection();
    void handleClientDisconnect();
    void handleReadyRead();

private:
    QString serverName;
    uint serverPort;
    QTcpServer *tcpServer;
    QUdpSocket *udpSocket;
    QTimer *udpTimer;
    QList<QTcpSocket*> clientList;
    QByteArray dataBuffer;
};

#endif // RBTSERVERSOCKET_H
