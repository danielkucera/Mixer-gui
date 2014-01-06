#include "externaloutput.h"
#include "ui_externaloutput.h"

ExternalOutput::ExternalOutput(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExternalOutput)
{
    ui->setupUi(this);

    connect(ui->enableBox,SIGNAL(stateChanged(int)),this,SLOT(enable(int)));

}

ExternalOutput::~ExternalOutput()
{
    delete ui;
}


void ExternalOutput::Init(Buffer* buf){
    buffer = buf;
}

void ExternalOutput::enable(int status){

    if (status==2){

        //QString program = "vlc";
        QString program = QString(ui->commandEdit->toPlainText()).arg(buffer->width).arg(buffer->height);
        //arguments << "-style" << "fusion";
        input = (char*)buffer->Open(ui->inputBox->value());

        myProcess = new QProcess();

        QThread* workThread = new QThread(this);
        //myProcess->moveToThread(workThread);
        workThread->start();

        connect(myProcess,SIGNAL(readyRead()),this,SLOT(printOutput()));
        connect(myProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(printOutput()));
        connect(myProcess,SIGNAL(readyReadStandardError()),this,SLOT(printOutput()));
        connect(myProcess,SIGNAL(finished(int)),this,SLOT(processExit()));

        qDebug() << program;

        myProcess->start(program);
        qDebug()<<"vykonalsom";

        connect(buffer->clock,SIGNAL(timeout()),this,SLOT(newFrame()));
    } else {
        myProcess->close();
    }
}

void ExternalOutput::printOutput(){
    QString text = myProcess->readAllStandardError();
    text.append(myProcess->readAllStandardOutput());
    QString cut = ui->outputBox->toPlainText();

    if (text.contains('\r')){
        cut.truncate(cut.lastIndexOf('\n',cut.lastIndexOf('\n')-1));
        ui->outputBox->clear();
        ui->outputBox->appendPlainText(cut);
    }
    ui->outputBox->appendPlainText(text);
    //qDebug() << myProcess->readAllStandardOutput();
}

void ExternalOutput::newFrame(){
    if (myProcess->bytesToWrite()==0){
        myProcess->write(input,buffer->buf_len);
    }
}

void ExternalOutput::processExit(){
    ui->enableBox->setChecked(false);
}


void ExternalOutput::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->commandEdit->setPlainText(arg1);
}
