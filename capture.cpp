#include "capture.h"
#include "stdio.h"


Capture::Capture(char* dev_name, int widt, int heigh)
{

	width=widt;
	height=heigh;

    buf_len = width*height*4;

	//init_read:
        buffer = malloc(buf_len);

        if (!buffer) {
                fprintf(stderr, "Out of memory\n");
                return;
        }

	//open_device
        struct stat st;

        if (-1 == stat(dev_name, &st)) {
                fprintf(stderr, "Cannot identify '%s': %d, %s\n", dev_name, errno,
                                strerror(errno));
                return;
        }

        if (!S_ISCHR(st.st_mode)) {
                fprintf(stderr, "%s is no device\n", dev_name);
                return;
        }

        fd = open(dev_name, O_RDWR /* required */| O_NONBLOCK, 0);

        if (-1 == fd) {
                fprintf(stderr, "Cannot open '%s': %d, %s\n", dev_name, errno,
                                strerror(errno));
                return;
        }

	//init_device:
        struct v4l2_capability cap;
        struct v4l2_cropcap cropcap;
        struct v4l2_crop crop;
        struct v4l2_format fmt;
        unsigned int min;

        if (-1 == xioctl(VIDIOC_QUERYCAP, &cap)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s is no V4L2 device\n", dev_name);
                        return;
                } else {
                        logerr("VIDIOC_QUERYCAP");
                        return;
                }
        }

        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
                fprintf(stderr, "%s is not a video capture device\n", dev_name);
                exit(EXIT_FAILURE);
        }

        if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
                fprintf(stderr, "%s does not support read i/o\n", dev_name);
                exit(EXIT_FAILURE);
        }
        //select video input, standard(not used) and tuner(not used) here
        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (-1 == xioctl(VIDIOC_CROPCAP, &cropcap)) {
                /* Errors ignored. */
        }
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */

        if (-1 == xioctl(VIDIOC_S_CROP, &crop)) {
                switch (errno) {
                case EINVAL:
                        /* Cropping not supported. */
                        break;
                default:
                        /* Errors ignored. */
                        break;
                }
        }

	//CLEAR
        memset (&(fmt), 0, sizeof (fmt));

        //set image properties
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width = width;
        fmt.fmt.pix.height = height;
        //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
        //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
//      fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565;
        fmt.fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;
        fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

        /*      switch (pixel_format)
         {
         case 0:
         fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
         break;
         case 1:
         fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565;
         break;
         case 2:
         fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32;
         break;
         }

         fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565; //lasdlalsdaslda
         */

        if (-1 == xioctl(VIDIOC_S_FMT, &fmt)){
                logerr("VIDIOC_S_FMT");
                return;
        }


        /* Note VIDIOC_S_FMT may change width and height. */

        //check the configuration data
        min = fmt.fmt.pix.width * 2;
        if (fmt.fmt.pix.bytesperline < min)
                fmt.fmt.pix.bytesperline = min;
        min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
        if (fmt.fmt.pix.sizeimage < min)
                fmt.fmt.pix.sizeimage = min;

        fprintf(stderr, "Video bytespreline = %d\n", fmt.fmt.pix.bytesperline);
        fprintf(stderr, "Using READ IO Method\n");
//      buffers=init_read (fmt.fmt.pix.sizeimage);

//      return buffers;
//        return fmt.fmt.pix.sizeimage;

//	pthread_t thread;
//	Capture c;
	pthread_create(&thread, NULL, Capture::staticEntryPoint, this);

    initialized = 1;


}

void * Capture::staticEntryPoint(void * c)
{
    ((Capture *) c)->loop();
    return NULL;
}

void * Capture::loop() 
{
        fd_set fds;
        struct timeval tv;
	int r;

        for (;;) {
                /* needed for select timeout */
                tv.tv_sec = 2;
                tv.tv_usec = 0;

                FD_ZERO(&fds);
                FD_SET(fd, &fds);

                //the classic select function, who allows to wait up to 2 seconds, until we have captured data,
                r = select(fd + 1, &fds, NULL, NULL, &tv);

                if (-1 == r) { //error
                        if (EINTR == errno)
                                continue;
                        logerr("select");
                }

                if (0 == r) {
                        fprintf(stderr, "select timeout\n");
                        exit(EXIT_FAILURE);
                }

                //read one frame from the device and put on the buffer
                read_frame();

        }
}

void Capture::logerr(const char *s) {
        fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
        //exit(EXIT_FAILURE);
}

//a blocking wrapper of the ioctl function
int Capture::xioctl(int request, void *arg) {
        int r;

        do
                r = ioctl(fd, request, arg);
        while (-1 == r && EINTR == errno);

        return r;
}

int Capture::read_frame() {
//	int *errno; // co to je naco?

        if (-1 == read(fd, buffer - 1, buf_len)) {
                switch (errno) {
                case EAGAIN:
                        return 0;

                case EIO:
                        //EIO ignored
                default:
                //        logerr("read")
			return 1;
                }
                return 0; 
        }

//      while (width*height*Bpp != read (*fd, buffers[0].start, buffers[0].length))
//      {
//              usleep(100);
//      }

        return 1;
}

