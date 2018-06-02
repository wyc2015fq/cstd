
#include "img/imgio.inl"
#include "img/imgopt.inl"
#include "inifile.h"
#include "cascadearr.h"
#include "face.inl"
#include "face_recog.inl"
#include "draw/imdraw1.inl"

#define rint floor


#ifdef _WIN32
#define cap_open capdshow_open
#else
#define cap_open capv4l_open
#endif

int test_facedetect_cam()
{
  int high = 0;
  capdev cap[1] = {0};
  
  if (cap_open(cap, 0, 640, 480) > 0) {
    int fram = 0;
    char ch = 0;
    img_t im[1] = {0};
    
    for (; 'q' != ch && 'Q' != ch; ++fram) {
      cap_getframe(cap, im, CAP_RGB);
      //printf("%d w=%d h=%d\n", fram, im->w, im->h);
      if (1) {
        int is_trans = 0;
        int deteyes = 1;
        double ss = 0.5;
        int xywh[12] = {0};
        int i;
        int n=0;
        {
          utime_start(_start_time);
          n = face_recog(im->h, im->w, im->tt.data, im->s, is_trans, "RGB", deteyes ? "DET" : "FDET", 0, ss, xywh, 0, 0, 0, 0, 0, 0, 0);
          double t=utime_elapsed(_start_time);
          printf("time=%5.2fms n=%d face=[%3d,%3d,%3d,%3d]\n", t, n, xywh[0], xywh[1], xywh[2], xywh[3]);
        }
        if (n>0) {
          for (i=0; i<3; ++i) {
            XRECT rc;
            rc.x = xywh[i*4+0];
            rc.y = xywh[i*4+1];
            rc.w = xywh[i*4+2];
            rc.h = xywh[i*4+3];
            DrawRectangle2( im, rc, CC_RGB( 0, 255, 0 ), 3 );
          }
        }
        //int t = utime_elapsed(_start_time)*1000;
        //utime_start(_start_time);
      }
      imshow(im);
      ch = cvWaitKey(10);
    }
    
    cap_close(cap);
    imfree(im);
  }
  
  return 0;
}

int test_facedetect() {
//int test_pic_dat1();
  //test_pic_dat1();
  //return test_facedetect_cam();
  int i, j, n=0;
  buf_t bf[1] = {0};
  img_t im[2] = {0};
  img_t* gry = im+1;
  double aa = rint(1.5);
  double bb = rint(-1.5);
  double aa1 = rint(1.2);
  double bb1 = rint(-1.2);
  XRECT out[200];
  bfsetsize(bf, 50*_1M);
  if (0) {
    sys_chdir("d:/pub/bin/face/detect");
    imread("12.jpg", 3, 1, im);
    im2gry(im, gry);
    n = face_detect( bf, vis_nesting_face20110713, gry->h, gry->w, gry->tt.data, gry->s,
      0, 0, 2, 1, 1000, 1.1, 1, 0.8, 5, out, countof(out), 0);
    for (i=0; i<n; ++i) {
      DrawRectangle2( im, out[i], CC_RGB( 0, 255, 0 ), 3 );
    }
    imshow(im);cvWaitKey(-1);
    freeims(im, 2);
  }
  if (1) {
    vstr_t sv[1] = {0};
    double ss=0.12;
    double sc=1.1;
    int is_trans=0;
    int deteyes=0;
    int stepxy=0;
#ifdef _DEBUG
    sys_chdir("E:/pub/yuvs");
    sys_chdir("E:/pub/bin/face/detect/frontdetect");
    sys_chdir("E:/pub/bin/face/detect/戴眼镜来自中南大学0328");
    sys_chdir("F:/dataset/facedetect");
#endif // _DEBUG
    if (1) {
      ininame("testc.ini");
      iniappname("test_facedetect");
      ss = inigetfloat("缩放");
      sc = inigetfloat("缩放比");
      is_trans = inigetfloat("旋转90度");
      stepxy = inigetint("步长");
      deteyes = inigetint("眼睛检测");
    }
    vstr_load("list.txt", sv);
    logclear();
    for (j=0; j<sv->n; ++j) {
      if (imread(sv->v[j].s, 3, 1, im)) {
        int xywh[1000];
        char buf[256];
        double t=0;
        im2gry(im, gry);
        {
          utime_start(_start_time);
          if (!deteyes) {
            n = face_detect( bf, vis_nesting_face20110713, gry->h, gry->w, gry->tt.data, gry->s,
              0, 0, ss, 1, 1000, sc, stepxy, 0.8, 2, out, countof(out), is_trans);
            t = utime_elapsed(_start_time)*1000;
              logprintf("time=%5.2fms n=%d face=[%3d,%3d,%3d,%3d] file=%s\n", t, n, out[0].x, out[0].y, out[0].w, out[0].h, GetFileNameExt(sv->v[j].s));
          } else {
            n = face_recog(gry->h, gry->w, gry->tt.data, gry->s, is_trans, "GRAY", deteyes ? "DET" : "FDET", 0, ss, xywh, 0, 0, 0, 0, 0, 0, 0);
            t = utime_elapsed(_start_time)*1000;
            logprintf("time=%5.2fms n=%d face=[%3d,%3d,%3d,%3d] file=%s\n", t, n, xywh[0], xywh[1], xywh[2], xywh[3], GetFileNameExt(sv->v[j].s));
            utime_start(_start_time);
          }
        }
        if (!deteyes) {
          for (i=0; i<n; ++i) {
            XRECT rc = out[i];
            if (rc.w) {
              //if (0==i)
              //font_t fo[1] = {0};
              //IRECT rc = {rc.x, rc.y, rc.x+rc.w, rc.y+rc.h};
              _snprintf(buf, 256, "t=%2.2f c=%d n=%d w=%d", t, g_cntrc, g_outlen, rc.w);
              //imdraw_text_v(im, NULL, buf, 0, (i+1)*40, 0, 1, _rgb(1,0,0));
              //font_init_memraster(fo, "asdf", zimo_songti_12);
              //imdraw_text(im, NULL, rc, buf, -1, fo, _rgb(1,0,0), 0, 0);
              DrawRectangle2( im, rc, CC_RGB( 0, 255, 0 ), 3 );
              if (deteyes) {
                DrawRectangle2( im, rc, CC_RGB( 0, 255, 0 ), 3 );
              }
            }
          }
        } else {
          if (xywh[2]) {
            //if (0==i)
            //font_t fo[1] = {0};
            //IRECT rc = {rc.x, rc.y, rc.x+rc.w, rc.y+rc.h};
            //_snprintf(buf, 256, "t=%2.2f c=%d n=%d w=%d", t, g_cntrc, g_outlen, rc.w);
            _snprintf(buf, 256, "t=%2.2f c=%d n=%d w=%d", t, g_cntrc, g_outlen, xywh[2]);
            //imdraw_text_v(im, NULL, buf, 0, (i+1)*40, 0, 1, _rgb(1,0,0));
            //font_init_memraster(fo, "asdf", zimo_songti_12);
            //imdraw_text(im, NULL, rc, buf, -1, fo, _rgb(1,0,0), 0, 0);
            for (i=0; i<3; ++i) {
              XRECT rc;
              rc.x = xywh[i*4+0];
              rc.y = xywh[i*4+1];
              rc.w = xywh[i*4+2];
              rc.h = xywh[i*4+3];
              DrawRectangle2( im, rc, CC_RGB( 0, 255, 0 ), 3 );
            }
          }
        }
        if (1) {
          sys_mkdir("out");
          _snprintf(buf, 256, "./out/%s.jpg", GetFileNameExt(sv->v[j].s));
          imwrite(buf, im);
        }
        if (1) {
          if (n) {
            imshow(im);
            cvWaitKey(-1);
          }
        }
      }
    }
  }
  return 0;
}
