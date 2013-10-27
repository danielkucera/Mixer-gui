#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdio.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    connect(ui->captureButton, SIGNAL(released()), this, SLOT(startCapture()));
    connect(ui->faderButton, SIGNAL(released()), this, SLOT(startFader()));


    connect(ui->choosePreview, SIGNAL(currentIndexChanged(int)), this, SLOT(startPreview(int)));
    ui->choosePreview->addItem("Preview...");

    for(int i; i<15; i++){
        ui->choosePreview->addItem(QString::number(i));
    }

    buffer = new Buffer(640, 480, 3);

}

MainWindow::~MainWindow()
{
    delete ui;
    exit(0);
}

void MainWindow::startCapture()
{
    Capture* cap = new Capture();
    cap->show();
    cap->Init(buffer);

}

void MainWindow::startPreview(int index)
{
    if (index!=0){
        Preview* prev = new Preview();
        prev->show();
        prev->start(buffer, index-1);
    }
}

void MainWindow::startFader()
{
    Fader* fad = new Fader();
    fad->show();
    fad->Init(buffer);

}
