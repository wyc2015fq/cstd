
#include "cstd.h"
#include "libfacerecog.h"
#include "img/filters.h"
#include "face/init.inl"
#include "face/pplight_16s.inl"
#include "face/faceRecog_txt.inl"
#include "face/faceRecog_feat.inl"

uchar stdface_100x100[] = {
#include "C://stdface_100x100.inl"
};

int test_facefeat_inldata() {
  lbpex_param_t lp={0};
  short feature[3000] = {0};
  int buflen = 1<<20;
  uchar* gbuf = (uchar*)malloc(buflen);
  lp.height = STDIMGH, lp.width = STDIMGW, lp.src = stdface_100x100, lp.srcstep = STDIMGW, lp.srccn = 1;
  lp.feature = (FEAT_T*)feature;//, lp.featurelen = p->featurelen;
  lp.select_type = 2, lp.fillborder = 1, lp.do_pp = 1;
  lp.buf = gbuf, lp.buflen = buflen;
  lbpex_process(&lp);
  free(gbuf);
  return 0;
}
