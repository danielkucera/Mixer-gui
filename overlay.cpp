#include "overlay.h"
#include "ui_overlay.h"

Overlay::Overlay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overlay)
{
    ui->setupUi(this);

    connect(ui->enableBox, SIGNAL(stateChanged(int)), this, SLOT(setEnabled(int)));
    connect(ui->colorButton, SIGNAL(released()),this,SLOT(showColorDlg()));
}

void Overlay::Init(Buffer *buf){
    buffer = buf;
}

void Overlay::showColorDlg(){
    QColor selectColor = QColorDialog::getColor(Qt::black, this);
    color = selectColor.red()*65536 + selectColor.green()*256 + selectColor.blue();

    QImage screen = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();

    QRgb pixel = screen.pixel(0,80);


    color = pixel.re *65536 + pixel.green()*256 + pixel.blue();

    fprintf(stderr, "color  %x\n",color);


}

void Overlay::setEnabled(int status){
    if (status==2){
//        startWorker();
        inputA = (uchar*)buffer->Open(ui->inputNumber->value());
        inputOver = (uchar*)buffer->Open(ui->inputOver->value());

        uchar sens = 0xFF << ui->sensSlider->value();
        sensitivity = sens + sens*256 + sens*65536;

        fprintf(stderr, "sensi  %x\n",sensitivity);

        if (ui->radioArray->isChecked()){ //mask from array
            method=0;
            inputMask = (uchar*)buffer->Open(ui->inputOver->value()+1);
        } else {
            method=1;
        }

        output = (uchar*)buffer->Open(ui->outputNumber->value());

        processFrame();
    } else {
        run = 0;
    }
}

void Overlay::processFrame(){
    if (method==0){
        for (int i=0; i < buffer->buf_len; i++){
            output[i]=inputA[i]*(1-(float)inputMask[i/3]/255) + inputOver[i]*(float)inputMask[i/3]/255;
        }
    } else {
        int pixel=0;
        // tak zatial to urobim skaredo....
        //int* inputOverInt = inputOver;
        for (int i=0; i < buffer->buf_len; i+=3){
            //memcpy(pixel*+1,inputOver[i],3);
            pixel=65536*inputOver[i]+255*inputOver[i+1]+inputOver[i+2];
            uchar* src = !((pixel^color)) ? &inputA[i] : &inputOver[i];
            memcpy(&output[i], src , 3);
        }
    }
}

Overlay::~Overlay()
{
    run = 0;
    delete ui;
}
