#include "fader.h"
#include "ui_fader.h"

Fader::Fader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Fader)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    for(int i; i<15; i++){
        ui->outputSelect->addItem(QString::number(i));
    }

    connect(ui->enableBtn, SIGNAL(stateChanged(int)), this, SLOT(controlChanged(int)));
//    connect(ui->checkBox_5, SIGNAL(stateChanged(int)), this, SLOT(faderPreview(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(controlChanged(int)));
    connect(ui->pushButton_2, SIGNAL(released()), this, SLOT(controlClicked()));
    connect(ui->pushButton_3, SIGNAL(released()), this, SLOT(controlClicked()));

}

Fader::~Fader()
{
    run=0;
    delete ui;
}

void Fader::start(){

    out=(unsigned char*)buffer->Open(ui->outputSelect->currentIndex());

    a=(unsigned char*)buffer->Open(0);
    b=(unsigned char*)buffer->Open(1);

    rate=0;

    run=1;

    pthread_create(&thread, NULL, Fader::staticEntryPoint, this);
}

void Fader::controlClicked(){
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

void Fader::controlChanged(int value){
    QWidget* sndr = (QWidget *)sender();
    QString name = sndr->objectName();
    fprintf (stderr,"%s sender %d\n",sndr->objectName().toStdString().c_str(), value);

    if(name.contains("horizontalSlider")){
        setRate(value);
    }

    if(name.contains("enableBtn")){
        start();
    }


}

void Fader::Init(Buffer* buf)
{
    buffer = buf;

}

void * Fader::staticEntryPoint(void * c)
{
    ((Fader *) c)->Thread();
    return NULL;
}

void Fader::setRate(int rat){
/*    if (rat==0){

        rate=0;

    } else if (rat==100){

        rate=1;

    } else {
    rate = 1+((-log((float)rat-1.01))/log(100));

    }
*/
    rate = (float)rat/100;
    rateSup = 1-rate;

    fprintf(stderr,"rate %f ratesup %f\n",rate,rateSup);

}

void Fader::Thread(){

    int y;
    int i;
    pos = buffer->frame[0];

    while(run){

        if (pos!=buffer->frame[0]){
        //fprintf(stderr,"rate %f\n",rate);
            pos=buffer->frame[0];

        if (rate==0) {

            memcpy(out,a,buffer->buf_len);

        } else if (rate==1){
            memcpy(out,b,buffer->buf_len);

        } else {

        //fprintf(stderr,"lolofarme\n");

        for(i=0; i<buffer->buf_len;i++){

            out[i] =(rateSup*(float)a[i] + rate*(float)b[i]) ;
        }

        }

    } else {
        usleep(4*1000);
    }
    }

}
