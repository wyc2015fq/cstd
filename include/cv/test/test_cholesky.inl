
//#include "cv/mat.hpp"
//#include "cv/core/matx.hpp"

//#include "test_img.inl"

//#include "sys/test_thread.inl"
//#include "sys/test_thread_matmul.inl"

int test_c() {
	int aa = 3;
	int bb = -~aa;
  int a = !(1) ??!??! (10);
  char c = 2["ABCD"];
  float b = 6.412355;
  printf("%.*f %c\n",a+3,b,c);
  return 0;
}

#include "cpu.h"
//#include "sys/threadpool.inl"

#include "img/imgio.inl"
//extension
int test_cholesky() {
  //CRect_<int> adsf; minMaxLoc
  void* aa = _alloca(10);
  struct bbb {union {} a;};
  int aa1 = sizeof(bbb);
  int l1 = get_cpu_info()->l1;
  int l2 = get_cpu_info()->l2;
  int l3 = get_cpu_info()->l3;
  //test_thread_matmul();
  //return 0;
  //matx33d aa=matx33d::eye(), bb=matx33d::eye(), cc;
  //cc= aa*(bb);
  //cc.matmul(aa, bb);
  //Img a = Img(aa).t();
	//Img a=Img::zeros(10, 10, CC_64F), b;
	//b = a+-a;

  //aa.at<double>(0) = 0;
  //test_img();
  //return 0;
  //test_cpcode();
  CRNG rng = 10;
  IM_BEGIN(im, 10);
  int n =  5;
  img_t* a = cvSetMat(im++, n, n, CC_32F);
  img_t* b = cvSetMat(im++, n, n, CC_32F);
  img_t* c = cvSetMat(im++, n, n, CC_32F);
  if (1) {
    cvRandArr(&rng, a, CC_RAND_NORMAL, cRealScalar(0), cRealScalar(1) );
    matmul(a, a, a, CC_GEMM_B_T);
    is_CholImpl1 = 1;
    invert(a, b, CC_CHOLESKY);
    matmul(a, b, c);
    double t = cvIsDiag(c);
    printf("err = %lf\n", t);
    ASSERT(t<0.001);
  }
  //cvOutMat(a); cvOutMat(b);  cvOutMat(c);
  {
    int n =  225;
    cvSetMat(a, n, n, CC_32F);
    cvSetMat(b, n, n, CC_32F);
    cvSetMat(b, n, n, CC_32F);
    cvRandArr(&rng, a, CC_RAND_NORMAL, cRealScalar(0), cRealScalar(1) );
    matmul(a, a, a, CC_GEMM_B_T);
    int i, times=10;
    is_CholImpl1 = 0;
    {
      utime_start(_start_time);
      for (i=0; i<times; ++i) {
        invert(a, b, CC_CHOLESKY);
        //cvInvert(a, b);
      }
      printf("%fms\n", 1000*utime_elapsed(_start_time)/times);
    }
    printf("my invert\n");
    is_CholImpl1 = 1;
    {
      utime_start(_start_time);
      for (i=0; i<times; ++i) {
        invert(a, b, CC_CHOLESKY);
      }
      printf("%fms\n", 1000*utime_elapsed(_start_time)/times);
    }
  }
  IM_END(im);
  return 0;
}

