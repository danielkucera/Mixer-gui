#include "pipworker.h"

PIPWorker::PIPWorker(QObject *parent) :
    QObject(parent)
{

}

void PIPWorker::setParams(double _scale, int _offsetX, int _offsetY){
    scale = _scale;
    offsetX = _offsetX;
    offsetY = _offsetY;
}

void PIPWorker::start(Buffer* buf, int outNumbe, int numBack, int numPIP){
    buffer=buf;
    output=(uchar*)buffer->Open(outNumbe);
    inputBack=(uchar*)buffer->Open(numBack);
    inputPIP=(uchar*)buffer->Open(numPIP);

    outNumber=outNumbe;

    connect(buffer->clock,SIGNAL(timeout()),this,SLOT(processFrame()));
}


void PIPWorker::processFrame(){
    scaleW = buffer->width / scale;
    scaleH = buffer->height / scale;

    memcpy(output, inputBack, buffer->buf_len);

    for (int y = 0; y < scaleH; y++){
        for (int x = 0; x < scaleW; x++){
            output[(y + offsetY) * 4 * buffer->width + (x + offsetX) * 4] = inputPIP[ (int)(y * scale) * 4 * buffer->width + (int)(x * scale) * 4];
            output[(y + offsetY) * 4 * buffer->width + (x + offsetX) * 4 + 1] = inputPIP[ (int)(y * scale) * 4 * buffer->width + (int)(x * scale) * 4 + 1];
            output[(y + offsetY) * 4 * buffer->width + (x + offsetX) * 4 + 2] = inputPIP[ (int)(y * scale) * 4 * buffer->width + (int)(x * scale) * 4 + 2];
            output[(y + offsetY) * 4 * buffer->width + (x + offsetX) * 4 + 3] = inputPIP[ (int)(y * scale) * 4 * buffer->width + (int)(x * scale) * 4 + 3];
        }
    }

    buffer->newFrame(outNumber);

}
