#include "overlayworker.h"

OverlayWorker::OverlayWorker(QObject *parent) :
    QObject(parent)
{

}

void OverlayWorker::start(Buffer* buf, int outNumbe, int numberA, int numberOver){
    buffer=buf;
    output=(uchar*)buffer->Open(outNumbe);
    inputA=(uchar*)buffer->Open(numberA);
    inputOver=(uchar*)buffer->Open(numberOver);
    outNumber=outNumbe;

    connect(buffer->clock,SIGNAL(timeout()),this,SLOT(processFrame()));
}

void OverlayWorker::setColor(QColor farba){
    red=farba.red();
    green=farba.green();
    blue=farba.blue();
}

void OverlayWorker::setSensitivity(int sensitivit){
    sensitivity=sensitivit;
}

void OverlayWorker::setMethod(int metho){
    method=metho;
}

void OverlayWorker::processFrame(){

    if (method==0){
        for (int i=0; i < buffer->buf_len; i+=4){
            int mask=inputOver[i+3];
            int invMask=255-mask;
            output[i]=((inputA[i]*invMask) + (inputOver[i]*mask))/256;
            output[i+1]=((inputA[i+1]*invMask) + (inputOver[i+1]*mask))/256;
            output[i+2]=((inputA[i+2]*invMask) + (inputOver[i+2]*mask))/256;
            output[i+3]=255;

        }
    } else {

        memcpy(output, inputA , buffer->buf_len);

            for (int i=0; i < buffer->buf_len; i+=4){

                //ptr=;

                if ((qAbs(blue-inputOver[i]) > sensitivity) || (qAbs(green-inputOver[i+1]) > sensitivity) || (qAbs(red-inputOver[i+2]) > sensitivity)){
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
