#ifndef RECIEVER_H
#define RECIEVER_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

class Reciever : public QObject
{
    Q_OBJECT
public:
    explicit Reciever(QObject *parent = 0);
    void start(QHostAddress ip);
    void stop();

private:
    QUdpSocket* udpSocket;
    QByteArray datagram;
    uchar* temp;
    int offset;
    int port;

    QHostAddress ip;
    
    QByteArray starter_str = QByteArray::fromHex("5446367A600200000000000303010026000000000234C2");
    QUdpSocket* reinitSocket;
    QTimer* reinitClock;

signals:
    void imageReceived(QByteArray data);
    
public slots:
    void processIncomingData();
    void reinit();
    
};

#endif // RECIEVER_H
