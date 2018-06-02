
typedef int bool;

//#include "debug.h"
#include "..\\imgproc\\capimg.h"
#include "findfile.h"
#include "utime.h"
#include "GPA.h"

//#pragma comment (lib,"GPAdll.lib")

int test_GPA() {
  //capcam * cap = cap_open_imgseq( "F:\\pub\\bin\\GPA\\DOUDONG\\test\\a.txt", 3 );
  capcam * cap = cap_open_avi( "F:\\pub\\bin\\GPA\\DOUDONG\\a.avi" );
  //capcam * cap = cam_start( 0 );
  if ( cap ) {
    int h, w, al, ai, bl = 0, bi = 0, frame=0;
    unsigned char* a;
    unsigned char* img = 0;
    unsigned char* buf = 0;
    unsigned char* out = 0;
    GPA* g=NULL;
    for ( ;; ++frame) {
      //cvtcolor( h, w, a, al, ai, img, bl, bi, T_YUYV, T_RGB );
      cam_QueryFrame( cap, h, w, a, al, ai );
      if (NULL==a) {
        break;
      }
      if (NULL==out) {
        out = MALLOC(uchar, h*w*3);
      }
      if (NULL==g) {
        g = GPA_new(h, w, a, al);
        if (NULL==g) break;
      } else {
        //utime_start(_start_time);
        GPA_update(g, a, al, out, w*3);
        //printf("%f\n", utime_elapsed(_start_time));
        cvShowImage( "纠正后", h, w, out, w*3, 3 );
      }
      cvShowImage( "原始", h, w, a, al, ai );
      if ( 'q' == cvWaitKey( 50 ) ) {
        break;
      }
    }
    SAFEFREE( buf );
    SAFEFREE( img );
    SAFEFREE( out );
    cam_stop( cap );
    if (g) GPA_free(g);
  }
  return 0;
}

int main() {
//MEM_CHECK_BEGIN()
  test_GPA();
//MEM_CHECK_END()
  return 0;
}



