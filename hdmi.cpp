#include "hdmi.h"
#include "ui_hdmi.h"


HDMI::HDMI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HDMI)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    connect(ui->enableBox, SIGNAL(stateChanged(int)), this, SLOT(enableHDMI(int)));

    QTimer* statusbarClock = new QTimer(this);
    statusbarClock->start(1000);

    connect(statusbarClock,SIGNAL(timeout()),this,SLOT(updateStatusbar()));

}

void HDMI::Init(Buffer* bufin)
{
    buffer = bufin;
}


HDMI::~HDMI()
{
    ui->enableBox->setEnabled(false);
    delete ui;
}

void HDMI::updateStatusbar(){

    ui->statusbar->showMessage(QString("%1x%2 %3FPS %4FRM" ).arg(jpg_w).arg(jpg_h).arg(frame_cnt-old_f).arg(frame_cnt));

    old_f = frame_cnt;
}

void HDMI::enableHDMI(int status){
    if (status==2){

        QHostAddress ip = QHostAddress(ui->ipEdit->text());

        number = ui->outputSelect->value();

        dest = buffer->Open(number);

        recvr = new Reciever();
        recvr->start(ip);

        QThread* recvThread = new QThread();
        recvr->moveToThread(recvThread);
        recvThread->start();

        connect(recvr,SIGNAL(imageReceived(QByteArray)),this,SLOT(process(QByteArray)));

    }else{
        delete recvr;
    }
}

void local_error_exit (jpeg_common_struct* cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  (*cinfo->err->output_message) (cinfo);

  longjmp(myerr->setjmp_buffer, 1);
}

void HDMI::process(QByteArray image){

    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;

    JSAMPARRAY jpeg_buffer;		/* Output row buffer */
    int row_stride;		/* physical row width in output buffer */

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = local_error_exit;

    if (setjmp(jerr.setjmp_buffer)) {

        jpeg_destroy_decompress(&cinfo);
        return;
    }

    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo, (uchar*)image.data(),image.length());

    (void) jpeg_read_header(&cinfo, TRUE);

    cinfo.dct_method = JDCT_FASTEST;
    cinfo.two_pass_quantize = FALSE;
    //cinfo.out_color_space = JCS_EXT_RGBA;
    cinfo.out_color_space = JCS_EXT_BGRA;

    (void) jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;

    jpeg_buffer = (*cinfo.mem->alloc_sarray)
          ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    int counter=0;
    while (cinfo.output_scanline < cinfo.output_height) {

      (void) jpeg_read_scanlines(&cinfo, jpeg_buffer, 1);
      memcpy(dest + counter,jpeg_buffer[0],row_stride);
      counter += row_stride;
    }

    emit(buffer->newFrame(number));

    frame_cnt++;
    jpg_w=cinfo.output_width;
    jpg_h=cinfo.output_height;

    (void) jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

//    QImage rgb = QImage::fromData((uchar*)inp, off).scaled(buffer->width, buffer->height).convertToFormat(QImage::Format_RGB888);
//    memcpy(dest,rgb.bits(),rgb.byteCount());

}



