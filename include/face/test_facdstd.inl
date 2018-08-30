
#include "face_recog.inl"

int test_facdstd() {
  vstr_t sv[1] = {0};
  const char* path = "D:/facedata";
  const char* outpath = "D:/facedata/stdface";
  int i, slen = strlen(path);
  char outfilename[256];
  img_t im[1] = {0};
  uchar stdface[100*100];
  SPFeatInfo feat[1];
  sys_chdir(path);
  vstr_load("list.txt", sv);
  sys_mkdir(outpath);
  {
    FILE* pf = fopen("feat.dat", "wb");
    for (i = 0; i < sv->n; ++i) {
      char* pfnameext = NULL;
      _snprintf(outfilename, countof(outfilename), "%s/%s", outpath, pfnameext = (sv->v[i].s + slen+1));
      if (imread(sv->v[i].s, 1, 1, im)) {
        int ret = face_recog(im->h, im->w, im->tt.data, im->s, 1, "gray", "df", 0, 0.2, NULL, stdface, feat->m_Feature, NULL, 0, 0, NULL, NULL);
        if (ret) {
          img_t im1[1] = {0};
          int len0 = strlen(outfilename);
          int len1 = cstr_rchr(outfilename, len0, '\\') - outfilename;
          int len2 = cstr_rchr(outfilename, len1, '\\') - outfilename;
          IMINIT(im1, 100, 100, stdface, 100, 1, 1);
          cstr_cpy(feat->m_PID.m_ID, ID_LEN, outfilename+len2+1, len1-len2-1);
          //imshow(im1); cvWaitKey(0);
          mkdirs(outfilename, len1);
          imwrite(outfilename, im1);
          fwrite(feat, sizeof(feat), 1, pf);
        }
      }
      printf("%s\n", pfnameext);
    }
    fclose(pf);
  }
  strv_free(sv);
  return 0;
}
