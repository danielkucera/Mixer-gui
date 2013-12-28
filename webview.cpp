#include "webview.h"
#include "ui_webview.h"
#include "QDebug"

WebView::WebView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WebView)
{
    ui->setupUi(this);

//    view = new QWebView(ui->label);
//    view = new QWebView(parent);

    view = ui->frame;

    connect(ui->loadButton,SIGNAL(released()),this,SLOT(loadFile()));
    connect(ui->urlButton,SIGNAL(released()),this,SLOT(loadUrl()));

}

void WebView::Init(Buffer *buf){
    buffer=buf;

}

WebView::~WebView()
{
    delete ui;
}

void WebView::loadFile(){

    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Load image"), "",
            tr("All Files (*)"));
    loadPage(fileName);
}

void WebView::loadUrl(){
    loadPage(ui->urlBar->text());
}

void WebView::loadPage(QString url){

    view->setRenderHints(0);

    view->load(QUrl(url));

//    view->setFixedSize(110,110);
//    view->set
    view->show();

    outNumber = ui->outNumber->value();

    output = buffer->Open(outNumber);

//    connect(buffer->clock,SIGNAL(timeout()),this,SLOT(newFrame()));
    connect(view->page(),SIGNAL(repaintRequested(const QRect)),this,SLOT(newFrame()));

}

void WebView::newFrame(){

    //qDebug()<< "nieco\n";

    //QImage rgb = QPixmap::grabWidget(view).toImage().copy(0,0,buffer->width,buffer->height).convertToFormat(QImage::Format_RGB888);
    QImage rgb = QPixmap::grabWidget(view).toImage().scaled(buffer->width,buffer->height).convertToFormat(QImage::Format_RGB888);

    memcpy(output,rgb.bits(),rgb.byteCount());

    buffer->newFrame(outNumber);

}

/*
class GrabFrame : public QRunnable
{
    WebView* parent;
    int dorun = 1;
public: void Init(WebView
                  * par){
        parent=par;
    }


public: void stop(){
        dorun = 0;
    }

    void run()
    {

        while(dorun){
            parent->newFrame(true);
            usleep(500*1000);
        }

    }
};
*/
