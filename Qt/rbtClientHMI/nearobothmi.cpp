#include "nearobothmi.h"
#include "ui_nearobothmi.h"

NEArobotHMI::NEArobotHMI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NEArobotHMI)
{
    ui->setupUi(this);
}

NEArobotHMI::~NEArobotHMI()
{
    delete ui;
}
