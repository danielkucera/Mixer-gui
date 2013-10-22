#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include "capture.h"
#include "preview.h"

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
    void startCapture(int state);
    void startPreview(int state);
    void handleSlider(int);

    
private:
    Ui::MainWindow *ui;
    QPushButton *m_button;
    Capture* foo[10];
    Preview* prev[10];
};

#endif // MAINWINDOW_H
