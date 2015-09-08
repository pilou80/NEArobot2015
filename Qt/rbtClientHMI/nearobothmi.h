#ifndef NEAROBOTHMI_H
#define NEAROBOTHMI_H

#include <QWidget>

namespace Ui {
class NEArobotHMI;
}

class NEArobotHMI : public QWidget
{
    Q_OBJECT

public:
    explicit NEArobotHMI(QWidget *parent = 0);
    ~NEArobotHMI();

private slots:

private:
    Ui::NEArobotHMI *ui;

};

#endif // NEAROBOTHMI_H
