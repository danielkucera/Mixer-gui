#include "externalinput.h"
#include "ui_externalinput.h"

ExternalInput::ExternalInput(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExternalInput)
{
    ui->setupUi(this);

    connect(ui->enableBox,SIGNAL(stateChanged(int)),this,SLOT(enable(int)));
}

ExternalInput::~ExternalInput()
{
    delete ui;
}

void ExternalInput::Init(Buffer* buf){
    buffer = buf;
}

void ExternalInput::enable(int status){

    if (status==2){

        QString program = QString(ui->commandEdit->toPlainText()).arg(buffer->width).arg(buffer->height);
        //arguments << "-style" << "fusion";
        outNum = ui->inputBox->value();
        output = (char*)buffer->Open(outNum);

        data.clear();

        myProcess = new QProcess();

        QThread* workThread = new QThread(this);
        //myProcess->moveToThread(workThread);
        workThread->start();

//        connect(myProcess,SIGNAL(readyRead()),this,SLOT(printOutput()));
//        connect(myProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(fillBuffer()));
        connect(buffer->clock,SIGNAL(timeout()),this,SLOT(fillBuffer()));
        connect(myProcess,SIGNAL(readyReadStandardError()),this,SLOT(printOutput()));
        connect(myProcess,SIGNAL(finished(int)),this,SLOT(processExit()));

        qDebug() << program;

        myProcess->start(program);
        qDebug()<<"vykonalsom";

//        connect(buffer->clock,SIGNAL(timeout()),this,SLOT(newFrame()));
    } else {
        myProcess->close();
    }
}

void ExternalInput::printOutput(){
    QString text = myProcess->readAllStandardError();
    QString cut = ui->outputBox->toPlainText();

    if (text.contains('\r')){
        cut.truncate(cut.lastIndexOf('\n',cut.lastIndexOf('\n')-1));
        ui->outputBox->clear();
        ui->outputBox->appendPlainText(cut);
    }
    ui->outputBox->appendPlainText(text);
    //qDebug() << myProcess->readAllStandardOutput();
}

void ExternalInput::fillBuffer(){

    data.append(myProcess->readAllStandardOutput());
    if (data.length() >= buffer->buf_len){
        memcpy(output, data.data(), buffer->buf_len);
        data.remove(0, buffer->buf_len);
        buffer->newFrame(ui->inputBox->value());
    }
//    output
}

void ExternalInput::newFrame(){
    if (myProcess->bytesToWrite()==0){
//        myProcess->write(input,buffer->buf_len);
    }
}

void ExternalInput::processExit(){
    ui->enableBox->setChecked(false);
}

void ExternalInput::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->commandEdit->setPlainText(arg1);
}
