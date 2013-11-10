#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include "capture.h"
#include "preview.h"
#include "fader.h"
#include "buffer.h"
#include "save.h"
#include "load.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setName(const QString &name);
    QString name() const;

private slots:
    void startCapture();
    void loadImage();
    void startPreview(int index);
    void startFader();
    void saveBuffer(int index);

    
private:
    Ui::MainWindow *ui;

    Buffer* buffer;
};

#endif // MAINWINDOW_H
