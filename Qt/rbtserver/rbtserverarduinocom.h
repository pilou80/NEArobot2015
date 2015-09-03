#ifndef RBTSERVERARDUINOCOM_H
#define RBTSERVERARDUINOCOM_H

#include <QObject>
#include <QByteArray>
#include <QtSerialPort/QSerialPort>

class rbtServerArduinoCom : public QObject
{
    Q_OBJECT
public:
    explicit rbtServerArduinoCom(QObject *parent = 0, QString serialPort);

signals:

public slots:
    void lidarSpeedReport(uint speed);
};

#endif // RBTSERVERARDUINOCOM_H
