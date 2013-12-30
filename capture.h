#ifndef CAPTURE_H
#define CAPTURE_H

#include <QCamera>
#include <QCameraViewfinder>
#include <QWidget>
#include <QDir>
#include "preview.h"
#include <unistd.h> // close
#include "buffer.h"

//precisti:
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
#include <sys/mman.h>           //mmap types
#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include <time.h>               //profiling


#define CLEAR(x) memset(&(x), 0, sizeof(x))

namespace Ui {
class Capture;
}

class Capture : public QWidget
{
    Q_OBJECT

public:
    explicit Capture(QWidget *parent = 0);
    ~Capture();

    void Init(Buffer *buf);
    Buffer* buf;
    
private:
    Ui::Capture *ui;

    void* out_buf;
    int run;
    void startCapture();
    void stopCapture();
    int outnum;
    int width;
    int height;
    void process_frame(void* input, int length);


    pthread_t thread;
    static void * staticEntryPoint(void * c);

    //capture sample copy
    enum io_method {
            IO_METHOD_READ,
            IO_METHOD_MMAP,
            IO_METHOD_USERPTR
    };
    enum io_method   io = IO_METHOD_MMAP;

    struct buffer_type {
            void   *start;
            size_t  length;
    };

    void * loop();
    void open_device();
    void init_device(void);
    void start_capturing(void);
    void stop_capturing(void);
    void uninit_device(void);
    void close_device(void);
    void logerr(const char *s);
    int read_frame(void);
    int xioctl(int fh, int request, void *arg);
    void init_read(unsigned int buffer_size);
    void init_mmap(void);
    void init_userp(unsigned int buffer_size);

    int fd = -1;
    char            *dev_name;
    struct buffer_type          *buffers;
    unsigned int     n_buffers;
    struct v4l2_format fmt;


private slots:
    void handleCheckbox(int status);

};


#endif // CAPTURE_H
