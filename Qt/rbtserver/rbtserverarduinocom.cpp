#include "rbtserverarduinocom.h"

rbtServerArduinoCom::rbtServerArduinoCom(QObject *parent, QString serialPort) : QObject(parent)
{

}

void rbtServerArduinoCom::sendFrame(QByteArray data)
{
    QByteArray dataToSend;

    dataToSend.append(0x55);
    dataToSend.append(0xAA);
    dataToSend.append( (quint8)((quint16)data.length() << 8) &0xFF);
    dataToSend.append( (quint8)((quint16)data.length()) &0xFF);
    dataToSend.append(data);

}

quint16 rbtServerArduinoCom::computeCRC(QByteArray data)
{

}

bool rbtServerArduinoCom::checkCRC(QByteArray data)
{

}
