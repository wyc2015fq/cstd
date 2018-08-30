
#ifndef _TEST_FACE_PIC_IMPL_INL_
#define _TEST_FACE_PIC_IMPL_INL_
#include "test_faceRecog.h"
//#include "cfile.h"
//#include "draw/imdraw.inl"
int g_mode=-1;

buf_t g_bf[1] = {0};

int test_faceRecog_pic_impl(img_t* im, int is_trans, const char* name) {
  facerecog_param_t p={0};
  int id=0, ret=0, fram=0;
  unsigned char *rgb=0;
  char ch = 0;
  INIT_CAS( vis_nesting_face0701 );
  INIT_CAS( vis_nesting_face20110713 );
  INIT_CAS( NIREYEWithoutGlass );
  INIT_CAS( NIRAllEYE );
  INIT_CAS( NIR_LeftEyeWithGlass20110717 );
  INIT_CAS( NIR_RightEyeWithGlass20110714 );
  //INIT_CAS( NIREYELeftWithGlass );
  //INIT_CAS( NIREYERightWithGlass );
#ifdef WIN32
  //INIT_CAS( VisAllEye_nesting );
  //INIT_CAS( NIRDoubleEYE );
  //INIT_CAS( NIREYELeftWithGlass );
  //INIT_CAS( NIREYERightWithGlass );
#endif

  //const HAARCASCADE* ca = (const HAARCASCADE*)pvis_nesting_face20110713;
  unsigned char* img = im->tt.data;
  int i=0, h=im->h, w=im->w, al=im->s, ai=im->c;
  int gl = w, cl=w*3, ci=3;
#ifdef WIN32
  sys_chdir("../../../../pub/bin/lbf_face_alignment");
#endif
  //BUF_LOAD("train_model_new_fix16.dat", bf);
  if (g_mode & FR_FACE_ALIGNMENT) {
    if (NULL==g_bf->data) {
      //BUF_LOAD("train_model_new.dat", g_bf);
      //BUF_LOAD("train_model_new_fix16.dat", g_bf);
    }
  }
#ifdef _LBF_FACE_ALIGNMENT_INL_
#endif
#define pp(_n, _d)  printf(#_n "=%" #_d "\n", _n)
  //printf("test_faceRecog_pic begin\n");
  //imwrite2("./bbb.bmp", im);
  if (NULL==p.buf) {
    p.height = h, p.width = w, p.datastep = al, p.datacn = ai, p.mask = 0, p.maskstep = 0;
    p.stepxy = 1, p.mincnt = 5, p.fastmode = 0, p.maxoutlen = 150, p.outimgcode = T_BGR;
    p.casnum = 1, p.cas[ 0 ] = pvis_nesting_face0701;
    p.casnum = 1, p.cas[ 0 ] = pvis_nesting_face20110713;
    p.leyecasnum = 3, p.leyecas[ 0 ] = pNIRAllEYE, p.leyecas[ 1 ] = pNIREYEWithoutGlass, p.leyecas[ 2 ] = pNIR_LeftEyeWithGlass20110717  ;
    p.reyecasnum = 3, p.reyecas[ 0 ] = pNIRAllEYE, p.reyecas[ 1 ] = pNIREYEWithoutGlass, p.reyecas[ 2 ] = pNIR_RightEyeWithGlass20110714 ;
    //p.leyecasnum = 3, p.leyecas[ 0 ] = pNIRAllEYE, p.leyecas[ 1 ] = pNIREYEWithoutGlass, p.leyecas[ 2 ] = NIREYELeftWithGlass  ;
    //p.reyecasnum = 3, p.reyecas[ 0 ] = pNIRAllEYE, p.reyecas[ 1 ] = pNIREYEWithoutGlass, p.reyecas[ 2 ] = NIREYERightWithGlass ;
    //p.leyecas[ 3 ] = pNIRDoubleEYE;
#ifdef WIN32
    p.leyecasnum = 3, p.leyecas[ 0 ] = pNIRAllEYE, p.leyecas[ 1 ] = pNIREYEWithoutGlass, p.leyecas[ 2 ] = NIREYELeftWithGlass ;
    p.reyecasnum = 3, p.reyecas[ 0 ] = pNIRAllEYE, p.reyecas[ 1 ] = pNIREYEWithoutGlass, p.reyecas[ 2 ] = NIREYERightWithGlass;

#endif
  if (g_mode & FR_FACE_ALIGNMENT) {
    if (g_bf->data) {
      p.leyecas[ 0 ] = g_bf->data;
    }
  }
  p.isc = (im->w>400) ? FR_F2I( 0.1 ) : FR_F2I( 1 );
    p.iss = FR_F2I( 1.1 ), p.ithd = FR_F2I( 0.8 );
    p.issmin = FR_F2I( 1. ), p.issmax = FR_F2I( 100. );
    p.roi.w = p.roi.h = 0;
    p.ithd=80, p.issmin=100, p.issmax=10000;
    p.datacode = 3==ai ? T_BGR : T_GRAY;
    if (2==ai) {
      p.datacode=T_UYVY;
    }
    p.mode = FR_FACE_DETECT|FR_EYES_DETECT|FR_FACE_STD|FR_FACE_FEATURE|FR_RECOG_TEST|FR_CVTCOLOR;
    p.mode |= FR_FEAT_23;
    p.mode = FR_FACE_DETECT|FR_EYES_DETECT|FR_CVTCOLOR;
    if (g_mode!=-1) {
      p.mode=g_mode;
    }
    //p.mode = 7;
    //if (is_trans) p.mode |= FR_TRANSPOSE;
    p.transopt = is_trans;
    //p.mode |= FR_ODDFIX;
    facerecog_memsize( &p );
    p.data = DSP_MALLOC( unsigned char, h * w * ai*2 );
    p.stdface = DSP_MALLOC( unsigned char, STDIMGH * STDIMGW );
    rgb = p.outimg = DSP_MALLOC( unsigned char, h*w*3 );
    p.feature = DSP_MALLOC( FEAT_T, p.featurelen );
    p.buf = DSP_MALLOC( unsigned char, p.buflen );
    if (p.mode&FR_FACE_RECOG) {
      char* featlib = NULL;
      featnum=100*20;
      p.featnum=featnum, p.featstep=featstep;
      featlib = (char*)MALLOC(char, featnum*featstep);
      p.featlib = (FEAT_T*)featlib;
      featstep = sizeof(SPFeatInfo);
      //featnum = loadfile("D:/facedata/feat.dat", featlib, featnum*featstep, 0)/featstep;
    }
  }
  memcpy((void*)p.data, img, h*w*ai);
  //TEST_BEG;
  //for (i=0; i<20; ++i) 
  {
    utime_start(_start_time2);
  //p.isc = (im->w>400) ? FR_F2I( 0.15 ) : FR_F2I( 20.*4/(100+i*10) );
    face_recog_process(&p);
    printf("%f\n", utime_elapsed(_start_time2));
  }
  if (p.featlib) {
    FREE(p.featlib);
  }
  //cvShowImage(".\\aaa.bmp", h, w, img, al, ai); cvWaitKey(-1);
  //printf( "(%3d,%3d,%3d,%3d)(%3d,%3d,%3d,%3d)(%3d,%3d,%3d,%3d) ", p.face.x, p.face.y, p.face.w, p.face.h,  p.leye.x, p.leye.y, p.leye.w, p.leye.h, p.reye.x, p.reye.y, p.reye.w, p.reye.h );
  //printf("face.count = %d\n", p.face.count);
  //TEST_END;
  {
    static int facen=0, leyen=0, reyen=0;
    facen+=p.face.w&&1;
    leyen+=p.face.w&&p.leye.w;
    reyen+=p.face.w&&p.leye.w&&p.reye.w;
    //if (facen) printf( "L=%3d%% R=%3d%% ", leyen*100/facen, reyen*100/facen );
  }
  if (0) {
    p.face.x = 208 , p.face.y = 150 , p.face.w = 200 , p.face.h = 200 , 
      p.deye.x = 228 , p.deye.y = 150 , p.deye.w = 80 , p.deye.h = 200 ,
      p.leye.x = 250 , p.leye.y = 283 , p.leye.w = 22 , p.leye.h = 44 ,
      p.reye.x = 248 , p.reye.y = 175 , p.reye.w = 22 , p.reye.h = 44;
  }
  ret = p.face.w>0;
#if 0
  if (name && p.face.w>0 && p.leye.w>0 && p.reye.w>0) {
    char buf[256];
    _snprintf(buf, 256, "%s_std.bmp", name);
    imwrite(buf, STDIMGH, STDIMGW, p.stdface, STDIMGW, 1);
  }
#endif
  printf( "(%3d,%3d,%3d,%3d)(%3d,%3d,%3d,%3d)(%3d,%3d,%3d,%3d)\n",
    p.face.x, p.face.y, p.face.w, p.face.h, 
    p.leye.x, p.leye.y, p.leye.w, p.leye.h, 
    p.reye.x, p.reye.y, p.reye.w, p.reye.h );
  if ( ret ) {
    img_t imrgb[1]={0};
#ifdef _IMDRAW_INL_
    XRECT re[3];
    IPOINT center1, center2;
    //rgb = im->tt.data;
    re[0]=p.face, re[1]=p.leye, re[2]=p.reye;
    //IM2IM( STDIMGH, STDIMGW, p.stdface, STDIMGW, 1, rgb, cl, ci );
    IMINIT(imrgb, h, w, rgb, cl, ci, 1);
    DrawRectangle2( imrgb, p.face, CC_RGB( 255, 0, 0 ), 3 );
    DrawRectangle2( imrgb, p.deye, CC_RGB( 255, 0, 0 ), 1 );
    DrawRectangle2( imrgb, p.leye, CC_RGB( 255, 0, 0 ), 1 );
    DrawRectangle2( imrgb, p.reye, CC_RGB( 255, 0, 0 ), 1 );
    //FixCascade(pNIRDoubleEYE);
    //printf("pNIRDoubleEYE %d %d\n", ((HAARCASCADE*)pNIRDoubleEYE)->w, CAISNESTING(((HAARCASCADE*)pNIRDoubleEYE)->ih, ((HAARCASCADE*)pNIRDoubleEYE)->stage) );

    if (0) {
      static int iii=0;
      static int aaa=0;
      static int bbb=0;
      //char buf[256];
      //_snprintf(buf, 256, "F:\\新建文件夹\\新建文件夹\\新建文件夹\\戴眼镜\\aaa\\%d.bmp", iii++);
      //imwrite(buf, imrgb);
      aaa+=!!p.face.w;
      bbb+=!!p.deye.w;
      printf("\nddd=%d", 100*bbb/aaa);
    }
    center1 = iPOINT(p.leye.x + p.leye.w / 2, p.leye.y + p.leye.h / 2);
    center2 = iPOINT(p.reye.x + p.reye.w / 2, p.reye.y + p.reye.h / 2);
    if (p.leye.w) {
      imdraw_circle1( imrgb, 0, center1.x, center1.y, 3, CC_RGB( 255, 255, 0 ), 0 );
    }
    if (p.reye.w) {
      imdraw_circle1( imrgb, 0, center2.x, center2.y, 3, CC_RGB( 255, 255, 0 ), 0 );
    }
    if ( center1.x && center2.x ) {
      //DrawLine( imrgb, center1, center2, CC_RGB( 255, 255, 0 ), 3, 0, 0 );
    }
#endif
  }
  //memcpy((void*)rgb, img, h*w*ai);
  //memset(rgb, 0, h*w*3);

  //cvShowImage("./ccc.bmp", imrgb); cvWaitKey(-1);
  if (1) {
    static int id=1;
    char buf[1024];
    img_t imrgb[1]={0};
    if (NULL==name) name="face";
    //_mkdir("out");
    if (0) {
      snprintf(buf, 1024, ".\\out\\%s_%d.jpg", name, id), ++id;
    } else {
      snprintf(buf, 1024, ".\\out\\%s_%d_(%3d,%3d,%3d,%3d)(%3d,%3d,%3d,%3d)(%3d,%3d,%3d,%3d).jpg", name, id,
        p.face.x, p.face.y, p.face.w, p.face.h, 
        p.leye.x, p.leye.y, p.leye.w, p.leye.h, 
        p.reye.x, p.reye.y, p.reye.w, p.reye.h ), ++id;
    }
    if (0) {
      const char* ss = strrchr(name, '_')+1;
      sscanf(ss, "(%3d,%3d,%3d,%3d)(%3d,%3d,%3d,%3d)(%3d,%3d,%3d,%3d)",
        &p.face.x, &p.face.y, &p.face.w, &p.face.h, 
        &p.leye.x, &p.leye.y, &p.leye.w, &p.leye.h, 
        &p.reye.x, &p.reye.y, &p.reye.w, &p.reye.h );
      printf( "(%3d,%3d,%3d,%3d)(%3d,%3d,%3d,%3d)(%3d,%3d,%3d,%3d)\n",
        p.face.x, p.face.y, p.face.w, p.face.h, 
        p.leye.x, p.leye.y, p.leye.w, p.leye.h, 
        p.reye.x, p.reye.y, p.reye.w, p.reye.h );
      IMINIT(imrgb, h, w, rgb, cl, ci, 1);
#ifdef _IMDRAW_INL_
      DrawRectangle2( imrgb, p.face, CC_RGB( 0, 255, 0 ), 3 );
      //DrawRectangle2( imrgb, p.deye, CC_RGB( 0, 255, 0 ), 1 );
      DrawRectangle2( imrgb, p.leye, CC_RGB( 0, 255, 0 ), 1 );
      DrawRectangle2( imrgb, p.reye, CC_RGB( 0, 255, 0 ), 1 );
#endif
    }
    //imwrite(buf, imrgb);
  }
  UNINIT_CAS( vis_nesting_face0701 );
  UNINIT_CAS( vis_nesting_face20110713 );
  UNINIT_CAS( NIREYEWithoutGlass );
  UNINIT_CAS( NIRAllEYE );
  UNINIT_CAS( NIR_LeftEyeWithGlass20110717 );
  UNINIT_CAS( NIR_RightEyeWithGlass20110714 );

  //UNINIT_CAS( NIREYELeftWithGlass );
  //UNINIT_CAS( NIREYERightWithGlass );

  //printf("test_faceRecog_pic end\n");
  DSP_FREE( p.buf );
  DSP_FREE( p.feature );
  DSP_FREE( p.stdface );
  DSP_FREE( p.data );
  DSP_FREE( p.outimg );
  printf("\n");
  return ret;
}

#endif // _TEST_FACE_PIC_IMPL_INL_
