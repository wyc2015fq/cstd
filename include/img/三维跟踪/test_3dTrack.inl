
#include <stdio.h>
#include "cstd.h"
#include "macro.h"

#include "NewTrack.h"
#include "../cap.inl"
#include "imresize.h"
//#include "NewTrack.inl"
#include "GMModel.inl"
//#include "calibration.inl"
//#include "TriDimModel.inl"
//#include "3dTrack.inl"

int get_no_zero_pix_number( int n, const unsigned int* x ) {
  int i, k;
  k = 0;
  for ( i = 0; i < n; ++i ) {
    k += !!x[ i ];
  }
  return k;
}

#include "ColorNames.inl"

static GMM g_gmm;

int test_3dTrack() {
  //capcam * cap = 0;
  int high = 0;
  unsigned char* gry = 0;
  unsigned char* buf = 0;
  unsigned char* rgb = 0;
  unsigned char* frg = 0;
  GMM* gmm = 0;
  FILE* plisttxt;
  char line[256];
  cvNamedWindow ( "cam", 1 );
  //cvCreateTrackbar ( "high", "cam", &high, 300, 0 );
  //cap = cam_start ( 0, 0 );
  _chdir("D:/pub/bin/imgproc");
  plisttxt = fopen("list.txt", "rb");
  if (NULL==plisttxt) return 0;
  if ( 1 ) {
    int fram = 0;
    char ch = 0;
    unsigned char*img;
    for ( ; 'q' != ch && 'Q' != ch && fgets(line, 256, plisttxt)>0; ++fram ) {
      //unsigned char* img = cam_begin ( cap );
      //int h = cam_height ( cap ), w = cam_width ( cap ), al = cam_step ( cap ), ai = cam_channel ( cap );
      int h, w, al, ai;//, h2, w2, al2;
      img_t* im2;
      strtrim(line, "\n\r");
      im2 = imread2(line, 3);
      img = im2->data;
      h = im2->height, w = im2->width, al = im2->step, ai = im2->channels;
      //h2=cvFloor(h*cs), w2=cvFloor(w*cs), al2 = w2*ai;

      if ( !buf ) {
        buf = MALLOC ( unsigned char, h * al );
        gry = MALLOC ( unsigned char, h * al );
      }
      if ( !frg ) {
        frg = MALLOC ( unsigned char, h * w * 3 );
      }
      ASSERT(frg);
      memset(frg, 0, h * w * 3);
      if ( 2 == ai ) {
        int w3 = w * 3;
        
        if ( !rgb ) {
          rgb = MALLOC ( unsigned char, h * w * 3 );
        }
        
        //icvYUV4222BGR_8u_C2C3R(h, w, img, al, rgb, w3, 3);
        {
          utime_start ( uu );
          colorcvt ( h, w, img, al, 2, rgb, w3, 3, T_YUYV, T_BGR );
          printf ( "%f\n", utime_elapsed ( uu ) );
        }
        al = w3;
        img = rgb;
        ai = 3;
      }
      
      {
        utime_start ( s );
        //IMTRANS(h, w, img, al, ai, gry, w, 1, BGR2GRAY);
        colorcvt ( h, w, img, al, ai, gry, w, 1, T_BGR, T_GRAY );
        //SKIP_FILTEREX(h, w, img, al, ai, buf, al, ai, 1, 1, DILATION3X3_FT1, unsigned char);
        //printf("%f\n", utime_elapsed(s));
      }
      if ( ch != -1 )
        printf ( "%x\n", ch );
      printf( "fram %d ", fram );
      if ( NULL == gmm ) {
        gmm = &g_gmm;
        gmm->Init( 0.005, 0.4, 5.0, 0.05, 0.95, 0.3, 0.3, 0.1, h, w, 3, 20, 0, 0, 0, 1 );
      }
      ASSERT( gmm != 0 );
      if ( 30 > fram ) {
        ASSERT( ai == 3 );
        utime_start( _start_time );
        gmm->SetupBG( img, al, fram + 1 );
        printf( " CHC_SetupBG = %f ", utime_elapsed( _start_time ) );
      } else {
        const unsigned int* p;
        int i, rectnum = 0;
        gmm->min_pts=20;
        utime_start( _start_time );
        rectnum = gmm->Detect( img, al, 0 );
        printf( " CHC_ForegroundDetect = %f ", utime_elapsed( _start_time ) );
        printf("%2d ", rectnum);
        p = gmm->blob;
        if ( 0 ) {
          for ( i = 0; i < rectnum; ++i ) {
            const unsigned int* rect = p + i * 4;
            DrawRectangle( h, w, img, al, ai, iPOINT( rect[ 0 ], rect[ 1 ] ), iPOINT( rect[ 2 ], rect[ 3 ] ), CC_RGB( 255, 0, 0 ), 3, 8, 0 );
          }
        } else {
          int k = get_no_zero_pix_number( h * w, p );
          printf( " pix = %f", k * 1. / ( h * w ) );
          for (i=0; i<h*w; ++i) {
            if (p[i]) {
              unsigned char* pix = frg + i*3;
              pix[0] = pix[1] = pix[2] = 255;
            }
          }
        }
      }
      printf( "\n" );
      {
#define countof(arr)  (sizeof(arr)/sizeof(arr[0]))
        unsigned char* bk = gmm->GetBk();
        int i;
        for (i=0; i<h*w; ++i) {
          int id = gmm->blob[i];
          if (id>1) {
            int col = ColorNamesTable[(id)%countof(ColorNamesTable)].col;
            frg[i*3]=255;
            frg[i*3+1]=255;
            frg[i*3+2]=255;
            frg[i*3]=GetRValue(col);
            frg[i*3+1]=GetGValue(col);
            frg[i*3+2]=GetBValue(col);
          }
        }
        int n = gmm->GetRect();
        for (i=1; i<MIN(n, 2); ++i) {
          //if (gmm->br[i].n>100) 
          {
            DrawRectangle( h, w, img, al, ai, iPOINT( gmm->br[i].l, gmm->br[i].t ), iPOINT( gmm->br[i].r, gmm->br[i].b ), CC_RGB( 255, 0, 0 ), 1, 8, 0 );
            DrawRectangle( h, w, bk, w*3, 3, iPOINT( gmm->br[i].l, gmm->br[i].t ), iPOINT( gmm->br[i].r, gmm->br[i].b ), CC_RGB( 255, 0, 0 ), 1, 8, 0 );;
            DrawRectangle( h, w, frg, w*3, 3, iPOINT( gmm->br[i].l, gmm->br[i].t ), iPOINT( gmm->br[i].r, gmm->br[i].b ), CC_RGB( 255, 0, 0 ), 1, 8, 0 );
          }
        }
        cvShowImage( "bk", h, w, bk, w*3, 3 );
        cvShowImage ( "frg", h, w, frg, w*3, 3 );
        cvShowImage ( "cam", h, w, img, al, ai );
        {
          char line2[256];
          _snprintf(line2, 256, "%s_fg.jpg", line);
          //imwrite(line2, h, w, frg, w*3, 3);
          _snprintf(line2, 256, "%s_bk.jpg", line);
          //imwrite(line2, h, w, bk, w*3, 3);
        }
      }
      //cvShowImage("cam", h, w, gry, w, 1);
      ch = cvWaitKey ( 10 );
      imfree(im2);
    }
    
    //cam_stop ( cap );
  }
  SAFEFREE ( buf );
  SAFEFREE ( gry );
  SAFEFREE ( rgb );
  return 0;
}

capcam * cap = 0;
int high = 0;
unsigned char* gry = 0;
unsigned char* buf = 0;
unsigned char* rgb = 0;
unsigned char* frg = 0;
unsigned char* img = 0;
struct GMM* gmm = 0;
int h, w, al, ai;
unsigned char* img2;
int transn=2;
int fram = 0;
double cs=0.5;
int h2, w2, al2;
int showwindow=0;
int waitms=10;
int rectn=0;
int fgpix=0;
int test_3dTrack_cam() {
  cvNamedWindow ( "cam", 1 );
  //cvCreateTrackbar ( "high", "cam", &high, 300, 0 );
  cap = cam_start( 0, 0 );
  if ( cap ) {
    char ch = 0;
    for ( ; 'q' != ch && 'Q' != ch && cam_GrabFrame ( cap ); ++fram ) {
      img2 = cam_begin ( cap );
      h = cam_height ( cap ), w = cam_width ( cap ), al = cam_step ( cap ), ai = cam_channel ( cap );
      if ( !buf ) {
        img = MALLOC ( unsigned char, h * al );
        buf = MALLOC ( unsigned char, h * al );
        gry = MALLOC ( unsigned char, h * al );
      }
      if ( !frg ) {
        frg = MALLOC ( unsigned char, h * w * 3 );
      }
      imtrans(h, w, img2, al, ai, &h, &w, img, &al, transn, 0);
      ASSERT(frg);
      memset(frg, 0, h * w * 3);
      if ( 2 == ai ) {
        int w3 = w * 3;
        
        if ( !rgb ) {
          rgb = MALLOC ( unsigned char, h * w * 3 );
        }
        
        //icvYUV4222BGR_8u_C2C3R(h, w, img, al, rgb, w3, 3);
        {
          utime_start ( uu );
          colorcvt ( h, w, img, al, 2, rgb, w3, 3, T_YUYV, T_BGR );
          printf ( "%f\n", utime_elapsed ( uu ) );
        }
        al = w3;
        img = rgb;
        ai = 3;
      }
      
      {
        int h2=h*cs, w2=w*cs, al2=w2*ai;
        IMRESIZE(h, w, img, al, ai, h2, w2, buf, al2, ai, INTER_BILINEARITY);
        memcpy(img, buf, h2*al2);
        h=h2, w=w2, al=al2;
      }
      {
        utime_start ( s );
        //IMTRANS(h, w, img, al, ai, gry, w, 1, BGR2GRAY);
        colorcvt ( h, w, img, al, ai, gry, w, 1, T_BGR, T_GRAY );
        //SKIP_FILTEREX(h, w, img, al, ai, buf, al, ai, 1, 1, DILATION3X3_FT1, unsigned char);
        //printf("%f\n", utime_elapsed(s));
      }
      if ( ch != -1 )
        printf ( "%x\n", ch );
      printf( "fram %d ", fram );
      if ( NULL == gmm ) {
        gmm = &g_gmm;
        gmm->Init( 0.005, 0.4, 5.0, 0.05, 0.95, 0.3, 0.3, 0.1, h, w, 3, 20, 0, 0, 0, 1 );
      }
      ASSERT( gmm != 0 );
      if ( fram < 10 ) {
        ASSERT( ai == 3 );
        utime_start( _start_time );
        gmm->SetupBG( img, al, fram + 1 );
        printf( " CHC_SetupBG = %f ", utime_elapsed( _start_time ) );
      } else {
        const unsigned int* p;
        int i, rectnum = 0;
        utime_start( _start_time );
        rectnum = gmm->Detect( img, al, 0 );
        printf( " CHC_ForegroundDetect = %f ", utime_elapsed( _start_time ) );
        p = gmm->blob;
        if ( 0 ) {
          for ( i = 0; i < rectnum; ++i ) {
            const unsigned int* rect = p + i * 4;
            DrawRectangle( h, w, img, al, ai, iPOINT( rect[ 0 ], rect[ 1 ] ), iPOINT( rect[ 2 ], rect[ 3 ] ), CC_RGB( 255, 0, 0 ), 3, 8, 0 );
          }
        } else {
          int k = get_no_zero_pix_number( h * w, p );
          printf( " pix = %f", k * 1. / ( h * w ) );
          for (i=0; i<h*w; ++i) {
            if (p[i]) {
              unsigned char* pix = frg + i*3;
              pix[0] = pix[1] = pix[2] = 255;
            }
          }
        }
      }
      printf( "\n" );
      {
        unsigned char* bk = gmm->GetBk();
        cvShowImage ( "frg", h, w, frg, w*3, 3 );
        cvShowImage ( "cam", h, w, img, al, ai );
        //cvShowImage("cam", h, w, gry, w, 1);
        cvShowImage( "bk", h, w, bk, w*3, 3 );
        if (0) {
          char* line="";
          char line2[256];
          _snprintf(line2, 256, "%s_fg.jpg", line);
          imwrite(line2, h, w, frg, w*3, 3);
          _snprintf(line2, 256, "%s_bk.jpg", line);
          imwrite(line2, h, w, bk, w*3, 3);
        }
      }
      ch = cvWaitKey ( 10 );
    }
    
    cam_stop ( cap );
  }
  SAFEFREE ( buf );
  SAFEFREE ( gry );
  SAFEFREE ( rgb );
  SAFEFREE ( img );
  return 0;
}

void* GMM3DTrack_next(int mode) {
  if (!cam_GrabFrame( cap )) return 0;
  img2 = cam_begin ( cap );
  h = cam_height ( cap ), w = cam_width ( cap ), al = cam_step ( cap ), ai = cam_channel ( cap );
  if ( !buf ) {
    img = MALLOC ( unsigned char, h * al );
    buf = MALLOC ( unsigned char, h * al );
    gry = MALLOC ( unsigned char, h * al );
  }
  if ( !frg ) {
    frg = MALLOC ( unsigned char, h * w * 3 );
  }
  imtrans(h, w, img2, al, ai, &h, &w, img, &al, transn, 0);
  ASSERT(frg);
  memset(frg, 0, h * w * 3);
  {
    h2=h*cs, w2=w*cs, al2=w2*ai;
    IMRESIZE(h, w, img, al, ai, h2, w2, buf, al2, ai, INTER_BILINEARITY);
    memcpy(img, buf, h2*al2);
    h=h2, w=w2, al=al2;
  }
  {
    //utime_start ( s );
    //IMTRANS(h, w, img, al, ai, gry, w, 1, BGR2GRAY);
    //colorcvt ( h, w, img, al, ai, gry, w, 1, T_BGR, T_GRAY );
    //SKIP_FILTEREX(h, w, img, al, ai, buf, al, ai, 1, 1, DILATION3X3_FT1, unsigned char);
    //printf("%f\n", utime_elapsed(s));
  }
  //printf( "fram %d ", fram );

  ASSERT( gmm != 0 );
  if ( fram < 10 ) {
    ASSERT( ai == 3 );
    utime_start( _start_time );
    gmm->SetupBG( img, al2, fram + 1 );
    //printf( " CHC_SetupBG = %f ", utime_elapsed( _start_time ) );
  } else {
    const unsigned int* p;
    int i, rectnum = 0;
    utime_start( _start_time );
    rectnum = gmm->Detect( img, al, 0 );
    //printf( " CHC_ForegroundDetect = %f ", utime_elapsed( _start_time ) );
    p = gmm->blob;
    if ( 0 ) {
      for ( i = 0; i < rectnum; ++i ) {
        const unsigned int* rect = p + i * 4;
        DrawRectangle( h, w, img, al, ai, iPOINT( rect[ 0 ], rect[ 1 ] ), iPOINT( rect[ 2 ], rect[ 3 ] ), CC_RGB( 255, 0, 0 ), 3, 8, 0 );
      }
    } else {
      fgpix = get_no_zero_pix_number( h * w, p );
      //printf( " pix = %f", k * 1. / ( h * w ) );
      for (i=0; i<h*w; ++i) {
        if (p[i]) {
          unsigned char* pix = frg + i*3;
          pix[0] = pix[1] = pix[2] = 255;
        }
      }
    }
  }
  
  fram++;
  //printf( "\n" );
  unsigned char* bk = gmm->GetBk();
  rectn = gmm->GetRect();
  if (showwindow) {
    int i;
    for (i=0; i<h*w; ++i) {
      int id = gmm->blob[i];
      if (id>1) {
        int col = ColorNamesTable[(id)%countof(ColorNamesTable)].col;
        frg[i*3]=255;
        frg[i*3+1]=255;
        frg[i*3+2]=255;
        frg[i*3]=GetRValue(col);
        frg[i*3+1]=GetGValue(col);
        frg[i*3+2]=GetBValue(col);
      }
    }
    for (i=1; i<MIN(rectn, 10); ++i) {
      DrawRectangle( h, w, img, al, ai, iPOINT( gmm->br[i].l, gmm->br[i].t ), iPOINT( gmm->br[i].r, gmm->br[i].b ), CC_RGB( 255, 0, 0 ), 1, 8, 0 );
      DrawRectangle( h, w, bk, w*3, 3, iPOINT( gmm->br[i].l, gmm->br[i].t ), iPOINT( gmm->br[i].r, gmm->br[i].b ), CC_RGB( 255, 0, 0 ), 1, 8, 0 );;
      DrawRectangle( h, w, frg, w*3, 3, iPOINT( gmm->br[i].l, gmm->br[i].t ), iPOINT( gmm->br[i].r, gmm->br[i].b ), CC_RGB( 255, 0, 0 ), 1, 8, 0 );
    }
    cvShowImage ( "frg", h, w, frg, w*3, 3 );
    cvShowImage ( "cam", h, w, img, al, ai );
    //cvShowImage("cam", h, w, gry, w, 1);
    cvShowImage( "bk", h, w, bk, w*3, 3 );
    if (0) {
      char* line="";
      char line2[256];
      _snprintf(line2, 256, "%s_fg.jpg", line);
      imwrite(line2, h, w, frg, w*3, 3);
      _snprintf(line2, 256, "%s_bk.jpg", line);
      imwrite(line2, h, w, bk, w*3, 3);
    }
  }
  if (waitms>0)
    cvWaitKey(waitms);
  return frg;
}

#define TEST_GMM3DTRACK
#include "GMM3DTrack.h"

int saveToImg(const char* fn, int h, int w, const unsigned char* img, int al, int ai) {
  imwrite(fn, h, w, img, al, ai);
  return 0;
}

void* GMM3DTrack(int mode) {
  if (GT_START==mode) {
    cap = cam_start( 0, 0 );
    if (NULL==cap) return 0;
    cam_GrabFrame ( cap );
    img2 = cam_begin ( cap );
    fram=0;
    h = cam_height ( cap ), w = cam_width ( cap ), al = cam_step ( cap ), ai = cam_channel ( cap );
    

    img = MALLOC ( unsigned char, h * al );
    buf = MALLOC ( unsigned char, h * al );
    gry = MALLOC ( unsigned char, h * al );
    frg = MALLOC ( unsigned char, h * al );
    
    h2=h*cs, w2=w*cs, al2=w2*ai;
    h=h2, w=w2, al=al2;

    gmm = &g_gmm;
    gmm->Init( 0.005, 0.4, 5.0, 0.05, 0.95, 0.3, 0.3, 0.1, h, w, 3, 20, 0, 0, 0, 1 );
    return cap;
  }
  if (GT_STOP==mode) {
    cam_stop ( cap );
    SAFEFREE ( buf );
    SAFEFREE ( gry );
    SAFEFREE ( rgb );
    SAFEFREE ( img );
    gmm->UnInit();
    fram=0;
    return 0;
  }
  if (GT_GETHEIGHT==mode) {return &h2;}
  if (GT_GETWIDTH==mode) {return &w2;}
  if (GT_GETCHANNEL==mode) {return &ai;}
  if (GT_GETPICH==mode) {return &al2;}
  if (GT_GETTRANSN==mode) {return &transn;}
  if (GT_GETDATA==mode) {return img2;}
  if (GT_GETSHOWWINDOW==mode) {return &showwindow;}
  if (GT_GETBK==mode) {return gmm->bk;}
  if (GT_GETFG==mode) {return frg;}
  if (GT_GEWAITMS==mode) {return &waitms;}
  if (GT_GETMINPIX==mode) {return &gmm->min_pts;}
  if (GT_GETNEXT==mode) {return GMM3DTrack_next(0);}
  if (GT_GETRECT==mode) {return gmm->br;}
  if (GT_GETRECTN==mode) {return &rectn;}
  if (GT_GETFGPIX==mode) {return &fgpix;}
  return 0;
}

int GMM3DTrackInt(int mode) {
  return *(int*)GMM3DTrack(mode);
}
int GMM3DTrackStart() {return NULL!=GMM3DTrack(GT_START);}
int GMM3DTrackGetNext(){return NULL!=GMM3DTrack_next(GT_GETNEXT);}
int GMM3DTrackGetfgpix(){return *(int*)GMM3DTrack(GT_GETFGPIX);}
int GMM3DTrackStop(){return NULL!=GMM3DTrack(GT_STOP);}
