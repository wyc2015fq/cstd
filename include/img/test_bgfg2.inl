
#include <stdio.h>
#include <direct.h>
#include "win.h"
#include "cstd.h"
#include "macro.h"
#include "imresize.h"

#include "str.h"
//#include "cap.inl"
#include "ui\window.inl"
#include "draw\imdraw.inl"
//#include "cvbgfg_gaussmix.inl"
//#include "cvbgfg_gaussmix_2.inl"
#include "img/cvbgfg_avg.inl"
#include "img/color.inl"
#include "fmtimg/fmtimg.inl"


int get_no_zero_pix_number(int n, const unsigned char* x)
{
  int i, k;
  k = 0;

  for (i = 0; i < n; ++i) {
    k += !!x[ i ];
  }

  return k;
}
//XRECT out[1000];
//#include "..\faceDetect\cascadearr.h"

#if 0

int test_bgfg2()
{
  capcam* cap = 0;
  int high = 0;
  float cs = 0.5f;
  unsigned char* gry = 0;
  unsigned char* buf = 0;
  unsigned char* rgb = 0;
  CvBGStatModel* pBGStatModel = NULL;
  //cvCreateTrackbar ( "high", "cam", &high, 300, 0 );
  cap = cam_start(0, 0);

  if (cap) {
    int fram = 0;
    char ch = 0;

    for (; 'q' != ch && 'Q' != ch && cam_GrabFrame(cap); ++fram) {
      unsigned char* img = cam_begin(cap);
      unsigned char* img2, * img1;
      int h = cam_height(cap), w = cam_width(cap), al = cam_step(cap), ai = cam_channel(cap);
      int h2 = cvFloor(h * cs), w2 = cvFloor(w * cs), al2 = w2 * ai;

      if (0 == fram) {
        cvNamedWindow("cam", 1);
        buf = MALLOC(unsigned char, h * al);
        img2 = MALLOC(unsigned char, h * al);
        img1 = MALLOC(unsigned char, h * al);
        gry = MALLOC(unsigned char, h * al);
      }

      IMRESIZE(h, w, img, al, ai, h2, w2, img2, al2, ai, INTER_BILINEARITY);
      colorcvt(h2, w2, img2, al2, ai, img1, w2, 1, T_BGR, T_GRAY);

      if (0 == fram) {
        //cvShowImage( "asdf", h2, w2, img2, al2, ai ); cvWaitKey(-1);
        pBGStatModel = cvCreateAvgBGModel(h2, w2, img2, al2, ai, 0);
        //pBGStatModel = cvCreateGaussianBGModel( h2, w2, img2, al2, ai, 0 );
        //pBGStatModel = cvCreateGaussianBG2Model( h2, w2, img2, al2, ai, 0 );
        //pBGStatModel = cvCreateFGDStatModel( h2, w2, img, al2, ai, 0 );
      }
      else {
        ASSERT(h2 == pBGStatModel->h && w2 == pBGStatModel->w && ai == pBGStatModel->nChannels);
        {
          int n = h2 * w2, m;
          utime_start(_start_time);
          cvUpdateBGStatModel(img2, al2, pBGStatModel);

          m = get_no_zero_pix_number(n, pBGStatModel->foreground);
          printf("%4d %1.4f %1.6f ", fram, m * 1. / n, utime_elapsed(_start_time));
        }
      }

#define RECT_MUL(a, b)  { (a).x=(int)((a).x*(b)); (a).y=(int)((a).y*(b)); (a).w=(int)((a).w*(b)); (a).h=(int)((a).h*(b)); }

#if 0

      if (CPM_FaceDetect(vis_nesting_face0701, h2, w2, img1, w2, pBGStatModel->foreground, w2,
          1, 1, 100, 1.1f, 1, 0.3f, 4, out, 1000)) {
        RECT_MUL(out[ 0 ], 1. / cs);
        DrawRectangle2(h, w, img, al, ai, out[ 0 ], CC_RGB(255, 0, 0), 3);
      }

#endif

      if (ch != -1) {
        printf("%x\n", ch);
      }

      printf("\n");
      cvShowImage("foreground", h2, w2, img1, w2, 1);
      cvShowImage("foreground", h2, w2, pBGStatModel->foreground, w2, 1);
      cvShowImage("background", h2, w2, pBGStatModel->background, al2, ai);
      cvShowImage("cam", h, w, img, al, ai);
      //cvShowImage("cam", h, w, gry, w, 1);
      ch = cvWaitKey(10);
    }

    cam_stop(cap);
  }

  cvReleaseBGStatModel(&pBGStatModel);
  FREE(buf);
  FREE(gry);
  FREE(rgb);
  return 0;
}

int test_bgfg_cam()
{
  capcam* cap = 0;
  int high = 0;
  float cs = 0.5f; // 图像缩放比例，用来减少建模时的计算量
  unsigned char* gry = 0;
  unsigned char* buf = 0;
  unsigned char* rgb = 0;
  bgmodel bg;
  //cvCreateTrackbar ( "high", "cam", &high, 300, 0 );
  cap = cam_start(0, 0);

  if (cap) {
    int fram = 0;
    char ch = 0;

    for (; 'q' != ch && 'Q' != ch && cam_GrabFrame(cap); ++fram) {
      unsigned char* img = cam_begin(cap);
      unsigned char* img2, * img1;
      int h = cam_height(cap), w = cam_width(cap), al = cam_step(cap), ai = cam_channel(cap);
      int h2 = cvFloor(h * cs), w2 = cvFloor(w * cs), al2 = w2 * ai;

      if (0 == fram) {   // 如果是第一帧，分配空间。
        cvNamedWindow("cam", 1);
        buf = MALLOC(unsigned char, h * al);
        img2 = MALLOC(unsigned char, h * al);
        img1 = MALLOC(unsigned char, h * al);
        gry = MALLOC(unsigned char, h * al);
      }

      // 把图像缩小
      IMRESIZE(h, w, img, al, ai, h2, w2, img2, al2, ai, INTER_BILINEARITY);
      // 把RGB转换成灰度
      colorcvt(h2, w2, img2, al2, ai, img1, w2, 1, T_BGR, T_GRAY);

      if (0 == fram) {// 如果是第一帧，建立模型
        //pBGStatModel = cvCreateAvgBGModel( h2, w2, img2, al2, ai, 0 );
        bg.height = h, bg.width = w, bg.step = al2, bg.nChannels = ai, bg.mode = BGMODEL_INIT;
        bg.frame = img2;
        bg.update_rate = (int)((0.2) * (1 << 13));
        bg.update_thd = 10;
        // 获取建模需要的内存大小。
        bgmodel_memsize(&bg);
        bg.buf = (char*)pmalloc(bg.buflen);
        bgmodel_process(&bg); // 初始化 bg.mode = BGMODEL_INIT
      }
      else {
        int n = h2 * w2, m;
        utime_start(_start_time);
        //cvUpdateBGStatModel( img2, al2, pBGStatModel );
        bg.height = al2, bg.nChannels = ai, bg.mode = BGMODEL_UPDATE;
        bg.frame = img2;
        bgmodel_process(&bg); // 更新背景 bg.mode = BGMODEL_UPDATE
        m = get_no_zero_pix_number(n, bg.foreground);
        // 打印的数据，第一列是帧编号，第二列是这一帧与背景的差异度（0~1之间），第三列是耗时(秒)
        logprintf("%4d %1.4f %1.6f \n", fram, m * 1. / n, utime_elapsed(_start_time));
      }

#define RECT_MUL(a, b)  { (a).x=(int)((a).x*(b)); (a).y=(int)((a).y*(b)); (a).w=(int)((a).w*(b)); (a).h=(int)((a).h*(b)); }

#if 0

      if (CPM_FaceDetect(vis_nesting_face0701, h2, w2, img1, w2, bg.foreground, w2,
          1, 1, 100, 1.1, 1, 0.3f, 4, out, 1000)) {
        RECT_MUL(out[ 0 ], 1. / cs);
        DrawRectangle2(h, w, img, al, ai, out[ 0 ], CC_RGB(255, 0, 0), 3);
      }

#endif

      if (ch != -1) {
        printf("%x\n", ch);
      }

      //printf( "\n" );

      cvShowImage("foreground", h2, w2, img1, w2, 1);   // 显示前景缩小后的灰度图
      cvShowImage("foreground1", h2, w2, bg.foreground, w2, 1);   // 显示前景
      cvShowImage("background", h2, w2, bg.background, al2, ai);   // 显示背景
      cvShowImage("cam", h, w, img, al, ai);   // 原图
      //cvShowImage("cam", h, w, gry, w, 1);
      ch = cvWaitKey(10);
    }

    cam_stop(cap);
  }

  FREE(bg.buf);
  FREE(buf);
  FREE(gry);
  FREE(rgb);
  return 0;
}

#include "test_bgfg1.h"
#include "imopt.inl"

int test_bgfg_pic()
{
  FILE* plisttxt;
  int high = 0, i, j;
  unsigned char* rgb = 0;
  //bgmodel bg;
  CvBGStatModel* pBGStatModel = NULL;
  char line[256];
  char buf[256];
  unsigned short* depth_data_2 = (unsigned short*)pmalloc(2 * 480 * 640);
  unsigned short* depth_data = (unsigned short*)pmalloc(2 * 480 * 640);
  headcounts_t ph[1] = {0};
  //cvCreateTrackbar ( "high", "cam", &high, 300, 0 );
  //_chdir("D:/pub/bin/imgproc");
  _chdir("C:/video/depth_data");
  _chdir("D:/pub/bin/imgproc/pic/depth_data");
  plisttxt = fopen("list.txt", "rb");

  if (NULL == plisttxt) {
    return 0;
  }

  {
    int fram = 0;
    char ch = 0;
    unsigned char* img, * img2 = 0, * img1 = 0, * img3 = 0, * img4 = 0;

    for (; 'q' != ch && 'Q' != ch && fgets(line, 256, plisttxt) > 0; ++fram) {
      int h, w, al, ai, h2, w2, al2, h3, w3, al3;
      int invs = 2;
      double ss = 1. / invs;
      img_t im2[1] = {0};
      img_t im3[1] = {0};
      {
        utime_start(_start_time1);
        {
          utime_start(_start_time2);
          strtrim(line, "\n\r");

          if (1) {
            img_t im4[1] = {0};
            img_t im5[1] = {0};
            FILE* pf = fopen(line, "rb");

            if (NULL == pf) {
              continue;
            }

            fread(depth_data_2, 2 * 480 * 640, 1, pf);
            h = 480, w = 640, ai = 3, al = w * ai;
            imsetsize(im3, h, w, 3, 1);
            imsetsize(im4, h, w, 1, 1);
            imsetsize(im5, h, w, 1, 1);

            for (i = 0; i < h * w; ++i) {
              int t = depth_data_2[i] * 255 / 5000;
              //if (t<20) t=0;
              im5->tt.data[i] = t;
            }

            imMedian(h, w, im5->tt.data, w, im4->tt.data, w, 3);
            imshow(im4);

            for (i = 0; i < h * w; ++i) {
              int t = depth_data_2[i] * 255 / 5000;
              t = im4->tt.data[i];
              //if (t<20) t=0;
              im3->tt.data[i * 3 + 0] = t;
              im3->tt.data[i * 3 + 1] = t;
              im3->tt.data[i * 3 + 2] = t;
            }

            imfree(im4);
            imfree(im5);
            fclose(pf);
          }
          else {
            imread(line, 3, 1, im3);

            if (NULL == im3) {
              continue;
            }
          }

          imresize2(im3, ss, im2);
          imfree(im3);
          img = im2->tt.data;
          h = im2->h, w = im2->w, al = im2->s, ai = im2->c;
          h2 = h, w2 = w, al2 = w2 * ai;
          h3 = h2 / 2, w3 = w2 / 2, al3 = w3 * ai;
          h3 = h2, w3 = w2, al3 = w3 * ai;

          for (i = 0; i < h2; ++i) {
            for (j = 0; j < w2; ++j) {
              ASSERT(i * invs * 640 + j * invs < 640 * 480);
              depth_data[i * w2 + j] = depth_data_2[(i * invs * 640) + j * invs];
            }
          }

          if (0 == fram) {   // 如果是第一帧，分配空间。
            cvNamedWindow("cam", 1);
            img1 = MALLOC(unsigned char, h * al);
            img2 = MALLOC(unsigned char, h * al);
            img3 = MALLOC(unsigned char, h * al);
            img4 = MALLOC(unsigned char, h * al);
          }

          IMRESIZE(h, w, img, al, ai, h2, w2, img2, al2, ai, INTER_BILINEARITY);
          //IMRESIZE(h2, w2, img2, al2, ai, h3, w3, img3, al3, ai, INTER_BILINEARITY);
          colorcvt(h2, w2, img2, al2, ai, img1, w2, 1, T_BGR, T_GRAY);

          printf("%s ", line);

          printf("t0=%.3f ", utime_elapsed(_start_time2));
        }
        {
          utime_start(_start_time);

          if (0 == fram) {
            //cvShowImage( "asdf", h2, w2, img2, al2, ai ); cvWaitKey(-1);
            //pBGStatModel = cvCreateAvgBGModel( h2, w2, img2, al2, ai, 0 );
            //pBGStatModel = cvCreateGaussianBGModel( h3, w3, img3, al3, ai, 0 );
            pBGStatModel = cvCreateGaussianBGModel(h2, w2, img2, al2, ai, 0);
            //pBGStatModel = cvCreateGaussianBG2Model( h2, w2, img2, al2, ai, 0 );
            //pBGStatModel = cvCreateFGDStatModel( h2, w2, img, al2, ai, 0 );
          }
          else {
            //ASSERT( h2 == pBGStatModel->h && w2 == pBGStatModel->w && ai == pBGStatModel->nChannels );
            {
              int n = h2 * w2, m = 0;
              //utime_start( _start_time );
              //cvUpdateBGStatModel( img3, al3, pBGStatModel );
              cvUpdateBGStatModel(img2, al2, pBGStatModel);
              //m = get_no_zero_pix_number(n, pBGStatModel->foreground);
              //printf( "%4d %1.4f %1.6f ", fram, m*1./n, utime_elapsed( _start_time ) );
            }
          }

          printf("t1=%.3f ", utime_elapsed(_start_time));
        }

#define RECT_MUL(a, b)  { (a).x=(int)((a).x*(b)); (a).y=(int)((a).y*(b)); (a).w=(int)((a).w*(b)); (a).h=(int)((a).h*(b)); }

#if 0

        if (CPM_FaceDetect(vis_nesting_face0701, h2, w2, img1, w2, pBGStatModel->foreground, w2,
            1, 1, 100, 1.1f, 1, 0.3f, 4, out, 1000)) {
          RECT_MUL(out[ 0 ], 1. / cs);
          DrawRectangle2(h, w, img, al, ai, out[ 0 ], CC_RGB(255, 0, 0), 3);
        }

#endif

        if (ch != -1) {
          printf("%x\n", ch);
        }

        //cvShowImage( "foreground", h2, w2, img1, w2, 1 );
        //cvShowImage( "org", h2, w2, img1, w2, 1 );
        cvShowImage("foreground", h3, w3, pBGStatModel->foreground, w3, 1);
        //cvShowImage( "background", h3, w3, pBGStatModel->background, al3, ai );
        cvShowImage("cam", h, w, img, al, ai);

        if (fram > 0) {
          char line2[256];
          int ii = 10;
          int aaa[][2] = {
            15, 3,
            15, 5,
            15, 8,
            15, 10,
            20, 3,
            20, 5,
            20, 8,
            20, 10,
            20, 15,
            25, 3,
            25, 5,
            25, 8,
            25, 10,
            25, 15,
            30, 3,
            30, 5,
            30, 8,
            30, 10,
            30, 15,
            30, 20,
            35, 3,
            35, 5,
            35, 8,
            35, 10,
            35, 15,
            35, 20,
          };

          if (NULL == ph->buf) {
            ph->h = h2, ph->w = w2;
            headcounts_init(ph);
            ph->depth = depth_data;
            ph->BI = img3;
          }

          memcpy(ph->BI, pBGStatModel->foreground, h2 * w2);
          //_snprintf(buf, 256, "%s_BI.dat", line); loaddata_11(buf, ph->BI, h2*w2, 0);
          _snprintf(line2, 256, "%s_fg_dep.bmp", line);
          //imwrite(line2, h2, w2, pBGStatModel->foreground, w2, 1);
          //IMRESIZE(h3, w3, pBGStatModel->foreground, w3, 1, h2, w2, img4, w2, 1, INTER_BILINEARITY);

          //for (ii=0; ii<26; ++ii)
          {
            double t3;
            ph->ImerodeThresh = 10 / invs; // 腐蚀阈值
            ph->ImdilateThresh = 15 / invs; // 膨胀阈值
            ph->AreaThresh = 1500 / (invs * invs); // 面积阈值
            ph->FilterLayerThresh = 150 / 4; // 深度滤波
            ph->FilterAreaThresh = 200; // 面积滤波
            ph->verson = 'Y'; // 选择进行腐蚀
            ph->BRsize = 6;
            ph->Order_verson = 1;
            ph->beishu = 1;
            ph->RN = 25, ph->RD = 5;
            {
              utime_start(_start_time3);
              headcounts(ph);
              t3 = utime_elapsed(_start_time3) + 0.01;
            }

            if (1) {
              uchar* FAI_id = (uchar*)pmalloc(h2 * w2 * 3);

              for (i = 0; i < h2 * w2; ++i) {
                FAI_id[3 * i + 0] = ph->FAI[i];
                FAI_id[3 * i + 1] = ph->FAI[i];
                FAI_id[3 * i + 2] = ph->FAI[i];
              }

              for (i = 0; i < ph->pi[0].ptn; ++i) {
                _snprintf(buf, 256, "%d", ph->pi[0].id[i]);
                xDrawText(h2, w2, FAI_id, w2 * 3, 3, ph->pi[0].pt[i].y - 10, ph->pi[0].pt[i].x - 10, "宋体", 20, 20, 0, 1, buf, CC_RGB(255, 0, 0));
              }

              for (i = 0; i < MAXOBJ; ++i) {
                if (ph->se[i].id > 0) {
                  IPOINT* pt = (IPOINT*)ph->se[i].pt;
                  PolyLineM(h2, w2, FAI_id, w2 * 3, 3, &pt, &ph->se[i].ptlen, 1, 0, CC_RGB(255, 0, 0), 5, 0, 0);
                }
              }

              cvShowImage("FAI_id", h2, w2, FAI_id, w2 * 3, 3);
              pfree(FAI_id);
            }

            if (0) {
              _snprintf(buf, 256, "%s_BI.dat", line);
              savedata_11(buf, ph->BI, h2 * w2, 0);
            }

            if (0) {
              _snprintf(buf, 256, "%s_BI.dat", line);
              savedata_11(buf, ph->BI, h2 * w2, 0);
              _snprintf(buf, 256, "%s_DI.dat", line);
              savedata_11(buf, depth_data, 2 * h2 * w2, 0);
              _snprintf(buf, 256, "%s_RDI.dat", line);
              savedata_11(buf, ph->RDI, 2 * h2 * w2, 0);
              _snprintf(buf, 256, "%s_FRDI.dat", line);
              savedata_11(buf, ph->FRDI, 2 * h2 * w2, 0);
              _snprintf(buf, 256, "%s_g.dat", line);
              savedata_11(buf, ph->g, 2 * h2 * w2, 0);
              _snprintf(buf, 256, "%s_FAI.dat", line);
              savedata_11(buf, ph->FAI, h2 * w2, 0);
              //cvWaitKey ( -1 );
            }

            if (0) {
              _snprintf(buf, 256, ".\\RN=%d_RD=%d\\%s_FAI_RN=%d_RD=%d_t=%.3f.bmp", ph->RN, ph->RD, line, ph->RN, ph->RD, utime_elapsed(_start_time1));
              imwrite(buf, h2, w2, ph->FAI, w2, 1);
              //cvWaitKey ( -1 );
            }

            if (0) {
              _snprintf(buf, 256, ".\\RN=%d_RD=%d", ph->RN, ph->RD);
              _mkdir(buf);
              _snprintf(buf, 256, ".\\RN=%d_RD=%d\\%s_g_RN=%d_RD=%d_t=%.3f.dat", ph->RN, ph->RD, line, ph->RN, ph->RD, t3);
              savedata_11(buf, ph->g, 2 * h2 * w2, 0);
              //cvWaitKey ( -1 );
            }

            if (1) {
              for (i = 0; i < h2 * w2; ++i) {
                ph->FAI[i] = ph->FAI[i] * 255 / (ph->lables + 1);
              }

              cvShowImage("BI", h2, w2, ph->BI, w2, 1);
              cvShowImage("FrontI", h2, w2, ph->FrontI, w2, 1);
              cvShowImage_u16("g", h2, w2, ph->g, w2, 1);
              cvShowImage_u16("FRDI", h2, w2, ph->FRDI, w2, 1);
              cvShowImage_u16("RDI", h2, w2, ph->RDI, w2, 1);
              cvShowImage("FAI", h2, w2, ph->FAI, w2, 1);
            }
          }

          //_snprintf(line2, 256, "%s_fg.bmp", line);
          //imwrite(line2, h2, w2, pBGStatModel->foreground, w2, 1);
          //_snprintf(line2, 256, "%s_bk.bmp", line);
          //imwrite(line2, h2, w2, pBGStatModel->background, al2, ai);
        }

        printf("%d,%d ", ph->top2bot, ph->bot2top);
        printf("t=%.3f", utime_elapsed(_start_time1));
      }
      printf("\n");
      //cvShowImage("cam", h, w, gry, w, 1);
      ch = cvWaitKey(50);
      imfree(im2);
    }

    FREE(img1);
    FREE(img2);
    FREE(img3);
    FREE(img4);
  }

  pfree(ph->buf);
  cvReleaseBGStatModel(&pBGStatModel);
  FREE(rgb);
  FREE(depth_data);
  FREE(depth_data_2);
  return 0;
}

#endif

#include "img/imgopt.inl"
#include "fileio.h"
//#include "file/imgopt.inl"

int test_bgfg_pic()
{
  vstr_t sv[1] = {0};
  int i = 0, ch = 0;
  img_t im[3] = {0};
  img_t* foreground = im + 1;
  img_t* background = im + 2;
  bgmodel b[1] = {0};

  //cvCreateTrackbar ( "high", "cam", &high, 300, 0 );
  //_chdir("D:/pub/bin/imgproc");
  _chdir("C:/video/depth_data");
  _chdir("D:/pub/bin/imgproc/pic/depth_data");
  _chdir("D:/pub/bin/imgproc/yaw");
  vstr_load("list.txt", sv);

  for (i = 0; 'q' != ch && 'Q' != ch && i < sv->n; ++i) {
    imread(sv->v[i].s, 1, 1, im);
    imresize_1(im, 8, 8, im);
    b->height = im->h, b->width = im->w, b->nChannels = im->c, b->step = im->s;
    b->frame = im->tt.data;
    b->mode = BGMODEL_UPDATE;

    if (0 == i) {
      int buflen = bgmodel_memsize(b);
      b->buf = (uchar*)pmalloc(buflen);
      b->buflen = buflen;
      b->mode = BGMODEL_INIT;
    }

    bgmodel_process(b);
    IMINIT(foreground, im->h, im->w, b->foreground, im->w, 1, 1);
    IMINIT(background, im->h, im->w, b->background, im->w, 1, 1);
    imshow(im);
    imshow(foreground);
    imshow(background);
    ch = cvWaitKey(100);
  }

  imfree(im);
  return 0;
}

