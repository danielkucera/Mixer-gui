#include "overlay.h"
#include "ui_overlay.h"
#include "QDebug"

Overlay::Overlay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overlay)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    connect(ui->enableBox, SIGNAL(stateChanged(int)), this, SLOT(setEnabled(int)));
    connect(ui->colorButton, SIGNAL(released()),this,SLOT(showColorDlg()));
    connect(ui->pickButton, SIGNAL(released()),this,SLOT(pickScreenColor()));
    connect(ui->sensSlider, SIGNAL(valueChanged(int)),this,SLOT(setSensitivity(int)));

}

void Overlay::Init(Buffer *buf){
    buffer = buf;
}

void Overlay::setSensitivity(int newSens){
    sensitivity = newSens;
    qDebug() << "sensit" << sensitivity;
}

void Overlay::setEnabled(int status){
    if (status==2){
//        startWorker();
        inputA = (uchar*)buffer->Open(ui->inputA->value());
        inputOver = (uchar*)buffer->Open(ui->inputOver->value());

        sensitivity = ui->sensSlider->value();

        fprintf(stderr, "sensi  %x\n",sensitivity);

        if (ui->radioArray->isChecked()){ //mask from array
            method=0;
            inputMask = (uchar*)buffer->Open(ui->inputOver->value()+1);
        } else {
            method=1;
        }

        outNumber = ui->outputNumber->value();

        output = (uchar*)buffer->Open(outNumber);

        connect(buffer->clock,SIGNAL(timeout()),this,SLOT(processFrame()));

    } else {
        disconnect(buffer->clock,SIGNAL(timeout()),this,SLOT(processFrame()));
    }
}


void Overlay::setColor(QColor farba){

    color = farba;

    QPalette paleta;

    paleta.setColor(QPalette::Background, farba);

    ui->colorFrame->setPalette(paleta);

    ui->colorFrame->update();

    qDebug()<< "color" << color;
}


void Overlay::showColorDlg(){
    setColor(QColorDialog::getColor(Qt::black, this));

}

QColor Overlay::grabScreenColor(const QPoint &p)
{
    QDesktopWidget *desktop = QApplication::desktop();
    QPixmap pixmap = QGuiApplication::screens().at(desktop->screenNumber())->grabWindow(desktop->winId(), p.x(), p.y(), 1, 1);
    QImage i = pixmap.toImage();
    return i.pixel(0, 0);
}

void Overlay::pickScreenColor()
{
    screenColorPicking=true;
    /*For some reason, q->grabMouse(Qt::CrossCursor) doesn't change
     * the cursor, and therefore I have to change it manually.
     */
    grabMouse();
    setCursor(Qt::CrossCursor);
    /* With setMouseTracking(true) the desired color can be more precisedly picked up,
     * and continuously pushing the mouse button is not necessary.
     */
    setMouseTracking(true);

    qDebug()<<grabScreenColor(QCursor::pos());
}

void Overlay::mouseReleaseEvent(QMouseEvent *e)
{
    if (screenColorPicking) {

        setColor(grabScreenColor(e->globalPos()));

        screenColorPicking=false;
        setCursor(Qt::ArrowCursor);
        setMouseTracking(false);
        releaseMouse();
        return;
    }
}

void Overlay::processFrame(){

    int red = color.red();
    int green = color.green();
    int blue = color.blue();

    if (method==0){
        for (int i=0; i < buffer->buf_len; i++){

            output[i]=inputA[i]*(1-(float)inputMask[i/3]/255) + inputOver[i]*(float)inputMask[i/3]/255;
        }
    } else {

        memcpy(output, inputA , buffer->buf_len);

            for (int i=0; i < buffer->buf_len; i+=3){

                //ptr=;

                if ((qAbs(red-inputOver[i]) > sensitivity) || (qAbs(green-inputOver[i+1]) > sensitivity) || (qAbs(blue-inputOver[i+2]) > sensitivity)){
                    memcpy(&output[i], &inputOver[i] , 3);
                }

                ////memcpy(pixel*+1,inputOver[i],3);
                //pixel=qAbs(color.red()-inputOver[i]) + qAbs(color.green()-inputOver[i+1]) + qAbs(color.blue()-inputOver[i+2]);
                //uchar* src = (pixel<sensitivity) ? &inputA[i] : &inputOver[i];
                //memcpy(&output[i], src , 3);

            }
        }

        buffer->newFrame(outNumber);
}

Overlay::~Overlay()
{
    run = 0;
    delete ui;
}
