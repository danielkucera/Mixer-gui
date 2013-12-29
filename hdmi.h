#ifndef HDMI_H
#define HDMI_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QRunnable>
#include "buffer.h"


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

private slots:
    void enableHDMI(int status);
    void process(void* inp,int off);
    void pktReceive();
    void reinit();

signals:
    void imgRdy(void* inp,int off);

};

#endif // HDMI_H
