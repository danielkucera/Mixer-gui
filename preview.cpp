#include "preview.h"
#include "ui_preview.h"

Preview::Preview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Preview)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    fprintf(stderr,"windid %d\n",ui->previewwidget->winId());
}

Preview::~Preview()
{

    run=0;
    usleep(500*1000);
    delete ui;
    ::fclose(fp);

}

void Preview::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   if (buffer == 0){
       return;
   }

   ui->mainwidget->width();

   int h2 = (ui->mainwidget->width()*buffer->height)/buffer->width;
   int w2 = (ui->mainwidget->height()*buffer->width)/buffer->height;

   //fprintf(stderr,"inoked ok!\n");


   if (h2<ui->mainwidget->height()){
       w2 = ui->mainwidget->width();
   } else {
       h2 = ui->mainwidget->height();
   }

   ui->previewwidget->setFixedHeight(h2);
   ui->previewwidget->setFixedWidth(w2);

   QPoint center = ui->previewwidget->parentWidget()->geometry().center();

   center.setX(center.x()-w2/2);
   center.setY(center.y()-h2/2);

   ui->previewwidget->move(center);
}

void Preview::start(Buffer *buf, int numbe)
{
    fp = NULL;

    buffer = buf;
    number = numbe;

    QResizeEvent* salam;
    resizeEvent(salam);


    setWindowTitle(QString("Buffer %1").arg(numbe));

//    int u_frame = 0;
//    int len = Bpp * width * height;
//    struct buffer output;
//    output.length = width * height * Bpp;
//    output.start = malloc(width * height * Bpp);


//      fp = fopen("/tmp/preview", "w");
//      pipe (fp);

    char cmd[300];
    sprintf(cmd, "mplayer -demuxer rawvideo - -rawvideo w=%d:h=%d:format=rgb24 -wid %d 2>/dev/null >/dev/null",buffer->width,buffer->height, ui->previewwidget->winId());

    fp = popen(cmd, "we");

    run=true;

    //std::thread test;
    //test->(Thread);

    pthread_create(&thread, NULL, Preview::staticEntryPoint, this);

}

void * Preview::staticEntryPoint(void * c)
{
    ((Preview *) c)->Thread();
    return NULL;
}

void Preview::Thread(){

    void* input = buffer->Open(number);

    //printf("output thread started\n");
    while (run) {

//        fprintf(stderr, "output thread started\n");

        //usleep(1000*1000/buffer->fps);
        usleep(1000*1000/10);

//        if (frame[out] != u_frame) {
//                      usleep(10*1000);
//            u_frame = frame[out];

            //          if (fp){
            //memcpy(output.start, buffers[out].start, len);
        fwrite(input+0, 1, buffer->buf_len, fp);
//                      } else {
            //                  fp = popen("cat > /tmp/kokosy", "w");
//                      }

            //          printf("*");
            //          fflush(stdout);

//        } else {
//            usleep(3000);
//        }
    }
}

