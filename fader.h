#ifndef FADER_H
#define FADER_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QRunnable>
#include <QThreadPool>
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
    void setFader(int rate);
    void setYellow(int next);
    void setRed(int next);
    void autofade();


    
private:
    Ui::Fader *ui;
    float rate;
    Buffer* buffer;
    void start();
    int pos;
    int run;

    pthread_t thread;
    void Thread();
    static void * staticEntryPoint(void * c);

    uchar* red;
    uchar* yel;
    uchar* out;

    int output_number;

private slots:

    void makeFrame();
    void controlClicked();
    void controlChanged(int value);

protected:
    void keyPressEvent(QKeyEvent *);

};

#endif // FADER_H
