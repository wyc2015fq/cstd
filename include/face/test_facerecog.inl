
/*

  #include "pic/pic.inl"
  
int test_face_recog() {
    int w = 360, h=288;
    int xywh[12] = {0};
    face_recog(h, w, gry_360x288, w, 1, "gray", "det", 1, 1, xywh);
    printf("face %d %d %d %d\n", xywh[0], xywh[1], xywh[2], xywh[3]);
    return 0;
}

*/

#include "face_recog.inl"

#if 0
int test_facerecog() {
  faceapp_t fa[1] = {0};
  facerecog_param_t fp[1] = {0};
  enum {MAXIM=500};
  img_t im[MAXIM] = {0};
  int i, n;
  DPOINT pts[MAXIM*68] = {0};
  buf_t bf[1] = {0};
  double pt[136] = {0};
  font_t fo[1] = {0};
  //double error_of_image;
  int npt = 68;
  img_t imgTe_gray[1] = {0};
  fa->mode = FR_FACE_REC;
  //int adf = test_get_sp();
  _chdir("D:/pub/bin/lbf_face_alignment/test/LFPW");
  n = loadimglist("Path_Images.txt", MAXIM, im, 3);
  font_init_memraster(fo, "宋体", zimo_songti_12);
  for (i=0; i<n; ++i) {
    img_t* imi = im+i;
    faceapp_run(fa, fp, imi, 0);
    faceapp_draw(imi, 0, imi, fa, fp, fo);
    imshow(imi);cvWaitKey(-1);
  }
  return 0;
}
#endif

#if 0
int test_facerecog() {
  img_t im[1] = {0};
  int i, n;
  cap_t cap[1] = {0};
  char buf[256] = {0};
  char outname[256] = {0};
  char* username=0;
  int ch;
  SPFeatInfo* lib = NULL;
  int libsize = 1000; //
  int libused = 0;
  int userid=0, ret=0;
  MYREALLOC(lib, libsize);
  n = cap_num(CAP_EROAD);
  printf("按r注册\n按c清除所有\n");
  if (n<=0) {
    printf("没有专用摄像头!");
    getchar();
    return 0;
  }
  cap_open(cap, 0, CAP_EROAD);
  for (i=0; 1; ++i) {
    double ss=0;
    cap_getframe(cap, im, T_RGB);
    ret = face_recog_run(im, "rgb", NULL, lib, libsize, &libused, username, outname, &ss, 0.46, im, NULL);
    if(-2==ret || -3==ret) {
      userid++;
      username = NULL;
    }
    imshow(im);
    ch = cvWaitKey(50);
    ch = tolower(ch);
    if ('q'==ch) {
      break;
    }
    switch(ch) {
    case 'r':
      _snprintf(buf, 256, "ID%06d", userid);
      username = buf;
      break;
    case 'c':
      libused = 0;
      break;
    }
  }
  cap_close(cap);
  imfree(im);
  return 0;
}
#endif
