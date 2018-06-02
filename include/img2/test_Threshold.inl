#include "cstd.h"
//#include "ui/window.inl"
#include "imgui/imgui.inl"
#include "imgui/ctrl.inl"

void DrawHistGram(img_t* im, const int* Histgram, int Thr)
{
  int X, Y, Max = 0;
  byte* P;
  for (Y = 0; Y < 256; Y++) {
    if (Max < Histgram[Y]) {
      Max = Histgram[Y];
    }
  }
  Max = MAX(Max, 1);
  for (X = 0; X < 256; X++)
  {
    P = im->tt.data + X;
    for (Y = 0; Y < 100; Y++)
    {
      if ((100 - Y) > Histgram[X] * 100 / Max)
        *P = 220;
      else
        *P = 0;
      P += im->s;
    }
  }
  
  P = (byte*)im->tt.data + Thr;
  for (Y = 0; Y < 100; Y++)
  {
    *P = 255;
    P += im->s;
  }
  return ;
}


int test_Threshold_ctrl(ctrl_t* c) {
  SYSIO;
  static ctrl_t cc[10] = {0};
  IRECT rc = c->rc, rcTop;
  static int inited = 0;
  static img_t im[10] = {0};
  static IPOINT pos[10] = {0};
  static double scale[10] = {1, 1, 1};
  static DPOINT dpos[10];
  int i=0, j;
  img_t* im1 = im+i++;
  img_t* imbin = im+i++;
  img_t* HistBmp = im+i++;
  img_t* SmoothHistBmp = im+i++;
  static int Thr = 100;
  static int HistGram[256] = {0};
  static int HistGramS[256] = {0};
  iRectCutT(&rc, 100, 0, &rcTop);
  if (!inited) {
    inited = 1;
    sys_chdir("E:/pub/bin/img2");
    imread("07112915-86bdb268ffd74e24acb2061932e060c8.png", 1, 1, im);
    imclone2(im, imbin);
    imsetsize(HistBmp, 100, 256, 1, 1);
    imsetsize(SmoothHistBmp, 100, 256, 1, 1);
  }
  ctrl_t* ccim;
  i = 0;
  {
    static ctrl_t slider1[1] = {0};
    iRectCutT(&rc, 20, 0, &slider1->rc);
    slider_int_ctrl(slider1, "thd = %d", &Thr, 0, 255);
  }
  flexboxlay(rc, countof(cc), cc, 0, flex_row);
#define THRESHOLDDEF_DEF(THRESHOLDDEF) \
  THRESHOLDDEF( 灰度平均值, MeanThreshold, (HistGram)) \
  THRESHOLDDEF( 黄式模糊阈值, HuangFuzzyThreshold, (HistGram, countof(HistGram))) \
  THRESHOLDDEF( 谷底最小值, MinimumThreshold, (HistGram, HistGramS)) \
  THRESHOLDDEF( 双峰平均值, IntermodesThreshold, (HistGram, HistGramS)) \
  THRESHOLDDEF( 百分比阈值, PTileThreshold, (HistGram)) \
  THRESHOLDDEF( 迭代阈值法, IterativeBestThreshold, (HistGram)) \
  THRESHOLDDEF( 大津法, OSTUThreshold, (HistGram)) \
  THRESHOLDDEF( 一维最大熵, 1DMaxEntropyThreshold, (HistGram)) \
  THRESHOLDDEF( 动能保持, MomentPreservingThreshold, (HistGram)) \
  THRESHOLDDEF( Kittler最小错误, KittlerMinError, (HistGram)) \
  THRESHOLDDEF( ISODATA法, IsoDataThreshold, (HistGram, countof(HistGram))) \
  THRESHOLDDEF( Shanbhag法, ShanbhagThreshold, (HistGram, countof(HistGram))) \
  THRESHOLDDEF( Yen法, YenThreshold, (HistGram, countof(HistGram)))
  {
    const char* s_thd = "|"
#define THRESHOLDDEF(a, b, c)   #a "|"
      THRESHOLDDEF_DEF(THRESHOLDDEF)
#undef THRESHOLDDEF
      ;
    enum {
#define THRESHOLDDEF(a, b, c)   ThresholdFlag##b, 
      THRESHOLDDEF_DEF(THRESHOLDDEF)
#undef THRESHOLDDEF
    };
    static int id = 0;
    if (rbox_ctrl(cc+i++, "使用方法", s_thd, &id)) {
      switch (id) {
#define THRESHOLDDEF(a, b, c)   case ThresholdFlag##b :  Thr = Get##b##c; break;
        THRESHOLDDEF_DEF(THRESHOLDDEF)
#undef THRESHOLDDEF
      }
    }
  }
  
  {
    IRECT rcs[2] = {0};
    texture_t tex[1] = {0};
    iRectMatrix(rcTop, 1, 2, 0, 0, TF_CENTER, 2, rcs);
    GetHistGram(im, HistGram);
    GetHistGram(im, HistGramS);
    BMPINITIM(tex, HistBmp);
    DoBinaryzation1(im, imbin, Thr);

    DrawHistGram(HistBmp, HistGram, Thr);
    gcRectImageR(g, rcs[0], tex, NULL);
    BMPINITIM(tex, SmoothHistBmp);
    DrawHistGram(SmoothHistBmp, HistGramS, Thr);
    gcRectImageR(g, rcs[1], tex, NULL);
  }
  {
    ccim = cc+i;
    for (j = 0; j<2; ++j) {
      texture_t tex[1] = {0};
      BMPINITIM(tex, im+j);
      image_ctrl(ccim+j, tex, pos, scale, dpos);
    }
  }
  return 0;
}

int hist_ctrl(ctrl_t* c, const int* hist) {
  SYSIO;
  IRECT rc = c->rc;
  return 0;
}

int test_Threshold() {
  window_host_t host[2] = {0};
  ctrl_t cc[10] = {0};
  double time1 = 0;
  sysio_t* io = sys_getio();
  font_t fo[1] = {0};
  io->font = fo;
  host_open(host, 200, 200, 800, 600);
  host_create(io, host, "asdf");
  soft_drv_Init(host, 0, 0);
  font_init_memraster(fo, "songti", songti_12);
  for (; sys_waitio(100) && host_begin(host);) {
    utime_start(_start_time);
    test_Threshold_ctrl(host->root->client);
    host_end(host, _rgb(1, 1, 1));
    time1 = utime_elapsed(_start_time);
  }
  return 0;
}
