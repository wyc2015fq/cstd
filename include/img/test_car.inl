
#include <stdio.h>
#include "cstd.h"
#include "macro.h"
#include "findfile.h"
#include "imresize.h"

#include "color.inl"
#include "cvcontours.inl"
#include "cvshapedescr.inl"
#include "cvthresh.inl"
#include "imdraw.inl"
#include "plate.inl"
#include "window.inl"
#include "svm.inl"
#include "../fmtimg/fmtimg.inl"



//#define TEST

#ifdef TEST
#undef WinDrawRectangle
#undef WinDrawRectangleR
#undef cvShowImage
#undef WinDrawLine
#undef cvWaitKey
#define WinDrawRectangle() (0)
#define WinDrawRectangleR() (0)
#define cvShowImage() (0)
#define WinDrawLine() (0)
#define cvWaitKey(a) (0)
#else
//#define imwrite() (0)
#endif

int imshow_th(void* x)
{
  img_t* a = (img_t*)x;
  char ch;

  for (; ;) {
    ch = cvWaitKey(10);

    if ('q' == ch) {
      break;
    }

    imshow("asdf", a);
  }

  return 0;
}

#include <process.h>

int fix_char_pic(int h, int w, const unsigned char* a, int al, unsigned char* b, int bl)
{
  int i, j, u, d, l, r;
  int ver[100] = {0};
  const unsigned char* a0;
  memset(ver, 0, sizeof(ver));
  {
    for (i = 0; i < h; ++i) {
      a0 = a + i * al;
      ver[i] = 0;

      for (j = 0; j < w; ++j) {
        ver[i] += a0[j] > 128;
      }
    }

    u = 0, d = h - 1;

    for (i = 0; i < h / 3; ++i) {
      if (0 == ver[i]) {
        u = i;
      }
    }

    for (i = h; i > h * 2 / 3; --i) {
      if (0 == ver[i]) {
        d = i;
      }
    }
  }
  {
    memset(ver, 0, sizeof(ver));

    for (i = u; i <= d; ++i) {
      a0 = a + i * al;

      for (j = 0; j < w; ++j) {
        ver[j] += a0[j] > 128;
      }
    }

    l = 0, r = w - 1;

    for (i = 0; i < w / 2; ++i) {
      if (0 == ver[i]) {
        l = i;
      }
    }

    for (i = h; i > w * 1 / 2; --i) {
      if (0 == ver[i]) {
        r = i;
      }
    }
  }
  {
    int h0 = d - u + 1, w0 = r - l + 1;
    a0 = a + u * al + l;
    IMRESIZE(h0, w0, a0, al, 1, h, w, b, bl, 1, INTER_BILINEARITY);
    //imwrite("./asdf.bmp", h, w, b, bl, 1);
    cvThreshold(h, w, b, bl, b, bl, 120, 255, CC_THRESH_BINARY);
  }
  return 0;
}

#include "svm.h"

static int print_log(const char* FMT, ...)
{
  FILE* pf = fopen("./log.txt", "a");
  va_list ARGLIST;
  va_start(ARGLIST, FMT);

  if (pf) {
#ifdef PTTIME
    fprintf(pf, "%8s ", PTTIME);
#endif

    vfprintf(pf, FMT, ARGLIST);
    fflush(pf);
  }

#ifdef PTTIME
  //printf("%8s ", PTTIME);
#endif
  vprintf(FMT, ARGLIST);
  va_end(ARGLIST);
  fclose(pf);
  return 0;
}

#define printf print_log

#define STDHEIGHT  48 //图像归一化后的高度24
#define STDWIDTH  24   //图像归一化后的宽度12
static double predict_result = 0;
static const char character[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //字符索引
static const int prob[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 17, 18, 19, 20, 21, 22, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};

#define rough_location_uninit(a) (0)
#define do_plate_detection(a, b) (0)

int plate_cutter_process(plate_cutter* pc);

int test_rough_location()
{
  strlist_t slist;
  int i, j, k, rectcount;
  char buf[ 1000000 ];
  char pathbuf[256];
  CvRect plate_rect[1000];
  svm_model* svm = NULL;
  //get_file_list_txt("F:/pub/bin/car/检测/list.txt", &slist);
  get_file_list_txt("F:/pub/bin/car/训练集/list.txt", &slist);

  for (i = 0; i < slist.used; ++i) {
    img_t* a = imread2(slist.strs[ i ], 3);

    if (a) {
      //utime_start(_start_time);
      rough_location rl = {0};
      rl.min_plate_width = 10, rl.max_plate_width = 45, rl.rectcount = 0;
      rl.plate_rect = plate_rect;
      printf("%4d/%4d %-40s", i, slist.used, slist.strs[ i ]);
      rl.img_height = a->h, rl.img_width = a->w;
      rl.bgr = a->data, rl.bgr_step = a->s;
      rl.maxcount = 4 * MAX(a->w, a->h);
      //rough_location_init(&rl, a->data, a->s, 0);
      rectcount = do_plate_detection(&rl, 0);

      if (1) {
        for (j = 0, k = 0; j < rectcount; ++j) {
          plate_rect[j].x      = MAX(0, plate_rect[j].x - 5);
          plate_rect[j].y      = MAX(0, plate_rect[j].y - 5);
          plate_rect[j].width  = MIN(a->w - plate_rect[j].x - 1, plate_rect[j].width + 10);
          plate_rect[j].height = MIN(a->h - plate_rect[j].y - 1, plate_rect[j].height + 10);

          if (do_plate_detection(&rl, plate_rect + j)) {
            plate_rect[k++] = plate_rect[j];
          }
        }

        rectcount = k;
      }

      // 分割
      for (j = 0; j < rectcount; ++j) {
        char str[16] = {0};
        plate_cutter pc = {0};
        int flag;
        str[0] = 0;

        if (1) {
          pc.plate.rect = plate_rect[j];
          pc.h = a->h, pc.w = a->w, pc.bgr_step = rl.bgr_step;
          pc.bgr = rl.bgr, pc.hsv = rl.hsv, pc.gray = rl.grayimg;
          pc.buf = buf, pc.buflen = sizeof(buf);
          memset(buf, 0, sizeof(buf));
          //if ( strstr( slist.strs[ i ], "142.jpg" ) ) {          int asdf = 0;        }
          flag = plate_cutter_process(&pc);

          if (flag) {
            int m, n, times = 0;
            double label;
            double prob_estimates[36];

            //printf("湘");
            if (NULL == svm) {
              svm = svm_load_model("./48_24_character_train.scale.model");
            }

            for (k = 1; k < 7; ++k) {
              int h = pc.plate.rect.height, w = pc.plate.R[ k ] - pc.plate.L[ k ];
              unsigned char* img = pc.plateRegion + pc.plate.L[ k ];
              svm_node sn[STDHEIGHT * STDWIDTH + 1] = {0};
              unsigned char stdimg1[STDHEIGHT * STDWIDTH] = {0};
              unsigned char stdimg[STDHEIGHT * STDWIDTH] = {0};
              //imwrite( "./plateRegion.bmp", car.plate.rect.height, car.plate.rect.width, car.plateRegion, car.plate.rect.width, 1 );
              IMRESIZE(h, w, img, pc.plate.rect.width, 1, STDHEIGHT, STDWIDTH, stdimg, STDWIDTH, 1, INTER_BILINEARITY);
              cvThreshold(STDHEIGHT, STDWIDTH, stdimg, STDWIDTH, stdimg1, STDWIDTH, 0, 255, CC_THRESH_OTSU);
              //imwrite( "./stdimg.bmp", STDHEIGHT, STDWIDTH, stdimg1, STDWIDTH, 1 );
              fix_char_pic(STDHEIGHT, STDWIDTH, stdimg1, STDWIDTH, stdimg, STDWIDTH);
              //imwrite( "./stdimg.bmp", STDHEIGHT, STDWIDTH, stdimg, STDWIDTH, 1 );
              _snprintf(pathbuf, 256, "%s_clip%d_char%d.bmp", slist.strs[ i ], j, k);

              //imwrite( "pathbuf", STDHEIGHT, STDWIDTH, stdimg, STDWIDTH, 1 );
              //FPRINT2D("./stdimg.txt", "%d,\r\n", "", STDHEIGHT, STDWIDTH, stdimg, STDWIDTH, 1);
              for (m = 0; m < STDWIDTH; ++m) {
                for (n = 0; n < STDHEIGHT; ++n) {
                  sn[m * STDHEIGHT + n].index = m * STDHEIGHT + n;
                  sn[m * STDHEIGHT + n].value = stdimg[m + n * STDWIDTH] > 128 ? 1 : -1;
                }
              }

              sn[STDHEIGHT * STDWIDTH].index = -1;
              // 利用置信度做识别，如果6个字符里有四个字符的置信度都低于0.3，
              // 说明为非车牌。或者置信度小于0.3的个数小于四，但为1的字符大于等于四个
              label = svm_predict_probability(svm, sn, prob_estimates);//概率预测
              //label = svm_predict(svm, sn);//概率预测
              //printf("%c", character[(int)label]);
              str[k - 1] = character[(int)label];

              if (prob_estimates[prob[(int)label]] < 0.3) {
                times++;
              }
            }

            if (times >= 4) {
              flag = 0; //非车牌
              str[0] = 0;
            }

            {
              static const char fix[10] = {'Q', 'Z', '\0', '\0', 'A', 'S', '\0', '\0', 'B', '\0'};

              if (str[0] >= '0' && str[0] <= '9' && fix[str[0] - '0']) {
                str[0] = fix[str[0] - '0'];
              }
            }

            printf(" %8s", str);
          }
        }

        if (1) {
          int h = plate_rect[j].height, w = plate_rect[j].width, x = plate_rect[j].x, y = plate_rect[j].y;
          int h2 = pc.plate.rect.height, w2 = pc.plate.rect.width;
          int si = -((y > (a->h) / 2) ? -1 : 1);
          int* L = pc.plate.L, *R = pc.plate.R;
          DrawRectangle(a->h, a->w, a->data, a->s, a->c, iPOINT(x, y), iPOINT(x + w, y + h), CC_RGB(255, 0, 0), 2, 8, 0);

          if (pc.plateRegion) {
            unsigned char* carI;
            carI = a->data + (y + si * h) * a->s + x * a->c;
            IM2IM(h2, w2, pc.plateRegion, w2, 1, carI, a->s, 3);
          }

          for (k = 0; k < 7; ++k) {
            WinDrawLine("asdf", iPOINT(x + L[ k ], y + si * h), iPOINT(x + L[ k ], y + si * h + h2), CC_RGB(255, 0, 0), 1, 0, 0);
            WinDrawLine("asdf", iPOINT(x + R[ k ], y + si * h), iPOINT(x + R[ k ], y + si * h + h2), CC_RGB(255, 0, 0), 1, 0, 0);
          }

          if (strlen(str)) {
            DrawRectangle(a->h, a->w, a->data, a->s, a->c, iPOINT(x, y + si * (h + h2)), iPOINT(x + 40 * 3, y + si * (h * 2) + 40),
                pc.plate.color_type == PLATE_COLOR_YELOOW ? CC_RGB(255, 255, 0) : CC_RGB(0, 0, 255), -1, 8, 0);
            xDrawText(a->h, a->w, a->data, a->s, a->c, y + si * (h + h2), x, "黑体", 40, 0, 0, 0, str,
                pc.plate.color_type == PLATE_COLOR_YELOOW ? CC_RGB(0, 0, 0) : CC_RGB(255, 255, 255));
          }

          //imwrite( pathbuf, h, w, car.I, w * 3, 3 );
        }
      }

      //printf(" %f\n", utime_elapsed(_start_time));
      //imshow("asdf", a);
      //cvWaitKey(-1);
      _snprintf(pathbuf, 256, "%s_out.jpg", slist.strs[ i ]);

      if (a->h > 10000) {
        IMRESIZE(a->h, a->w, a->data, a->s, a->c,
            a->h / 4, a->w / 4, a->data, (a->w / 4) * 3, 3, INTER_BILINEARITY);
        a->h /= 4, a->w /= 4;
      }

      //imwrite( pathbuf, a->h, a->w, a->data, a->s, a->c );
      rough_location_uninit(&rl);
      imfree(a);
      printf("\n");
    }
  }

  free_strlist(&slist);
  return 0;
}

int test_svm()
{
  int m, n;
  double label;
  //  double prob_estimates[36];
  svm_model* svm;
  svm_node sn[STDHEIGHT * STDWIDTH + 1] = {0};
  unsigned char stdimg[STDHEIGHT * STDWIDTH] = {
#include "./stdimg.txt"
  };

  cvThreshold(STDHEIGHT, STDWIDTH, stdimg, STDWIDTH, stdimg, STDWIDTH, 0, 255, CC_THRESH_OTSU);
  //imwrite( "./stdimg.jpg", STDHEIGHT, STDWIDTH, stdimg, STDWIDTH, 1 );
  svm = svm_load_model("./48_24_character_train.scale.model");

  for (m = 0; m < STDWIDTH; ++m) {
    for (n = 0; n < STDHEIGHT; ++n) {
      sn[m * STDHEIGHT + n].index = m * STDHEIGHT + n;
      sn[m * STDHEIGHT + n].value = stdimg[(m + n * STDWIDTH)] > 128 ? 1 : -1;
    }
  }

  sn[STDHEIGHT * STDWIDTH].index = -1;
  //label = svm_predict_probability(svm, sn, prob_estimates);//概率预测
  label = svm_predict(svm, sn);//概率预测
  printf("%c", character[(int)label]);
  return 0;
}

#define BUFLEN 21000000
char g_buf[BUFLEN];

int test_plate_detect()
{
  strlist_t slist;
  int i;
  char pathbuf[256];
  svm_model* svm = NULL;
  int mode;
  mode = PLATE_DO_DETECT | PLATE_DO_CUTTER;
  mode = PLATE_DO_DETECT;

  //get_file_list_txt("F:/pub/bin/car/检测/list.txt", &slist);
  if (mode & PLATE_DO_DETECT) {
    _chdir("F:/pub/bin/car/测试集");
  }
  else {
    _chdir("F:/pub/bin/car/分割/rect/pos");
  }

  get_file_list_txt("list.txt", &slist);

  for (i = 0; i < slist.used; ++i) {
    img_t* a = imread2(slist.strs[ i ], 3);

    if (a) {
      plate_algo pa = {0};
      img_t* gry = imnew(a->h, a->w, 1);
      //imwrite2txt("./test_car_ccs_pic.txt", a);
      printf("%-40s ", slist.strs[ i ]);
      imshow("asdf", a);
      //cvNamedWindow( "asdf", 0 );
      //NameWindowImage("asdf", a->h, a->w);
      {
        utime_start(_start_time);
        pa.h = a->h, pa.w = a->w, pa.bgr = a->data, pa.bgr_step = a->s;
        pa.buf = g_buf, pa.buflen = BUFLEN;
        pa.mode = mode;
        ColorQuantificationInHSV2(pa.h, pa.w, pa.bgr, pa.bgr_step, 0, 0, gry->data, gry->s);
        imshow("gry", gry);
        cvWaitKey(-1);
        //plate_algo_process(&pa);
        printf("%f", utime_elapsed(_start_time));
      }
      //WinImRotate("asdf", pa)
#if 0
      {
        int j, j1;

        if (pa.mode & PLATE_DO_DETECT) {
          for (j = 0; j < pa.platelen; ++j) {
            WinDrawRectangleR("asdf", pa.plate[j].rect, CC_RGB(255, 0, 0), 2, 8, 0);
          }

          _snprintf(pathbuf, 256, "%s_out%02d.jpg", slist.strs[ i ], j);
          //WinSave("asdf", pathbuf);
        }

        for (j = 0; j < pa.platelen; ++j) {
          CvRect r = pa.plate[j].rect;
          int* L = pa.plate[j].L, *R = pa.plate[j].R;
          int x = r.x, y1 = r.y, y2 = y1 + r.height;

          for (j1 = 0; j1 < 7; ++j1) {
            WinDrawLine("asdf", iPOINT(x + L[ j1 ], y1), iPOINT(x + L[ j1 ], y2), CC_RGB(255, 0, 0), 1, 0, 0);
            WinDrawLine("asdf", iPOINT(x + R[ j1 ], y1), iPOINT(x + R[ j1 ], y2), CC_RGB(255, 0, 0), 1, 0, 0);
          }

          if (pa.mode & PLATE_DO_DETECT) {
            r = RectExtend(r, cvRound(0.05 * r.width), cvRound(0.05 * r.width),
                cvRound(0.1 * r.height), cvRound(0.1 * r.height), a->h, a->w);
            _snprintf(pathbuf, 256, "%s_rect%02d.jpg", slist.strs[ i ], j);
          }
          else {
            _snprintf(pathbuf, 256, "%s_out%02d.jpg", slist.strs[ i ], j);
          }

          //imwrite(pathbuf, r.height, r.width, a->data+r.y*a->s+r.x*3, a->s, 3);
        }
      }
      cvWaitKey(-1);
#endif
      imfree(a);
      printf("\n");
      //break;
    }
  }

  free_strlist(&slist);
  return 0;
}

#define CHARN(N)  (((char*)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ")+N)

int main()
{
  //NameWindowImage("asdf", 600, 800);
  //WindowRectangle("asdf", iPOINT(10, 10), iPOINT(110, 110), CC_RGB(255, 0, 0), -1, 8, 0);
  //cvWaitKey(-1);
  test_plate_detect();
  //test_svm();
  //test_rough_location();
  //test_car();
}
