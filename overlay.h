#ifndef OVERLAY_H
#define OVERLAY_H

#include <QMainWindow>
#include "buffer.h"
#include <QColorDialog>
#include <QDesktopWidget>
#include <QScreen>
#include <QMouseEvent>

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

    
private:
    Ui::Overlay *ui;

    uchar* inputA;
    uchar* inputOver;
    uchar* inputMask;
    uchar* output;

    int method;
    QColor color;
    int sensitivity=1;

    int outNumber;

    Buffer* buffer;

    int run;

    void setColor(QColor farba);

    void mouseReleaseEvent(QMouseEvent *e);
    QColor grabScreenColor(const QPoint &p);

    bool screenColorPicking=true;

private slots:
    void setEnabled(int status);
    void setSensitivity(int newSens);
    void showColorDlg();
    void pickScreenColor();
    void processFrame();

};

#endif // OVERLAY_H
