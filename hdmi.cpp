#include "hdmi.h"
#include "ui_hdmi.h"

#include "fstream"

#include <unistd.h>

#define ETH_FRAME_LEN 1518

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
    QThreadPool::globalInstance()->children()->stop();
    delete ui;
}


class ReinitTask : public QRunnable
{
    QHostAddress host;
    QObject* parent;
    int dorun = 1;

public: void Init(QHostAddress hos){
        host = hos;
    }

public: void stop(){
        dorun = 0;
    }

    void run()
    {

        QByteArray starter_str =  QByteArray::fromHex("5446367A600200000000000303010026000000000234C2");

        QUdpSocket* udpSocketSend = new QUdpSocket();
        udpSocketSend->connectToHost(host,48689);

        while(dorun){
            udpSocketSend->write(starter_str.data(),starter_str.length());
            usleep(500*1000);
        }

    }
};


void HDMI::enableHDMI(int status){
    if (status==2){

        udpSocket = new QUdpSocket(this);
        //udpSocket->bind(QHostAddress::AnyIPv4, 2068, QUdpSocket::ShareAddress);
        udpSocket->bind( QHostAddress("0.0.0.0"), 2068, QUdpSocket::ShareAddress);
        //fprintf(stderr,"pustam %d\n",udpSocket->BoundState);

        udpSocket->joinMulticastGroup(QHostAddress("226.2.2.2"));

        s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));


        if (s == -1) {
            qDebug() << "nepodarilo sa open socket\n";
            //errorhandling ...
        } else {

            run=1;

            number = ui->outputSelect->value();

            dest = buffer->Open(number);

            input = malloc(500*1024); //500kb jpeg
            temp = malloc(500*1024); //500kb jpeg

            ReinitTask *reinit = new ReinitTask();
            reinit->Init(QHostAddress(ui->ipEdit->text()));
            // QThreadPool takes ownership and deletes 'hello' automatically
            QThreadPool::globalInstance()->start(reinit);

            //connect(udpSocket,SIGNAL(readyRead()),this,SLOT(pktReceive()));

            pthread_create(&thread, NULL, HDMI::staticEntryPoint, this);

        }

    }else{
        udpSocket->close();
        run=0;
    }
}


void * HDMI::staticEntryPoint(void * c)
{
    ((HDMI *) c)->loop();
    return NULL;
}

void HDMI::pktReceive(){

    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize()+8);
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        offset += datagram.size();

        memcpy(temp + offset, datagram.data()+4,datagram.size()-4);

        qDebug() << datagram.at(3);

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

void HDMI::loop(){

    void* input = malloc(500*1024); //500kb jpeg
    void* temp = malloc(500*1024); //500kb jpeg

    int length = 0; /*length of the received frame*/


    int dlzka=0;


    QHostAddress* host  = new QHostAddress(ui->ipEdit->text());
    int host_ip = host->toIPv4Address();

    while(run){

        length = recvfrom(s, packet, ETH_FRAME_LEN, 0, NULL, NULL);

        if (length != -1) {

            int ip = packet[26]*16777216+packet[27]*65536+packet[28]*256+packet[29];


            if (( ip == host_ip ) && (packet[36]==0x08) && (packet[37]==0x14)){

                if (packet[45]==0){
                    offset=0;
                }

                //42,42 - frame 44 - end; 45 - part

                memcpy(temp + offset, (char*)packet+46,length-46);

                offset += length-46;

                if (packet[44]!=0){
                    memcpy(input,temp,offset);
                    emit(imgRdy(input,offset));

                }



            }
        }
    }
}
