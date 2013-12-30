#include "hdmi.h"
#include "ui_hdmi.h"

HDMI::HDMI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HDMI)
{
    ui->setupUi(this);

    this->moveToThread(QApplication::instance()->thread());

    connect(ui->enableBox, SIGNAL(stateChanged(int)), this, SLOT(enableHDMI(int)));

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

        udpSocket = new QUdpSocket();

        QThread* thread = new QThread(this);
        //udpSocket->moveToThread(thread);
        //QObject::connect(thread, SIGNAL(started()), myObj, SLOT(run()));
        thread->start();


        udpSocket->moveToThread(QApplication::instance()->thread());

        int port = 10500+ip.toIPv4Address()%256;

        qDebug() <<port;

        udpSocket->bind( QHostAddress("0.0.0.0"), port, QUdpSocket::ShareAddress);

        number = ui->outputSelect->value();

        dest = buffer->Open(number);

        input = malloc(500*1024); //500kb jpeg
        temp = malloc(500*1024); //500kb jpeg

        QTimer* recvClock = new QTimer(this);
        recvClock->start(5);

        //connect(udpSocket,SIGNAL(readyRead()),this,SLOT(pktReceive()));

        //je to smutne ale toto je efektivnejsie
        connect(recvClock,SIGNAL(timeout()),this,SLOT(pktReceive()));

    }else{
        udpSocket->close();
        reinitClock->stop();
        reinitSocket->close();
    }
}

void local_error_exit (jpeg_common_struct* cinfo)
{
  char msg[1024];
  qDebug()<<"sme tu";
  sprintf (msg, "error_exit: %d", cinfo->err->msg_code);

  // std::cout << "jpeg: local_error_exit: " << msg << std::endl;
  // You can do something with this error_code if needed
  //  int error_code = cinfo->err->msg_code;
  throw 128;
}

void HDMI::process(void* inp,int off){

    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;

    JSAMPARRAY jpeg_buffer;		/* Output row buffer */
    int row_stride;		/* physical row width in output buffer */

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = local_error_exit;

    if (setjmp(jerr.setjmp_buffer)) {

        jpeg_destroy_decompress(&cinfo);
        return;
    }

    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo, (uchar*)inp,off);

    (void) jpeg_read_header(&cinfo, TRUE);

    cinfo.dct_method = JDCT_FASTEST;
    cinfo.two_pass_quantize = FALSE;

    (void) jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;

    jpeg_buffer = (*cinfo.mem->alloc_sarray)
          ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    int counter=0;
    while (cinfo.output_scanline < cinfo.output_height) {

      (void) jpeg_read_scanlines(&cinfo, jpeg_buffer, 1);
      memcpy(dest + counter,jpeg_buffer[0],row_stride);
      counter += row_stride;
    }

    emit(buffer->newFrame(number));

    (void) jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);


//    QImage rgb = QImage::fromData((uchar*)inp, off).scaled(buffer->width, buffer->height).convertToFormat(QImage::Format_RGB888);
//    memcpy(dest,rgb.bits(),rgb.byteCount());

}

void HDMI::pktReceive(){

    while (udpSocket->state()==QAbstractSocket::BoundState && udpSocket->hasPendingDatagrams() ) {

        datagram.resize(udpSocket->pendingDatagramSize());

        udpSocket->readDatagram(datagram.data(), datagram.size());

        int leng = 1020*(uchar)datagram.at(3);

        memcpy(temp + leng, datagram.data()+4,1020);

        if (datagram.at(2)!=0){

            leng+=1020;

            memcpy(input,temp,leng);

            //trosku velmi pomale
            //QtConcurrent::run(this, &HDMI::process, temp, leng);

            //toto sa stiha ak je pouzity turbojpeg, zatial neriesim, staci to
            process(temp,leng);

            offset=0;
        }

    }
}


