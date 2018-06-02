

typedef struct asm_model_t {
  float* meanshape;
  int npt;
} asm_model_t;

//#include "cv1/meansdv.inl"
int asm_free(asm_model_t* s) {
  FREE(s->meanshape);
  return 0;
}

int asm_train(asm_model_t* s, const vstr_t* flist, img_t* ptslist) {
  int npt = s->npt = ptslist->w/2;
  int i, j, nsamp = ptslist->h;
  img_t im[1] = {0};
  MYREALLOC(s->meanshape, npt*2);
  for (j=0; j<ptslist->w; ++j) {
    s->meanshape[j] = 0;
  }
  for (i=0; i<ptslist->h; ++i) {
    float* pts = img_row(float, ptslist, i);
    for (j=0; j<ptslist->w; ++j) {
      s->meanshape[j] += pts[j];
    }
    imread(flist->v[i].s, 3, 1, im);
    lbf_show_shape(im, pts, npt, _rgb(1,0,0), -1);
  }
  for (j=0; j<ptslist->w; ++j) {
    s->meanshape[j] /= nsamp;
  }
  imfree(im);
  return 0;
}

