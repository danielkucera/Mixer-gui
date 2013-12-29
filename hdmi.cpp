#include "hdmi.h"
#include "ui_hdmi.h"

HDMI::HDMI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HDMI)
{
    ui->setupUi(this);

    connect(ui->enableBox, SIGNAL(stateChanged(int)), this, SLOT(enableHDMI(int)));
    connect(this,SIGNAL(imgRdy(void* ,int)),this,SLOT(process(void* ,int)));

}

void HDMI::Init(Buffer* bufin)
{
    buffer = bufin;
}


HDMI::~HDMI()
{

    delete ui;
}


void HDMI::reinit(){

    reinitSocket->write(starter_str.data(),starter_str.length());

}


void HDMI::enableHDMI(int status){
    if (status==2){

        QHostAddress ip = QHostAddress(ui->ipEdit->text());

        reinitSocket = new QUdpSocket();
        reinitSocket->connectToHost(ip,48689);

        reinitClock = new QTimer(this);
        reinitClock->start(500);

        connect(reinitClock,SIGNAL(timeout()),this,SLOT(reinit()));



        udpSocket = new QUdpSocket(this);

        int port = 10500+ip.toIPv4Address()%256;

        qDebug() <<port;

        udpSocket->bind( QHostAddress("0.0.0.0"), port, QUdpSocket::ShareAddress);

        number = ui->outputSelect->value();

        dest = buffer->Open(number);

        input = malloc(500*1024); //500kb jpeg
        temp = malloc(500*1024); //500kb jpeg

        connect(udpSocket,SIGNAL(readyRead()),this,SLOT(pktReceive()));

    }else{
        udpSocket->close();
        reinitClock->stop();
        reinitSocket->close();
    }
}

void HDMI::pktReceive(){

    while (udpSocket->hasPendingDatagrams()) {

        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        memcpy(temp + offset, datagram.data()+4,datagram.size()-4);

        offset += datagram.size()-4;

        if (datagram.at(2)!=0){
            memcpy(input,temp,offset);

            emit(imgRdy(input,offset));

            offset=0;
        }

    }
}

void HDMI::process(void* inp,int off){

    QImage rgb = QImage::fromData((uchar*)inp, off).scaled(buffer->width, buffer->height).convertToFormat(QImage::Format_RGB888);

    memcpy(dest,rgb.bits(),rgb.byteCount());

    emit(buffer->newFrame(number));

}
