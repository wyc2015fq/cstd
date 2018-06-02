
//#include "debug.h"
#include "cstd.h"
#include "str.h"
#include "cfile.h"
#include "str/scanf.inl"
#include "ui/window.inl"
#include "img/imgio.inl"
#include "draw/imdraw.inl"
//#include "face/face.inl"
#include "face/face_recog.inl"
#include "face/cascadearr.h"
#include "color_tracker.inl"


int ldown=0;
int frame=0;
IRECT g_rc = {0};

int test_color_tracking_onmouse(int event, int x, int y, int flags, void* param) {
  switch(event) {
  case CV_EVENT_LBUTTONDOWN:
    ldown = 1;
    g_rc.l = g_rc.r = x, g_rc.t = g_rc.b = y;
    break;
  case CV_EVENT_MOUSEMOVE:
    if (ldown) {
      g_rc.r = x, g_rc.b = y;
    }
    break;
  case CV_EVENT_LBUTTONUP:
    if (ldown) {
      ldown = 0;
      g_rc.r = x, g_rc.b = y;
      color_tracker_init((color_tracker_t*)param, g_rc);
      frame = 0;
    }
    break;
  default:
    break;
  }
  return 0;
}

int test_debug() {
  char* p = 0;
  MEM_CHECK_BEGIN();
  p = (char*)malloc(2);
  p[0] = 'A';
  p[1] = 'B';
  MEM_CHECK_END();
  free(p);
  return 0;
}

int test_color_tracking() {
  char buf[256];
  buf_t bf[1] = {0};
  int frames[2];
  int n = 0;
  img_t im[10] = {0};
  double* ground_truth = NULL;
  color_tracker_t params[1] = {0};
  IRECT rc, rc1;

  test_debug();
  _chdir("E:/code/cstd_bak/ColorTracking_code/sequences/deer_bmp");
  
  buf_load("deer_frames.txt", bf);
  str_scanf_arr((char*)bf->data, bf->len, 0, "%d,%d", 1, frames);
  buf_load("deer_gt.txt", bf);
  n = frames[1] - frames[0] + 1;
  ground_truth = MALLOC(double, n*4);
  str_scanf_arr((char*)bf->data, bf->len, 0, "%lf,%lf,%lf,%lf", n, ground_truth);
  //cvShowMat("asdf", "%5.1I64f", n, 4, ground_truth, sizeof(double)*4, sizeof(double), -1);
  
  buf_load("w2crs.dat", bf);
  params->w2c = (double* )bf->data;
  //cvShowMat("w2c", "%5.4I64f", W2C_H/10, W2C_W, params->w2c, sizeof(double)*W2C_W, sizeof(double), -1);
  
  if (0) {
    uchar img_data[128*128*3] = {0};
    rc = iRECT2(10, 10, 10, 10);
    IMINIT(im, 128, 128, img_data, 128*3, 3, 1);
    MEM_CHECK_BEGIN();
    color_tracker_init(params, rc);
    color_tracker_run(params, frame, im, &rc1);
    color_tracker_free(params);
    MEM_CHECK_END();
    return 0;
  }
  // x y ������
  if (0) {
    rc = iRECT2((int)ground_truth[0], (int)ground_truth[1], (int)ground_truth[2], (int)ground_truth[3]);
    color_tracker_init(params, rc);
    //n=2;
    for (frame=0; frame<n; ++frame) {
      _snprintf(buf, 256, "imgs\\img%05d.bmp", frame+frames[0]);
      if (imread(buf, 3, 1, im)) {
        //double* r = ground_truth + frame*4;
        //color_tracker(params);
        //imdrawaa_rect(im, 0, r[0], r[1], r[0]+r[2], r[1]+r[3], 0, rgb(1,0,0), 2);
        //imshow(im);
        //imshowmat_u1_cn(im, 2);cvWaitKey(-1);
        //cvWaitKey(100);
        // load image
        color_tracker_run(params, frame, im, &rc1);
        //save position
        //positions(frame,:) = [pos target_sz];
        {
          //time = time + toc;
          
          //visualization
          if (1) {
            imdraw_rect(im, 0, rc1, 0, rgb(1,0,0), 2, BDM_ALL);
            imshow(im); cvWaitKey(30);
          }
        }
      }
    }
    color_tracker_free(params);
    free(ground_truth);
    bffree(bf);
    imfrees2(im);
  }
  if (1) {
    capdev cap[1] = {0};
    const char* wname = "cam";
    int iframe=0, ch;
    facerecog_param_t m_fr[1] = {0};
    cap_open(cap, 0, CAP_DSHOW);
    cvNamedWindow(wname, 0);
    cvSetMouseCallback(wname, test_color_tracking_onmouse, params);
    //g_rc = iRECT2(100, 100, 100, 100); color_tracker_init(params, g_rc);
    facerecog_set(m_fr);
    for (frame=0; frame<10000000; ++iframe) {
      XRECT* face = NULL;
      XRECT* leye = NULL;
      XRECT* reye = NULL;
      cap_getframe(cap, im, CAP_RGB);
      if (iframe%10==0)
      {
        if (1) {
          facerecog_run(m_fr, im, T_BGR, FR_FACE_DET);
          if (m_fr->face.w) {
            face = &m_fr->face;
            g_rc = iRECT2(face->x, face->y, face->w, face->h);
            color_tracker_init(params, g_rc);
            frame = 0;
          }
          if (m_fr->leye.w) {
            leye = &m_fr->leye;
          }
          if (m_fr->reye.w) {
            reye = &m_fr->reye;
          }
        }
      }
      if (0==ldown && RCW(&g_rc)>0) {
        {utime_start(_start_time);
        color_tracker_run(params, frame, im, &rc1);
printf("%f\n", utime_elapsed(_start_time));}
        imdraw_rect(im, 0, rc1, 0, frame ? rgb(1,0,0) : rgb(0,1,0), 2, BDM_ALL);
        if (0==frame) {
          if (leye) {
            rc = iRECT2(leye->x, leye->y, leye->w, leye->h);
            imdraw_rect(im, 0, rc, 0, rgb(0,1,0), 2, BDM_ALL);
          }
          if (reye) {
            rc = iRECT2(reye->x, reye->y, reye->w, reye->h);
            imdraw_rect(im, 0, rc, 0, rgb(0,1,0), 2, BDM_ALL);
          }
        }
        ++frame;
      } else {
        imdraw_rect(im, 0, g_rc, 0, rgb(0,1,0), 2, BDM_ALL);
      }
      imshow2(wname, im);
      ch = cvWaitKey(30);
      if ('q'==ch || 'Q'==ch) {
        break;
      }
    }
    cap_close(cap);
  }
  imfrees2(im);
  color_tracker_free(params);
  //cvWaitKey(-1);
  //FREE(ground_truth);
  bffree(bf);
  return 0;
}
