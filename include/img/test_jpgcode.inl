
#include <direct.h>
#include "sys/findfile.h"
#include "fmtimg/jpgcode.inl"

int test_jpgcode()
{
  img_t im[1] = {0};
  bufio_t ls[1] = {0};
  _chdir("D:/pub/bin/plate/±ê×¼Í¼Æ¬");
  imread("1.jpg", 3, 1, im);
  //imshow(im);
  //jpgwrite();
  ls->buflen = im->h * im->s;
  ls->buf = MALLOC(uchar, ls->buflen);
  //imwrite2("_adf.jpg");
  jpgwrite(IMARG(im), ls);
  savedata_11("aaa.jpg", ls->buf, ls->i, 0);
  FREE(ls->buf);
  return 0;
}
