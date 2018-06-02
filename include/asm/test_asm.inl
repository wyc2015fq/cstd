
//#include "sys/cpu.inl"

#include "cstd.h"
#include "cfile.h"
#include "test_helen.inl"
#include "img/imgio.inl"
#include "draw/imdraw.inl"

int lbf_show_shape(const img_t* im, const float* shape, int npt, COLOR clr, int delay) {
  img_t im2[1] = {0};
  int i;
  imclone2(im, im2);
  for (i=0; i<npt; ++i) {
    double x = shape[i];
    double y = shape[i+npt];
    imdrawaa_circle(im2, NULL, x, y, 2, clr, 0, 0);
  }
  //imdraw_dpoint(im2, 0, pts, npt, clr);
  //im_trans(im2, im2, ISROT3);
  imshow(im2);cvWaitKey(delay);
  imfree(im2);
  return 0;
}
#include "asm.inl"

int test_asm() {
  vstr_t flist[1] = {0};
  img_t ptslist[1] = {0};
  asm_model_t s[1] = {0};
  test_helen_loaddata(flist, ptslist);
  
  //test_helen_pre_treated();
  //test_cpu();
  asm_train(s, flist, ptslist);
  strv_free(flist);
  imfree(ptslist);
  asm_free(s);
  return 0;
}

