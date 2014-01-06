#include "overlay.h"
#include "ui_overlay.h"
#include "QDebug"

Overlay::Overlay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overlay)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    connect(ui->enableBox, SIGNAL(stateChanged(int)), this, SLOT(setEnabled(int)));
    connect(ui->colorButton, SIGNAL(released()),this,SLOT(showColorDlg()));
    connect(ui->pickButton, SIGNAL(released()),this,SLOT(pickScreenColor()));
    connect(ui->sensSlider, SIGNAL(valueChanged(int)),this,SLOT(setSensitivity(int)));

}

void Overlay::Init(Buffer *buf){
    buffer = buf;
}

void Overlay::setSensitivity(int newSens){
    worker->setSensitivity(newSens);
    qDebug() << "sensit" << newSens;
}

void Overlay::setEnabled(int status){
    if (status==2){

        worker = new OverlayWorker();

        QThread* workThread = new QThread();
        worker->moveToThread(workThread);
        workThread->start();

        if (ui->radioArray->isChecked()){ //mask from array
            worker->setMethod(0);

        } else {
            worker->setMethod(1);
        }

        worker->start(buffer, ui->outputNumber->value(), ui->inputA->value(), ui->inputOver->value());

    } else {

        delete worker;

    }
}


void Overlay::setColor(QColor farba){

    QPalette paleta;

    paleta.setColor(QPalette::Background, farba);

    ui->colorFrame->setPalette(paleta);

    ui->colorFrame->update();

    qDebug()<< "color" << farba;

    worker->setColor(farba);

}


void Overlay::showColorDlg(){
    setColor(QColorDialog::getColor(Qt::black, this));

}

QColor Overlay::grabScreenColor(const QPoint &p)
{
    QDesktopWidget *desktop = QApplication::desktop();
    QPixmap pixmap = QGuiApplication::screens().at(desktop->screenNumber())->grabWindow(desktop->winId(), p.x(), p.y(), 1, 1);
    QImage i = pixmap.toImage();
    return i.pixel(0, 0);
}

void Overlay::pickScreenColor()
{
    screenColorPicking=true;
    /*For some reason, q->grabMouse(Qt::CrossCursor) doesn't change
     * the cursor, and therefore I have to change it manually.
     */
    grabMouse();
    setCursor(Qt::CrossCursor);
    /* With setMouseTracking(true) the desired color can be more precisedly picked up,
     * and continuously pushing the mouse button is not necessary.
     */
    setMouseTracking(true);

    qDebug()<<grabScreenColor(QCursor::pos());
}

void Overlay::mouseReleaseEvent(QMouseEvent *e)
{
    if (screenColorPicking) {

        setColor(grabScreenColor(e->globalPos()));

        screenColorPicking=false;
        setCursor(Qt::ArrowCursor);
        setMouseTracking(false);
        releaseMouse();
        return;
    }
}

Overlay::~Overlay()
{
    delete worker;
    delete ui;
}
