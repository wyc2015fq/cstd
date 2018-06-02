

#ifndef _WIN32
#define iris_detect(p) iris_recog_process(p)
#endif

#include "test_iris_base.inl"
#include "img/color.inl"


uchar S2001L01_640x480[] = {0
//#include "pic/S2001L01_640x480.inl"
};

//#include "fileio.h"
// 测试输出
// 时间    虹膜矩形框        内圆(x y r) 外圆         上眼睑抛物线 最小距离 id
// 0.1738 (210 100 280 280)(127 142  48)(125 136  90)(  0   0 103)(4122   0)
// 性能测试结果
// 20个特征
// 0.0191 IR_BOOST_DETECT
// 0.0557 IR_BOOST_DETECT | IR_N_DETECT
// 0.1457 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT
// 0.1700 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT
// 0.2071 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT
// 0.2083 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG
// 0.2108 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG | IR_FEATURE
// 0.2109 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG | IR_FEATURE | IR_RECOG
//  (210 100 280 280)(127 142  48)(125 136  90)(  0   0 103)(4122   0)

// 2000个特征
// 0.0270 IR_BOOST_DETECT
// 0.0636 IR_BOOST_DETECT | IR_N_DETECT
// 0.1536 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT
// 0.1780 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT
// 0.2150 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT
// 0.2165 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG
// 0.2187 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG | IR_FEATURE
// 0.2309 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG | IR_FEATURE | IR_RECOG
//  (210 100 280 280)(127 142  48)(125 136  90)(  0   0 103)(4122   0)

// 20000个特征
// 0.0425 IR_BOOST_DETECT
// 0.0791 IR_BOOST_DETECT | IR_N_DETECT
// 0.1692 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT
// 0.1935 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT
// 0.2306 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT
// 0.2318 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG
// 0.2342 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG | IR_FEATURE
// 0.3567 IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG | IR_FEATURE | IR_RECOG
//  (210 100 280 280)(127 142  48)(125 136  90)(  0   0 103)(4122   0)

#include "codec/huff.inl"

static int calc_sum1(int n, const unsigned char* A) {
  int i = 0, k = 0;
  for (; i < n; ++i) {
    k += A[i];
  }
  
  return k;
}


int test_iris_detect_arm()
{
  //img_t im[1] = {0};
  img_t gry[1] = {0};
  img_t uyvy[1] = {0};
  const char* fn = "S2001L01.jpg";
  buf_t bf[1] = {0};
  enum {MAXBUF2 = 1 << 24};
#if 0

  for (fram = 0; fgets(buf, 256, plisttxt) > 0; ++fram) {
    imread(fn, 3, 1, im);
  }

  im2gry(im, gry);
  savedata_inl("S2001L01.inl", gry->tt.data, gry->w * gry->h);
#else
  IMINIT(gry, 480, 640, S2001L01_640x480, 640, 1, 1);
#endif
  bf->len = MAXBUF2;
  bf->data = MALLOC(uchar, bf->len);
  bf_imcolorcvt(bf, gry, uyvy, T_GRAY, T_UYVY);
  //printf("%10s ", fn);
  //imshow(gry);

  iris_recog_open();

  if (gry->tt.data) {
    int i;
    int featlen, distthd, distmax;
    uchar* buf = 0;

    printf("%10s %d %d\n", fn, gry->h, gry->w);
    buf = (uchar*)dsp_malloc(MAXBUF2);
    memset(buf, 0, MAXBUF2);

    {
      irisrecog_t* ir = (irisrecog_t*)buf;
      memset(ir, 0, sizeof(irisrecog_t));
      iris_recog_process(ir); // 获取信息
      featlen = ir->featlen;  // 特征长度
      distthd = ir->distthd;  // 推荐阈值
      distmax = ir->distmax;  // 最大距离
      printf("特征长度=%d, 推荐阈值=%d, 最大距离=%d\n", featlen, distthd, distmax);
    }

    for (i = 0; i < 1; ++i) {
      irisrecog_t* ir = (irisrecog_t*)buf;
      memset(ir, 0, sizeof(irisrecog_t));
      ir->img_off = sizeof(irisrecog_t);
#if 1
      ir->height = uyvy->h, ir->width = uyvy->w;
      ir->color_type = T_UYVY;
      memcpy(buf + ir->img_off, uyvy->tt.data, uyvy->h * uyvy->s);
      printf("====================T_UYVY\n");
      ir->feat_off = ir->img_off + ir->height * ir->width*2;
#else
      ir->height = gry->h, ir->width = gry->w;
      ir->color_type = T_GRAY;
      memcpy(buf + ir->img_off, gry->tt.data, gry->h * gry->s);
      ir->feat_off = ir->img_off + ir->height * ir->width;
#endif
      //ir->std_off = sizeof(irisrecog_t) + height*width;
      ir->featstep = 5404;
      ASSERT(ir->featstep >= featlen);
      // 必须>=特征长度
      ir->lib_off = ir->feat_off + 10000;
      ir->featnum = 2000;
      ir->buf_off = ir->lib_off + ir->featnum * ir->featstep;
      ir->buflen = MAXBUF2 - ir->buf_off;
      ASSERT(ir->buflen > 1024 * 1024); // 保证有1M空间
#if 1
      {
        buf_t bf[1] = {0};
        int sz;

#ifdef _WIN32
        _chdir("D:\\pub\\cstd\\DSP\\dm6446\\");
#endif

        if (1) {
          ir->featnum = 0;
          buf_load("2001.dat", bf);
          sz = HUFF_Decode(bf->data, bf->len, buf + ir->lib_off + ir->featnum*ir->featstep, ir->buflen);
          ir->featnum += 10;
          
          buf_load("4002.dat", bf);
          sz = HUFF_Decode(bf->data, bf->len, buf + ir->lib_off + ir->featnum*ir->featstep, ir->buflen);
          ir->featnum += 10;
          
          buf_load("4001.dat", bf);
          memcpy(buf + ir->lib_off + ir->featnum*ir->featstep, bf->data, bf->len);
          ir->featnum += 10;
          //memshift()
          memswap(10*ir->featstep, buf + ir->lib_off, buf + ir->lib_off + 10*ir->featstep);
          {
            uchar* fb = buf + ir->lib_off;
            int ss = calc_sum1(featlen, fb + 1 * ir->featstep);
            printf("%d  %d\n", ss, ir->maxdist_id);
          }
        }
        bffree(bf);
      }
#endif

      ir->mode = IR_ALL_DETECT;
      //ir->mode = IR_BOOST_DETECT|IR_N_DETECT|IR_U_DETECT;
      ir->mode = IR_BOOST_DETECT | IR_N_DETECT;
      ir->mode = IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT;
      //ir->mode |= IR_STDIMG;
      //ir->mode |= IR_FEATURE;
      //ir->mode |= IR_RECOG;
      //ir->in[0] = 1000;
      ir->blurrate;
      ir->maxdist_id;
      ir->maxdist;
#define TESTMODE(mm)    if (1) {utime_start(_start_time);ir->mode = mm;iris_recog_process(ir);printf("%.4f id=%d ds=%d %s\n", utime_elapsed(_start_time), ir->maxdist_id, ir->maxdist, #mm );}
#if 0
      TESTMODE(IR_BOOST_DETECT);
      TESTMODE(IR_BOOST_DETECT | IR_N_DETECT);
      TESTMODE(IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT);
      TESTMODE(IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT);
      TESTMODE(IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT);
      TESTMODE(IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG);
      TESTMODE(IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG | IR_FEATURE);
#endif
      TESTMODE(IR_BOOST_DETECT | IR_N_DETECT | IR_U_DETECT | IR_W_DETECT | IR_D_DETECT | IR_STDIMG | IR_FEATURE | IR_RECOG);

      {
        uchar* fb = buf + ir->lib_off;
        int ss = calc_sum1(featlen, fb + 1 * ir->featstep);
        printf("%d  %d\n", ss, ir->maxdist_id);
      }
      {
        buf_t bf[1] = {0};
        bfinit(bf, buf+ir->feat_off, ir->featlen);
        buf_save("feat.dat", bf);
      }
      if (0) {
        buf_t bf[1] = {0};
        int i;
        buf_load("D:\\pub\\cstd\\DSP\\dm6446\\feat.dat", bf);
        i = memcmp(bf->data, buf+ir->feat_off, ir->featlen);
        bffree(bf);
      }
      if (1) {
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
        printf("(%4d %3d)", ir->maxdist, ir->maxdist_id);
      }

#ifdef WIN32
#if 1
      //DrawIris(gry->h, gry->w, gry->tt.data, gry->s, gry->c, ir, CV_RGB(255, 255, 255), 2);
      DrawIris(uyvy, ir, CV_RGB(255, 255, 255), 0, 0, 2);

      //DrawIris(im->h, im->w, im->data, im->s, 3, &iris2, CV_RGB(0, 255, 0), 1, 8, 0);
      if (0) {
        //char buf2[256];
        //_snprintf(buf2, 256, "%s_out.jpg", fn);
        //imwrite2(buf2, gry);
      }
      else {
        img_t rgb1[1] = {0};
        bf_imcolorcvt(bf, uyvy, rgb1, T_UYVY, T_BGR);
        imshow(rgb1);
        cvWaitKey(-1);
        bf_imfree(bf, rgb1);
      }

#endif
#endif
      printf("\n");
    }

    dsp_free(buf);
  }

  iris_recog_close();
  printf("\n");

  FREE(bf->data);
  //imfree(im);
  //imfree(gry);
  return 0;
}
