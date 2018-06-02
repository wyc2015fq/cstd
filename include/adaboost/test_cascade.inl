
#include "face/init.inl"
#include "face/faceDetect.inl"
//#include "../faceDetect/faceRecog.inl"
#include "img/color.inl"
#include "cap/cap.inl"
#include "draw/imdraw.inl"
#include "ui/window.inl"

#define CASCADE_PATH1 "\\\\192.168.1.99\\public\\bin\\可见光行人检测训练\\cas.txt"
#define CASCADE_PATH2 ".\\cas.txt"
#define CASCADE_PATH  CASCADE_PATH2
#define BUFSIZE       (1000)

//int rect_partition( XRECT* B0, XRECT* B, real thd, int mincnt, int sort, void* buf, int* pbuflen );
XRECT rbuf[ BUFSIZE ];
XRECT out[ BUFSIZE ];

int test_cascadepic()
{
  //char buf[256];
  const char* cascade_path = CASCADE_PATH2;
  CASCADE* ca;
  int i, stepxy = 2;
  int allpic = 0, passpic = 0;
  char buf[ 256 ];
  img_t im[1] = {0};
  img_t imrgb[1] = {0};
  FILE* pf;
  const char* piclist = "testlist.txt";
  char picname[256];
  int buflen = 0;
  //_chdir("D:/pub/bin/iris/adaboost");
  _chdir("D:/pub/bin/iris/adaboost");
  //ca = cascade_scanf( pf, 0, 0 );
  ca = cascade_read();

  if (NULL == ca) {
    printf("当前目录没有分类器!\n");
    system("pause");
    return 0;
  }

  pf = fopen(piclist, "rb");

  for (; fscanf(pf, "%s", picname) > 0;) {
    imread(picname, 1, 1, im);
    imread(picname, 3, 1, imrgb);

    if (im->h > 0) {
      int bodynum = 0;
      ++allpic;
      {
        utime_start(time);
        bodynum = detect(ca, im->h, im->w, im->tt.data, im->s, 8.f, 100.f, 1.1f, 2, out, BUFSIZE);
        printf("%14.9g %5d %s\n", utime_elapsed(time), bodynum, picname);
        passpic += !!bodynum;
      }

      if (file_exist("./neg")) {
        //ASSERT( buflen <= sizeof( rbuf ) );
        for (i = 0; i < bodynum; ++i) {
          //XRECT r = out[ i ];
          //_snprintf( buf, 256, ".//neg//%s_%04d.bmp", picname, i );
          //imwrite( buf, r.h, r.w, data + r.y * step + r.x, step, channels );
        }
      }

      bodynum = rect_partition(out, out + bodynum, 0.5f, 0, 0, rbuf, &buflen);

      //ASSERT( buflen <= sizeof( rbuf ) );
      for (i = 0; i < bodynum; ++i) {
        if (out[ i ].count > 0) {
          DrawRectangle2(imrgb, out[ i ], CV_RGB(255, 0, 0), 2);
          imdraw_circle(imrgb, out[ i ].x + out[ i ].w / 2, out[ i ].y + out[ i ].h / 2, 2, 0, CV_RGB(255, 0, 0), 1);
        }
      }

      imshow(imrgb);
      cvWaitKey(-1);

      if (file_exist("./out")) {
        int buflen = 0;
        _snprintf(buf, 256, ".//out//%s", picname);
        imwrite2(buf, imrgb);
      }
    }
  }

  fclose(pf);
  imfree(imrgb);
  imfree(im);
  del_cascade(ca);
  printf("allpic=%5d passpic=%5d passrate=%16.9g\n", allpic, passpic, passpic / allpic);
  system("pause");
  return 0;
}

#if 0
int test_cascadecam(void)
{
  char picname[ 256 ];
  const char* cascade_path = CASCADE_PATH;
  FILE* pf;
  PCASCADE ca, eyeca;
  capcam* cap = 0;
  real mincs = 2.f;
  unsigned char* gry = 0;
  unsigned char* rgb = 0;
  unsigned char* buf = 0;
  int merg = 1;
  int maxcnt = 0;
  pf = fopen(cascade_path, "r");
  ca = cascade_scanf(pf, 0, 0);
  fclose(pf);
  pf = fopen("cas.txt", "r");
  eyeca = cascade_scanf(pf, 0, 0);
  fclose(pf);

  printf("退出 = q\n合并数 = 0-9\n最小框: 按[减小1.1倍 按]增加1.1倍\n");
  //assert(ca->type==ST_EOH);
  cvNamedWindow("cam", 1);
  cap = cam_start(0, 0);

  if (cap) {
    int fram = 0;
    char ch = 0;

    for (; 'q' != ch && 'Q' != ch && cam_GrabFrame(cap); ++fram) {
      int i, bodynum = 0;
      unsigned char* img = cam_begin(cap), t = 0;
      int h = cam_height(cap), w = cam_width(cap), al = cam_step(cap), ai = cam_channel(cap);

      if (!buf) {
        buf = malloc(h * al);
        gry = malloc(h * al);
      }

      //FLIP2D( h, w * ai, img, al, 1, t );
      if (2 == ai) {
        rgb = malloc(h * w * 3);
        //icvYUV4222BGR_8u_C2C3R(h, w, img, al, rgb, w * 3, 3);
        colorcvt(h, w, img, al, 2, rgb, w * 3, 3, T_YUYV, T_BGR);
        img = rgb;
        al = w * 3;
        ai = 3;
      }

      //IMTRANS(h, w, img, al, ai, gry, w, 1, BGR2GRAY);
      colorcvt(h, w, img, al, ai, gry, w, 1, T_BGR, T_GRAY);
      //imwrite(".\\a.bmp", h, w, gry, w, 1);
      {
        utime_start(time);
        bodynum = detect(eyeca, h, w, gry, w, mincs, 100.f, 1.25f, 2, out, BUFSIZE);
        printf("%14.9g %5d\n", (real) utime_elapsed(time), bodynum);

        if (merg && bodynum) {
          int buflen = 0;
          bodynum = rect_partition(out, out + bodynum, 0.5f, 0, 0, rbuf, &buflen);
          ASSERT(buflen <= sizeof(rbuf));

          if (0 && eyeca) {
            int neye;
            XRECT* eyes;
            eyes = out + bodynum;
            neye = detect(eyeca, out[ 0 ].h, out[ 0 ].w / 2,
                gry + w * out[ 0 ].y + out[ 0 ].x, w, 1.f, 100.f, 1.1f, 2, eyes, BUFSIZE - bodynum);
            neye = rect_partition(eyes, eyes + neye, 0.9f, 10, 1, rbuf, &buflen);
            ASSERT(buflen <= sizeof(rbuf));
            bodynum += neye;
          }
        }
      }

      for (i = 0; i < bodynum; ++i) {
        if (out[ i ].count > maxcnt) {
          DrawRectangle2(h, w, img, al, ai, out[ i ], CV_RGB(255, 0, 0), 2);
        }
      }

      //imwrite( ".\\a.bmp", h, w, img, al, ai );
      cvShowImage("cam", h, w, img, al, ai);
      ch = cvWaitKey(10);

      if ('[' == ch && mincs >= 1.f) {
        mincs /= 1.1f;
      }

      if (']' == ch && mincs <= 20.f) {
        mincs *= 1.1f;
      }

      if ('m' == ch || 'M' == ch) {
        merg = !merg;
        ch = 0;
      }

      if ('0' <= ch && '9' >= ch) {
        maxcnt = ch - '0';
      }

      if ('c' == ch || 'C' == ch) {
        _snprintf(picname, 256, "out//%d.bmp", fram);
        imwrite(picname, h, w, img, al, ai);
        ch = 0;
      }
    }

    cam_stop(cap);
  }

  if (buf) {
    free(buf);
    free(gry);
  }

  del_cascade(ca);
  del_cascade(eyeca);
  return 0;
}
#endif

int cascade_conv(const char* cascade_path, const char* hpp_path, const char* inl_path, const char* f_inl_path)
{
  //char buf[256];
  CASCADE* ca;
  //ca = cascade_scanf(pf, 0, 0);
  ca = cascade_read();
  //cascade2hpp( ca, hpp_path );
  cascade2mem(ca, f_inl_path, 0, 0);
  cascade2mem(ca, inl_path, 0, 1);
  del_cascade(ca);
  return 0;
}
#define CASCADE_CONV(_NAME) cascade_conv(#_NAME".txt", #_NAME".hpp", #_NAME".inl", #_NAME"_f.inl")

int test_cascade2mem()
{
  _chdir("D:/pub/bin/iris/adaboost");
  //CASCADE_CONV(NIR_LeftEyeWithGlass20110717);
  CASCADE_CONV(nir_iris20140725);
  return 0;
}

#define XMLFN "./haarcascade_frontalface_alt.xml"

unsigned char lena[] = {
#include "pic/lena_128x128.txt"
};

int test_read_cascade_xml()
{
  int i, n;
  int buflen;
  CASCADE* ca;
  ca = read_cascade_xml(XMLFN);

  for (i = 0; i < 10; ++i) {
    //utime.h
    utime_start(_start_time);
    n = detect(ca, 128, 128, lena, 128, 1, 100, 1.1f, 1, out, 100);
    printf("%f\n", utime_elapsed(_start_time));
  }

  n = rect_partition(out, out + n, 0.8f, 5, 1, rbuf, &buflen);

  for (i = 0; i < n; ++i) {
    img_t im[1] = {0};
    IMINIT(im, 128, 128, lena, 128, 1, 1);
    DrawRectangle2(im, out[ i ], CV_RGB(255, 0, 0), 2);
    imwrite("asdfaf.bmp", 128, 128, lena, 128, 1);
  }

  return 0;
}


