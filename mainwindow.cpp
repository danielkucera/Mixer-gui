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
    connect(ui->loadButton, SIGNAL(released()), this, SLOT(loadImage()));
    connect(ui->overlayButton, SIGNAL(released()), this, SLOT(startOverlay()));

    //preview
    connect(ui->choosePreview, SIGNAL(currentIndexChanged(int)), this, SLOT(startPreview(int)));
    ui->choosePreview->addItem("Preview...");
    for(int i; i<15; i++){
        ui->choosePreview->addItem(QString::number(i));
    }

    //save
    connect(ui->saveBuffer, SIGNAL(currentIndexChanged(int)), this, SLOT(saveBuffer(int)));
    ui->saveBuffer->addItem("Save...");
    for(int i; i<15; i++){
        ui->saveBuffer->addItem(QString::number(i));
    }

    buffer = new Buffer(1280, 720, 3);

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

void MainWindow::loadImage()
{
    Load* load = new Load();
    load->Init(buffer);
    load->show();
}


void MainWindow::startPreview(int index)
{
    if (index!=0){
        Preview* prev = new Preview();
        prev->show();
        prev->start(buffer, index-1);
    }
    ui->choosePreview->setCurrentIndex(0);
    ui->choosePreview->update();
}

void MainWindow::startOverlay()
{
    Overlay* overlay = new Overlay();
    overlay->Init(buffer);
    overlay->show();
    //overlay->start(buffer, index-1);
}

void MainWindow::saveBuffer(int index)
{
    if (index!=0){
        Save(buffer,index-1);
    }

    ui->saveBuffer->setCurrentIndex(0);
    ui->saveBuffer->update();
}

void MainWindow::startFader()
{
    Fader* fad = new Fader();
    fad->show();
    fad->Init(buffer);

}
