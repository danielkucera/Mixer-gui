#ifndef PREVIEW_H
#define PREVIEW_H

#include "stdio.h"
#include "unistd.h"
#include <pthread.h>
#include <thread>
#include <string>
#include <QThread>

class Preview
{
public:
    Preview(void *buffer, int buf_len, int fps, int width, int height);
    ~Preview();
private:
    pthread_t thread;
    void Thread();
    static void * staticEntryPoint(void * c);
    void * buffer;
    int buf_len;
    int fps;
    int width;
    int height;
    FILE *fp;
    bool run;
};

class PreviewThread : public QThread
{
    Q_OBJECT

protected:
    void run();
};

#endif // PREVIEW_H
