#include "rbtserverarduinocom.h"


rbtServerArduinoCom::rbtServerArduinoCom(QObject *parent, QString portName) : QObject(parent)
{
    serial = new QSerialPort(this);
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setParity(QSerialPort::NoParity);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);



    serial->open(QSerialPort::ReadWrite);
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
    if(data.count() >= 2) return (quint16)data.at(0) + ((quint16)data.at(1) << 8);
    else return 0;
}

bool rbtServerArduinoCom::checkCRC(QByteArray data)
{
    if(data.count() >= 2) return true;
    else return false;
}



void rbtServerArduinoCom::sendStepperConfig(enumStepControl stepControl, bool resetLeft, bool resetRight)
{

}

void rbtServerArduinoCom::sendStepperCommand(enumStepperCommandMode commandMode,
                                             uint accelerationLeft, uint accelerationRight,
                                             uint speedLeft, uint speedRight,
                                             int valueLeft, int valueRight)
{

}

void rbtServerArduinoCom::sendLidarSpeed(uint speed)
{

}



