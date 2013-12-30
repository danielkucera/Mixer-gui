#ifndef HDMI_H
#define HDMI_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QRunnable>
#include <QtConcurrent/QtConcurrent>
#include "buffer.h"

#define JPEG_LIB_VERSION 80
#include "jpeglib.h"
#include <setjmp.h>



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

    int offset;
    void* dest;
    int number;

    void* input;
    void* temp;

    QUdpSocket *udpSocket;

    QByteArray starter_str =  QByteArray::fromHex("5446367A600200000000000303010026000000000234C2");
    QUdpSocket* reinitSocket;
    QTimer* reinitClock;

    struct my_error_mgr {
      struct jpeg_error_mgr pub;        /* "public" fields */

      jmp_buf setjmp_buffer;    /* for return to caller */
    };

    typedef struct my_error_mgr * my_error_ptr;

     QByteArray datagram;

     void process(void* inp, int off);

     void startSocket(int port);

private slots:
    void enableHDMI(int status);
    void pktReceive();
    void reinit();

signals:
    void imgRdy(void* inp,int off);

};

#endif // HDMI_H
