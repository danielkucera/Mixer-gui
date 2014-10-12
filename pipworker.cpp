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

PIPWorker::~PIPWorker(){

    disconnect(buffer->clock,SIGNAL(timeout()),this,SLOT(processFrame()));
}



void PIPWorker::processFrame(){
    double _scale = scale;
    int _offsetX = offsetX;
    int _offsetY = offsetY;

    int scaleW = buffer->width / _scale;
    int scaleH = buffer->height / _scale;

    memcpy(output, inputBack, buffer->buf_len);

    for (int y = 0; y < scaleH && y + _offsetY < buffer->height; y++){
        for (int x = 0; x < scaleW && x + _offsetX < buffer->width; x++){
            output[(y + _offsetY) * 4 * buffer->width + (x + _offsetX) * 4] = inputPIP[ (int)(y * _scale) * 4 * buffer->width + (int)(x * _scale) * 4];
            output[(y + _offsetY) * 4 * buffer->width + (x + _offsetX) * 4 + 1] = inputPIP[ (int)(y * _scale) * 4 * buffer->width + (int)(x * _scale) * 4 + 1];
            output[(y + _offsetY) * 4 * buffer->width + (x + _offsetX) * 4 + 2] = inputPIP[ (int)(y * _scale) * 4 * buffer->width + (int)(x * _scale) * 4 + 2];
            output[(y + _offsetY) * 4 * buffer->width + (x + _offsetX) * 4 + 3] = inputPIP[ (int)(y * _scale) * 4 * buffer->width + (int)(x * _scale) * 4 + 3];
        }
    }

    buffer->newFrame(outNumber);

}
