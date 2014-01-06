#ifndef EXTERNALOUTPUT_H
#define EXTERNALOUTPUT_H

#include <QMainWindow>
#include <QProcess>
#include <QThread>
#include <QDebug>
#include <buffer.h>

namespace Ui {
class ExternalOutput;
}

class ExternalOutput : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ExternalOutput(QWidget *parent = 0);
    ~ExternalOutput();

    void Init(Buffer *buf);
    
private:
    Ui::ExternalOutput *ui;

    QProcess *myProcess;
    Buffer* buffer;
    char* input;

private slots:
    void enable(int status);
    void printOutput();
    void newFrame();
    void processExit();
    void on_comboBox_currentTextChanged(const QString &arg1);
};

#endif // EXTERNALOUTPUT_H
