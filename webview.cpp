#include "webview.h"
#include "ui_webview.h"
#include "QDebug"

WebView::WebView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WebView)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

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
    loadPage("file://"+fileName);
}

void WebView::loadUrl(){
    loadPage(ui->urlBar->text());
}

void WebView::loadPage(QString url){

    qDebug()<< "loading "<<url;

    //view->setRenderHints(0);

    //view->load(QUrl(url));

    //page = new QWebPage;

    page.mainFrame()->load(url);

//    view->setFixedSize(110,110);
//    view->set
    //view->show();

    outNumber = ui->outNumber->value();

    output = buffer->Open(outNumber);

    painter = new QPainter();

    page.setViewportSize(QSize(buffer->width,buffer->height));

    rgb = new QImage(buffer->width,buffer->height, QImage::Format_ARGB32);

    //view->setFixedSize(buffer->width,buffer->height);

    //page->setViewportSize(QSize(buffer->width,buffer->height));

    painter->begin(rgb);

    page.mainFrame()->render(painter);

    //connect(buffer->clock,SIGNAL(timeout()),this,SLOT(newFrame()));
    connect(&page,SIGNAL(repaintRequested(const QRect)),this,SLOT(newFrame(const QRect)));

    connect(&page,SIGNAL(loadFinished(bool)),this,SLOT(pageRender(bool)));

}

void WebView::pageRender(bool status){
    newFrame(QRect(0,0,buffer->width,buffer->height));
}

void WebView::newFrame(QRect area){

    static int last=0;

    if (buffer->frames>last){
        last=buffer->frames;

        page.mainFrame()->render(painter,area);

        memcpy(output,rgb->bits(),rgb->byteCount());

        buffer->newFrame(outNumber);
    }
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
