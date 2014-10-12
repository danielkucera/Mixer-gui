#ifndef EXTERNALINPUT_H
#define EXTERNALINPUT_H

#include <QMainWindow>
#include <QProcess>
#include <QThread>
#include <QDebug>
#include <buffer.h>

namespace Ui {
class ExternalInput;
}

class ExternalInput : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExternalInput(QWidget *parent = 0);
    ~ExternalInput();

    void Init(Buffer *buf);

private:
    Ui::ExternalInput *ui;

    QProcess *myProcess;
    Buffer* buffer;
    char* output;
    int outNum;
    QByteArray data;

private slots:
    void enable(int status);
    void printOutput();
    void fillBuffer();
    void newFrame();
    void processExit();
    void on_comboBox_currentTextChanged(const QString &arg1);
};

#endif // EXTERNALINPUT_H
