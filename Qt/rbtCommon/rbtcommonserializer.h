#ifndef RBTCOMMONSERIALIZER_H
#define RBTCOMMONSERIALIZER_H

#include <QObject>

class rbtCommonSerializer : public QObject
{
    Q_OBJECT
public:
    explicit rbtCommonSerializer(QObject *parent = 0);

    static void serialize(QString dataName, QByteArray *data, QByteArray *dataSerial);
    static bool deSerialize(QString *dataName, QByteArray *data, QByteArray *dataSerial);

    static void serializeDatagram(int port, QString serverName, QByteArray *data);
    static bool deSerializeDatagram(int *port, QString *serverName, QByteArray *data);
signals:

public slots:
};

#endif // RBTCOMMONSERIALIZER_H
