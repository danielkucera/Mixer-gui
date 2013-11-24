#ifndef HDMI_H
#define HDMI_H

#include <QMainWindow>
#include <QUdpSocket>
#include "buffer.h"

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <netinet/in.h>


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

    void* packet_r;
    uchar packet[1600];

    int run;

    void* input;
    int offset;
    QImage obraz;
    void* dest;

    pthread_t thread;
    static void * staticEntryPoint(void * c);
    void loop();


    QUdpSocket *udpSocket;

    QAbstractSocket *allSocket;

    int s; /*socketdescriptor*/


private slots:
    void enableHDMI(int status);
    void process(void* inp,int off);

signals:
    void imgRdy(void* inp,int off);

};

#endif // HDMI_H
