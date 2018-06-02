
#include "img/color.inl"

uchar buf[10000000] = {0};
uchar buf2[10000000] = {0};
int test_yuv2bmp()
{
  char outfilename[256];
  int i = 0;
  _chdir("D:\\pub\\cstd\\DSP\\dm6446\\raw");
  _chdir("D:\\pub\\cstd\\DSP\\dm6446\\ttt2\\raw");
  _chdir("D:\\pub\\cstd\\raw");
  begin_find(info, "*.uyvy");
  FILE* pf = fopen(filename, "rb");
  _snprintf(outfilename, 256, "%s.bmp", filename);

  if (pf) {
    int sz = fsize(pf);
    int len, h = 1250 / 2, w = 728 + 4 + 4;
    w *= 2;
    h = 600, w = 800;
    h = 288, w = 352;
    h = 640, w = 800;
    h = 800, w = 640;
    h = 240, w = 320;
    h = 480, w = 640;
    h = 576, w = 720;
    len = h * w * 2;
    fread(buf, sz, 1, pf);
    //memset(buf2, 0, 1000000);
#if 0

    for (i = 0; i < sz - len + 1; i += len) {
      colorcvt(h, w, buf + i, w * 2, 2, buf2, w * 3, 3, T_YUYV, T_BGR);
      imwrite(outfilename, h, w, buf2, w * 3, 3);
      //cvShowImage(outfilename, h, w, buf2, w*3, 3);
      //cvWaitKey(-1);
    }

#else
    colorcvt(h, w, buf, w * 2, buf2, w, T_UYVY, T_GRAY);
    {
      img_t im[1] = {0};
      IMINIT(im, h, w, buf2, w, 1, 1);
      //imwrite(outfilename, im);
    }
#endif
  }

  end_find(info);
  return 0;
}
