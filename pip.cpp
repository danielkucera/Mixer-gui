#include "pip.h"
#include "ui_pip.h"

PIP::PIP(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PIP)
{
    ui->setupUi(this);
    connect(ui->enableBox, SIGNAL(stateChanged(int)), this, SLOT(setEnabled(int)));
    connect(ui->offsetX, SIGNAL(valueChanged(QString)), this, SLOT(setParams(QString)));
    connect(ui->offsetY, SIGNAL(valueChanged(QString)), this, SLOT(setParams(QString)));
    connect(ui->scaleBox, SIGNAL(valueChanged(QString)), this, SLOT(setParams(QString)));
}

void PIP::Init(Buffer *buf){
    buffer = buf;
}

void PIP::setParams(QString nic){

    worker->setParams(ui->scaleBox->value(), ui->offsetX->value(), ui->offsetY->value());

}

void PIP::setEnabled(int status){
    if (status==2){

        worker = new PIPWorker();
        setParams("");

        QThread* workThread = new QThread();
        worker->moveToThread(workThread);
        workThread->start();

        worker->start(buffer, ui->output->value(), ui->inputBack->value(), ui->inputPIP->value());

    } else {

        delete worker;

    }
}


PIP::~PIP()
{
    delete ui;
}
