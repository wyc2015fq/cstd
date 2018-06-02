
int test_blurdetect() {
  img_t im[1] = {0};
  double xx1, xx2;
  _chdir("D:/pub/bin/iris/评价函数");
  imread("1.jpg", 1, 1, im);
  xx1 = blurdetect(im)/256.;
  imread("4.jpg", 1, 1, im);
  xx2 = blurdetect(im)/256.;
  printf("%f %f", xx1, xx2);
  return 0;
}

int test_iris_detect_pic_impl(img_t* im, unsigned char* buf1, int buflen, const char* name) {
  img_t gry[1] = {0};
  char buf12[256];
  //double d=0;
  irisrecog_t* ir = (irisrecog_t*)buf1;
  memcpy2d(im->tt.data+im->s, im->s*2, im->tt.data, im->s*2, im->h/2, im->s);
  
  im2gry(im, gry);
  //savedata("img_640x480.dat", gry->tt.data, gry->h*gry->w);
  
  memset(ir, 0, sizeof(irisrecog_t));
  iris_detect(ir); // 获取featlen一般不会超过 10000 ,现在用的特征==1350
  ir->height = gry->h, ir->width = gry->s;
  ir->img_off = sizeof(irisrecog_t);
  //ir->std_off = sizeof(irisrecog_t) + height*width;
  ir->feat_off = ir->img_off + ir->height * ir->width*2;
  ir->featnum = 20;
  ir->featstep = 1350; // 必须>=特征长度
  ir->lib_off = ir->feat_off + ir->featstep;
  ir->buflen = buflen - ir->lib_off - ir->featnum * ir->featstep;
  ir->buf_off = ir->lib_off + ir->featnum * ir->featstep;
  //ir->buf = gry->tt.data;
  ir->mode = IR_NU_DETECT;
  ir->mode = IR_NUWD_DETECT;
  ir->mode = IR_BOOST_DETECT;
  ir->mode = IR_NUWD_DETECT | IR_STDIMG;
  ir->mode = IR_BOOST_DETECT | IR_NUWD_DETECT | IR_STDIMG;
  ir->mode |= IR_FEATURE;
  ir->mode |= IR_RECOG;
  ir->color_type = T_GRAY;
  //ir->mode = IR_ALL_DETECT;
  memcpy(buf1 + ir->img_off, gry->tt.data, gry->s * gry->h);
  ir->color_type = T_YUYV;
  colorcvt(gry->h, gry->w, gry->tt.data, gry->s, buf1 + ir->img_off, gry->w*2, T_GRAY, ir->color_type);
  //printf("%10s ", buf);
  {
    utime_start(_start_time);
    iris_detect(ir);
    printf(" %.4f", utime_elapsed(_start_time));
  }
  
  //imshow(gry);cvWaitKey(-1);
  if (ir->mode & IR_BOOST_DETECT && ir->rc.w <= 0) {
    printf("\n");
    free(gry);
    return 0;
  }
  
  if (1) {
    printf("  blurrate = %f\n", ir->blurrate/256.);
  }
  if (1 && (ir->mode & IR_BOOST_DETECT)) {
    img_t im2[1] = {0};
    imrefrc(im, ir->rc, im2);
    _snprintf(buf12, 256, "../detectout/%sB.jpg", name);
    //imdraw_circle(im2, im2->w / 2, im2->h / 2, 5, rgb(1, 0, 0), 0, 0);
    imwrite(buf12, im2);
    //imshow(im2);cvWaitKey(-1);
  }
  
  //DrawIris(im->h, im->w, im->data, im->s, 3, &iris2, CV_RGB(0, 255, 0), 1, 8, 0);
  
  //d = dist_iris(&ir, &iris2);
  
  if (1 && (ir->mode & IR_STDIMG)) {
    img_t v_normalize[1] = {0};
    int tt = ir->cnt_zero / 1000;
    if (ir->blurrate>25*256) {
      tt = BOUND(tt, 0, 9);
      IMINIT(v_normalize, IRIS_H, IRIS_W, ir->stdimg, IRIS_W, 1, 1);
      _snprintf(buf12, 256, "./feat/%sF%1d_b=%d.bmp", name, tt, (int)(ir->blurrate/256));
      imwrite(buf12, v_normalize);
    }
  }
  
  if (1) {
    DrawIris(im, ir, CV_RGB(255, 0, 0), 0, 0, 2);
    imshow(im);
    if (ir->blurrate>25*256) {
      _snprintf(buf12, 256, "./out/%sB_b=%d.jpg", name, (int)(ir->blurrate/256));
      imwrite(buf12, im);
      cvWaitKey(10);
    }
  }
  
  if (0) {
    const char* outdir = "out_270";
    DrawIris(im, ir, CV_RGB(255, 0, 0), 0, 0, 2);
    _snprintf(buf12, 256, "./%s/%s.jpg", outdir, name);
    imwrite(buf12, im);
  }
  
  cvWaitKey(-1);
  imfree(gry);
  return 0;
}

int test_iris_detect_pic() {
  img_t im[1] = {0};
  uchar* imgbuf=NULL;
  FILE* pf=NULL;
  enum {MAXBUF = 1 << 22};
  uchar* buf1 = 0;
  int h = 480, w = 640;
  imgbuf = (uchar*)malloc(h*w);
  buf1 = (uchar*)malloc(MAXBUF);
  pf = fopen("E:/pub/bin/iris/CASIA-IrisV3-Lamp/img_640x480.dat", "rb");
  fread(imgbuf, 1, h*w, pf);
  IMINIT(im, h, w, imgbuf, w, 1, 1);
  test_iris_detect_pic_impl(im, buf1, MAXBUF, "name");
  free(buf1);
  free(imgbuf);
  return 0;
}

int test_iris_detect()
{
  FILE* plisttxt;
  int fram, i = 0;
  //int allpic = sizeof(test_result) / sizeof(test_result[0]);
  int testerrfile = 0;
  img_t im[1] = {0};
  img_t gry[1] = {0};
  int test1 = 0;
  char buf[256];
  const char* flist;
  enum {MAXBUF = 1 << 22};
  uchar* buf1 = 0;
  buf1 = (uchar*)malloc(MAXBUF);
  _chdir("E:/pub/bin/iris/detect");
test_iris_detect_pic();
  if (!test1) {
    _chdir("D:/pub/bin/iris/testPic");
  }
  else {
    _chdir("D:/pub/bin/iris/test1pic");
  }

  _chdir("D:/pub/bin/iris/testpic");
  _chdir("D:/pub/bin/iris/DSP摄像头拍的/2001a");
  _chdir("D:/pub/cstd/DSP/dm6446/ttt2/raw");
  _chdir("E:/bak/pub/cstd/DSP/dm6446/ok");
  _chdir("E:/pub/bin/iris/tt");
  _chdir("E:/pub/bin/iris/detect");
  _chdir("E:/pub/bin/iris/CASIA-IrisV3-Lamp");

  //_chdir("D:/pub/bin/iris/adaboost");

  //mkdir("feat");
  //mkdir("mask");
  flist = "err.txt";
  flist = "list.txt";
  plisttxt = fopen(flist, "rb");

  if (plisttxt != 0) {
    testerrfile = fsize(plisttxt);
  }

  //for ( fram = 0; fgets(buf, 256, plisttxt)>0; ++fram )
  for (fram = 0; fgets(buf, 256, plisttxt); ++fram) {
    strtrim(buf, "\n\r");

    if (imread(buf, 3, 1, im)) {
      char* name = strrchr(buf, '\\') ? (strrchr(buf, '\\') + 1) : buf;
      *strrchr(buf, '.') = 0;
      printf("%4d ", fram);
      printf("%10s ", buf);
      test_iris_detect_pic_impl(im, buf1, MAXBUF, name);
    }

    printf("\n");
  }

  free(buf1);
  imfree(im);
  imfree(gry);
  fclose(plisttxt);
  return 0;
}
