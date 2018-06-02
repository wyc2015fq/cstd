#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "cstd.h"
int touchdown0 = 0;
#ifdef _WIN32
#define ROOTPATH "./"
#define DATAPATH "./facedata.dat"
#define am4373_savefile savefile
#define am4373_loadfile(a, b, c) loadfile(a, b, c, 0)
int WT588D_cmd_Play(uint8 ui8AddrIndex)
{
  return 0;
}
#define GET_MSG()
#else
#define ROOTPATH "0:/MTK"
#define DATAPATH "0:/MTK/facedata.dat"
int am4373_savefile(const char* fn, const void* buf, int n);
int am4373_loadfile(const char* fn, void* buf, int n);
int pop_msg();
#define GET_MSG() io->msg = (WindowMsg)pop_msg()
int WT588D_cmd_Play(uint8 ui8AddrIndex);
#endif
#ifdef _WIN32
//linux下通过gettimeofday获取的us数
double get_sys_time()
{
  double total = clock();
  return total;
}
#else
#include <unistd.h>
#include <sys/time.h>
//linux下通过gettimeofday获取的us数
double get_sys_time()
{
#define TIME_NANO_SEC (1000000000U)
#define TIME_NANO_SEC_FL (1000000000.0f)
  uint64 TIMEGetTimeStamp(void);
  double total = TIMEGetTimeStamp() / TIME_NANO_SEC_FL;
  return total;
}
#endif
#include "cstd.h"
//#include "font/font.inl"
#include "easydraw/easydraw.inl"
#include "img/imgio.inl"
//#include "ui/window.inl"
//#include "face/face.inl"
#include "face/face_recog.inl"
#include "pic/pic.inl"
#include "imgui/imgui.inl"
#include "imgui/ctrl.inl"
int showmode = 0;
int picmode = 1;
int mode = 0;
int curid = 0;
int cur_pic_count = 0;
int on_btn_reg(int x)
{
  mode = 1;
  cur_pic_count = 0;
  return 0;
}
typedef struct myfeat_t {
  char feat[4800];
  char userid[64];
  int right; // 权限 0 管理员 1 普通用户
} myfeat_t;
myfeat_t curfeat;
#define MAXPIC 20
#define MAXMEN 10
typedef struct {
  uchar stdface[100 * 100];
} men_t;
men_t curmen;
vstr_t rec[1] = {0};
double time1 = 0;
typedef struct facedata_t {
  int mennum;
  men_t men[MAXMEN];
  myfeat_t featlib[MAXMEN* MAXPIC];
} facedata_t;
facedata_t facedata = {0};
//uchar pic_01_jpg[] = {#include "pic_01_jpg.inl"};
#if 0
static uchar pic_01_720x576x1[] = {
#include "res/pic_01_720x576x1.txt"
};
#endif
static uchar record_60x60x4[] = {
#include "res/记录查询_60x60x4.txt"
};
static uchar seting_60x60x4[] = {
#include "res/设置_60x60x4.txt"
};
static uchar userreg_60x60x4[] = {
#include "res/用户登记_60x60x4.txt"
};
static uchar manage_60x60x4[] = {
#include "res/用户管理_60x60x4.txt"
};
static uchar return_45x45x4[] = {
#include "res/返回_45x45x4.txt"
};
static char test_membuf[10 * _Mb];
int myframe(int h, int w, void* img, int step, int cn, int t, int out_h, int out_w, uchar* out_buf, int out_step, int out_cn)
{
  //int face_recog(int h, int w, const void * data, int step, int cn, const char * s_pixtype, const char * s_mode, int is_trans, double scale,
  // int * xywh, void * stdface, void * feature, const void * featlib, int featstep, int featnum, int * pid, int * pdis);
  //aaa(12);
  char buf[256] = {0};
  int xywh[12] = {0}; // 人脸 左眼 右眼 矩形框位置 [x, y, w, h]
  double get_sys_time();
  double t1 = 0, t2 = 0;
  double sc = 0.2;
  img_t im[1] = {0};
  int ret = 0;
  int featstep = sizeof(myfeat_t);
  int pid = -1;
  int pdis = 0;
  int featnum = facedata.mennum * MAXPIC;
  static const char* smode[] = {"DET", "REG", "REC", "FEAT", "FDET"};
  static int inited = 0;
  static int loging_ok = 0;
  img_t out[1] = {0};
  typedef enum {
    ui_recog, // 识别界面
    ui_menu, // 菜单界面
    ui_record, // 记录查询
    ui_seting, // 基本设置
    ui_userreg, //用户登记
    ui_manage, //用户管理
  } ui_type;
  static ui_type uitype = ui_recog;
  if (!inited) {
    inited = 1;
    MEM_INIT(test_membuf);
    am4373_loadfile(DATAPATH, &facedata, sizeof(facedata));
  }
  if (0 && picmode) {
    img_t im1[1] = {0};
    if (NULL == im1->tt.data) {
      char* fn = 0;
      fn = "pic_01.bmp";
      //imread(fn, 1, 1, im1);
      //imload_mem(pic_01_jpg, countof(pic_01_jpg), 4, im1);
      //savedata_inl("E:/code/cstd/include/easydraw/pic_01_720x576x4.inl", im1->tt.data, im1->h*im1->s);
#if 0
      h = 720, w = 576, cn = 1;
      IMINIT(im1, h, w, pic_01_720x576x1, w * cn, cn, 1);
#endif
      //imshow(im1);
      //imshow_cn("asdf", im1, 1);
      //cvWaitKey(-1);
    }
    h = im1->h, w = im1->w, img = im1->tt.data, step = im1->s, cn = im1->c;
    sc = 0.12;
  }
  else {
    sc = 0.3;
    IMINIT(im, h, w, (uchar*)img, step, cn, 1);
  }
  //if (get_sys_time() - time1 > 1)
  IMINIT(out, out_h, out_w, (uchar*)out_buf, out_step, out_cn, 1);
  IMINIT(im, h, w, (uchar*)img, step, cn, 1);
  if (facedata.mennum == 0) {
    loging_ok = 1;
  }
  if (ui_recog == uitype || ui_userreg == uitype) {
    t1 = get_sys_time();
    //face(0, 0, 0, 0);
    ret = face_recog(h, w, img, step, cn, 1 == cn ? "GRAY" : 3 == cn ? "BGR" : 4 == cn ? "BGRA" : NULL,
        1 == mode ? "REG" : "REC", t, sc, xywh, curmen.stdface, &curfeat, facedata.featlib, featstep, featnum, &pid, &pdis);
    //ret = iris_recog(h, w, img, w, 1, "GRAY", smode[mode], t, sc, xywh, stdface, &curfeat, featlib, featstep, featnum, &pid, &pdis);
    //imput_rect(out, xywh[0], xywh[1], xywh[2], xywh[3], 0, _rgb(1, 0, 0), 2);
    //snprintf(buf, 256, "%d %d", h, w);
    //imput_str(out, 20, 20, buf, -1, 0xffffffff);
    //imshow(out);cvWaitKey(-1);
    t2 = get_sys_time();
    time1 = get_sys_time();
  }
  if (1 == mode) {
    if (ret) {
      snprintf(curfeat.userid, 64, "id_%d", curid);
      curfeat.right = facedata.mennum > 0 ? 0 : 1;
      facedata.featlib[featnum + cur_pic_count] = curfeat;
      ++cur_pic_count;
    }
    if (cur_pic_count >= MAXPIC) {
      facedata.mennum = BOUND(facedata.mennum, 0, MAXMEN - 1);
      facedata.men[facedata.mennum] = curmen;
      cur_pic_count = 0;
      mode = 2;
      uitype = ui_menu;
      ++facedata.mennum;
      curid = facedata.mennum;
      am4373_savefile(DATAPATH, &facedata, sizeof(facedata));
    }
  }
  else {
    if (pid >= 0 && pid < facedata.mennum) {
      loging_ok = 1;
      strv_push_cstr(rec, facedata.featlib[pid].userid, -1);
    }
  }
  if (1) {
    static window_host_t host[2] = {0};
    softgc sg[1] = {0};
    gc_t g[1] = {0};
    static img_t im2[1] = {0};
    int ret1 = 0;
    sysio_t* io = sys_getio();
    static int inited = 0;
    static font_t fo[1] = {0};
    if (!inited) {
      inited = 1;
      sysio_t* io = sys_getio();
      //if (host_create(io, host, name))
      easydraw_font_init(fo);
      io->font = fo;
      soft_drv_Init(host, 0, 1);
      //createTexFont(host->drv, io->font);
    }
    softgc_init(sg, 1);
    IMINIT(sg->im, out_h, out_w, out_buf, out_step, out_cn, 1);
    host->rect = *sg->clip = iRECT2(0, 0, out_w, out_h);
    gc_set_softgc(g, sg);
    ret1 = host_begin(host);
    io->g = g;
    if (1) {
      //#include "mycode_faceui.inl"
#include "mycode_faceui2.inl"
    }
    {
      if (!(io->mouse->down[0] || io->mouse->down[1] || io->mouse->down[2])) {
        io->hitid = NULL;
      }
    }
    //snprintf(buf, 256, "我我 %d %d %d %d %d %d", 0, 0, 0, rec_num0, 0, (int)io->font->h);
    //imput_str(out, 20, 20, buf, -1, 0xffffffff);
  }
  return 0;
}
#include "sys_am437x.inl"

