

#include "face.inl"
#include "test_face_pic_impl.inl"
#include "pic/pic.inl"
//#include "img/imgio.inl"

//#include "imgui/imui/imui.h"
//#include "imgui/imui/imui.inl"
//#include "imgui/imui/softgc.inl"

//void SWIG_CSharpException(int code, const char *msg);

//#include "test_facdstd.inl"
//#pragma comment(lib, "gdcmsharpglue.lib")
//#include "test_facefeat_inldata.inl"

int test_face_lena() {
  //img_t* im = imread2( "F:/pub/bin/FaceDetect/FaceImage/030.bmp", 1 );
  facerecog_param_t p={0};
  //SWIG_CSharpException(1, "");
  int id=0, ret=0, fram=0, trans=0;
  unsigned char *rgb=0;
  char ch = 0;
  img_t im[1]={0};
  img_t im2[1]={0};
  //test_facefeat_inldata();
#if 0
  {
    img_t im[1] = {0};
    int i;
    FILE* pf=fopen("1.txt", "wb");
    img_t* im2=imresize(im, 0.25);
    imread2("D:\\pub\\bin\\faceRecog\\asdf\\1.bmp", 1);
    //imwrite2("./bbb.bmp", im2);
    for (i=0; i<im2->height*im2->step; ++i) {
      fprintf(pf, "%d,\n",im2->data[i]);
    }
    fclose(pf);
    imfree(im);
    imfree(im2);
  }
#endif
#if 0
  {
    char* name1="C:\\asdf\\ÈËÁ³¼ì²âÍ¼Æ¬\\out\\2008_03_28_11_35_26_328.bmp";
    char* name2="D:\\pub\\bin\\faceRecog\\asdf\\2.bmp";
    img_t* im=imread2(name2, 1);
    FILE* pf=fopen("1.dat", "wb");
    img_t* im2=imcvt(im, T_GRAY, T_UYVY);
    fwrite(im2->data, im2->height*im2->step, 1, pf);
    imfree(im);
    imfree(im2);
    fclose(pf);
  }
#endif
#if 1
  printf("lena2\n");
  IMINIT(im, 144, 180, gry_180x144, 180, 1, 1), trans=1;
  IMINIT(im, 128, 128, lena_128x128, 128, 1, 1), trans=0;
  //imref(im, 0, 20, 100, 100, im);
  //imshow(im);cvWaitKey(-1);
  //imread("gry.bmp", 1, 1, &im);
#endif

#if 0
  {
    char* name;
#ifdef WIN32
    sys_chdir("E:/pub/aaa/testc");
    sys_chdir("E:/pub/bin/face/detect/ÌÆ¸ë");
#endif
    name = "16.bmp";
    imread(name, 1, 1, im2);
    *im = *im2;
    trans=1;
  }
#endif
#if 0
  //im.data = gry_360x288, im.height = 288, im.width = 360, im.step = im.width, im.cn = 1;
  IMINIT(im, 144, 180, gry_180x144, 180, 1, 1), trans=1;
  
  {
    FILE* pf=fopen("1.dat", "rb");
    if (NULL==pf) {
      printf("NULL==pf\n");
      return 0;
    }
    im.height = 640, im.width = 480, im.cn = 2; trans=0;
    im.height = 576, im.width = 720, im.cn = 2; trans=1;
    printf("im height=%d width=%d channels=%d trans=%d\n", im.height, im.width, im.cn, trans);
    im.step = im.width*im.cn;
    im.data=(char*)malloc(im.height*im.step);
    fread(im.data, im.height*im.step, 1, pf);
    fclose(pf);
    //imwrite2("./bbb.bmp", pim);
  }
#endif
  face_recog_open();
#define TEST_FACERECOG_LENA2(_MODE) {utime_start(_start_time); printf("mode=%s\n", #_MODE ); g_mode=_MODE; test_faceRecog_pic_impl(im, trans, 0); printf("alltime = %f\n", utime_elapsed(_start_time));}
  
  if (0) {
    int xywh[12] = {0};
    //face_detect_defaut(im->h, im->w, im->tt.data, im->s, im->c, "gray", trans, 1, xywh);
    printf("%d %d\n", xywh[0], xywh[1]);
  }
  //TEST_FACERECOG_LENA2(FR_FACE_DETECT|FR_EYES_DETECT|FR_FACE_STD|FR_FACE_FEATURE);
#if 0
  if (0) {
    test_faceRecog_pic_impl(im, trans, 0);
  } else {
    findinfo_t si[1] = {0};
    fileinfo_t fi[1] = {0};
    for (;find_next_file(si, "./", "*.bmp", fi, 0);) {
      if (imread(fi->name, 1, 1, im2)) {
        *im = *im2;
        printf("%s\n", fi->name);
        test_faceRecog_pic_impl(im, trans, 0);
      }
    }
  }
#else
  TEST_FACERECOG_LENA2(FR_FACE_DETECT);
  TEST_FACERECOG_LENA2(FR_FACE_DETECT|FR_EYES_DETECT);
  TEST_FACERECOG_LENA2(FR_FACE_DETECT|FR_EYES_DETECT|FR_FACE_STD);
  TEST_FACERECOG_LENA2(FR_FACE_DETECT|FR_EYES_DETECT|FR_FACE_STD|FR_FACE_FEATURE);
  TEST_FACERECOG_LENA2(FR_FACE_DETECT|FR_EYES_DETECT|FR_FACE_STD|FR_FACE_FEATURE|FR_RECOG_TEST);
  TEST_FACERECOG_LENA2(FR_FACE_DETECT|FR_EYES_DETECT|FR_FACE_STD|FR_FACE_FEATURE|FR_FACE_RECOG);
  //TEST_FACERECOG_LENA2(FR_FACE_DETECT|FR_EYES_DETECT|FR_FACE_STD|FR_FACE_FEATURE|FR_RECOG_TEST|FR_CVTCOLOR);
#endif
  face_recog_close();
  imfree(im2);
  //free(im.data);
  return ret;
}

