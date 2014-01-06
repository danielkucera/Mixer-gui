#ifndef OVERLAYWORKER_H
#define OVERLAYWORKER_H

#include <QObject>
#include <buffer.h>
#include <QColor>

class OverlayWorker : public QObject
{
    Q_OBJECT
private:
    int red = 0;
    int green = 0;
    int blue = 0;

    int method=0;

    uchar* inputA;
    uchar* inputOver;
    uchar* inputMask;
    uchar* output;

    int outNumber;

    Buffer* buffer;

    int sensitivity=1;

public:
    explicit OverlayWorker(QObject *parent = 0);
    void start(Buffer* buf, int outNumber, int numberA, int numberOver);
    void setColor(QColor farba);
    void setSensitivity(int sensitivity);
    void setMethod(int method);
    
signals:
    
public slots:
    void processFrame();
    
};

#endif // OVERLAYWORKER_H
