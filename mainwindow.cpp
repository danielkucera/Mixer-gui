#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdio.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Create the button, make "this" the parent
    //m_button = ui->pushButton;
    // set size and location of the button
    //m_button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));

    // Connect button signal to appropriate slot

    ui->setupUi(this);

    connect(ui->enableInput0, SIGNAL(stateChanged(int)), this, SLOT(startCapture(int)));
    connect(ui->preview0, SIGNAL(stateChanged(int)), this, SLOT(startPreview(int)));


    connect(ui->checkBox_4, SIGNAL(stateChanged(int)), this, SLOT(startFader(int)));
    connect(ui->checkBox_5, SIGNAL(stateChanged(int)), this, SLOT(faderPreview(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(controlChanged(int)));
    connect(ui->pushButton_2, SIGNAL(released()), this, SLOT(controlClicked()));
    connect(ui->pushButton_3, SIGNAL(released()), this, SLOT(controlClicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::controlClicked(){
    QWidget* sndr = (QWidget *)sender();
    QString name = sndr->objectName();
    fprintf (stderr,"%s sender\n",sndr->objectName().toStdString().c_str());

    if(name.contains("pushButton_3")){
        ui->horizontalSlider->setValue(100);
    }

    if(name.contains("pushButton_2")){
        ui->horizontalSlider->setValue(0);
    }


}

void MainWindow::controlChanged(int value){
    QWidget* sndr = (QWidget *)sender();
    QString name = sndr->objectName();
    fprintf (stderr,"%s sender\n",sndr->objectName().toStdString().c_str());

    if(name.contains("horizontalSlider")){
        fad->rate=value;
    }

}

void MainWindow::startCapture(int state)
{
    if (state==2){


    foo[1] = new Capture("/dev/video0",640,480);

    ui->ctrlFrame0->setEnabled(true);

    if (!foo[1]->initialized){
        //delete foo[1];
    }
        }
    if (state==0){
            ui->centralWidget->findChild<QFrame *>("ctrlFrame0")->setEnabled(false);

    }

}

void MainWindow::startPreview(int state)
{
    if (state==2){
        prev[1] = new Preview(foo[1]->buffer,foo[1]->buf_len, 10, 640,480);

        }
    if (state==0){
        prev[1]->~Preview();

        }

}

void MainWindow::faderPreview(int state)
{
    if (state==2){
        prev[8] = new Preview(fad->out,fad->buf_len, 10, 640,480);

        }
    if (state==0){
        //delete prev[1];
        prev[8]->~Preview();

        }
}


void MainWindow::startFader(int state)
{
    if (state==2){
        fad = new Fader(foo[1]->buf_len, foo[1]->buffer, foo[1]->buffer, 25);

        }
    if (state==0){

        }

}


