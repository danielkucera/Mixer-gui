#include "buffer.h"
#include "stdlib.h"
#include "stdio.h"

Buffer::Buffer(int widt, int heigh, int bp, int fp)
{
    width=widt;
    height=heigh;
    bpp=bp;
    fps=fp;

    buf_len = width*height*bpp;

    fprintf(stderr,"Create buffer, set buf_len %d\n",buf_len);

    for (int i=0; i<32; i++){
        isopen[i]=0;
    }

    clock = new QTimer(this);
    clock->start(1000/fps);

}

void* Buffer::Open(int number){

    fprintf(stderr, "Opening %d\n",number);

    if (isopen[number]==0){
        //fprintf(stderr, "Trying malloc for %d with buf_len %d\n",number, buf_len);
        start[number] = malloc(buf_len);
        memset(start[number],0,buf_len);
    }

    if (!start[number]) {
            fprintf(stderr, "Out of memory for %d\n",number);
            return 0;
    } else {

        isopen[number]=1;
        return start[number];
    }

}

void Buffer::newFrame(int number){
    emit(newFrameSignal(number));
}
