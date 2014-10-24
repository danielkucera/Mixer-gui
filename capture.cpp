#include "capture.h"
#include "ui_capture.h"

Capture::Capture(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Capture)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    connect(ui->enableInput, SIGNAL(stateChanged(int)), this, SLOT(handleCheckbox(int)));

    QDir path("/dev");
    QStringList files = path.entryList(QDir::System).filter("video");

    ui->inputName->addItems(files);

}

Capture::~Capture()
{

    ui->enableInput->setChecked(false);
    delete ui;
}

void Capture::handleCheckbox(int status){
    if (status==2){
        startCapture();
    } else {
        stopCapture();
    }
}


void Capture::Init(Buffer* bufin)
{
    buf = bufin;
}

void Capture::startCapture(){

    outnum = ui->outputNumber->value();

    out_buf = buf->Open(outnum);

    dev_name = ui->inputName->currentText().prepend("/dev/").toLocal8Bit().data();

    open_device();
    init_device();
    start_capturing();

//	pthread_t thread;
//	Capture c;
        run=1;

    pthread_create(&thread, NULL, Capture::staticEntryPoint, this);

//    while (run){
//        usleep(1000*1000);
//        ui->frameCount->setPlainText(QString::number(buffer->frame[outnum]));
//    }

    //potom nakoniec toto
    //ui->ctrlFrame->setEnabled(false);

}

void Capture::stopCapture(){

    run=0;

    void* ret;

    pthread_join(thread, &ret);

    stop_capturing();
    uninit_device();
    close_device();

    //ui->ctrlFrame->setEnabled(true);

}

void * Capture::staticEntryPoint(void * c)
{
    ((Capture *) c)->loop();
    return NULL;
}

void * Capture::loop()
{
    unsigned int count;

    buf->frame[outnum]=0;

    while (run) {

        buf->frame[outnum]++;

            for (;;) {
                    fd_set fds;
                    struct timeval tv;
                    int r;

                    FD_ZERO(&fds);
                    FD_SET(fd, &fds);

                    /* Timeout. */
                    tv.tv_sec = 2;
                    tv.tv_usec = 0;

                    r = select(fd + 1, &fds, NULL, NULL, &tv);

                    if (-1 == r) {
                            if (EINTR == errno)
                                    continue;
                            logerr("select");
                    }

                    if (0 == r) {
                            fprintf(stderr, "select timeout\n");
                            exit(EXIT_FAILURE);
                    }

                    if (read_frame())
                            break;

                    /* EAGAIN - continue select loop. */
            }
    }

}

void Capture::logerr(const char *s) {
        fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
        //exit(EXIT_FAILURE);
}

void Capture::open_device()
{
        struct stat st;

        if (-1 == stat(dev_name, &st)) {
                fprintf(stderr, "Cannot identify '%s': %d, %s\n",
                         dev_name, errno, strerror(errno));
                exit(EXIT_FAILURE);
        }

        if (!S_ISCHR(st.st_mode)) {
                fprintf(stderr, "%s is no device\n", dev_name);
                exit(EXIT_FAILURE);
        }

        fd = open(dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

        if (-1 == fd) {
                fprintf(stderr, "Cannot open '%s': %d, %s\n",
                         dev_name, errno, strerror(errno));
                exit(EXIT_FAILURE);
        }
}

//a blocking wrapper of the ioctl function
int Capture::xioctl(int fh, int request, void *arg)
{
        int r;

        do {
                r = ioctl(fh, request, arg);
        } while (-1 == r && EINTR == errno);

        return r;
}

void Capture::stop_capturing(void)
{
        enum v4l2_buf_type type;

        switch (io) {
        case IO_METHOD_READ:
                /* Nothing to do. */
                break;

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
                type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
                        logerr("VIDIOC_STREAMOFF");
                break;
        }
}

void Capture::init_device(void)
{
        struct v4l2_capability cap;
        struct v4l2_cropcap cropcap;
        struct v4l2_crop crop;
        unsigned int min;

        if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s is no V4L2 device\n",
                                 dev_name);
                        exit(EXIT_FAILURE);
                } else {
                        logerr("VIDIOC_QUERYCAP");
                }
        }

        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
                fprintf(stderr, "%s is no video capture device\n",
                         dev_name);
                exit(EXIT_FAILURE);
        }
/*
        switch (io) {
        case IO_METHOD_READ:
                if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
                        fprintf(stderr, "%s does not support read i/o\n",
                                 dev_name);
                        exit(EXIT_FAILURE);
                }
                break;

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
                if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
                        fprintf(stderr, "%s does not support streaming i/o\n",
                                 dev_name);
                        exit(EXIT_FAILURE);
                }
                break;
        }
*/
        if ((cap.capabilities & V4L2_CAP_READWRITE)) {
                fprintf(stderr, "%s does support read i/o\n", dev_name);
                io = IO_METHOD_READ;
        } else if (cap.capabilities & V4L2_CAP_STREAMING) {
            fprintf(stderr, "%s does support streaming i/o\n", dev_name);
            io = IO_METHOD_USERPTR;
        }

        /* Select video input, video standard and tune here. */


        CLEAR(cropcap);

        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
                crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                crop.c = cropcap.defrect; /* reset to default */

                if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
                        switch (errno) {
                        case EINVAL:
                                /* Cropping not supported. */
                                break;
                        default:
                                /* Errors ignored. */
                                break;
                        }
                }
        } else {
                /* Errors ignored. */
        }


        CLEAR(fmt);

        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (1) { //force_format
                fmt.fmt.pix.width       = buf->width;
                fmt.fmt.pix.height      = buf->height;
                //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
                fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
                fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

                if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
                        logerr("VIDIOC_S_FMT");

                /* Note VIDIOC_S_FMT may change width and height. */
        } else {
                /* Preserve original settings as set by v4l2-ctl for example */
                if (-1 == xioctl(fd, VIDIOC_G_FMT, &fmt))
                        logerr("VIDIOC_G_FMT");
        }

        char* pixfmt;

        switch (fmt.fmt.pix.pixelformat){

            case V4L2_PIX_FMT_UYVY:
                pixfmt = "V4L2_PIX_FMT_UYVY";
                break;
            case V4L2_PIX_FMT_YUYV:
                pixfmt = "V4L2_PIX_FMT_YUYV";
                break;
            case V4L2_PIX_FMT_MJPEG:
                pixfmt = "V4L2_PIX_FMT_MJPEG";
                break;
            default:
                pixfmt = "unknown";
                break;
        }

        fprintf(stderr,"pixelformat %s w:%d h:%d buf_len: %d\n", pixfmt, fmt.fmt.pix.width, fmt.fmt.pix.height, fmt.fmt.pix.sizeimage);

        width = fmt.fmt.pix.width;
        height = fmt.fmt.pix.height;

        //exit(66);

        /* Buggy driver paranoia. */
        /*
        min = fmt.fmt.pix.width * 4;
        if (fmt.fmt.pix.bytesperline < min)
                fmt.fmt.pix.bytesperline = min;
        min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
        if (fmt.fmt.pix.sizeimage < min)
                fmt.fmt.pix.sizeimage = min;
                */

        fprintf(stderr,"sizeimage %d\n",fmt.fmt.pix.sizeimage);

        switch (io) {
        case IO_METHOD_READ:
                init_read(fmt.fmt.pix.sizeimage);
                break;

        case IO_METHOD_MMAP:
                init_mmap();
                break;

        case IO_METHOD_USERPTR:
                init_userp(fmt.fmt.pix.sizeimage);
                break;
        }
}

void Capture::init_read(unsigned int buffer_size)
{
        buffers = (buffer_type*)calloc(1, sizeof(*buffers));

        if (!buffers) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }

        buffers[0].length = buffer_size;
        buffers[0].start = malloc(buffer_size);

        if (!buffers[0].start) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }
}

void Capture::init_mmap(void)
{
        struct v4l2_requestbuffers req;

        CLEAR(req);

        req.count = 4;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;

        if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s does not support memory mapping\n", dev_name);
                        exit(EXIT_FAILURE);
                } else {
                        logerr("VIDIOC_REQBUFS");
                }
        }

        if (req.count < 2) {
                fprintf(stderr, "Insufficient buffer memory on %s\n",
                         dev_name);
                exit(EXIT_FAILURE);
        }

        buffers = (buffer_type*)calloc(req.count, sizeof(*buffers));

        if (!buffers) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }

        for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
                struct v4l2_buffer buf;

                CLEAR(buf);

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = n_buffers;

                if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
                        logerr("VIDIOC_QUERYBUF");

                buffers[n_buffers].length = buf.length;
                buffers[n_buffers].start =
                        mmap(NULL /* start anywhere */,
                              buf.length,
                              PROT_READ | PROT_WRITE /* required */,
                              MAP_SHARED /* recommended */,
                              fd, buf.m.offset);

                if (MAP_FAILED == buffers[n_buffers].start)
                        logerr("mmap");
        }
}

void Capture::init_userp(unsigned int buffer_size)
{
        struct v4l2_requestbuffers req;

        CLEAR(req);

        req.count  = 4;
        req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_USERPTR;

        if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s does not support "
                                 "user pointer i/o\n", dev_name);
                        exit(EXIT_FAILURE);
                } else {
                        logerr("VIDIOC_REQBUFS");
                }
        }

        buffers = (buffer_type*)calloc(4, sizeof(*buffers));

        if (!buffers) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }

        for (n_buffers = 0; n_buffers < 4; ++n_buffers) {
                buffers[n_buffers].length = buffer_size;
                buffers[n_buffers].start = malloc(buffer_size);

                if (!buffers[n_buffers].start) {
                        fprintf(stderr, "Out of memory\n");
                        exit(EXIT_FAILURE);
                }
        }
}

void Capture::uninit_device(void)
{
        unsigned int i;

        switch (io) {
        case IO_METHOD_READ:
                free(buffers[0].start);
                break;

        case IO_METHOD_MMAP:
                for (i = 0; i < n_buffers; ++i)
                        if (-1 == munmap(buffers[i].start, buffers[i].length))
                                logerr("munmap");
                break;

        case IO_METHOD_USERPTR:
                for (i = 0; i < n_buffers; ++i)
                        free(buffers[i].start);
                break;
        }

        free(buffers);
}

void Capture::start_capturing(void)
{
        unsigned int i;
        enum v4l2_buf_type type;

        switch (io) {
        case IO_METHOD_READ:
                /* Nothing to do. */
                break;

        case IO_METHOD_MMAP:
                for (i = 0; i < n_buffers; ++i) {
                        struct v4l2_buffer buf;

                        CLEAR(buf);
                        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                        buf.memory = V4L2_MEMORY_MMAP;
                        buf.index = i;

                        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                                logerr("VIDIOC_QBUF");
                }
                type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
                        logerr("VIDIOC_STREAMON");
                break;

        case IO_METHOD_USERPTR:
                for (i = 0; i < n_buffers; ++i) {
                        struct v4l2_buffer buf;

                        CLEAR(buf);
                        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                        buf.memory = V4L2_MEMORY_USERPTR;
                        buf.index = i;
                        buf.m.userptr = (unsigned long)buffers[i].start;
                        buf.length = buffers[i].length;

                        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                                logerr("VIDIOC_QBUF");
                }
                type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
                        logerr("VIDIOC_STREAMON");
                break;
        }
}
void Capture::close_device(void)
{
        if (-1 == ::close(fd))
                logerr("close");

        fd = -1;
}

int Capture::read_frame() {
    struct v4l2_buffer buf;
    unsigned int i;

    switch (io) {
    case IO_METHOD_READ:
            if (-1 == read(fd, buffers[0].start, buffers[0].length)) {
                    switch (errno) {
                    case EAGAIN:
                            return 0;

                    case EIO:
                            /* Could ignore EIO, see spec. */

                            /* fall through */

                    default:
                            logerr("read");
                    }
            }

            process_frame(buffers[0].start, buffers[0].length);
            break;

    case IO_METHOD_MMAP:
            CLEAR(buf);

            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;

            if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
                    switch (errno) {
                    case EAGAIN:
                            return 0;

                    case EIO:
                            /* Could ignore EIO, see spec. */

                            /* fall through */

                    default:
                            logerr("VIDIOC_DQBUF");
                    }
            }

            assert(buf.index < n_buffers);

            process_frame(buffers[buf.index].start, buf.bytesused);

            if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                    logerr("VIDIOC_QBUF");
            break;

    case IO_METHOD_USERPTR:
            CLEAR(buf);

            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_USERPTR;

            if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
                    switch (errno) {
                    case EAGAIN:
                            return 0;

                    case EIO:
                            /* Could ignore EIO, see spec. */

                            /* fall through */

                    default:
                            logerr("VIDIOC_DQBUF");
                    }
            }

            for (i = 0; i < n_buffers; ++i)
                    if (buf.m.userptr == (unsigned long)buffers[i].start
                        && buf.length == buffers[i].length)
                            break;

            assert(i < n_buffers);

            process_frame((void *)buf.m.userptr, buf.bytesused);

            if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                    logerr("VIDIOC_QBUF");
            break;
    }

    return 1;

}

void Capture::process_frame(void* inp, int length) {

    //memcpy(out_buf,input,length);

    int r1, g1, b1;
    int r2, g2, b2;
    uchar y, u, y2, v;

    uchar* input = (uchar*)inp;
    uchar* output = (uchar*)out_buf;
    uchar* tmp_buf[width*height*6];

    for (int i=0; i< width*height/2; i++){


        if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV){

            //YUYV
            y = input[i*4];
            u = input[i*4+1];
            y2 = input[i*4+2];
            v = input[i*4+3];
        } else if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_UYVY){

            //UYVY
            u = input[i*4];
            y = input[i*4+1];
            v = input[i*4+2];
            y2 = input[i*4+3];
        } else {
            perror("unknown format\n");
            return;
        }

        int c = y-16, d = u - 128, e = v - 128;

        r1 = (298 * c           + 409 * e + 128) >> 8;
        g1 = (298 * c - 100 * d - 208 * e + 128) >> 8;
        b1 = (298 * c + 516 * d           + 128) >> 8;

        // Even with proper conversion, some values still need clipping.

        if (r1 > 255) r1 = 255;
        if (g1 > 255) g1 = 255;
        if (b1 > 255) b1 = 255;
        if (r1 < 0) r1 = 0;
        if (g1 < 0) g1 = 0;
        if (b1 < 0) b1 = 0;

        c = y2-16, d = u - 128, e = v - 128;

        r2 = (298 * c           + 409 * e + 128) >> 8;
        g2 = (298 * c - 100 * d - 208 * e + 128) >> 8;
        b2 = (298 * c + 516 * d           + 128) >> 8;

        // Even with proper conversion, some values still need clipping.

        if (r2 > 255) r2 = 255;
        if (g2 > 255) g2 = 255;
        if (b2 > 255) b2 = 255;
        if (r2 < 0) r2 = 0;
        if (g2 < 0) g2 = 0;
        if (b2 < 0) b2 = 0;


        tmp_buf[i*8] = b1; //B
        tmp_buf[i*8+1] = g1; //G
        tmp_buf[i*8+2] = r1; //R
        tmp_buf[i*8+3] = 255; //A

        tmp_buf[i*8+4] = b2;
        tmp_buf[i*8+5] = g2;
        tmp_buf[i*8+6] = r2;
        tmp_buf[i*8+7] = 255;



    }

     //memcpy(buf->Open(2),tmp_buf,640*3*300);
/*
    int maxH = (height<buf->height) ? height : buf->height;
    int maxW = (width<buf->width) ? width : buf->width;

    for (int riadok=0; riadok < maxH; riadok++){
        memcpy((void*)output+buf->width*riadok*4,(void*)tmp_buf+width*riadok*4,maxW*4);
    }
*/
    buf->Resample(tmp_buf,width,height,output);

    //QImage tento = QImage(tmp_buf,width,height,QImage::Format_RGB888).scaled(buf->width,buf->height);
    //memcpy((void*)output,tento.bits(),tento.byteCount());

    emit(buf->newFrame(outnum));

}

