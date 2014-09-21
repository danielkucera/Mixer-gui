#ifndef PIPWORKER_H
#define PIPWORKER_H

#include <QObject>
#include <buffer.h>

class PIPWorker : public QObject
{
    Q_OBJECT

public:
    explicit PIPWorker(QObject *parent = 0);

    void setParams(double _scale, int _offsetX, int _offsetY);
    void start(Buffer* buf, int outNumbe, int numBack, int numPIP);
private:
    Buffer* buffer;

    double scale;
    int offsetX;
    int offsetY;
    int scaleH;
    int scaleW;

    uchar* output;
    uchar* inputBack;
    uchar* inputPIP;
    int outNumber;

public slots:
    void processFrame();
};

#endif // PIPWORKER_H
