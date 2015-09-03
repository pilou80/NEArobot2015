#ifndef RBTSERVERARDUINOCOM_H
#define RBTSERVERARDUINOCOM_H

#include <QObject>
#include <QByteArray>
#include <QtSerialPort/QSerialPort>
#include "ArduinoCom.h"

class rbtServerArduinoCom : public QObject
{
    Q_OBJECT
public:
    explicit rbtServerArduinoCom(QObject *parent = 0, QString portName = "/dev/ttyUSB0");

signals:

public slots:
    void sendLidarSpeed(uint speed);
    void sendStepperConfig(enumStepControl stepControl, bool resetLeft=false, bool resetRight=false);
    void sendStepperCommand(enumStepperCommandMode commandMode,
                            uint accelerationLeft, uint accelerationRight,
                            uint speedLeft, uint speedRight,
                            int valueLeft, int valueRight);

private slots:
    void sendFrame(QByteArray data);
    quint16 computeCRC(QByteArray data);
    bool checkCRC(QByteArray data);

private:
    QSerialPort *serial;
};

#endif // RBTSERVERARDUINOCOM_H
