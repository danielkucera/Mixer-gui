#include "preview.h"
#include "ui_preview.h"

Preview::Preview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Preview)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    fprintf(stderr,"windid %d\n",ui->centralwidget->winId());
}

Preview::~Preview()
{

    run=0;
    usleep(500*1000);
    delete ui;
    ::fclose(fp);

}

void Preview::start(Buffer *buf, int numbe)
{
    fp = NULL;

    buffer = buf;
    number = numbe;

    setWindowTitle(QString("Buffer %1").arg(numbe));

//    int u_frame = 0;
//    int len = Bpp * width * height;
//    struct buffer output;
//    output.length = width * height * Bpp;
//    output.start = malloc(width * height * Bpp);


//      fp = fopen("/tmp/preview", "w");
//      pipe (fp);

    char cmd[300];
    sprintf(cmd, "mplayer -demuxer rawvideo - -rawvideo w=%d:h=%d:format=rgb32 -wid %d -name TEST 2>/dev/null >/dev/null",buffer->width,buffer->height, ui->centralwidget->winId());

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
        fwrite(input, 1, buffer->buf_len, fp);
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

