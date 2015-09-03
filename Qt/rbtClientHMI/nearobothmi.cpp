#include "nearobothmi.h"
#include "ui_nearobothmi.h"

NEArobotHMI::NEArobotHMI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NEArobotHMI)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    player->setVideoOutput(ui->video);


}

NEArobotHMI::~NEArobotHMI()
{
    delete ui;
}

void NEArobotHMI::on_btnPlay_clicked()
{
    if(content != NULL) delete content;
    content = new QMediaContent(QUrl("/storage/external_SD/testvideo.mpg"));
    player->setMedia(content);
}

void NEArobotHMI::on_btnStop_clicked()
{

}
