#ifndef RBTNEATOLIDAR_H
#define RBTNEATOLIDAR_H
#include <QString>
#include <QSerialPort>
#include <QTimer>

class rbtNeatoLidar : public QObject
{
    Q_OBJECT
public:
    explicit rbtNeatoLidar(QObject *parent = 0, QString portName = "/dev/ttyUSB0");
    typedef struct
    {
        quint16 distance;
        bool invalid;
        bool strengthWarning;
        quint16 strength;
    } structLidarPoint;

    static float convertSpeedToHumain(quint16 speed);
    static quint16 convertSpeedToMachine(float speed);

    structLidarPoint *points[360];
signals:
    void reportSpeed(quint16 speed);
    void reportSpeed(float speed);
    void setPower(bool state);
    void setSpeedToMotorDriver(quint16 speed);

    //if state == false all point have not been received
    void reportFullRotation(bool state);

public slots:
    void setSpeedReportHMI(uint msec);
    void setSpeedReportDriver(uint msec);
    void start();
    void stop();
    void setSpeed(float speed);
    void speedControlState(void state); //If the speedControler is ready to receive control

private slots:
    void handleReadyRead();
    bool handleSpeedReportHMI();
    bool handleSpeedReportDriver();

private:
    uint dropBytes;
    bool started;
    bool welcomeReceived;
    bool pointZeroReceived;
    uint receivedPoints;
    bool driverReady;

    QSerialPort *serial;
    QTimer *speedReportHMI;
    QTimer *speedReportDriver;


    quint16 lastSpeedRead;
    quint16 targetSpeed;
    QByteArray buffer;
    QString firmware;

    void deserialiseLidarPoint(QByteArray data, uint index);
    quint16 checksum(QByteArray data);
};

#endif // RBTNEATOLIDAR_H
