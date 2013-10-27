#ifndef CAPTURE_H
#define CAPTURE_H

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
#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include <time.h>               //profiling

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

    Buffer* buffer;
    
private:
    Ui::Capture *ui;
    void* window;
    void startCapture();
    void stopCapture();
    int run;
    int outnum;



    int fd;
    pthread_t thread;
    int xioctl(int, void*);
    int read_frame();
    static void * staticEntryPoint(void * c);
    void * loop();
    void logerr(const char *);

private slots:
    void handleCheckbox(int status);

};

#endif // CAPTURE_H
