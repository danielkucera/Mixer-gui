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
    connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(controlClicked()));

    connect(ui->redButtons, SIGNAL(buttonReleased(int)), this, SLOT(controlChanged(int)));
    connect(ui->yellowButtons, SIGNAL(buttonReleased(int)), this, SLOT(controlChanged(int)));

    connect(ui->fadeButton, SIGNAL(clicked()), this, SLOT(controlClicked()));



    serialPort.setPortName("/dev/ttyUSB0");

    serialPort.open(QIODevice::WriteOnly);

    int serialPortBaudRate = QSerialPort::Baud9600;
    serialPort.setBaudRate(serialPortBaudRate);
    serialPort.setStopBits(QSerialPort::TwoStop);



}

Fader::~Fader()
{
    run=0;
    delete ui;
}

void Fader::keyPressEvent(QKeyEvent* event) {
    fprintf(stderr,"key %d\n", event->key());
    if (event->key()>47 and event->key()<56){
//obmedz
        ui->yelFrame->findChild<QRadioButton*>(QString("yel%1").arg(event->key()-48))->click();
    }
    if (event->key()==16777221){
        autofade();
    }

}

void Fader::start(){

    output_number=ui->outputSelect->currentIndex();

    out=(uchar*)buffer->Open(output_number);

    red=(uchar*)buffer->Open(0);
    yel=(uchar*)buffer->Open(0);

    rate=0;

    connect(buffer->clock,SIGNAL(timeout()),this,SLOT(makeFrame()));

}

void Fader::controlClicked(){
    QWidget* sndr = (QWidget *)sender();
    QString name = sndr->objectName();
    //fprintf (stderr,"%s sender\n",sndr->objectName().toStdString().c_str());

    if (name.contains("horizontalSlider") && rate==1){
        QString newRed = QString("red%1").arg(ui->yellowButtons->checkedButton()->objectName().at(3).digitValue());
        QString newYel = QString("yel%1").arg(ui->redButtons->checkedButton()->objectName().at(3).digitValue());

        ui->centralwidget->findChild<QRadioButton*>(newRed)->click();
        ui->horizontalSlider->setValue(0);
        ui->centralwidget->findChild<QRadioButton*>(newYel)->click();
    }

    if (name.contains("fadeButton")){
        autofade();
    }

}

void Fader::controlChanged(int value){
    QWidget* sndr = (QWidget *)sender();
    QString name = sndr->objectName();
    //fprintf (stderr,"sender:%s value:%d\n",sndr->objectName().toStdString().c_str(), value);

    if(name.contains("horizontalSlider")){

        rate = ((float)value)/100;

        if(!ui->horizontalSlider->isSliderDown() && value==100){
            QString newRed = QString("red%1").arg(ui->yellowButtons->checkedButton()->objectName().at(3).digitValue());
            QString newYel = QString("yel%1").arg(ui->redButtons->checkedButton()->objectName().at(3).digitValue());

            ui->centralwidget->findChild<QRadioButton*>(newRed)->click();
            ui->horizontalSlider->setValue(0);
            ui->centralwidget->findChild<QRadioButton*>(newYel)->click();
        }
    }

    if(name.contains("enableBtn")){
        //dorobit stop
        start();
    }

    if(name.contains("yellowButtons")){
        int y = ui->yellowButtons->checkedButton()->objectName().at(3).digitValue();
        //fprintf (stderr,"checked %d\n",ui->buttonGroup_2->checkedButton()->objectName().at(3).digitValue());
        setYellow(y);
        setLed(y, 0);
    }

    if(name.contains("redButtons")){
        int r = ui->redButtons->checkedButton()->objectName().at(3).digitValue();
        //fprintf (stderr,"checked %d\n",ui->buttonGroup_2->checkedButton()->objectName().at(3).digitValue());
        setRed(r);
        setLed(r, 1);
    }

}

void Fader::setLed(int val, int color){
    static int red = 0;
    static int yel = 0;

    if (color == 0){
        yel = 1 << (2 * val);
    } else {
        red = 1 << (2 * val + 1);

    }

    int send = yel | red;

    serialPort.putChar(send);

    fprintf (stderr,"r%d y%d send %d\n",red, yel, send);

}

class AutofadeTask : public QRunnable
{
    Fader* rodic;
    int usleeptime;

public: void Init(Fader* rodi, int usleeptim){
        rodic = rodi;
        usleeptime = usleeptim;
    }

    void run()
    {
        for (int value=0; value<=100; value++){
            usleep(usleeptime*1000);
            rodic->setFader(value);
        }
        //fprintf(stderr,"runable\n");
    }
};

void Fader::autofade(){
    int leng = ui->fadeLength->value();

    AutofadeTask *bezec = new AutofadeTask();
    bezec->Init(this, leng/100);
    // QThreadPool takes ownership and deletes 'hello' automatically
    QThreadPool::globalInstance()->start(bezec);

}

void Fader::setYellow(int next){
    //fprintf(stderr,"setting %d\n",next);
    yel = (uchar*) buffer->Open(next);
}

void Fader::setRed(int next){
    //fprintf(stderr,"setting %d\n",next);
    red = (uchar*) buffer->Open(next);

}

void Fader::Init(Buffer* buf)
{
    buffer = buf;

}

void Fader::setFader(int rat){
    ui->horizontalSlider->setValue(rat);

}

void Fader::makeFrame(){

    //fprintf(stderr,"rate %f\n",rate);
    pos=buffer->frame[0];

    if (rate==0) {

        memcpy(out,red,buffer->buf_len);

    } else {
        float rateSup=1-rate;

        for(int i=0; i<buffer->buf_len;i++){

            out[i] =(rateSup*(float)red[i] + rate*(float)yel[i]) ;
        }

    }

    buffer->newFrame(output_number);

}
