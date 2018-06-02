
#include "cap.h"
#include "facetrack.inl"
//#include "test_ASM.inl"
#ifdef _WIN32
#define USE_GUI
#endif // _WIN32



#include "cv/imgproc/drawing.inl"
#include "img/imgio.inl"

int save_face(const char* fn, const img_t* im, CRectf face, int d) {
  IRECT rc = {face.x, face.y, face.x+face.w, face.y+face.h};
  img_t im1[1] = {0};
  rc = iRectInflate1(rc, d);
  imsubref(im, rc, im1);
  imwrite(fn, im1);
  return 0;
}
int save_face_bf(buf_t* bf, const img_t* im, CRectf face, int d) {
  IRECT rc = {face.x, face.y, face.x+face.w, face.y+face.h};
  img_t im1[1] = {0};
  rc = iRectInflate1(rc, d);
  imsubref(im, rc, im1);
  bfsetsize(bf, im1->h*im1->w*im->c*im->f);
  bf->len = imsave_mem(bf->data, bf->len, "jpg", im1->h, im1->w, im1->tt.data, im1->s, im1->c);
  return bf->len;
}


#ifdef USE_GUI
#define PT_COLOR CC_RGB(255, 0, 0)        // 点的颜色
#define SELECT_PT_COLOR CC_RGB(0, 255, 0) // 被选择点的颜色
//#include "draw/imdraw.inl"
int draw_faceobj( img_t* im, const IRECT* pclip, const faceobj_t* fo, bool drawshape, bool drawid) {
  char buf[ 25 ];
#if 0
  int i;
  if (drawshape) {
  const FPOINT* pt = fo->landmark;
    for ( i = 0; i < shape_result_len; ++i ) {
      int Before, After;
      IPOINT pt1, pt2;
      GetBeforeAfterPts( i, contours_ending_points, contours_ending_points_len, &Before, &After );
      pt1.x = ( int ) pt[ i ].x;
      pt1.y = ( int ) pt[ i ].y;
      pt2.x = ( int ) pt[ Before ].x;
      pt2.y = ( int ) pt[ Before ].y;
      //imdraw_circle(im, pclip, pt1.x, pt1.y, radius, PT_COLOR, 1);
      imdrawaa_circle(im, pclip, pt[ i ].x, pt[ i ].y, 2, 0, PT_COLOR, 1);
      //imdrawaa_circle( im, pclip, pt1.x, pt1.y, radius, PT_COLOR, 0, 0 );
      //imdraw_line( im, pclip, pt1, pt2, CC_RGB( 255, 255, 0 ), 1 );
      imdrawaa_line( im, pclip, pt[ i ].x, pt[ i ].y, pt[ Before ].x, pt[ Before ].y, CC_RGB( 255, 255, 0 ), 1 );
      
      if ( drawid ) {
        _snprintf( buf, 25, "%d", i );
        //imdrawaa_textv( im, false, pclip, buf, pt1.x, pt1.y, MIN( MAX( radius, 5 ) + 5, 20 ), 0.3, SELECT_PT_COLOR );
      }
    }
    
    if ( drawid ) {
      for ( i = 0; i < contours_ending_points_len; ++i ) {
        FPOINT pt1 = face_part_center(pt, i);
        _snprintf( buf, 25, "%d", i );
        imdrawaa_textv( im, false, pclip, buf, pt1.x, pt1.y, MIN( MAX( 2, 5 ) + 5, 20 ), 0.3, SELECT_PT_COLOR, 1 );
      }
    }
  }
  {
    //putText(im, "L", -1, cPoint(100, 100), 0, 1, cScalar(0,255,255), 1, CC_LINE_AA);
    //imdrawaa_textv(im, false, pclip, "L", 100, 100, 0, 1, _rgb(0,1,1), 2);    imshow_(im);WaitKey(-1);
    DRECT rc = dRectXRECT(fo->face);
    imdrawaa_rect(im, false, pclip, rc, 0, _rgb(1,0,0), 2);
    rc.b = rc.t + 30;
    imdrawaa_rect(im, false, pclip, rc, _rgba(0,0,0,0.2), 0, 0);
    if (fo->inited) {
      _snprintf( buf, 25, "id=%d mem=%d age=%d", fo->id, fo->gender, fo->age);
    } else {
      _snprintf( buf, 25, "id=%d ???", fo->id);
    }
    imdrawaa_textv(im, false, pclip, buf, rc.l, rc.t, CC_FONT_HERSHEY_SIMPLEX, 0.5, _rgb(0,1,1), 1);
    //putText(im, buf, -1, cPoint(rc.l, rc.t), CC_FONT_HERSHEY_SIMPLEX, 0.5, cScalar(0,255,255), 1, CC_LINE_AA);
  }
#endif
  
  {
    //putText(im, "L", -1, cPoint(100, 100), CC_FONT_HERSHEY_SIMPLEX, 1, cScalar(0,255,255), 1, CC_LINE_AA);
    //imdrawaa_textv(im, false, pclip, "L", 100, 100, 0, 1, _rgb(0,1,1), 2);    imshow_(im);WaitKey(-1);
    CRect rc = cRect(fo->face.rect.x, fo->face.rect.y, fo->face.rect.w, fo->face.rect.h);
    rectangle(im, rc, cScalar(0,0,255), 2);
    //rc.b = rc.t + 30;
    //imdrawaa_rect(im, false, pclip, rc, _rgba(0,0,0,0.2), 0, 0);
    if (fo->inited) {
      _snprintf( buf, 25, "id=%d mem=%d age=%d", fo->id, fo->gender, fo->age);
    } else {
      _snprintf( buf, 25, "%d %d %d", fo->id, fo->gender, fo->age);
    }
    _snprintf( buf, 25, "%d %d %d", fo->id, fo->gender, fo->age);
    putText(im, buf, -1, cPoint(rc.x, rc.y), CC_FONT_HERSHEY_SIMPLEX, 1, cScalar(255,255,0), 1, CC_LINE_AA);
  }
  return 0;
}
#endif

//#define USE_FACEPLUSPLUS
#ifdef USE_FACEPLUSPLUS
#include "facedetect/faceplusplus.inl"
#endif // USE_FACEPLUSPLUS
//#include "cap/capimgseq.inl"

#ifdef _WIN32
#define cap_open capdshow_open
#else
#define cap_open capv4l_open
#endif

#if 0

#include "pic/pic.inl"
int test_facetrack() {
  img_t im1[1] = {0};
  img_t im[1] = {0};
  faceobj_t fo[10];
  int nfo=0, i=0;
  IMINIT(im1, 288, 360, gry_360x288, 360, 1, 1);
  imtrans2(im1, im, 1, 0);
  im2rgb(im, im, 3);
  printf("facetrack\n");
  nfo = facetrack(im, fo, nfo, countof(fo));
#ifdef USE_GUI
  draw_faceobj(im, NULL, fo, true, true);
  imshow(im);cvWaitKey(-1);
#else
#endif // USE_GUI
  printf("%d [%d,%d,%d,%d]\n", fo[i].id, fo[i].face.x, fo[i].face.y, fo[i].face.w, fo[i].face.h);
  printf("end\n");
  imfree(im);
  return 0;
}
#else

int test_facetrack() {
  cap_t cap[1] = {0};
  img_t im[10] = {0};
  faceobj_t fo[10];
  int nfo = 0, frame=0, ch=0;
  int i;
  bool drawshape = false, drawid = false;
  //cap_open( cap, 0, 640, 480 );
  //cap_open( cap, 0, 1092, 1080);
  //capvfwavi_open(cap, "D:/dt/dt.avi");
  //capimgseq_open(cap, "D:/dt/dt_5/list.txt", 0);
  int ret = cap_open(cap, 0, 1280, 960);
  int face_code=0;
  if (ret>0) {
    for ( ; 'q' != ch && 'Q' != ch; ++frame ) {
      img_t* rgb = im+1;
      switch(ch) {
      case 's':drawshape = !drawshape;break;
      }
      if (!cap_getframe(cap, im, CAP_RGB)) {
        break;
      }
      //{utime_start(_start_time);
      nfo = facetrack(im, fo, nfo, countof(fo), &face_code);
      //printf("%f\n", utime_elapsed(_start_time));}
#ifdef USE_FACEPLUSPLUS
      for (i=0; i<nfo; ++i) {
        if (fo[i].inited==0) {
          if (0) {
            faceplusplus_image(im, fo+i);
          }
        }
      }
#endif // USE_FACEPLUSPLUS
      
      if (0) {
        for (i=0; i<nfo; ++i) {
          if (fo[i].event==1) {
            if (1) {
              char buf[256];
              _snprintf(buf, 256, "D:/dt/out/%05d_%05d.jpg", frame, fo[i].id);
              save_face(buf, im, fo[i].face.rect, fo[i].face.rect.w);
            }
          }
        }
      }
      for (i=0; i<nfo; ++i) {
        if (fo[i].event==1) {
          andprintf("%d in[%d,%d,%d,%d]\n", fo[i].id, (int)fo[i].face.rect.x, (int)fo[i].face.rect.y, (int)fo[i].face.rect.w, (int)fo[i].face.rect.h);
        } else if (fo[i].event==-1) {
          andprintf("%d out\n", fo[i].id);
        }
#ifdef USE_GUI
          draw_faceobj(im, NULL, fo+i, drawshape, true);
#endif // USE_GUI
      }
      nfo = faceobj_clear(fo, nfo);
#ifdef USE_GUI
      imshow_(im);
      ch = WaitKey(1);
#else
      sys_sleep(5);
#endif // USE_GUI
      //printf("frame %d w=%d h=%d\n", frame, im->w, im->h);
    }
    cap_close( cap );
  }
  imfrees(im, 10);

  return 0;
}

#endif
