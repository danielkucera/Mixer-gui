#include "fader.h"

Fader::Fader(int i_buf_len, void * i_a, void * i_b, int i_fps)
{
    buf_len=i_buf_len;
    out=malloc(buf_len);
    a=i_a;
    b=i_b;
    fps=i_fps;

    rate=0;

    pthread_create(&thread, NULL, Fader::staticEntryPoint, this);
}

void * Fader::staticEntryPoint(void * c)
{
    ((Fader *) c)->Thread();
    return NULL;
}


void Fader::Thread(){

    int y;
    int i;

    while(true){
        usleep(1000*1000/fps);
        y=100-rate;

        if (rate==100) {

            memcpy(out,b,buf_len);

        } else if (rate==0){
            memcpy(out,a,buf_len);

        } else {

        for(i=0; i<buf_len;i++){

            (*(char*)(out + i)) = 0;

            //(*(char*)(*out + i))=(char)((a[i]*rate+b[i]*y)/100);

 //           (*(char*)(*out + i))=(rate*((char*) a)[i] + y*((char*) b)[i])/100;

//            (*(char*)(out + i)) =(rate*((char*) a)[i] + y*((char*) b)[i])/100  ;
            (*(char*)(out + i)) =(rate*((char*) a)[i] + y*(255))/100  ;

           // (*(char*)(out + i))= 5;

            //char lolo = test[1];

        }

        }

    }

}
