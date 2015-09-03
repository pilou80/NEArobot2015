#ifndef RBTNEATOLIDAR_H
#define RBTNEATOLIDAR_H
#include <QString>
#include <QtSerialPort/QSerialPort>

class rbtNeatoLidar : public QObject
{
    Q_OBJECT
public:
    explicit rbtNeatoLidar(QObject *parent = 0, QString serialPort);

signals:
    void reportSpeed(uint speed);
public slots:
};

#endif // RBTNEATOLIDAR_H
