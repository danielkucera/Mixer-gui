#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdio.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    menuBar()->setNativeMenuBar(false);

    for(int i; i<15; i++){
        QAction* current;
        QString number = QString("%1").arg(i);

        // previews
        current = ui->menuPreview->addAction("Buffer "+number);
        current->setObjectName(number);
        connect(current,SIGNAL(triggered()),this,SLOT(startPreview()));

        //saves
        current = ui->menuSave_buffer->addAction("Buffer "+number);
        current->setObjectName(number);
        connect(current,SIGNAL(triggered()),this,SLOT(saveBuffer()));
    }

    buffer = new Buffer(1280, 720, 4, 25);

}

void MainWindow::changeEvent(QEvent *e)
{
    if(e->type() == QEvent::ActivationChange){
        foreach (QWindow *wind, QApplication::allWindows()) {
            wind->raise();
        }
    }
}

MainWindow::~MainWindow()
{
    exit(0);
    delete ui;

}

void MainWindow::startPreview()
{
    int index = sender()->objectName().toInt();
    Preview* prev = new Preview();
    prev->show();
    prev->start(buffer, index);
}

void MainWindow::saveBuffer()
{
    int index = sender()->objectName().toInt();
    Save(buffer,index);
}

void MainWindow::on_actionHDMI_triggered()
{
    HDMI* hdmi = new HDMI();
    hdmi->show();
    hdmi->Init(buffer);
}

void MainWindow::on_actionLoad_image_triggered()
{
    Load* load = new Load();
    load->Init(buffer);
    load->show();
}

void MainWindow::on_actionHTML_page_triggered()
{
    WebView* zobrazovac = new WebView();
    zobrazovac->Init(buffer);
    zobrazovac->show();

}

void MainWindow::on_actionOverlay_triggered()
{
    Overlay* overlay = new Overlay();
    overlay->Init(buffer);
    overlay->show();
}

void MainWindow::on_actionFader_triggered()
{
    Fader* fad = new Fader();
    fad->show();
    fad->Init(buffer);
}

void MainWindow::on_actionCapture_device_triggered()
{
    Capture* cap = new Capture();
    cap->show();
    cap->Init(buffer);
}

void MainWindow::on_actionExternal_triggered()
{
    ExternalOutput* ext = new ExternalOutput();
    ext->show();
    ext->Init(buffer);

}
