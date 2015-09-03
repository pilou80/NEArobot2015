#ifndef NEAROBOTHMI_H
#define NEAROBOTHMI_H

#include <QWidget>
#include <QMultimedia>
#include <QMediaPlayer>

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
    void on_btnPlay_clicked();

    void on_btnStop_clicked();

private:
    Ui::NEArobotHMI *ui;
    QMediaPlayer *player;
    QMediaContent *content;
};

#endif // NEAROBOTHMI_H
