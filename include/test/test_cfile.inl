
#ifdef TEST
int test_dffile() {
  file_t f[1] = {0};
  FILE* pf;
  img_t im[10] = {0};
  img_t* im1 = im+1;
  int i, len;
  imsetsize(im, 21, 33, 3, 11);
  len = im->h*im->s*im->f;
  for (i=0; i<len; ++i) {
    im->tt.data[i] = rand();
  }
  if (fstream_init(f, pf = fopen("D://aaa.df", "wb"))) {
    df_saveimage(f, 0, im);
    fclose(pf);
  }
  if (fstream_init(f, pf = fopen("D://aaa.df", "rb"))) {
    df_loadimage(f, 0, im1);
    fclose(pf);
  }
  ASSERT(0==memcmp(im->tt.data, im1->tt.data, len));
  imfrees(im, 10);
  return 0;
}

int test_buf_serialization() {
  int i=0, i1;
  float f = 3.14, f1;
  double d = 4.222, d1;
  FPOINT pt[2] = {1,2,3,4};
  FPOINT* p = pt, *p1;
  int l = sizeof(pt), l1;
  int ipos;
  char* buf = NULL;
  ipos = buf_serialization(NULL, 0, 1, "ifdp", &i, &f, &d, &l, &p);
  buf = (char*)sys_malloc(ipos);
  ipos = buf_serialization(buf, 0, 1, "ifdp", &i, &f, &d, &l, &p);
  ipos = buf_serialization(buf, 0, 0, "ifdp", &i1, &f1, &d1, &l1, &p1);
  ASSERT(i==i1 && f==f1 && d==d1 && l==l1 && 0==memcmp(p, p1, l1));
  sys_free(buf);
  return 0;
}
#endif // TEST