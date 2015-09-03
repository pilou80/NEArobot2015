#include "rbtneatolidar.h"
#include <QDebug>

rbtNeatoLidar::rbtNeatoLidar(QObject *parent, QString portName) : QObject(parent)
{
    for(int i=0; i<360; i++)
        points[i] = (structLidarPoint *)malloc(sizeof(structLidarPoint));

    firmware = "2.4";
    started = false;
    driverReady = false;

    serial = new QSerialPort(this);
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setParity(QSerialPort::NoParity);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);

    connect(serial, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));

    speedReportHMI = new QTimer(this);
    speedReportHMI->setSingleShot(false);
    speedReportHMI->setInterval(500);

    speedReportDriver = new QTimer(this);
    speedReportDriver->setSingleShot(false);
    speedReportDriver->setInterval(100);

    if(!serial->open(QSerialPort::ReadWrite))
        qCritical() << "rbtNeatoLidar::rbtNeatoLidar Unable to open Serial ! " << serial->portName();
}

void rbtNeatoLidar::start()
{
    started = true;
    welcomeReceived = false;
    receivedPoints = 0;
    pointZeroReceived = false;
    emit(setPower(true));
    speedReportHMI->start();
    speedReportDriver->start();
    emit(setSpeedToMotorDriver(targetSpeed));
}

void rbtNeatoLidar::stop()
{
    emit(setPower(false));
    started = false;
    speedReportHMI->stop();
    speedReportDriver->stop();
    emit(setSpeedToMotorDriver(0));
    buffer.clear();
}

void rbtNeatoLidar::setSpeed(float speed)
{
    if(speed < 180
       && speed != 0) qWarning() << "rbtNeatoLidar::setSpeed :" << speed << " is too Slow to work properly";
    if(speed > 320) qWarning() << "rbtNeatoLidar::setSpeed :" << speed << " is too Fast to work properly (UART bottleneck)";

    targetSpeed = convertSpeedToMachine(speed);
    emit(setSpeedToMotorDriver(targetSpeed));
}

void rbtNeatoLidar::speedControlState(void state)
{
    driverReady = state;
    if(started) emit(setSpeedToMotorDriver(targetSpeed));
    else emit(setSpeedToMotorDriver(0));
}

void rbtNeatoLidar::setSpeedReportHMI(uint msec)
{
    speedReportHMI->setInterval(msec);
}

void rbtNeatoLidar::setSpeedReportDriver(uint msec)
{
    speedReportDriver->setInterval(msec);
}

void rbtNeatoLidar::deserialiseLidarPoint(QByteArray data, uint index)
{
    Q_ASSERT(data.count() == 4);
    Q_ASSERT(index < 360);

    structLidarPoint *point = points[index];

    point->distance = (quint16)data.at(0) + (((quint16)data.at(1) & 0x3F) << 8);
    point->invalid = ((quint8)data.at(1) & 0x40) == 0x40;
    point->strengthWarning = ((quint8)data.at(1) & 0x80) == 0x80;
    point->strength = (quint16)data.at(2) + ((quint16)data.at(3) << 8);

    if(index == 0)
    {
        pointZeroReceived = true;
        receivedPoints = 0;
    }
    receivedPoints++;
    if( receivedPoints == 359 )
    {
        if(started) emit(reportFullRotation(pointZeroReceived));
        pointZeroReceived = false;
    }
}

void rbtNeatoLidar::handleReadyRead()
{
    if(!started)
    {
        serial->flush();
    }

    buffer.append(serial->readAll());
    if(!welcomeReceived)
    {

    }
    else if( (firmware == "2.4") || (firmware == "2.6"))
    {
        while(buffer.count() >= 22)
        {
            //Wait for the first byte to be 0xFA and the checksum is correct
            if((quint8)buffer.at(0) == 0xFA &&
                    (checksum(buffer.mid(0,20)) ==
                    ((quint16)buffer.at(20) + ((quint16)buffer.at(21) << 8))))
            {
                quint8 index = (quint8)buffer.at(1);
                if(index < 0xA0 || index > 0xF9)
                {
                    qWarning() << "rbtNeatoLidar::handleReadyRead => index is not in the range :" << index;
                }

                quint16 speed = (quint16)buffer.at(2)
                             + ((quint16)buffer.at(3) << 8);
                lastSpeedRead = speed; //TODO faire une moyenne des mesures (accumulation division lors de l'envoie)

                for(uint i; i<4; i++)
                    deserialiseLidarPoint(buffer.mid(4 + i*4, 4), (index-0xA0) * 4 + i);

                buffer.remove(0, 22);
            }
            else
            {
                buffer.remove(0, 1);
            }
        }
        dropBytes++;
        if(dropBytes>22) qWarning << "rbtNeatoLidar::handleReadyRead too much data Dropped !";
    }
    else
    {
        qCritical() << "rbtNeatoLidar::handleReadyRead firmware insupported :" << firmware;
    }
}

bool rbtNeatoLidar::handleSpeedReportHMI()
{
    emit(reportSpeed(convertSpeedToHumain(lastSpeedRead)));
}

bool rbtNeatoLidar::handleSpeedReportDriver()
{
    emit(reportSpeed(lastSpeedRead));
}

float rbtNeatoLidar::convertSpeedToHumain(quint16 speed)
{
    return ((float)speed) / 64;
}

quint16 rbtNeatoLidar::convertSpeedToMachine(float speed)
{
    return (quint16)(speed * 64);
}

quint16 rbtNeatoLidar::checksum(QByteArray data)
{
    // Compute and return the checksum as an int.
    // group the data by word, little-endian
    QByteArray dataTemp;
    for(t=0; t<10;t++)
        dataTemp.append( data[2*t] + (data[2*t+1]<<8) );

    // compute the checksum on 32 bits
    quint32 chk32 = 0;
    foreach(char d, dataTemp)
        chk32 = (chk32 << 1) + (quint8)d;

    // return a value wrapped around on 15bits, and truncated to still fit into 15 bits
    quint16 checksum;
    checksum = (chk32 & 0x7FFF) + ( chk32 >> 15 ); // wrap around to fit into 15 bits
    checksum = checksum & 0x7FFF; // truncate to 15 bits
    return checksum;
}
