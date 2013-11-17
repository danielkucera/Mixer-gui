#include "overlay.h"
#include "ui_overlay.h"

Overlay::Overlay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overlay)
{
    ui->setupUi(this);

    connect(ui->enableBox, SIGNAL(stateChanged(int)), this, SLOT(setEnabled(int)));
}

void Overlay::Init(Buffer *buf){
    buffer = buf;
}

void Overlay::setEnabled(int status){
    if (status==2){
//        startWorker();
        inputA = (uchar*)buffer->Open(ui->inputNumber->value());
        inputOver = (uchar*)buffer->Open(ui->inputOver->value());

        if (1){ //mask from array
            inputMask = (uchar*)buffer->Open(ui->inputOver->value()+1);
        }

        output = (uchar*)buffer->Open(ui->outputNumber->value());

        processFrame();
    } else {
        run = 0;
    }
}

void Overlay::processFrame(){
    for (int i=0; i < buffer->buf_len; i++){
        output[i]=inputA[i]*(1-(float)inputMask[i/3]/255) + inputOver[i]*(float)inputMask[i/3]/255;
    }
}

Overlay::~Overlay()
{
    run = 0;
    delete ui;
}
