#ifndef PIP_H
#define PIP_H

#include <QMainWindow>
#include <QThread>
#include <buffer.h>
#include <pipworker.h>

namespace Ui {
class PIP;
}

class PIP : public QMainWindow
{
    Q_OBJECT

public:
    explicit PIP(QWidget *parent = 0);
    ~PIP();
    void Init(Buffer *buf);

private:
    Ui::PIP *ui;

    Buffer* buffer;
    PIPWorker* worker;

private slots:
    void setEnabled(int status);
    void setParams(QString);

};

#endif // PIP_H
