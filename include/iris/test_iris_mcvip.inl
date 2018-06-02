
#define COLOR_REV
#include "draw/imdraw.inl"
//#include "fmtimg/fmtimg.inl"
#include "./mcvip/test_mcvip.inl"

//typedef int (*CaptureCB)( char* pData , int w , int h , int step , int param , int chID );

enum {MAXBUF2 = 1 << 24, FEATLEN = 1350, MAXPICS = 20, MAXFEATN = MAXPICS * 100, MAXNAME = 32};


typedef struct irisfeat_t {
  char data[FEATLEN];
  char regname[MAXNAME];
} irisfeat_t;

enum {FEATSTEP = sizeof(irisfeat_t)};

typedef struct iriswork_t {
  irisrecog_t* ir;
  uchar* buf;
  int featlen, distthd, distmax;
  int featnum;
  int isreg; // 注册模式
  char regname[MAXNAME];
  int curr_pic; //
} iriswork_t;

#define IRLIBFILENAME  "iris.dat"

int iris_init(iriswork_t* iw, int isreg, char* regname)
{
  uchar* buf = (uchar*)dsp_malloc(MAXBUF2);
  memset(buf, 0, MAXBUF2);
  irisrecog_t* ir = (irisrecog_t*)buf;
  memset(ir, 0, sizeof(irisrecog_t));
  iris_recog_process(ir); // 获取信息
  iw->featlen = ir->featlen;  // 特征长度
  iw->distthd = ir->distthd;  // 推荐阈值
  iw->distmax = ir->distmax;  // 最大距离
  printf("特征长度=%d, 推荐阈值=%d, 最大距离=%d\n", iw->featlen, iw->distthd, iw->distmax);
  iw->ir = ir;
  iw->buf = buf;
  printf("ir : %08x\n", ir);
  {
    FILE* pf = fopen(IRLIBFILENAME, "rb");

    if (pf) {
      fread(&iw->featnum, 4, 1, pf);
      int size = MAXFEATN * FEATSTEP;
      ir->lib_off = MAXBUF2 - size;
      fread(buf + ir->lib_off, iw->featnum * FEATSTEP, 1, pf);
      fclose(pf);
    }
  }

  if (isreg && regname && strlen(regname) > 0) {
    iw->isreg = 1;
    strncpy(iw->regname, regname, 30);
  }

  return 0;
}

int frame = 0;
// 上下翻转
CC_INLINE int flip_ud(int h, int w, void* A, int al)
{
  int i, j, t;
  char* A0 = (char*)A;

  for (i = 0; i < h / 2; ++i) {
    char* A1 = A0 + i * al;
    char* A2 = A0 + (h - 1 - i) * al;

    for (j = 0; j < w; ++j) {
      CV_SWAP(A1[j], A2[j], t);
    }
  }

  return 0;
}

CC_INLINE int UYVY2GRAY(int height, int width, const uchar* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, int isflip)
{

  const unsigned char* s;
  unsigned char* d;
  int l, c, y1, y2;

  for (l = 0; l < height; ++l) {
    s = src + (l&(~1)) * srcstep;

    if (isflip) {
      d = dst + (height - 1 - l) * dststep;
    }
    else {
      d = dst + l * dststep;
    }

    for (c = 0; c < width; c += 4) {
      y1 = s[1];
      y2 = s[3];

      d[0] = y1;
      d[1] = y2;

      y1 = s[5];
      y2 = s[7];

      d[2] = y1;
      d[3] = y2;

      s += 8;
      d += 4;
    }
  }

  return CV_OK;
}

int identifyCB(char* pData , int w , int h , int step , int param , int chID)
{
  return 0;
}

int identifyCB1(char* pData , int w , int h , int step , int param , int chID)
{
  return 0;
}
int dispalyCB1(char* pData , int w , int h , int step , int param , int chID)
{
  return 0;
}

int dispalyCB(char* pData , int w , int h, int step , int param , int chID)
{
  iriswork_t* iw = (irisrecog_t*)param;
  irisrecog_t* ir = iw->ir;
  uchar* buf = iw->buf;
  int cn = step / w;
  uchar* pData2 = (uchar*)memdup(pData, h * step);
  //printf("%d %d %d %d\n", h, w, step, chID);
  printf("%d ", chID);
  //return 0;

  {
    uchar* pGray = (uchar*)malloc(h * w);

    if (w < 100) {
      //printf("ir : %08x\n", ir);
      //printf("w : %d\n", w);
      return 0;
    }

    //printf("ir : %08x\n", buf);
    img_t im[1] = {0};
    IMINIT(im, h, w, pGray, w, 1, 1);
    UYVY2GRAY(h, w, pData2, step, step / w, pGray, w, 1, 1);
    memset(ir, 0, sizeof(irisrecog_t));
    ir->img_off = sizeof(irisrecog_t);
#if 1
    ir->height = im->h, ir->width = im->w;
    ir->color_type = T_GRAY;
    memcpy(buf + ir->img_off, im->tt.data, im->h * im->w);

    //ir->std_off = sizeof(irisrecog_t) + height*width;
    ir->feat_off = ir->img_off + im->h * im->w;
    ir->featnum = iw->featnum;
    ir->featstep = FEATSTEP;
    ASSERT(ir->featstep >= featlen);
    // 必须>=特征长度
    ir->buf_off = ir->feat_off + 10000;
    int size = MAXFEATN * FEATSTEP;
    ir->buflen = MAXBUF2 - ir->buf_off - size;
    ASSERT(ir->buflen > 1024 * 1024); // 保证有1M空间
    ir->lib_off = MAXBUF2 - size;

    ir->mode = IR_ALL_DETECT;
    //ir->mode = IR_BOOST_DETECT|IR_N_DETECT|IR_U_DETECT;
    ir->mode = IR_BOOST_DETECT | IR_N_DETECT;
    ir->mode = IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT;
#endif

    //#define TESTMODE(mm)    if (1) {utime_start(_start_time);ir->mode = mm;iris_recog_process(ir);printf("%.4f %s\n", utime_elapsed(_start_time), #mm );}
#define TESTMODE(mm)    if (1) {utime_start(_start_time);ir->mode = mm;iris_recog_process(ir);printf("%.4f ", utime_elapsed(_start_time));}
    //#define TESTMODE(mm)    if (1) {ir->mode = mm;iris_recog_process(ir);}
    TESTMODE(IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG | IR_FEATURE | IR_RECOG);

    if (ir->rc.w > 0) {
      img_t im[1] = {0};
      char* s = 0;
      char bufout[256];
      IMINIT(im, h, w, pData2, step, cn, 1);

      //imdraw_rect_i(im, 100, 100, 300, 300, rgba(1, 0, 0, 0.5), rgba(0, 1, 0, 0.5), 3);
      if (ir->maxdist_id >= 0 && ir->maxdist_id < ir->featnum) {
        irisfeat_t* irlib = (irisfeat_t*)(buf + ir->lib_off);
        s = bufout;
        sprintf(bufout, "%s, %d", irlib[ir->maxdist_id].regname, ir->maxdist);
        //printf(" %s \n", bufout);
      }

      DrawIris(im, ir, iw->isreg ? CV_RGB(255, 255, 255) : CV_RGB(111, 111, 111), s, 1);
      memcpy(pData, pData2, h * step);
    }

    if (ir->rc.w > 0) {
      int shift1 = 1 << IRIS_SHIFT1;
      int shift2 = 1 << IRIS_SHIFT2;
      printf(" (%3d %3d %3d %3d)", ir->rc.x, ir->rc.y, ir->rc.w, ir->rc.h);
#if 1
      printf("(%3d %3d %3d)", ir->in[0] / shift1, ir->in[1] / shift1, ir->in[2] / shift1);
      printf("(%3d %3d %3d)", ir->iw[0] / shift1, ir->iw[1] / shift1, ir->iw[2] / shift1);
      printf("(%3d %3d %3d)", ir->iu[0] / shift2, ir->iu[1] / shift2, ir->iu[2] / shift2);
      //printf("(%3d %3d %3d)", ir->id[0]/ shift2, ir->id[1]/ shift2, ir->id[2]/ shift2);
#else
      printf("(%3d %3d %3d)", ir->in[0], ir->in[1], ir->in[2]);
      printf("(%3d %3d %3d)", ir->iw[0], ir->iw[1], ir->iw[2]);
      printf("(%3d %3d %3d)", ir->iu[0], ir->iu[1], ir->iu[2]);
      //printf("(%3d %3d %3d)", ir->id[0], ir->id[1], ir->id[2]);
#endif
      printf("(%4d %3d)%d", ir->maxdist, ir->maxdist_id, ir->cnt_zero);
    }

    printf("\n");

    if (iw->isreg) {
      irisfeat_t* irlib = (irisfeat_t*)(buf + ir->lib_off);
      irisfeat_t* irfeat = irlib + iw->featnum + iw->curr_pic;
      memcpy(irfeat->data, buf + ir->feat_off, FEATLEN);
      memcpy(irfeat->regname, iw->regname, MAXNAME);
      iw->curr_pic++;

      if (iw->curr_pic >= MAXPICS) {
        iw->curr_pic = 0;
        iw->isreg = 0;
        iw->featnum += MAXPICS;
        {
          FILE* pf = fopen(IRLIBFILENAME, "wb");

          if (pf) {
            fwrite(&iw->featnum, 4, 1, pf);
            fwrite(irlib, iw->featnum * FEATSTEP, 1, pf);
            fclose(pf);
          }
        }
      }
    }

#ifdef _FMTIMG_INL_

    if (0) {
      img_t im2[1] = {0};
      imcolorcvt(im, im2, T_UYVY, T_RGB);
      ++frame;
      char buf1[256];
      //snprintf(buf, 256, "aa%d.bmp");
      imwriteA("aaaa.jpg", im2, 0);
      imfree(im2);
    }

#endif // _FMTIMG_INL_
    free(pGray);
  }
  free(pData2);
  return 0;
}

#include "test_send_pic.inl"

int test_iris_mcvip(int argc, char* argv[])
{
  int numMCVIP;
  int debug = 0;
  numMCVIP = 1;
  iriswork_t iw[1] = {0};
  // return test_iris_detect_arm();
  iris_recog_open();
  iris_init(iw, argc > 1, argv[1]);
  TVP5158Init(0 , iw, dispalyCB , iw);

  int start = 1;

  while (getchar() != 'q') {
    if (start) {
      DISPLAY_stop();
      CAPTURE_stop();
      start = 0;
    }
    else {
      DISPLAY_start();
      CAPTURE_start();
      start = 1;
    }
  }

  TVP5158UnInit();

  dsp_free(iw->buf);
  iris_recog_close();
  return 0;
}


