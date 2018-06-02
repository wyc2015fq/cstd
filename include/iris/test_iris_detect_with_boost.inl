
int test_iris_detect_with_boost()
{
  FILE* plisttxt;
  int fram, i = 0;
  const char* outdir = "out_270";
  img_t im[1] = {0};
  img_t gry[1] = {0};
  enum {MAXBUF = 1 << 24};
  uchar buf[MAXBUF];
  _chdir("D:/pub/bin/ir/adaboost");
  _chdir("D:/pub/bin/iris/CASIA-IrisV3-Lamp");
  plisttxt = fopen("list.txt", "rb");

  ASSERT(plisttxt);
  //if (NULL==plisttxt) {    return 0;  }
  _mkdir(outdir);

  //for ( fram = 0; fgets(buf, 256, plisttxt)>0; ++fram )
  for (fram = 0; fgets(buf, 256, plisttxt) > 0; ++fram) {
    strtrim(buf, "\n\r");
    imread(buf, 3, 1, im);
    im2gry(im, gry);
    //printf("%10s ", buf);

    if (gry->tt.data) {
      int t;
      irisrecog_t* ir = (irisrecog_t*)buf;

      memset(ir, 0, sizeof(irisrecog_t));
      ir->height = gry->h, ir->width = gry->w, ir->step = gry->s;
      ir->mode = IR_ALL_DETECT;
      memcpy(ir + 1, gry->tt.data, gry->w * gry->h);

      //printf("%10s ", buf);
      {
        utime_start(_start_time);
        iris_detect(gry, ir);
        printf(" %.4f", utime_elapsed(_start_time));
      }
      DrawIris(im->h, im->w, im->tt.data, im->s, 3, ir, CV_RGB(255, 0, 0), 1, 8, 0);
      //DrawIris(im->h, im->w, im->data, im->s, 3, &iris2, CV_RGB(0, 255, 0), 1, 8, 0);

      printf("  %3.f %3.f %3.f", ir->n[0], ir->n[1], ir->n[2]);

      if (0) {
        char buf2[256];
        *strrchr(buf, '.') = 0;
        _snprintf(buf2, 256, "./%s/%s_out.jpg", outdir, buf);
        imwrite2(buf2, im);
      }
      else {
        imshow(im), cvWaitKey(-1);
      }
    }

    printf("\n");
  }

  imfree(im);
  imfree(gry);
  return 0;
}