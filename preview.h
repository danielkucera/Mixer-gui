#ifndef PREVIEW_H
#define PREVIEW_H

#include <QMainWindow>
#include "buffer.h"
#include "unistd.h"

namespace Ui {
class Preview;
}

class Preview : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Preview(QWidget *parent = 0);
    ~Preview();

    void start(Buffer* buf, int number);

    
private:
    Ui::Preview *ui;
    void resizeEvent(QResizeEvent* event);

    pthread_t thread;
    void Thread();
    static void * staticEntryPoint(void * c);

    Buffer* buffer = 0;
    int number;
    void* input;

    FILE *fp;
    bool run;

private slots:
    void showFrame(int number);

};

#endif // PREVIEW_H
