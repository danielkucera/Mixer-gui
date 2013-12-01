#ifndef HDMI_H
#define HDMI_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QRunnable>
#include <QThreadPool>
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

    int offset;
    QImage obraz;
    void* dest;
    int number;

    pthread_t thread;
    static void * staticEntryPoint(void * c);
    void loop();

    void* input;
    void* temp;


    QUdpSocket *udpSocket;

    QAbstractSocket *allSocket;

    void reinit(QHostAddress host);

    int s; /*socketdescriptor*/


private slots:
    void enableHDMI(int status);
    void process(void* inp,int off);
    void pktReceive();

signals:
    void imgRdy(void* inp,int off);

};

#endif // HDMI_H
