
//#include "win.h"
//#include "debug.h"
//#include "gmalloc.h"
#include "cstd.h"

#include "str.h"
#include "cfile.h"
#include <direct.h>
//#include "sys/findfile.h"
//#include "fmtimg/fmtimg.inl"
#if 1
#include "img/imgio.inl"
#include "ui/window.inl"
#include "draw/imdraw.inl"
#endif
//#include "math/math.def"

#include "iris/iris_interface.inl"
#include "test_omkernel.inl"
#include "test_iris_recog.inl"

//#include "iris/levelOM.inl"
//#include "test_iris_base.inl"
//#include "test_ransac.inl"

#if 1
#include "test_iris_detect.inl"
//#include "test_popcount.inl"
#include "test_findrepeatimg.inl"
#include "test_iris_detect_arm.inl"
#else
#include "test_canny.inl"
#include "test_levelom.inl"
#include "test_glassRemoval.inl"
#include "test_Irisnormalize.inl"
#include "test_Daugmannormalized.inl"
#include "test_iris_detect_with_boost.inl"

//#include "test_solve.inl"
//#include "src/fmtimg/test_color_quantization.inl"
#endif

#include "test_save_net_pic.inl"


int test_makeom() {
  int featlen = 0;
  featlen = makeom_bit_select(0, 0);
  featlen = makeom_bit(0, 0);
  featlen = makeom_i1(0, 0, 0, 0);
  featlen = makeom_bit_block_select(0, 0, 0);
  return 0;
}

enum { GBUFLEN2 = 2<<24 };
char g_buf[GBUFLEN2];

int test_feat_dat() {
  buf_t bf[1] = {0};
  buf_t bf1[1] = {0};
  int i, j;
  int featlen = 5376;
  int featstep = 5404;
  _chdir("D:\\pub\\cstd\\DSP\\dm6446");
  bfsetsize(bf, featstep*10*3);
  buf_load("D:\\pub\\cstd\\DSP\\dm6446\\4001.dat", bf1);
  memcpy(bf->data, bf1->data, featstep*10);
  buf_load("D:\\pub\\cstd\\DSP\\dm6446\\2001.dat", bf1);
  i = HUFF_Decode(bf1->data, bf1->len, bf->data + 10*featstep, bf->len);
  buf_load("D:\\pub\\cstd\\DSP\\dm6446\\4002.dat", bf1);
  i = HUFF_Decode(bf1->data, bf1->len, bf->data + 20*featstep, bf->len);
  for (i=0; i<5; ++i) {
    for (j=0; j<20; ++j) {
      //printf("%3d ", bf->data[i*5404 + j]);
    }
    //printf("\n");
  }
  for (i = 0; i < 30; ++i) {
    uchar* fa = bf->data + i*featstep;
    for (j=i+1; j<30; ++j) {
      uchar* fb = bf->data + j*featstep;
      int dis = count_xor(featlen, fa, fb);
      logprintf("%4d %3d %3d %3d\n", dis, i, j, ((i/10)==(j/10)));
    }
  }
  bffree(bf1);
  bffree(bf);
  return 0;
}

int test_iris()
{
  GMALLOC_BEGIN(g_buf, GBUFLEN2);
#if 1
  test_iris_detect();
#else
  test_iris_detect_arm();
  test_feat_dat();
  test_feat_dat();
  test_iris_recognition_2pic2();
  test_iris_recognition_rate();
  test_iris_recognition_2pic();
  test_blurdetect();
  test_save_net_pic();
  test_findrepeatimg();
  test_iris_detect_with_boost();
  test_iris_recognition_2pic();
  test_popcount();
  test_omkernel_list();
  test_omkernel();
  test_omkernel();
  test_levelOM();
  MEM_CHECK_BEGIN();
  test_glassRemoval();
  test_canny();
  test_color_quantization();
  test_dib();
  test_Daugmannormalized();
  MEM_CHECK_END();
#endif
  GMALLOC_END();
  return 0;
}
