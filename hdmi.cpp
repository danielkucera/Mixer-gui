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
    delete ui;
}

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
            //packet_r = (void*)malloc(ETH_FRAME_LEN); /*Buffer for ethernet frame*/
            run=1;

            dest = buffer->Open(ui->outputSelect->value());

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

void HDMI::process(void* inp,int off){

    QImage rgb = QImage::fromData((uchar*)inp, off).scaled(buffer->width, buffer->height).convertToFormat(QImage::Format_RGB888);

    memcpy(dest,rgb.bits(),rgb.byteCount());

}

void HDMI::loop(){

    void* input = malloc(500*1024); //500kb jpeg
    void* temp = malloc(500*1024); //500kb jpeg

    int length = 0; /*length of the received frame*/

    char* starter = (char*)malloc(2048);
    int dlzka=0;


    std::ifstream infile;
    infile.open ("init2.bin");
    infile.seekg (0, infile.end);
    dlzka = infile.tellg();
    infile.seekg (0, infile.beg);
    infile.read(starter,dlzka);
    infile.close();

    QUdpSocket* udpSocketSend = new QUdpSocket(this);
    QHostAddress* host  = new QHostAddress(ui->ipEdit->text());
    udpSocketSend->connectToHost(*host,48689);
    udpSocketSend->write(starter,dlzka);

//    qDebug() << "poslai sme?\n" <<host->toIPv4Address();

    int host_ip = host->toIPv4Address();
    //char* src_ip = "abcd";

//    qDebug() << host_ip;

//    exit(2);

    while(run){

        length = recvfrom(s, packet, ETH_FRAME_LEN, 0, NULL, NULL);

        if (length != -1) {

            //qDebug() << "toto:" << host_ip;
            int ip = packet[26]*16777216+packet[27]*65536+packet[28]*256+packet[29];

            //qDebug() << "toto2:" << *(uint *)(&packet + 25);
            //qDebug() << "toto2:" << ip;

            //qDebug() << "dump";
            /*
            for (int i = 0; i < 10; i ++) {
                uchar ch = *(uchar *)(&host_ip +i);
                fprintf(stderr,"%x ", ch& 0xff);
            }
            */

/*
            for (int i = 0; i < 50; i ++) {
                uchar ch = packet[i];
                fprintf(stderr,"%x ", ch& 0xff);
            }
            */
            //fprintf(stderr,"\n\n");

            //if ((0==memcmp ( (char*)(packet+26), &host_ip, 4) ) && (packet[36]==0x08) && (packet[37]==0x14)){
            if (( ip == host_ip ) && (packet[36]==0x08) && (packet[37]==0x14)){

                if (packet[45]==0){
                    offset=0;
                }

                if (packet[43]%30==0){
                    udpSocketSend->write(starter,dlzka);
                }

                //qDebug() << "offset" << offset;
                //qDebug() << packet[44] << "-"<<packet[45];

                //42,42 - frame 44 - end; 45 - part

                memcpy(temp + offset, (char*)packet+46,length-46);

                offset += length-46;

                if (packet[44]!=0){
                    memcpy(input,temp,offset);
                    emit(imgRdy(input,offset));

                    //qDebug() << "robim obrazok\n";

                    /*
                    std::ofstream outfile;
                    outfile.open ("0.jpeg");
                    outfile.write ((char*)input,offset);
                    outfile.close();

*/
                    //obraz = QImage::fromData((uchar*)input, offset);
                    //qDebug() << obraz.width();
                    //QImage rgb = obraz.scaled(buffer->width, buffer->height).convertToFormat(QImage::Format_RGB888);

                    //memcpy(dest,rgb.bits(),rgb.byteCount());
                    //rgb = obraz->convertToFormat()
                                //buffer->Open(8)
                }



            }
        }
    }
}
