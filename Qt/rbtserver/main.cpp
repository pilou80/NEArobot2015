#include <QCoreApplication>
#include <nearobotcore.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    NEARobotCore robot;
    robot.start();
    return a.exec();
}
