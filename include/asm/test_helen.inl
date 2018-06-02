
int text_merge(const char* ifn, const char* ofn) {
  vstr_t sv[3] = {0};
  vstr_t* sv1 = sv+1;
  vstr_t* sv2 = sv+2;
  int i;
  vstr_load(ifn, sv);
  strv_setsize(sv2, sv->n);
  for (i=0; i<sv->n; ++i) {
    str_t* s1 = sv2->v+i;
    vstr_load(sv->v[i].s, sv1);
    strv_merge(sv1, s1, ",");
    str_delchr(s1, ' ');
    //printf("%s\n", sv2->v[i].s);
  }
  strv_save(ofn, sv2);
  strv_frees(sv, 3);
  return 0;
}

int test_helen_pre_treated() {
  sys_chdir("F:/dataset/face/helen/annotation");
  text_merge("list.txt", "out.txt");
  return 0;
}

int test_helen_loaddata(vstr_t* flist, img_t* im) {
  sys_chdir("F:/dataset/face/helen/helen");
  vstr_t sv[1] = {0};
  vstr_load("annotation.txt", sv);
  //imdraw_dpoint(im, 0, pts, npt, clr);
  int i, j, nsamp;
  enum { npt = 194 };
  nsamp = MIN(sv->n, 10);
  imsetsize_f4(im, nsamp, npt*2, 1, 1);
  strv_setsize(flist, nsamp);
  for (i=0; i<nsamp; ++i) {
    char* s = sv->v[i].s;
    int l = sv->v[i].l;
    str_t s1[1] = {0};
    int k = cstr_split(s, 0, l, ",", " ", 0, s1 );
    float* pts = img_row(float, im, i);
    str_cat2(flist->v+i, *s1, STR1(".jpg"));
    //printf("%s\n", flist->v[i].s);
    for (j=0; j<npt && k<l; ) {
      str_t sx[1] = {0};
      str_t sy[1] = {0};
      k = cstr_split(s, k, l, ",", " ", 0, sx);
      k = cstr_split(s, k, l, ",", " ", 0, sy);
      pts[j] = atof(sx->s);
      pts[j+npt] = atof(sy->s);
      ++j;
    }
  }
  strv_free(sv);
  return 0;
}