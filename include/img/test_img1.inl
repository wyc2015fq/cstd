
unsigned char lela[] = {
#include "../faceDetect/lena.txt"
};

int test_cvtColor()
{
  img_t* a = imread2("../faceDetect/ccc.bmp", 3);
  CvCvt cv;
  cv.height = a->h, cv.width = a->w, cv.src_cn = a->c;
  cv.src_step = a->s, cv.src = a->data;
  cv.src_type = T_BGR, cv.dst_type = T_YUYV, cv.depth = 2;
  cv.dst = MALLOC(unsigned char, a->h * a->w * 4);
  cvtColor(&cv);
  SAFEFREE(cv.dst);
  imfree(a);
  return 0;
}

int test_capvfw1()
{
  //char buf[256];
  static const camera_param_t cp = { 720, 576, 15};
  capcam* cap = cam_start(0, &cp);
  capcam* cap2 = NULL;

  if (cap) {
    int h, w, al, ai, bl = 0, bi = 0;
    unsigned char* a;
    unsigned char* img = 0;

    for (;;) {
      char buf[256];
      cam_QueryFrame(cap, h, w, a, al, ai);

      if (NULL == img) {
        img = MALLOC(unsigned char, h * w * 3);
      }

      //CVTCOLOR( h, w, a, al, ai, img, bl, bi, T_YUYV, T_RGB );
      //cvShowImage( "cam", h, w, img, bl, bi );
      imtrans(h, w, a, al, ai, &h, &w, img, &al, 2, 0);
      cvShowImage("cam", h, w, img, al, ai);
      sf_get_time();


      if (NULL != a && file_exist(".\\bmp")) {
        static int i = 0;
        //_snprintf(buf, 256, ".\\bmp\\pic%06d.bmp", i);
        _snprintf(buf, 256, "./bmp/cap%04d%02d%02d%02d%02d%02d%03d.bmp",
            sfyear(), sfmonth(), sfday(), sfhour(), sfminute() , sfsecond() , sfmillis());
        printf("%s\n", buf);
        imwrite(buf, h, w, img, al, ai);
        ++i;
      }

      if (NULL != a && file_exist(".\\jpg")) {
        static int i = 0;
        //_snprintf(buf, 256, ".\\jpg\\pic%06d.jpg", i);
        _snprintf(buf, 256, "./jpg/cap%04d%02d%02d%02d%02d%02d%03d.jpg",
            sfyear(), sfmonth(), sfday(), sfhour(), sfminute() , sfsecond() , sfmillis());
        printf("%s\n", buf);
        imwrite(buf, h, w, img, al, ai);
        ++i;
      }

      if (NULL == cap2 && file_exist(".\\avi")) {
        cap2 = cap_create_avi(".\\avi\\a.avi", h, w, ai);
      }

      if (cap2) {
        cap_writeFrame(cap2, img, al);
      }

      if ('q' == cvWaitKey(10)) {
        break;
      }
    }

    SAFEFREE(img);
    cam_stop(cap);

    if (cap2) {
      cam_stop(cap2);
    }
  }

  return 0;
}

int test_capvfw()
{
  //char buf[256];
  static const camera_param_t cp = { 720, 576, 15};//视频参数 720×576 15帧每秒
  capcam* cap = cam_start(0, 0);    //启动视频
  int trans_flag = 1;//旋转参数
  char ch;

  if (cap) {
    int h, w, al, ai, bl = 0, bi = 0;
    unsigned char* a;
    unsigned char* img = 0;

    for (;;) {
      cam_QueryFrame(cap, h, w, a, al, ai);  //从视频流中获取一帧a

      if (NULL == img) {   //分配空间img
        img = MALLOC(unsigned char, h * w * 3);
      }

      // 旋转图片
      imtrans(h, w, a, al, ai, &h, &w, img, &al, trans_flag, 0);
      cvShowImage("cam", h, w, img, al, ai);  // 显示图片img
      ch = cvWaitKey(10);   //获取用户按键并延迟10ms

      if ('q' == ch) {
        break;
      }

      if ('t' == ch) {
        trans_flag++;
        trans_flag %= 4;
      }
    }

    SAFEFREE(img);   //释放图片内存
    cam_stop(cap);   //停止视频
  }

  return 0;
}

int resi(int ah, int aw, const unsigned char* a, int al,
    int bh, int bw, unsigned char* b, int bl, int ai)
{
  int i, j, x, y, N = 10, n;
  const unsigned char* a0;
  unsigned char* b0, *a2, *a20, *w, *w0;
  w = MALLOC(unsigned char, bh * bw);
  a2 = MALLOC(unsigned char, bh * bw);

  for (i = 0; i < ah; ++i) {
    y = i * bh / ah;
    a0 = a + i * al, b0 = b + i * bl;
    w0 = w + y * bw, a20 = a2 + y * bw, b0 = b + y * bl;

    for (j = 0; j < aw; ++j) {
      x = j * bw / aw;
      w0[x] = N, a20[x] = a0[j], b0[x] = a0[j];
    }
  }

  for (i = 0; i < bh; ++i) {
    w0 = w + i * bw, a20 = a2 + i * bw, b0 = b + i * bl;

    for (j = 0; j < bw; ++j) {
      if (w0[j] != N) {
        w0[j] = 0;
        a20[j] = 0;
        b0[j] = 0;
      }
    }
  }

  // 主循环，渐进插值
  for (n = 0; n < N; ++n) {
    cvShowImage("asdf", bh, bw, b, bl, 1);
    cvShowImage("a2", bh, bw, a2, bw, 1);
    cvShowImage("www", bh, bw, w, bw, 1);

    //cvWaitKey(10);
    //printf("%d\n", n);
    for (i = 0; i < bh; ++i) {
      int tmp, i1, i2, j1, j2;
      i1 = (MAX(0, i - 1) - i) * bw, i2 = (MIN(bh - 1, i + 1) - i) * bw;
      w0 = w + i * bw, a20 = a2 + i * bw, b0 = b + i * bl;

      for (j = 0; j < bw; ++j) {
        if (w0[j] != N) {
          j1 = MAX(0, j - 1), j2 = MIN(bw - 1, j + 1);
          tmp = w0[j] + w0[i1 + j] + w0[i2 + j] + w0[j1] + w0[j2];

          if (tmp) {
            b0[j] = (w0[j] * a20[j] + w0[i1 + j] * a20[i1 + j] + w0[i2 + j] * a20[i2 + j] + w0[j1] * a20[j1] + w0[j2] * a20[j2]) / tmp;
          }

          w0[j] = n;
        }
      }
    }

    for (i = 0; i < bh; ++i) {
      a20 = a2 + i * bw, b0 = b + i * bl;
      memcpy(a20, b0, bw);
    }
  }

  SAFEFREE(w);
  SAFEFREE(a2);
  return 0;
}

#include "imresize.h"


img_t* imresize(img_t* a, int height, int width, int method)
{
  img_t* b;

  if (0 == width) {
    width = height * a->w / a->h;
  }

  if (0 == height) {
    height = width * a->h / a->w;
  }

  b = imnew(height, width, a->c);

  if (0 == method) {
    IMRESIZE(a->h, a->w, a->data, a->s, a->c,
        b->h, b->w, b->data, b->s, b->c, INTER_BILINEARITY);
  }
  else {
    resi(a->h, a->w, a->data, a->s,
        b->h, b->w, b->data, b->s, a->c);
  }

  return b;
}

int test_resi()
{
  int i;
  strlist_t slist = {0};
  get_file_list_txt(".list.txt", &slist);

  for (i = 0; i < slist.used; ++i) {
    img_t* a = imread2(slist.strs[i], 1), *b, *c;
    cvNamedWindow("asdf", 0);
    cvNamedWindow("www", 0);

    if (a) {
      {
        utime_start(_start_time);
        b = imresize(a, a->h * 3, a->w * 3, 0);
        printf("%f\n", utime_elapsed(_start_time));
      }
      {
        utime_start(_start_time);
        c = imresize(b, a->h * 3, a->w * 3, 1);
        printf("%f\n", utime_elapsed(_start_time));
      }
      imshow(a);
      imshow(b);
      imshow(c);
      cvWaitKey(-1);
      imfree(a);
      imfree(b);
      imfree(c);
    }
  }

  free_strlist(&slist);
  return 0;
}

int test_aa()
{
  FILE* pf = fopen(".\\VideoCtrl.dll", "rb");
  int n = 0, nn;
  unsigned char* buf;

  if (pf) {
    fseek(pf, 0, SEEK_END);
    n = ftell(pf);
    buf = pmalloc(n + 20);
    fseek(pf, 0, SEEK_SET);
    nn = fread(buf, 1, n, pf);
    DATA2MEN(".\\VideoCtrl.dll.txt", n, buf, 0);
    //FPRINT1D(".\\VideoCtrl.dll.txt", "%d,\r\n", n, buf, 1);
    pfree(buf);
    fclose(pf);
  }

  return 0;
}

int test_img2avi()
{
  FILE* plisttxt;
  int i;
  char buf[256];
  img_t* im;
  capcam* cap2 = NULL;
  int caph = 0, capw = 0;
  plisttxt = fopen("list.txt", "rb");

  if (NULL == plisttxt) {
    return 0;
  }

  for (i = 0; fgets(buf, 256, plisttxt) > 0;) {
    img_t* im2;
    strtrim(buf, "\n\r");
    im2 = imread2(buf, 3);

    if (NULL == im2) {
      continue;
    }

    im = imresize(im2, 0, 720, 0);
    imfree(im2);

    if (NULL == cap2 // && file_exist(".\\avi")
       ) {
      caph = im->h, capw = im->w;
      cap2 = cap_create_avi(".\\a.avi", im->h, im->w, 3);
    }

    if (cap2) {
      printf("%s:%3dx%3d\n", buf, im->h, im->w);

      if (caph == im->h && capw == im->w) {
        cap_writeFrame(cap2, im->data, im->s);
      }
      else {
      }
    }

    imfree(im);
  }

  if (cap2) {
    cam_stop(cap2);
  }

  return 0;
}
