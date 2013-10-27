#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>

class Buffer
{
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

private:
    int isopen[32];


};

#endif // BUFFER_H
