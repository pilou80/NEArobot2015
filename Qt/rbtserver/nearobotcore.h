#ifndef NEAROBOTCORE_H
#define NEAROBOTCORE_H

#include <QObject>
#include "rbtserversocket.h"
#include "rbtserverarduinocom.h"

class NEARobotCore : public QObject
{
    Q_OBJECT
public:
    explicit NEARobotCore(QObject *parent = 0);
    void start();
signals:

public slots:

private:
    rbtserversocket *server;
    rbtServerArduinoCom *arduino;
};

#endif // NEAROBOTCORE_H
