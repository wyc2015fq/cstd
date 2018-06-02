
#include <io.h>
#include <direct.h>
#include <tchar.h>

#define FOR1_BEGIN(_N, _AI, _IA) do { for (_IA=0; _IA<(_N)*(_AI); _IA+=_AI) {
#define FOR1_END  }} while(0);

#define FOR2_BEGIN(_H, _W, _AL, _AI, _IA)                                     \
  do {                                                                          \
  int _LA;                                                                    \
  for (_LA=0; _LA<(_H)*(_AL); _LA+=_AL) {                                     \
for (_IA=_LA; _IA<((_W)*(_AI)+_LA); _IA+=_AI) {

#define FOR2_END  }}} while(0);

#define FPRINT1D(_F, FMT, _N, _A, _I)                                         \
  do {                                                                        \
  int _IA;                                                                  \
  FILE* pf = fopen(_F, "wb");                                               \
  fprintf(pf, "//[%d]\n", _N);                                              \
  FOR1_BEGIN(_N, _I, _IA)                                                   \
  fprintf(pf, FMT, *(_A+_IA));                                              \
  FOR1_END;                                                                 \
  fprintf(pf, "\n");                                                        \
  fclose(pf);                                                               \
  } while(0)

#define FPRINT2D(_F, FMT, FMTL, _H, _W, _A, _L, _I)                           \
  do {                                                                        \
  int _IA;                                                                  \
  FILE* pf = _tfopen(_F, _T("wb"));                                         \
  /*fprintf(pf, "[%d %d]"ENDL, _H, _W);*/                                   \
  FOR2_BEGIN(_H, _W, _L, _I, _IA)                                           \
  _ftprintf(pf, FMT, *(_A+_IA));                                            \
  } _ftprintf(pf, FMTL);/**/ } } while(0);                                    \
  fclose(pf);                                                                 \
  } while(0)

#include "monofilt.inl"
#include "LBP3.inl"

const unsigned char mapping3[] = {
#include "../faceDetect/txt/mapping_3.txt"
                                   };
const unsigned char featurelist3[] = {
//#include "./featurelist3_10x10.txt"
//#include "../faceDetect/txt/Feature_List_3.txt"
#include "../faceDetect/txt/Feature_List_20121008.txt"
                                   };

typedef unsigned short ushort;

#define INTCAST(x)  (int)(x)


#define SKIP_HIST(h, w, _A, al, ai, _hist)                                    \
do {                                                                          \
  int a0, a1, _h=(h)*(al), _w=(w)*(ai);                                       \
  if (_w==(al)) {_w*=(h), _h=1; }                                             \
  for (a1=0; a1<_h; a1+=(al)) {                                               \
    for (a0=a1; a0<(_w+a1); a0+=(ai)) {                                       \
      ++(*(_hist+INTCAST( *(_A+a0) ))); /* 计数加1 */                         \
    }                                                                         \
  }                                                                           \
} while(0)

#define countof(arr)  ((sizeof(arr))/(sizeof(arr[0])))
typedef ushort FEAT_T;
typedef IplImage* IplImage_ptr;

//#define monofilt_feat monofilt_feat_impl
//#define test_feat lbp_feat
#define test_feat monofilt_feat_impl
//#define MONOFILT_FEAT monofilt_feat_adaboost
#define MONOFILT_FEAT monofilt_feat_adaboost
//#define MONOFILT_FEAT lbp_feat

int monofilt_feat_impl1(const IplImage * img, ushort* feat, int featurelistlen, const unsigned char* featurelist)
{
  int i, j, v;
  int rows,cols;
  int minWaveLength       =  4;
  int region_num          =  8;
  
  int nscale       =  3;
  int orientWrap   =  0;
  int bin_num_a    =  60;
  double sigmaOnf     =  0.65;
  double mult         =  2;
  double bh_n         =  4;
  double bw_n         =  5;
  double sh_n         =  1;
  double sw_n         =  1;
  double neigh        =  8;
  double radius       =  3;
  double NumTarget    =  10;

  IplImage_ptr f1    [10]={0};
  IplImage_ptr h1f1  [10]={0};
  IplImage_ptr h2f1  [10]={0};
  IplImage_ptr A1    [10]={0};
  IplImage_ptr theta1[10]={0};
  IplImage_ptr psi1  [10]={0};
  IplImage_ptr Tem_img, LBPHIST1, N_LBPHIST;
  ushort* it=feat;
  uchar binmap[256];
  if (NULL==feat) {
    return nscale*bin_num_a*featurelistlen*sizeof(FEAT_T);
  }
  for (i=0; i<256; ++i) {binmap[i]=i*bin_num_a/256;}
  for (i=1; i<=nscale; ++i) {
    f1    [i] = cvCreateImage(cvGetSize(img),IMG_DEPTH_64F, 1);
    h1f1  [i] = cvCreateImage(cvGetSize(img),IMG_DEPTH_64F, 1);
    h2f1  [i] = cvCreateImage(cvGetSize(img),IMG_DEPTH_64F, 1);
    A1    [i] = cvCreateImage(cvGetSize(img),IMG_DEPTH_64F, 1);
    theta1[i] = cvCreateImage(cvGetSize(img),IMG_DEPTH_64F, 1);
    psi1  [i] = cvCreateImage(cvGetSize(img),IMG_DEPTH_64F, 1);
  }
  //{utime_start(_start_time);
  monofilt(img, nscale, minWaveLength, mult, sigmaOnf, orientWrap, f1, h1f1, h2f1, A1, theta1, psi1);
  //printf("%f\n", utime_elapsed(_start_time));}
  
  //FPRINT1D("h2f1{1}.txt", "%lf,\n", 10000, ((double*)(h2f1[1]->imageData)), 1);
  //FPRINT1D("f1{1}.txt", "%lf,\n", 10000, ((double*)(f1[1]->imageData)), 1);
  //FPRINT1D("h1f1{1}.txt", "%lf,\n", 10000, ((double*)(h1f1[1]->imageData)), 1);
  //FPRINT1D("h2f1{1}.txt", "%lf,\n", 10000, ((double*)(h2f1[1]->imageData)), 1);
  //FPRINT1D("A1{1}.txt", "%lf,\n", 10000, ((double*)(A1[1]->imageData)), 1);
  //FPRINT1D("theta1{1}.txt", "%lf,\n", 10000, ((double*)(theta1[1]->imageData)), 1);

  Tem_img = cvCreateImage(cvGetSize(img),IMG_DEPTH_8U, 1);
  LBPHIST1 = cvCreateImage(cvGetSize(img),IMG_DEPTH_8U, 1);
  N_LBPHIST = cvCreateImage(cvGetSize(img),IMG_DEPTH_8U, 1);
  
  rows=img->height, cols = img->width;
  for (v=1; v<=nscale; ++v) {
    //Tem_img=uint8((A1[v]-min(A1[v](:)))./(max(A1[v](:))-min(A1[v](:))).*255);
    //LBPHIST1=LBP3(Tem_img,mapping3);
#if 0
    double minVal = 0, maxVal = 0;
    double scale = 0, shift = 0;
    cvMinMaxLoc( A1[v], &minVal, &maxVal, 0, 0, 0 );
    scale = 255/(maxVal - minVal);
    shift = -minVal * scale;
    cvConvertScale(A1[v], Tem_img, scale, shift);
    LBP3_u8(Tem_img, mapping3, LBPHIST1);
#else
    LBP3_f64(A1[v], mapping3, LBPHIST1);
#endif
    
    //FPRINT1D("Tem_img.txt", "%d,\n", 10000, ((uchar*)(Tem_img->imageData)), 1);
    //FPRINT2D("LBPHIST1.txt", "%3d,", "\n", 50, 100, ((uchar*)(LBPHIST1->imageData)), 100, 1);
    //cvNamedWindow("A1",0); cvShowImage("A1",A1[v]);
    //matrix2(h1f1[v]>0)=0;matrix2(h1f1[v]<=0)=1;
    //matrix3(h2f1[v]>0)=0;matrix3(h2f1[v]<=0)=1;
    //N_LBPHIST=matrix2*128+matrix3*64+double(LBPHIST);// for uniform lbp;
    
    {
      for( j = 0; j < rows; j++ ) {
        for( i = 0; i < cols; i++ ) {
#if 0
          int t1 = CC_IMAGE_ELEM( LBPHIST1, uchar, j, i);
          double f1 = CC_IMAGE_ELEM( h1f1[v], double, j, i);
          double f2 = CC_IMAGE_ELEM( h2f1[v], double, j, i);
          int t2 = t1+((f1<=0)*128)+((f2<=0)*64);
          CC_IMAGE_ELEM( N_LBPHIST, uchar, j, i) = binmap[t2];
#else
#define M_PI (3.1415926)
#define M_PI2 (3.1415926/2)
          // 3 1 0 2
          int aaa[4]={192,64,0,128};
          int t1 = CC_IMAGE_ELEM( LBPHIST1, uchar, j, i);
          double f1 = CC_IMAGE_ELEM( theta1[v], double, j, i);
          int t2 = (int)((f1+M_PI)/(M_PI2));
          int t3 = t1+aaa[t2];
          assert(t2>=0 && t2<4);
          CC_IMAGE_ELEM( N_LBPHIST, uchar, j, i) = (uchar)(t3/4.32);
#endif
        }
      }
      //FPRINT2D("N_LBPHIST.txt", "%3d,", "\n", 50, 100, ((uchar*)(N_LBPHIST->imageData)), 100, 1);
      {
        int _bin;
        int bin = bin_num_a;
        int len = featurelistlen;
        const unsigned char* rect = featurelist;
        int srcw=N_LBPHIST->widthStep;
        uchar* img2 = (uchar*)N_LBPHIST->imageData;
        //utime_start(_start_time);
        for ( _bin = 0; _bin < len; ++_bin, rect += 4, it += bin ) {
          //int y = rect[ 0 ], x = rect[ 1 ], cy = rect[ 2 ], cx = rect[ 3 ];
          int x = rect[ 0 ], y = rect[ 1 ], cx = rect[ 2 ], cy = rect[ 3 ];
          unsigned char* pos = img2 + y * ( srcw ) + x;
          assert(y+cy<=rows);
          assert(x+cx<=cols);
          memset(it, 0, sizeof(FEAT_T)*bin);
          SKIP_HIST( cy, cx, ( pos ), srcw, 1, ( it ) );
          {
            int i=0, s=0;
            for (; i<bin; ++i) {
              s+=it[i];
            }
            assert(s==cx*cy);
          }
        }
      }
    }
    //cvNamedWindow("LBPHIST1",0); cvShowImage("LBPHIST1",LBPHIST1);
    //cvNamedWindow("N_LBPHIST",0); cvShowImage("N_LBPHIST",N_LBPHIST);
  }
  cvReleaseImage(&Tem_img);
  cvReleaseImage(&LBPHIST1);
  cvReleaseImage(&N_LBPHIST);
  for (i=1; i<=nscale; ++i) {
    cvReleaseImage(&f1    [i]);
    cvReleaseImage(&h1f1  [i]);
    cvReleaseImage(&h2f1  [i]);
    cvReleaseImage(&A1    [i]);
    cvReleaseImage(&theta1[i]);
    cvReleaseImage(&psi1  [i]);
  }
  //cvWaitKey(0);
  return (it-feat)*sizeof(FEAT_T);
}

const unsigned char featurelist4[] = {
  //#include "./featurelist3_10x10.txt"
  //#include "../faceDetect/txt/Feature_List_3.txt"
//#include "./复件 采样参数.txt"
#include "./newindex.txt"
                                   };

int monofilt_feat_impl(const IplImage * img, ushort* feat)
{
  int featurelist_len = countof(featurelist4)/4;
  return monofilt_feat_impl1(img, feat, featurelist_len, featurelist4);
}

int lbp_feat(const IplImage * img, ushort* feat)
{
  int minWaveLength       =  4;
  int region_num          =  8;
  IplImage_ptr LBPHIST1;
  int bin_num_a    =  60;
  int featurelist3len = countof(featurelist3)/4;
  ushort* it=feat;
  if (NULL==feat) {
    return bin_num_a*featurelist3len*sizeof(FEAT_T);
  }
  
  LBPHIST1 = cvCreateImage(cvGetSize(img),IMG_DEPTH_8U, 1);

  {
    //LBPHIST1=LBP3(Tem_img,mapping3);
    LBP3_u8(img, mapping3, LBPHIST1);
    //cvNamedWindow("A1",0); cvShowImage("A1",A1[v]);
    //matrix2(h1f1[v]>0)=0;matrix2(h1f1[v]<=0)=1;
    //matrix3(h2f1[v]>0)=0;matrix3(h2f1[v]<=0)=1;
    //N_LBPHIST=matrix2*128+matrix3*64+double(LBPHIST);// for uniform lbp;
    
    
    {
      int _bin;
      int bin = bin_num_a;
      int len = featurelist3len;
      const unsigned char* rect = featurelist3;
      int srcw=LBPHIST1->widthStep;
      uchar* img2 = (uchar*)LBPHIST1->imageData;
      //utime_start(_start_time);
      for ( _bin = 0; _bin < len; ++_bin, rect += 4, it += bin ) {
        int y = rect[ 0 ], x = rect[ 1 ], cy = rect[ 2 ], cx = rect[ 3 ];
        unsigned char* pos = img2 + y * ( srcw ) + x;
        memset(it, 0, sizeof(FEAT_T)*bin);
        SKIP_HIST( cy, cx, ( pos ), srcw, 1, ( it ) );
      }
    }
    
    //cvNamedWindow("LBPHIST1",0); cvShowImage("LBPHIST1",LBPHIST1);
    //cvNamedWindow("N_LBPHIST",0); cvShowImage("N_LBPHIST",N_LBPHIST);
  }
  cvReleaseImage(&LBPHIST1);
  //cvWaitKey(0);
  return (it-feat)*sizeof(FEAT_T);
}

int monofilt_feat2( int srch, int srcw, const unsigned char* src, int srcstep, int cn, int type, void* featptr ) {
  int ret=0;
  IplImage* img;
  if (NULL==src||NULL==featptr)
    return monofilt_feat_impl(0, 0);
  img = cvCreateImage(cvSize(srcw, srch),IMG_DEPTH_8U, 1);
  memcpy(img->imageData, src, srch*srcw);
  ret = monofilt_feat_impl(img, (FEAT_T*)featptr);
  cvReleaseImage(&img);
  return ret;
}

int monofilt_feat3( int srch, int srcw, const unsigned char* src, int srcstep, int cn, int type, void* featptr, int featlistlen, const unsigned char* featlist ) {
  int ret=0;
  IplImage* img;
  if (NULL==src||NULL==featptr)
    return monofilt_feat_impl(0, 0);
  img = cvCreateImage(cvSize(srcw, srch),IMG_DEPTH_8U, 1);
  memcpy(img->imageData, src, srch*srcw);
  ret = monofilt_feat_impl1(img, (FEAT_T*)featptr, featlistlen, featlist);
  cvReleaseImage(&img);
  return ret;
}

int lbp59_feat( int srch, int srcw, const unsigned char* src, int srcstep, int cn, int type, void* featptr ) {
  int ret=0;
  IplImage* img;
  if (NULL==src||NULL==featptr)
    return lbp_feat(0, 0);
  img = cvCreateImage(cvSize(srcw, srch),IMG_DEPTH_8U, 1);
  memcpy(img->imageData, src, srch*srcw);
  ret = lbp_feat(img, (FEAT_T*)featptr);
  cvReleaseImage(&img);
  return ret;
}

//static const char* logname(const char*) {}
static char* _logfilename() {
  static char __logfilename[256]=".\\log.txt";
  return __logfilename;
}
#define LOGFILENAME _logfilename()
static int fclear(const char* fname) {
  FILE* pf = fopen(fname, "wb");
  if (pf) {
    fclose(pf);
    return 1;
  }
  return 0;
}

#define logclear()   fclear(LOGFILENAME)
#define logset(_name)   strcpy(LOGFILENAME, _name)
static int* get__logecho() {static int __logecho=1;return &__logecho;}
static int* get__logtime() {static int __logtime=1;return &__logtime;}
#define __logecho  (*(get__logecho()))
#define __logtime  (*(get__logtime()))
#define logecho(_ON_OFF)   (__logecho=_ON_OFF)
#define logtime(_ON_OFF)   (__logtime=_ON_OFF)
// 日志打印 pf - 日志文件 如果为空则只打印在标准输出 stdout上
//#ifndef PTTIME
#include <time.h>
static char _time_buffer[ 16 ];
#define STRDATE()  _strdate(_time_buffer)
#define STRTIME()  _strtime(_time_buffer)

#define PTTIME STRTIME()
//#endif

static int logprintf( const char* fmt, ... ) {
  FILE* pf;
  pf = fopen(LOGFILENAME, "a");
  if (pf) {
    va_list ARGLIST;
    va_start( ARGLIST, fmt );
    if (__logtime) { fprintf( pf, "%8s ", PTTIME );}

    vfprintf( pf, fmt, ARGLIST );
    fflush( pf );
    if (__logecho) {vprintf( fmt, ARGLIST );}
    va_end( ARGLIST );
    fclose(pf);
  }
  return 0;
}


static char* strtrim(char* s, const char* sp) {
  char* e = s + strlen(s);
  for (;s<e--;) {
    if (strchr(sp, *e)==NULL) {
      e[1]=0;
      break;
    }
  }
  return s;
}

int test_face_feature2() {
  void * disp = 0;
  int i, j, id = 0, ret = 0, fram = 0, grylen = 0;
  char ch = 0;
  char buf[256];
  FILE* plisttxt;
  FILE* outtxt;
  IplImage* im;
  FEAT_T feat[20*4000];
  //FEAT_T featall[100000];
  int featnum=0;
  int featstep=4000;
  int picnum=0;
  //#ifdef _DEBUG
  _chdir("E:\\pub\\bin\\feat");
  //#endif
  plisttxt = fopen("list.txt", "rb");
  for ( i = 0; i<20 && fgets(buf, 256, plisttxt)>0; ++i ) {
    strtrim(buf, "\n\r");
    im = cvLoadImage1(buf, 1);
    featnum=test_feat(im, feat+i*featstep);
    cvReleaseImage(&im);
    printf("%s\n", buf);
  }
  fclose(plisttxt);
  picnum=i;
  outtxt = fopen("dist.txt", "wb");
  for ( i = 0; i<picnum; ++i ) {
    for ( j = i+1; j<picnum; ++j ) {
      //double d = get_chi_dist(feat+i*featstep, feat+j*featstep)/25000.f;
      //fprintf(outtxt, "%2d,%2d,%f,\r\n", i+1, j+1, d);
    }
  }
  fclose(outtxt);
  return 0;
}
#define MALLOC(_TYPE, _N)     (_TYPE*)pmalloc((unsigned int)(sizeof(_TYPE)*(_N)))
#define REALLOC(_TYPE, _PTR, _N)     (_TYPE*)prealloc(_PTR, (unsigned int)(sizeof(_TYPE)*(_N)))
#define MEMSET(_F, _V, _N)     memset(_F, _V, sizeof(*(_F))*(_N))
#define MEMZERO(_F, _N)        memset(_F, 0, sizeof(*(_F))*(_N))
#define MEMCPY(_P, _F, _N)     memcpy(_P, _F, sizeof(*(_F))*(_N))

#define MEMMOVE(_P, _F, _N)    memmove(_P, _F, sizeof(*(_F))*(_N))
#define GET_TXT_LINES(pf, buf, bufsize, lines)  for (lines=0, fseek(pf, 0, SEEK_SET); fgets(buf, bufsize, pf); ++lines)

char* trim( char* str ) {
  char * p = str + strlen( str );
  for ( ; str < p && '\n' == p[ -1 ] || '\r' == p[ -1 ] || '\r' == p[ -1 ] || ' ' == p[ -1 ]; --p ) {}
  *p = '\0';
  return str;
}
int* strlist2id(int n, char** strlist) {
  int i;
  int* id = MALLOC(int, n);
  for (i=0; i<n; ++i) {
    id[i] = atoi(strlist[i]);
  }
  return id;
}

int read_strlist(const char* txtpath, char*** strlist) {
  char buf[ 1024 ];
  FILE* ptxt = fopen( txtpath, "rb" );
  int i=0, lines_n=0;
  if ( !ptxt ) {
    printf( "err: %s文件打开失败!\n", txtpath );
    return 0;
  }
  if (NULL!=strlist) {
    GET_TXT_LINES( ptxt, buf, 1024, lines_n );
    fseek(ptxt, 0, SEEK_SET);
    *strlist = MALLOC(char*, lines_n+1);
    memset(*strlist, 0, sizeof(char*)*(lines_n+1));
    GET_TXT_LINES( ptxt, buf, 1024, i ) {
      char * fname;
      trim(buf);
      fname = strrchr( buf, '\\' );
      (*strlist)[i] = strdup( fname ? fname + 1 : buf );
    }
  }
  //strlist2id(lines_n, *strlist);
  fclose( ptxt );
  return lines_n;
}
enum FILE_ACCESS_MODE {
  AS_EXIST = 0 , AS_EXECUTE = 1, AS_WRITE = 2, AS_READ = 4, AS_WRITE_READ = 6
};

#define file_exist(name)       (0 == _taccess(name, AS_EXIST))

#define RSKIP(str, chr)                                                       \
do {                                                                          \
  char* pp = strrchr(buf, chr);                                               \
  if (pp) {                                                                   \
    *pp = '\0';                                                               \
  }                                                                           \
} while(0)

int monofilt_feat_adaboost_list[]={
#include ".\\cas\\monofilt_feat_adaboost_list.txt"
};
ushort g_feat[100000];
int monofilt_feat_adaboost(const IplImage* img, ushort* feat) {
  int i;
  int feat_n = countof(monofilt_feat_adaboost_list);
  int allfeat_n = test_feat(0, 0)/sizeof(FEAT_T);
  assert(100000>allfeat_n);
  if (NULL==feat) {return feat_n*sizeof(FEAT_T);}
  test_feat(img, g_feat);
  for (i=0; i<feat_n; ++i) {
    int j = monofilt_feat_adaboost_list[i];
    feat[i] = g_feat[j];
  }
  return feat_n*sizeof(FEAT_T);
}

int get_feat_from_imgfile( const char* imgname, int feattype, FEAT_T* feats ) {
  int featsize=0;
  IplImage *src = cvLoadImage1(imgname,0);
  if ( src ) {
    if (100!=src->height||100!=src->width) {
      IplImage* src2 = cvCreateImage1(cvSize(100, 100), IMG_DEPTH_8U, 1);
      cvResize(src, src2, CC_INTER_LINEAR);
      cvSaveImage(imgname, src2);
      featsize = MONOFILT_FEAT( src2, feats );
      //cvNamedWindow("src2",0); cvShowImage("src2",src2); cvWaitKey(0);
      cvReleaseImage(&src2);
    } else {
      featsize = MONOFILT_FEAT( src, feats );
    }
    cvReleaseImage(&src);
  }
  return featsize;
}

int load_sample_set( int* plines_n, int* pfeatsize, FEAT_T** pft, const char* datpath ) {
  int lines_n;
  int featsize;
  FILE * pf = fopen( datpath, "rb" );
  if ( pf ) {
    fread( &lines_n, sizeof( int ), 1, pf );
    fread( &featsize, sizeof( int ), 1, pf );
    *pft = ( FEAT_T* ) pmalloc( lines_n * featsize );
    if ( *pft ) {
      fread( *pft, ( lines_n ) * ( featsize ), 1, pf );
    }
    fclose( pf );
  }
  if ( plines_n )
    * plines_n = lines_n;
  if ( pfeatsize )
    * pfeatsize = featsize;
  return 1;
}
int new_sample_set( const char* txtpath, int feattype, int* plines_n, int* pfeatsize, FEAT_T** pft, const char* datpath ) {
  int ah = 100, aw = 100, al = 100, ai = 1;
  char buf[ 1024 ];
  int lines_n, i;
  int featsize;
  featsize = MONOFILT_FEAT( 0, NULL );
  //real* ft = NULL;
  if (datpath) {
    if ( file_exist( datpath ) ) {
      load_sample_set(plines_n, pfeatsize, pft, datpath);
      return 1;
    }
  }
  {
    FILE* ptxt = fopen( txtpath, "rb" );
    if ( !ptxt ) {
      printf( "err: %s文件打开失败!\n", txtpath );
      return 0;
    }
    GET_TXT_LINES( ptxt, buf, 1024, lines_n );
    fseek(ptxt, 0, SEEK_SET);
    {
      *pft = ( FEAT_T* ) pmalloc( lines_n * featsize );
      memset(*pft, 0, lines_n * featsize);
      GET_TXT_LINES( ptxt, buf, 1024, i ) {
        FEAT_T * ft2 = *pft + i * featsize/sizeof(FEAT_T);
        if ( 1 ) {
          char * fname = strrchr( buf, '\\' );
          //printf( fname ? fname + 1 : buf );
          printf("%d/%d\r", i, lines_n);
        }
        RSKIP( buf, 0x0d );
        RSKIP( buf, 0x0a );
        get_feat_from_imgfile( buf, feattype, ft2 );
      }
      lines_n = i;
    }
    fclose( ptxt );
  }
  if ( plines_n )
    * plines_n = lines_n;
  if ( pfeatsize )
    * pfeatsize = featsize;
  if ( pft && datpath ) {
    FILE * pf;
    pf = fopen( datpath, "wb" );
    if ( pf ) {
      fwrite( &lines_n, sizeof( int ), 1, pf );
      fwrite( &featsize, sizeof( int ), 1, pf );
      fwrite( *pft, ( lines_n ) * ( featsize ), 1, pf );
      fclose( pf );
    }
  }
  return 1;
}

static int CPM_AbsSub( int n, const FEAT_T* a, const FEAT_T* b ) {
  int i;
  int out = 0;
  for ( i = 0; i < n; ++i ) {
    out += ( a[ i ] > b[ i ] ? ( a[ i ] - b[ i ] ) : ( b[ i ] - a[ i ] ) );
  }
  return out;
}

static int CPM_Distance( int n, const void* a, const void* b, int type ) {
  return CPM_AbsSub(n/sizeof(FEAT_T), (const FEAT_T*)a, (const FEAT_T*)b);
}
int free_strlist1(char** strlist) {
  int i=0;
  for (; strlist[i]; ++i) pfree(strlist[i]);
  pfree(strlist);
  return i;
}
#define ASSERT assert
typedef struct FACELIB {
  char** flist;
  int nlist;
  FEAT_T *feat;
  int featsize;
}FACELIB;

int read_facelib(const char* piclistfile, FACELIB* lib, const char* datfile) {
  int train_n=0;
  lib->nlist = read_strlist("list.txt", &lib->flist);
  new_sample_set( piclistfile, 0, &train_n, &lib->featsize, &lib->feat, datfile );
  ASSERT(train_n==lib->nlist);
  return 0;
}
int free_facelib(FACELIB* lib) {
  pfree(lib->feat);
  free_strlist1(lib->flist);
  return 0;
}
// 测试识别率
int test_faceRecognition4() {
  FACELIB train, test;
  int i, j;
  int featsize, featdim;
  int okk=0, err=0, jujue=0;
  int minnj=10000000;
  _chdir("D:\\tttt\\face\\Normal_face\\");
  read_facelib("list.txt", &train, "newfeat.dat");
  _chdir("D:\\tttt\\face\\Expression_face\\");
  read_facelib("list.txt", &test, "newfeat.dat");
  _chdir("D:\\tttt\\face\\");

  ASSERT(train.featsize==train.featsize);
  featsize = train.featsize;
  featdim = train.featsize;
  okk=0, err=0;
  //test.nlist/=10;
  //train.nlist/=10;
  for ( i = 0; i < test.nlist; ++i ) {
    int mindist=10000000;
    int minid=-1;
    for ( j = 0; j < train.nlist; ++j ) {
      FEAT_T *ft_i = test.feat+(featsize/sizeof(FEAT_T))*i;
      FEAT_T *ft_j = train.feat+(featsize/sizeof(FEAT_T))*j;
      int dis = CPM_Distance( featdim, ft_i, ft_j, 0 );
      if (dis<mindist) {
        minid=j;
        mindist=dis;
      }
      if (0!=strncmp(test.flist[i], train.flist[j], 9)) {
        if (dis<minnj) {
          minnj = dis;
        }
      }
    }
    printf("%d/%d\r", i, test.nlist);
    // 是否分对
    if (0==strncmp(test.flist[i], train.flist[minid], 9)) {
      ++okk;
    } else {
      ++err;
      //logprintf("%d %s %s\r\n", (int)mindist, test.flist[i], train.flist[minid]);
    }
  }
  logprintf("ok=%d err=%d all=%d 错误率=%f 识别率=%f\r\n", okk, err, test.nlist, (err*1./ test.nlist), (okk*1./ test.nlist));
  logprintf("阈值=%d\r\n", minnj);
  okk=0, err=0;
  jujue=0; // 拒绝
  for ( i = 0; i < test.nlist; ++i ) {
    int mindist=10000000;
    int minid=-1;
    for ( j = 0; j < train.nlist; ++j ) {
      FEAT_T *ft_i = test.feat+(featsize/sizeof(FEAT_T))*i;
      FEAT_T *ft_j = train.feat+(featsize/sizeof(FEAT_T))*j;
      int dis = CPM_Distance( featdim, ft_i, ft_j, 0 );
      if (dis<mindist) {
        minid=j;
        mindist=dis;
      }
    }
    printf("%d/%d\r", i, test.nlist);
    // 是否分对
    if (mindist<minnj) {
      if (0==strncmp(test.flist[i], train.flist[minid], 9)) {
        ++okk;
      } else {
        ++err;
      }
    } else {
      ++jujue;
    }
  }
  logprintf("拒绝=%d\r\n", jujue);
  logprintf("ok=%d err=%d all=%d 错误率=%f 识别率=%f\r\n", okk, err, test.nlist, (err*1./ test.nlist), (okk*1./ test.nlist));
  free_facelib(&train);
  free_facelib(&test);
  return 0;
}
#include <stdarg.h>
static int savedata(const char* fn, ...) {
  FILE* pf = fopen(fn, "wb");
  va_list marker;
  char* buf;
  size_t n, rn;
  if(pf) {
    va_start( marker, fn );
    while(1) {
      buf = va_arg( marker, char*);
      if (0==buf) break;
      n = va_arg( marker, size_t);
      if (0==n) break;
      rn = fwrite(buf, 1, n, pf);
      ASSERT(rn==n);
    }
    va_end( marker );
    fclose(pf);
  }
  return 0;
}
int test_faceRecognition5() {
  FACELIB train;
  int i, j;
  int featsize, featdim;
  int okk=0, err=0, jujue=0, cntnn=0, cntnj=0;
  int minnj=10000000;
  //char* featname="newalgo_60_10x10a_adaboost4.dat";
  //char* featname="newalgo_60_lbp3x3.dat";
  char* featname="newalgo_60_10x10a_adaboost5a.dat";
  //_chdir("E:\\pub\\bin\\adaboost\\");
  _chdir("D:/pub/cstd/NIR/NIRdatabase2");
  {
    char** flist;
    int nlist = read_strlist("list.txt", &flist);
    int* id = strlist2id(nlist, flist);
    savedata("idlist.dat", id, nlist*sizeof(int), 0);
    pfree(id);
    pfree(flist);
  }

  read_facelib("list.txt", &train, featname);
  logprintf("特征文件=%s 特征数=%d\r\n", featname, (train.featsize/2));

  featsize = train.featsize;
  featdim = train.featsize;
  okk=0, err=0;
  cntnj=0;
  //test.nlist/=10;
  //train.nlist/=10;
  // minnj 类间最小距离
  for ( i = 0; i < train.nlist; ++i ) {
    int minid=-1;
    for ( j = i+1; j < train.nlist; ++j ) {
      if (0!=strncmp(train.flist[i], train.flist[j], 4)) {
        FEAT_T *ft_i = train.feat+(featsize/sizeof(FEAT_T))*i;
        FEAT_T *ft_j = train.feat+(featsize/sizeof(FEAT_T))*j;
        int dis = CPM_Distance( featdim, ft_i, ft_j, 0 );
        if (dis<minnj) {
          minnj = dis;
        }
        cntnj++;
      }
    }
    printf("%d/%d\r", i, train.nlist);
    // 是否分对
  }
  logprintf("阈值=%d 类间总数=%d\r\n", minnj, cntnj);
  okk=0, err=0;
  jujue=0; // 类内拒绝个数
  cntnn=0;
  for ( i = 0; i < train.nlist; ++i ) {
    int minid=-1;
    for ( j = i+1; j < train.nlist; ++j ) {
      if (0==strncmp(train.flist[i], train.flist[j], 4)) {
        FEAT_T *ft_i = train.feat+(featsize/sizeof(FEAT_T))*i;
        FEAT_T *ft_j = train.feat+(featsize/sizeof(FEAT_T))*j;
        int dis = CPM_Distance( featdim, ft_i, ft_j, 0 );
        if (dis>minnj) {
          jujue++;
        }
        ++cntnn;
      }
    }
    printf("%d/%d\r", i, train.nlist);
    // 是否分对
  }
  logprintf("类内拒绝个数=%d 类内总数=%d\r\n", jujue, cntnn);
  free_facelib(&train);
  return 0;
}
int test_faceRecognition3() {
  int n1=0, n2=0, i, j;
  int TESTNUM=5, TRAINNUM=5;
  int algo = 0;
  int feattype=0;
  int ret=0;
  int train_n, featsize=0;
  FEAT_T *train;
  int dtype = 0;
  int is_dist = 0, bSaveDat=1;
  char** flist;
  int nlist;
  int featdim;
  double thd;
  is_dist = 1;
  
  _chdir("E:\\pub\\bin\\adaboost\\");
  _chdir("D:\\tttt\\NIR_face_dataset\\std\\");
  //_chdir("E:\\pub\\bin\\faceRecog\\图片");
  
  nlist = read_strlist("list.txt", &flist);
  {
    int* id = strlist2id(nlist, flist);
    savedata("idlist.dat", id, nlist*sizeof(int), 0);
    pfree(id);
  }
//typedef float real;
  printf( "计算图片特征...\n" );
  new_sample_set( "list.txt", feattype, &train_n, &featsize, &train, "newfeat.dat" );
  featdim = featsize;
  ASSERT(train_n==nlist);
  for (thd=0.43; thd<0.63; thd+=0.01) {
    FILE* resultfile[2];
    int mindis[2]={100000, 100000};
    int allcnt[2]={0, 0};
    int errcnt[2]={0, 0};
    resultfile[0] = fopen( "result_类间.txt", "w" );
    resultfile[1] = fopen( "result_类内.txt", "w" );
    for ( i = 0; i < train_n; ++i ) {
      for ( j = i+1; j < train_n; ++j ) {
        FEAT_T *ft_i = train+(featsize/sizeof(FEAT_T))*i;
        FEAT_T *ft_j = train+(featsize/sizeof(FEAT_T))*j;
        int dis = CPM_Distance( featdim, ft_i, ft_j, dtype );
        int is_nn = 0==strncmp(flist[i], flist[j], 5); // 是同一类
        if (0==dis) {
          int asdf=0;
          int dis = CPM_Distance( featdim, ft_i, ft_j, dtype );
        }
        if (dis<mindis[is_nn]) {mindis[is_nn] = dis;}
        //#define THD  11000
//#define THD  (0.48*25000)
#define THD  (thd*25000)
        if (is_nn ? dis>THD : dis<THD) {
          errcnt[is_nn]++;
          if (0==is_nn) {
            logprintf("%d %s %s\r\n", (int)dis, flist[i], flist[j]);
          }
        }
        allcnt[is_nn]++;
        fprintf(resultfile[is_nn], "%d,\r\n", (int)dis);
      }
      if (i%10==0) {
        //logprintf("%d, 内间错误=%g, 类内错误=%g\r\n", i, errcnt[0]*1./allcnt[0], errcnt[1]*1./allcnt[1]);
      }
    }
    fclose( resultfile[0] );
    fclose( resultfile[1] );
    //logclear();
    logprintf("内间最小距离=%d, 类内最小距离=%d\n", mindis[0], mindis[1]);
    logprintf("thd=%g, 类间样本被接受=%d, 类内样本被拒绝=%d\r\n", thd, errcnt[0], errcnt[1]);
    logprintf("thd=%g, 类间样本总数%d, 类内样本总数%d\r\n", thd, allcnt[0], allcnt[1]);
    logprintf("thd=%g, 内间错误=%g, 类内错误=%g\r\n", thd, errcnt[0]*1./allcnt[0], errcnt[1]*1./allcnt[1]);
  }
  pfree(train);
  free_strlist1(flist);
  return 0;
}


//#include "macro.h"

// 直方图交集距离
int CPM_Histinter2( int n, const FEAT_T* a, const FEAT_T* b ) {
  int i;
  FEAT_T out = 0;
  for ( i = 0; i < n; ++i ) {
    out += ( a[ i ] < b[ i ] ? a[ i ] : b[ i ] );
  }
  return out;
}

int CPM_AbsSub2( int n, const FEAT_T* a, const FEAT_T* b ) {
  int i;
  int out = 0;
  for ( i = 0; i < n; ++i ) {
    out += ( a[ i ] > b[ i ] ? ( a[ i ] - b[ i ] ) : ( b[ i ] - a[ i ] ) );
  }
  return out;
}

const unsigned char monofilt_block[]={
#include "../monofilt/分块.txt"
  };

int test_monofilt() {
  IplImage *img;  //源图像
  IplImage *img2;  //源图像
  FEAT_T* feat;
  FEAT_T* feat2;
  int di, fl, l, featlen;
  const unsigned char* pp;
  l = sizeof(monofilt_block)/3/4;
  l = sizeof(featurelist4)/4;
  pp = monofilt_block+l*4*2;
  pp = featurelist4;
  featlen = monofilt_feat_impl1(0, 0, l, pp);
  fl=featlen/sizeof(FEAT_T);
  _chdir("D:\\code\\cstd\\src\\monofilt\\6张图片\\212");
  feat=(FEAT_T*)pmalloc(featlen);
  feat2=(FEAT_T*)pmalloc(featlen);
  //img = cvLoadImage1("test_1_100x100.bmp",0);
  img = cvLoadImage1("bbb.bmp",0);
  img2 = cvLoadImage1("2.bmp",0);
  //img = cvLoadImage1("test_1_128x128.bmp",0);
  //img = cvLoadImage1("test_1_166x166.bmp",0);
  
  {utime_start(_start_time);
  monofilt_feat_impl1(img, feat, l, pp);
  //monofilt_feat_impl1(img2, feat2, l, pp);
  {
    int i=0, s=0;
    for (; i<fl; ++i) {
      //s+=feat[i];
    }
    //assert(s==15000);
  }
  //FPRINT1D("3.txt", "%d,\n", fl, feat, 1);
  //FPRINT1D("13.txt", "%d,\n", fl, feat2, 1);
  
  di = CPM_AbsSub2(fl, feat, feat2);
  
  printf("%f\n", utime_elapsed(_start_time));}
  
  pfree(feat);
  pfree(feat2);
  cvReleaseImage(&img);
  cvReleaseImage(&img2);
  return 0;
}
