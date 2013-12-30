#ifndef HDMI_H
#define HDMI_H

#include <QMainWindow>
#include <QRunnable>
#include <QThread>
#include "buffer.h"
#include "reciever.h"

#define JPEG_LIB_VERSION 80
#include "jpeglib.h"
#include <setjmp.h>

struct my_error_mgr {
  struct jpeg_error_mgr pub;        /* "public" fields */

  jmp_buf setjmp_buffer;    /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;


namespace Ui {
class HDMI;
}

class HDMI : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit HDMI(QWidget *parent = 0);
    ~HDMI();
    void Init(Buffer* buffer);
    
private:
    Ui::HDMI *ui;

    Buffer* buffer;
    Reciever* recvr;

    void* dest;
    int number;

    int frame_cnt=0;
    int old_f = 0;
    int jpg_w=0;
    int jpg_h=0;

    //void local_error_exit (jpeg_common_struct* cinfo);

private slots:
    void enableHDMI(int status);
    void process(QByteArray image);
    void updateStatusbar();

signals:
    void imgRdy(void* inp,int off);

};

#endif // HDMI_H
