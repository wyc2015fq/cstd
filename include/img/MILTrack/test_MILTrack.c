
#include <math.h>
#include <stdio.h>
#include "cap.h"
#include "macro.h"
#include "MILTrack.h"
#include "_MILTrack.h"
#include "direct.h"
#include "cap.h"
#include "window.h"
#include "sort.h"
#include "utime.h"
#include "chalg.h"
#include "chalg.h"
#include "debug.h"
#include "inifile.h"
#include "imcolor.h"

const char* _pwd() {
  static char buf[ MAX_PATH ];
  return _getcwd( buf, MAX_PATH );
}
#define  pwd _pwd()

#define tic {utime_start(_start_time)
#define toc printf("%f\n", utime_elapsed(_start_time)); }

//int if_face_detect = 1;//是否进行人脸检测，为0则不进行人脸检测，用感兴趣区域选择函数
//int if_replace_clf = 0;//是否替换(分类能力差的)分类器,为0则不替换

//*以下为参数设置*//
char seqname[ 256 ] = ".\\新序列\\avi%04d.bmp";

int pos[ 4 ] = {
#include "F:\\pub\\bin\\MILTrack\\pos.txt"
               };
               
               
#define IniFileName ".\\test_MILTrack.ini"
#define IniAppName "test_MILTrack"
               
MILTRACK* pmt = NULL;

int init_MILTrack() {
  char buf[ 256 ];
  pmt = CPM_NewMilTrack(
          IniGetInt( IniFileName, IniAppName, "M", 0 ), //M
          IniGetInt( IniFileName, IniAppName, "K", 0 ), //K
          IniGetInt( IniFileName, IniAppName, "num_neg", 65 ), //负样本数
          IniGetInt( IniFileName, IniAppName, "num_pos", 45 ), //正样本数
          IniGetInt( IniFileName, IniAppName, "num_candidate_result", 500 ), //搜索样本数
          IniGetInt( IniFileName, IniAppName, "beita1", 0 ), //beita1
          IniGetInt( IniFileName, IniAppName, "beita2", 0 ), //beita2
          IniGetInt( IniFileName, IniAppName, "gama", 0 ), //gama
          IniGetInt( IniFileName, IniAppName, "s", 0 ), //K
          IniGetInt( IniFileName, IniAppName, "num_fix_cr", 0 ), //K
          IniGetFloat2( IniFileName, IniAppName, "v" )  //K
        );
  IniGetString( IniFileName, IniAppName, "初始框位置", "91,62,99,116", buf, 256 );
  IniGetString( IniFileName, IniAppName, "测试序列", "", seqname, 256 );
  sscanf( buf, "%d,%d,%d,%d", &pos[ 0 ], &pos[ 1 ], &pos[ 2 ], &pos[ 3 ] );
  return 0;
}

#if 1
#define SEQ_START(x)     seq_start(x)
#define SEQ_GRABFRAME(x) seq_GrabFrame(x, 1)
#define SEQ_STOP         seq_stop
#define SEQ_HEIGHT       seq_height
#define SEQ_WIDTH        seq_width
#define SEQ_STEP         seq_step
#define SEQ_CHANNEL      seq_channel
#define SEQ_BEGIN        seq_begin

typedef IMGSEQ SEQ;
#else

typedef capcam SEQ;
#define SEQ_START(x)     NewCAM_VFW(0, 240, 320)
#define SEQ_GRABFRAME    cam_GrabFrame
#define SEQ_STOP         cam_stop
#define SEQ_HEIGHT       cam_height
#define SEQ_WIDTH        cam_width
#define SEQ_STEP         cam_step
#define SEQ_CHANNEL      cam_channel
#define SEQ_BEGIN        cam_begin
#endif

int icvYUV4222BGR_8u_C2C3R( int height, int width, const unsigned char* src, int srcstep, int srccn,
                             unsigned char* dst, int dststep, int dstcn );

int test_MILTrack() {
  SEQ * seq = 0;
  int high = 0;
  unsigned char* gry = 0;
  unsigned char* buf = 0;
  unsigned char* rgb = 0;
  //int i, j, m, k;
  init_MILTrack();
  cvNamedWindow( "seq", 1 );
  cvCreateTrackbar( "high", "seq", &high, 300, 0 );
  seq = SEQ_START( seqname );
  //seq = seq_start(".\\测试序列\\avi%04d.bmp");
  if ( seq ) {
    int t = 0;
    char ch = 0;
    for ( ; 'q' != ch && 'Q' != ch && SEQ_GRABFRAME( seq ); ++t ) {
      unsigned char* I = seq_begin( seq );
      int pic_h = SEQ_HEIGHT( seq ), pic_w = SEQ_WIDTH( seq ), al = SEQ_STEP( seq ), ai = SEQ_CHANNEL( seq );
      if ( !buf ) {
        buf = pmalloc( pic_h * al );
      }
      if ( 2 == ai ) {
        int w3 = pic_w * 3;
        
        if ( !rgb ) {
          rgb = MALLOC( unsigned char, pic_h * pic_w * 3 );
        }
        
        icvYUV4222BGR_8u_C2C3R( pic_h, pic_w, I, al, 2, rgb, w3, 3 );
        al = w3;
        I = rgb;
        ai = 3;
      }
      
      if ( 3 == ai ) {
        if ( NULL == gry ) {
          gry = MALLOC( unsigned char, pic_h * pic_w );
        }
        IMTRANS( pic_h, pic_w, I, al, ai, gry, pic_w, 1, BGR2GRAY );
        al = pic_w;
        I = gry;
        ai = 1;
      }
      
      tic;
      
      //////////////////////////////////////////////////////////////////////
      // step1:检测人脸以及随机选择M个特征 //
      //////////////////////////////////////////////////////////////////////
      ////////////////////////////
      // step2:分类
      ////////////////////////////
      if ( t >= 1 ) {
        CvPoint pt1 = {0}, pt2;
        CPM_MilTrack( pmt, pic_h, pic_w, I, pic_w, &pt1.y, &pt1.x );
        pt2.x = pt1.x + pos[ 2 ];
        pt2.y = pt1.y + pos[ 3 ];
        printf( "%3d: %3d %3d ", t, pt1.y, pt1.x );
        DrawRectangle( pic_h, pic_w, I, al, ai, pt1, pt2, CC_RGB( 255, 0, 0 ), 1, 8, 0 );
        //plot_location(X1,Y1,X3,Y3,I_temp,d2,t,2);//以下为保存跟踪结果，2表示绿色
      } else {
        //第一帧进行人脸检测，记录矩形框尺度以及左上角点坐标(X1,Y1)右下角点坐标(X3,Y3)
        //patch_pos_sample = I(Y1:Y3,X1:X3);//第一帧的正样本
        //sample_w = size(patch_pos_sample);
        CPM_MilTrackSelectObject( pmt, pic_h, pic_w, I, pic_w, pos[ 1 ], pos[ 0 ], pos[ 3 ], pos[ 2 ] );
      }
      
      //if (if_replace_clf ==1){//如果设置为改换特征(分类器),这个写成C++代码的时候可以先不加
      //  replaceClk = randomMFeatures(sample_w,sample_h,num_replace_clk);//随机产生用于替换的num_replace_clk个分类器(特征)
      //  MFeatures[L_ind(M-num_replace_clk+1:M)] = replaceClk;
      //}
      
      toc;
      //break
      
      if ( ch != -1 )
        printf( "%x\n", ch );
      cvShowImage( "seq", pic_h, pic_w, I, al, ai );
      //cvShowImage("cam", h, w, img, al, ai);
      ch = cvWaitKey( 100 );
    }
    SEQ_STOP( seq );
  }
  SAFEFREE( buf );
  SAFEFREE( gry );
  SAFEFREE( rgb );
  CPM_DelMilTrack( pmt );
  cvDestroyWindow( "seq" );
  return 0;
}

int test_imgseq() {
  IMGSEQ * seq = 0;
  int high = 0;
  unsigned char* gry = 0;
  unsigned char* buf = 0;
  cvNamedWindow( "seq", 1 );
  cvCreateTrackbar( "high", "seq", &high, 300, 0 );
  seq = seq_start( ".\\测试序列\\avi%04d.bmp" );
  if ( seq ) {
    int fram = 0;
    char ch = 0;
    for ( ; 'q' != ch && 'Q' != ch && SEQ_GRABFRAME( seq ); ++fram ) {
      unsigned char* img = seq_begin( seq );
      int h = seq_height( seq ), w = seq_width( seq ), al = seq_step( seq ), ai = seq_channel( seq );
      if ( !buf ) {
        buf = pmalloc( h * al );
        gry = pmalloc( h * al );
      }
      if ( ch != -1 )
        printf( "%x\n", ch );
      cvShowImage( "seq", h, w, img, al, ai );
      //cvShowImage("cam", h, w, img, al, ai);
      ch = cvWaitKey( 100 );
    }
    seq_stop( seq );
  }
  if ( buf ) {
    pfree( buf );
    pfree( gry );
  }
  return 0;
}

int test_imadjust() {
  const TCHAR * picname = _T( "lena_gray.bmp" );
  struct fmtimgreader* reader = new_reader( picname );
  if ( reader ) {
    int height = reader->m_height;
    int width = reader->m_width;
    int channels = reader->m_iscolor ? 3 : 1;
    int step;
    unsigned char* data;
    //channels = 1;
    step = width * channels;
    data = ( unsigned char* ) pmalloc( height * step );
    memset( data, 128, height * step );
    imread( reader, data, step, channels > 1 );
    imadjust( height * width, data, data );
    imwrite( _T( "lena_gray_imadjust.bmp" ), height, width, data, step, channels );
    pfree( data );
    free_reader( reader );
  }
  return 0;
}

int test_randomMFeatures() {
  enum { M = 50 };
  MILHAAR MFeatures[ M ];
  int i;
  unsigned char img[ 100 * 100 ];
  int InterData[ 101 * 101 ];
  randomMFeatures( 100, 100, M, MFeatures );
  for ( i = 0; i < 100*100; ++i ) {
    img[ i ] = unidrnd( 255, 1, 1 );
  }
  icvIntegralImage( img, 100, InterData, 101, 100, 100 );
  for ( i = 0; i < M; ++i ) {
    int y = GetOneFeatureValue_2( MFeatures[ i ].Type, MFeatures[ i ].StPos, MFeatures[ i ].Scale,
                                  InterData, 101 );
    printf( "%d\n", y );
  }
  return 0;
}

int test_randomSelectSample() {
  MILPOINT YX_all[ 200 ];
  int ret = randomSelectSample( 10, 10, 240, 380, 640, 480, 50, 80, 20, 100, 100, 0, YX_all );
  return 0;
}

int test_exp() {
  int i, n = 500 * 110;
  double sum = 0.;
  {
    utime_start( _start_time );
    for ( i = 0; i < n; ++i ) {
      sum += exp( unidrnd( n, 1, 1 ) );
      sum += exp( unidrnd( n, 1, 1 ) );
      sum += exp( unidrnd( n, 1, 1 ) );
      sum = log( sum );
    }
    printf( "%f\n", utime_elapsed( _start_time ) );
  }
  printf( "%f\n", sum );
  return 0;
}

int test_std() {
  int i;
#if 0
  
  real a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
#else
  
  real a[] = {
#include "F:\\pub\\bin\\MILTrack\\65.txt"
             };
#endif
  
  int n = sizeof( a ) / sizeof( a[ 0 ] );
  real std0 = 0;
  real std1 = 0;
  real sum = 0, sumsq = 0;
  for ( i = 0; i < n; ++i ) {
    sum += a[ i ];
    sumsq += a[ i ] * a[ i ];
  }
  sum /= n;
  sumsq /= n;
  std0 = sqrt( sumsq - sum * sum );
  for ( i = 0; i < n; ++i ) {
    std1 += ( a[ i ] - sum ) * ( a[ i ] - sum );
  }
  std1 /= n;
  std1 = sqrt( std1 );
  return 0;
}

int main( int argc, char* argv[] ) {
  //test_imadjust();
  double a = 1.2323232323;
  double b = log( a );
  double c = exp( b );
  MEM_CHECK_BEGIN()
  //test_randomSelectSample();
  test_MILTrack();
  MEM_CHECK_END()
  //test_imgseq();
  //test_imadjust();
  //test_randomMFeatures();
  //test_exp();
  //test_randomMFeatures();
  return 0;
}
