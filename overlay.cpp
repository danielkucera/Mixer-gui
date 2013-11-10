#include "overlay.h"
#include "ui_overlay.h"

Overlay::Overlay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overlay)
{
    ui->setupUi(this);
}

Overlay::~Overlay()
{
    delete ui;
}
