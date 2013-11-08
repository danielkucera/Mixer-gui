#include "save.h"

Save::Save(Buffer* buf, int index)
{
    std::ofstream outfile;
    char filename[50];

    time_t now = time(0);

    sprintf(filename, "buffer%d-%ld.raw", index, now);
    outfile.open (filename);
    outfile.write ((const char*)buf->Open(index),buf->buf_len);
    outfile.close();

    sprintf (filename, "buffer%d-%ld.png", index, now);
    QImage img((uchar *)buf->Open(index),buf->width, buf->height, QImage::Format_RGB888);
    img.save(filename);


}
