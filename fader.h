#ifndef FADER_H
#define FADER_H

#include <pthread.h>
#include <stdlib.h>
#include "unistd.h"
#include <cstring>

class Fader
{
public:
    Fader(int, void *i_a, void *i_b, int i_fps);
    void * a;
    void * b;
    void * out;
    int rate;
    int buf_len;

private:
    pthread_t thread;
    void Thread();
    static void * staticEntryPoint(void * c);
    int fps;
};

#endif // FADER_H
