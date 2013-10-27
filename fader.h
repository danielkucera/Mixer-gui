#ifndef FADER_H
#define FADER_H

#include <QMainWindow>
#include "buffer.h"
#include "unistd.h"
#include <cmath>

namespace Ui {
class Fader;
}

class Fader : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Fader(QWidget *parent = 0);
    ~Fader();

    void Init(Buffer* buffer);
    void setRate(int rate);


    
private:
    Ui::Fader *ui;
    float rate;
    float rateSup;
    Buffer* buffer;
    void start();
    int pos;
    int run;

    pthread_t thread;
    void Thread();
    static void * staticEntryPoint(void * c);

    unsigned char* a;
    unsigned char* b;
    unsigned char* out;

private slots:

    void controlClicked();
    void controlChanged(int value);

};

#endif // FADER_H
