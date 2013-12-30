#include "load.h"
#include "ui_load.h"

Load::Load(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Load)
{
    ui->setupUi(this);

    connect(ui->loadButton, SIGNAL(released()), this, SLOT(loadButton()));

}

Load::~Load()
{
    delete ui;
}

void Load::Init(Buffer* buf)
{
    buffer = buf;

}

void Load::loadButton(){
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Load image"), "",
            tr("All Files (*)"));

    loadImage(fileName,ui->numberSelect->value());
}

int Load::loadImage(QString fileName, int number){

    QImage img(fileName);

    if (img.isNull())
        return -1;

    QImage scaled = img.scaled(buffer->width,buffer->height);

    QImage alpha = scaled.alphaChannel();

    QImage rgb = scaled.convertToFormat(QImage::Format_RGB888);

    fprintf(stderr,"leng %d %p\n", rgb.byteCount(),buffer->Open(number));

    memcpy(buffer->Open(number),rgb.bits(),rgb.byteCount());
    memcpy(buffer->Open(number+1),alpha.bits(),alpha.byteCount());

    buffer->newFrame(number);

    buffer->frame[number]=-1;
}
