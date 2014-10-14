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

    connect(clock,SIGNAL(timeout()),this,SLOT(incFrame()));

}

void Buffer::incFrame(){
    frames++;
}

void* Buffer::Open(int number){

    fprintf(stderr, "Opening %d\n",number);

    if(0){ //use local/shared memory

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

    } else {

        int shmid;
        void *shm;

        fprintf(stderr, "Opening %d\n",number);

        if ((shmid = shmget(number + 16000, buf_len, IPC_CREAT | 0666)) < 0) {
            perror("shmget");
            return 0;
        }

        if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
            perror("shmat");
            return 0;
        }

        return shm;
    }

}

void Buffer::newFrame(int number){
    emit(newFrameSignal(number));
}

// Perform a basic 'pixel' enlarging resample.
void Buffer::Resample(uchar* input, int oldWidth, int oldHeight, uchar* output)
{

    double scaleWidth =  (double)width / (double)oldWidth;
    double scaleHeight = (double)height / (double)oldHeight;

    for(int cy = 0; cy < height; cy++)
    {
        for(int cx = 0; cx < width; cx++)
        {
            int pixel = (cy * (width *4)) + (cx*4);
            int nearestMatch =  (((int)(cy / scaleHeight) * (oldWidth *4)) + ((int)(cx / scaleWidth) *4) );

            memcpy(&output[pixel],&input[nearestMatch],3);
/*
            output[pixel    ] =  input[nearestMatch    ];
            output[pixel + 1] =  input[nearestMatch + 1];
            output[pixel + 2] =  input[nearestMatch + 2];
                    */
        }
    }

    //
//    delete[] _data;
//    _data = newData;
 //   _width = width;
//    _height = height;

//    return true;
}
