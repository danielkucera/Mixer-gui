#include "preview.h"
#include "ui_preview.h"

Preview::Preview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Preview)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
}

Preview::~Preview()
{

    disconnect();
    delete ui;

}

void Preview::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   if (buffer == 0){
       return;
   }

   ui->mainwidget->width();

   int h2 = (ui->mainwidget->width()*buffer->height)/buffer->width;
   int w2 = (ui->mainwidget->height()*buffer->width)/buffer->height;

   //fprintf(stderr,"inoked ok!\n");


   if (h2<ui->mainwidget->height()){
       w2 = ui->mainwidget->width();
   } else {
       h2 = ui->mainwidget->height();
   }

   ui->imgLabel->setFixedHeight(h2);
   ui->imgLabel->setFixedWidth(w2);

   QPoint center = ui->imgLabel->parentWidget()->geometry().center();

   center.setX(center.x()-w2/2);
   center.setY(center.y()-h2/2);

   ui->imgLabel->move(center);
}


void Preview::start(Buffer *buf, int numbe)
{

    buffer = buf;
    number = numbe;

    QResizeEvent* resize;
    resizeEvent(resize);

    setWindowTitle(QString("Buffer %1").arg(numbe));

    input = buffer->Open(number);

    connect(buffer,SIGNAL(newFrameSignal(int)),this,SLOT(showFrame(int)));

    showFrame(number);

}

void Preview::showFrame(int numbe){
    if (number==numbe){

        QImage myImage((uchar *)input,buffer->width, buffer->height, QImage::Format_RGB32);

        ui->imgLabel->setPixmap(QPixmap::fromImage(myImage.scaled(ui->imgLabel->width(),ui->imgLabel->height())));

    }
}


