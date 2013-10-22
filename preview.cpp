#include "preview.h"


Preview::Preview(void * i_buffer, int i_buf_len, int i_fps, int i_width, int i_height)
{
    fp = NULL;

    buffer=i_buffer;
    buf_len=i_buf_len;
    fps=i_fps;
    width=i_width;
    height=i_height;

//    int u_frame = 0;
//    int len = Bpp * width * height;
//    struct buffer output;
//    output.length = width * height * Bpp;
//    output.start = malloc(width * height * Bpp);


//	fp = fopen("/tmp/preview", "w");
//	pipe (fp);

    char cmd[300];
    sprintf(cmd, "mplayer -demuxer rawvideo - -rawvideo w=%d:h=%d:format=rgb32 -name TEST 2>/dev/null >/dev/null",width,height);

    fp = popen(cmd, "we");

    run=true;

    //std::thread test;
    //test->(Thread);

    pthread_create(&thread, NULL, Preview::staticEntryPoint, this);

}

Preview::~Preview(){
    run=false;
    usleep(1000*1000/fps);
    pclose(fp);
}

void * Preview::staticEntryPoint(void * c)
{
    ((Preview *) c)->Thread();
    return NULL;
}



void Preview::Thread(){

    //printf("output thread started\n");
    while (run) {

        usleep(1000*1000/fps);

//        if (frame[out] != u_frame) {
//			usleep(10*1000);
//            u_frame = frame[out];

            //		if (fp){
            //memcpy(output.start, buffers[out].start, len);
            fwrite(buffer, 1, buf_len, fp);
//			} else {
            //			fp = popen("cat > /tmp/kokosy", "w");
//			}

            //		printf("*");
            //		fflush(stdout);

//        } else {
//            usleep(3000);
//        }
    }
}
