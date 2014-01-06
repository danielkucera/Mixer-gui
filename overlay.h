#ifndef OVERLAY_H
#define OVERLAY_H

#include <QMainWindow>
#include "buffer.h"
#include <QColorDialog>
#include <QDesktopWidget>
#include <QScreen>
#include <QMouseEvent>
#include <overlayworker.h>
#include <QThread>

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

    OverlayWorker* worker;

    Buffer* buffer;

    void mouseReleaseEvent(QMouseEvent *e);
    QColor grabScreenColor(const QPoint &p);

    bool screenColorPicking=true;

    void setColor(QColor farba);

private slots:
    void setEnabled(int status);
    void setSensitivity(int newSens);
    void showColorDlg();
    void pickScreenColor();

};

#endif // OVERLAY_H
