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

    QImage rgb = img.convertToFormat(QImage::Format_RGB888).scaled(buffer->width,buffer->height);

    fprintf(stderr,"leng %d %p\n", rgb.byteCount(),buffer->Open(number));

    memcpy(buffer->Open(number),rgb.bits(),rgb.byteCount());
    buffer->frame[number]=-1;
}
