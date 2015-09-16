#include "nearobotcore.h"


NEARobotCore::NEARobotCore(QObject *parent) : QObject(parent)
{
    server = new rbtserversocket(this, "NEArobot", 1236);
    arduino = new rbtServerArduinoCom(this, "/dev/ttyUSB0");

    logger = new rbtLogger(this, true);
}

void NEARobotCore::start()
{

}
