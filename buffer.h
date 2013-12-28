#ifndef BUFFER_H
#define BUFFER_H

#include <QObject>
#include <cstring>
#include <QTimer>

class Buffer : public QObject
{
    Q_OBJECT
public:
    Buffer(int widt, int heigh, int bp, int fps);

    void* Open(int number);

    void* start[32];
    int frame[32]={0};
    int width;
    int height;
    int bpp;
    int fps;
    int buf_len;
    void newFrame(int number);

    QTimer *clock;

private:
    int isopen[32];


signals:
    void newFrameSignal(int number);


};

#endif // BUFFER_H
