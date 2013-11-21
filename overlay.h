#ifndef OVERLAY_H
#define OVERLAY_H

#include <QMainWindow>
#include "buffer.h"
#include <QColorDialog>

namespace Ui {
class Overlay;
}

class Overlay : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Overlay(QWidget *parent = 0);
    ~Overlay();

    void Init(Buffer* buf);
    void processFrame();
    
private:
    Ui::Overlay *ui;

    uchar* inputA;
    uchar* inputOver;
    uchar* inputMask;
    uchar* output;

    int method;
    int color=0;
    int sensitivity=0xF8F8F8;

    Buffer* buffer;

    int run;

private slots:
    void setEnabled(int status);
    void showColorDlg();
};

#endif // OVERLAY_H
