#ifndef CAPTURE_H
#define CAPTURE_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>             /* getopt_long() */
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include <time.h>               //profiling


class Capture
{
public:
    Capture(char*, int, int);
    int width;
    int height;
    void * buffer;
    size_t buf_len;
    int initialized=0;

private:
    int fd;
    pthread_t thread;
    int xioctl(int, void*);
    int read_frame();
    static void * staticEntryPoint(void * c);
    void * loop();
    void logerr(const char *);

};

#endif // CAPTURE_H
