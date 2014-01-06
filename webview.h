#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QMainWindow>
#include <QWebView>
#include <QWebFrame>
#include "buffer.h"
#include <QRunnable>
#include <QFileDialog>

namespace Ui {
class WebView;
}

class WebView : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit WebView(QWidget *parent = 0);
    ~WebView();
    void Init(Buffer* buffer);
    
private:
    Ui::WebView *ui;

    QWebView *view;
    QWebPage page;

    QPainter *painter;
    QImage *rgb;

    Buffer* buffer = 0;
    void* output;
    int outNumber;
    void loadPage(QString url);

private slots:
    void newFrame(QRect area);
    void pageRender(bool status);
    void loadUrl();
    void loadFile();

};

#endif // WEBVIEW_H
