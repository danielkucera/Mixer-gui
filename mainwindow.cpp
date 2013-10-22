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

    connect(ui->checkBox_2, SIGNAL(stateChanged(int)), this, SLOT(startCapture(int)));
    connect(ui->checkBox_3, SIGNAL(stateChanged(int)), this, SLOT(startPreview(int)));

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(handleSlider(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setName(const QString &name)
{
    ui->lineEdit->setText(name);
}

QString MainWindow::name() const
{
    return ui->lineEdit->text();
}

void MainWindow::startCapture(int state)
{
    if (state==2){
    // change the text
    //printf("%s",ui->lineEdit->text().toLocal8Bit().data());
    ui->lineEdit_2->setText(ui->lineEdit->text());
    foo[1] = new Capture("/dev/video0",640,480);

    ui->checkBox_3->setEnabled(true);

    if (!foo[1]->initialized){
        //delete foo[1];
        fprintf(stderr,"haluz\n");
    }
        }
    // resize button
    //m_button->resize(100,100);
}

void MainWindow::startPreview(int state)
{
    if (state==2){
        prev[1] = new Preview(foo[1]->buffer,foo[1]->buf_len, 10, 640,480);

        ui->checkBox_2->setEnabled(false);

        }
    if (state==0){
        //delete prev[1];
        prev[1]->~Preview();

        ui->checkBox_2->setEnabled(true);

        }
    // resize button
    //m_button->resize(100,100);
}

void MainWindow::handleSlider(int value)
{
    char buffer [50];
    // change the text
    //printf("%s",ui->lineEdit->text().toLocal8Bit().data());

    sprintf (buffer,"%d chars long\n",value);



    ui->lineEdit_2->setText(buffer);
    // resize button
    //m_button->resize(100,100);
}

