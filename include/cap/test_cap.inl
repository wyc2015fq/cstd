
//#include "sftime.h"
#include "cstd.h"
#include "cfile.h"
#include "cap.h"
#include "net.h"
//#include "draw/imdraw.inl"
//#include "ui/window.inl"
#include "img/color.inl"
#include "img/imgopt.inl"

#ifdef _WIN32
#define cap_open capvfw_open
#else
#define cap_open capv4l_open
#endif

int test_cam()
{
  int high = 0;
  capdev cap[1] = {0};

  if (capdshow_open(cap, 0, 640, 480) > 0) {
    int fram = 0;
    char ch = 0;
    img_t im[1] = {0};

    for (; 'q' != ch && 'Q' != ch; ++fram) {
      cap_getframe(cap, im, CAP_RGB);
      //printf("%d w=%d h=%d\n", fram, im->w, im->h);
      imshow_(im);
      ch = WaitKey(10);
    }

    cap_close(cap);
    imfree(im);
  }

  return 0;
}

#ifdef _WIN32
// 双目
int test_cam2()
{
  int high = 0;
    capdev cape[1] = {0};
    capdev capd[1] = {0};

  if (capdshow_open(cape, 0, 640, 480) > 0 && capdshow_open(capd, 0, 640, 480) > 0) {
    int fram = 0;
    char ch = 0;
    img_t im[3] = {0};

    for (; 'q' != ch && 'Q' != ch; ++fram) {
      cap_getframe(cape, im + 0, 0);
      cap_getframe(capd, im + 1, 0);
      imcath(2, im, im + 2);

      if (ch != -1) {
        printf("%x\n", ch);
      }

      imshow_(im + 2);
      ch = WaitKey(10);
    }

    cap_close(cape);
    cap_close(capd);
    freeims(im, countof(im));
  }

  return 0;
}

double xinxidu1(int n, uchar* A)
{
  double me = 0;
  double mq = 0;
  int i;

  for (i = 0; i < n; ++i) {
    me += A[i];
    mq += A[i] * A[i];
  }

  me /= n;
  mq /= n;
  return sqrt(mq - me * me);
}

double qinxidu(int h, int w, uchar* A, int al)
{
  int i, j;
  double me = 0;

  for (i = 0; i < h; ++i) {
    uchar* A0 = A + i * al;

    for (j = 1; j < w; ++j) {
      int t = (A0[j] - A0[j - 1]);
      me += abs(t * t * t);
    }
  }

  me /= h * w;
  return me;
}

#include "draw/imdraw.inl"

// 测试清晰度
int test_cam_qinxidu()
{
  int high = 0;
  unsigned char* gry = 0;
  unsigned char* buf = 0;
  unsigned char* rgb = 0;
  capdev cap[1] = {0};
#if 0
  if (cap_num(CAP_EROAD) > 0) {
    mode = CAP_EROAD;
  } else {
    const char* text = "没有EROAD专用摄像头";
    MessageBoxExA(NULL, text, text, 0, 0);
  }
#endif
  //cvCreateTrackbar("high", "cam", &high, 300, 0);
  if (capdshow_open(cap, 0, 640, 480) > 0) {
    int fram = 0;
    char ch = 0;
    img_t im[1] = {0};
    //cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0f, 1.0f, 0, 3);
    //cvNamedWindow("cam", 1);

    for (; 'q' != ch && 'Q' != ch; ++fram) {
      cap_getframe(cap, im, CAP_RGB);
      {
        double ee;
        char buf[256];
        utime_start(s);
        //IMTRANS(h, w, img, al, ai, gry, w, 1, BGR2GRAY);
        //colorcvt(h, w, img, al, ai, gry, w, 1, T_BGR, T_GRAY);
        ee = qinxidu(im->h, im->w, im->tt.data, im->s);
        _snprintf(buf, 256, "%.4f", ee);
        //_snprintf(buf, 256, "%.4f", 46871341.165);
        imdrawaa_textv(im, 0, NULL, buf, 50, 50, 0, 1, ColorRed, 2);
        //SKIP_FILTEREX(h, w, img, al, ai, buf, al, ai, 1, 1, DILATION3X3_FT1, unsigned char);
        //printf("%f\n", utime_elapsed(s));
      }

      if (ch != -1) {
        printf("%x\n", ch);
      }
      //printf("%d %d\n", im->w, im->h);

      imshow_(im);
      //cvShowImage("cam", h, w, gry, w, 1);
      ch = WaitKey(10);
    }

    cap_close(cap);
    imfree(im);
  }

  return 0;
}
int test_avi()
{
#if 0
  char buf[ 256 ];
  printf("AVI => BMP 转换工具! ver 1.0\n（拷贝到AVI目录双击即可!）\n");
  begin_find(info, "*.avi");
  capcam* cap = 0;
  char* picname = fileinfo_filename(info);
  char* ext = strrchr(picname, '.');
  //unsigned char t;
  cap = cap_open_avi(picname);

  if (cap) {
    int fram = 0;
    //cam_SetFps ( cap, 30 );
    printf("打开avi文件(%s)成功!\n", picname);
    _snprintf(buf, 256, "%s_bmp", picname);
    _mkdir(buf);

    for (; caperoad_getframe(cap, im); ++fram) {
      _snprintf(buf, 256, "%s_bmp\\%05d.bmp", picname, fram);
      //break;
      //FLIP2D ( h, w * ai, img, al, 1, t );
      //imwrite(buf, h, w, img, al, ai);
      printf("%s\r", buf);
    }

    caperoad_close(cap);
  }
  else {
    printf("打开avi文件(%s)失败!\n", picname);
  }

  end_find(info);
#endif
  return 0;
}
#endif

//#include "img/imgio.inl"
int test_cap()
{
#ifdef _WIN32
  test_cpcode();
#endif
  capdev cap[1] = {0};
  int id = 0;
  //id = capdshow_open(cap, 0, 640, 480);
  id = capdshow_open(cap, 0, 1280, 720);
  if (id) {
    img_t im[1] = {0};
    char* path;
    int issave = 0;
    path = "D:/pub/bin/cap/bmp1";
    sys_mkdir(path);
    sys_chdir(path);
    for (; 1;) {
      char ch;
      cap_getframe(cap, im, CAP_RGB);

      if (issave) {
#ifdef _IMGIO_INL_
        char buf[256];
        static int fram = 1;
        sys_time_t sf[1] = {0};
        sys_gettime(sf);
        _snprintf(buf, 256, "./cap_%02d_%02d_%02d_%03d.bmp", sf->hour, sf->minute , sf->second , sf->millis);
        imwrite(buf, im);
#endif
      }

      printf("%d %d\n", im->h, im->w);
      //colorcvt ( h, w, img, al, ai, gry, w, 1, T_BGR, T_GRAY );
      //SKIP_HISTEQ(h, w, gry, w, 1, gry, w, 1);
      //cvShowImage ( "gry", h, w, gry, w, 1 );
      imshow_(im);
      ch = WaitKey(10);
      ch = tolower(ch);
      if ('s' == ch) {
        issave = !issave;
      }
      if ('q' == ch) {
        break;
      }
    }

    imfree(im);
    cap_close(cap);
  }

  return 0;
}

