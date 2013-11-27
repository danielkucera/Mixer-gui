#ifndef BUFFER_H
#define BUFFER_H

#include <QObject>
#include <cstring>

class Buffer : public QObject
{
    Q_OBJECT
public:
    Buffer(int widt, int heigh, int bp);

    void* Open(int number);

    void* start[32];
    int frame[32]={0};
    int fps;
    int width;
    int height;
    int bpp;
    int buf_len;
    void newFrame(int number);

private:
    int isopen[32];

signals:
    void newFrameSignal(int number);


};

#endif // BUFFER_H
