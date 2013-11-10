#ifndef LOAD_H
#define LOAD_H

#include <QMainWindow>
#include <QFileDialog>
#include "buffer.h"

namespace Ui {
class Load;
}

class Load : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Load(QWidget *parent = 0);
    ~Load();

    void Init(Buffer* buffer);
    int loadImage(QString fileName, int number);
    
private:
    Ui::Load *ui;
    Buffer* buffer = 0;

private slots:
    void loadButton();
};

#endif // LOAD_H
