#ifndef IAA_PICTURE_CLIENT_H
#define IAA_PICTURE_CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>

class iaa_picture_client : public QObject
{
    Q_OBJECT
public:
    explicit iaa_picture_client(QObject *parent = 0);

signals:
    void connectionStatusChange(bool state);

public slots:
    void dataSend(QString datatype, QByteArray data);

private slots:
    void handleClientConnected();
    void handleClientDisconnected();
    void handleUdpDatagramReady();

private:
    QUdpSocket *udpSocket;
    QTcpSocket *tcpSocket;
};

#endif // IAA_PICTURE_CLIENT_H
