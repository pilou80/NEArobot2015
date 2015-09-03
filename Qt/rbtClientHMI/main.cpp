#include "nearobothmi.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NEArobotHMI w;
    w.show();

    return a.exec();
}
