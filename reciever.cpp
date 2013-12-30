#include "reciever.h"

Reciever::Reciever(QObject *parent) :
    QObject(parent)
{

}

void Reciever::start(QHostAddress ipe){
    port = 10500 + ipe.toIPv4Address()%256;
    ip = ipe;

    reinitSocket = new QUdpSocket(this);

    reinitClock = new QTimer(this);
    reinitClock->start(500);

    connect(reinitClock,SIGNAL(timeout()),this,SLOT(reinit()));

    udpSocket = new QUdpSocket(this);

    udpSocket->bind( QHostAddress("0.0.0.0"), port, QUdpSocket::ShareAddress);

    temp = (uchar*)malloc(500*1024); //500kb jpeg

    QTimer* recvClock = new QTimer(this);
    recvClock->start(5);

    //connect(udpSocket,SIGNAL(readyRead()),this,SLOT(pktReceive()));

    //je to smutne ale toto je efektivnejsie
    connect(recvClock,SIGNAL(timeout()),this,SLOT(processIncomingData()));
}

void Reciever::reinit(){

    reinitSocket->writeDatagram(starter_str,ip, 48689);

}


void Reciever::processIncomingData() {

    while (udpSocket->state()==QAbstractSocket::BoundState && udpSocket->hasPendingDatagrams() ) {

        datagram.resize(udpSocket->pendingDatagramSize());

        udpSocket->readDatagram(datagram.data(), datagram.size());

        int leng = 1020*(uchar)datagram.at(3);

        memcpy(temp + leng, datagram.data()+4,1020);

        if (datagram.at(2)!=0){

            leng+=1020;

            //trosku velmi pomale
            //QtConcurrent::run(this, &HDMI::process, temp, leng);

            //toto sa stiha ak je pouzity turbojpeg, zatial neriesim, staci to
            //process(temp,leng);

            emit(imageReceived(QByteArray((char*)temp,leng)));

            offset=0;
        }

    }
}

void Reciever::stop(){
    udpSocket->close();
    reinitClock->stop();
    reinitSocket->close();
}
