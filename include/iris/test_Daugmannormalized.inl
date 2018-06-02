
int test_Daugmannormalized()
{
  irisrecog_t iris[1] = {
    104.0000,  96.0000,   25.2872,  104.0000,  108.0000,   84.8980,
    0.0017,   -0.3712,   82.5988,
    -0.0011,    0.2573,  123.6591,
  };
  int z_top[1000];
  int z_bot[1000];
  int ROI_col;
  img_t im[1] = {0};
  img_t gry[1] = {0};
  img_t mask_iris[1] = {0};
  img_t v_normalize[1] = {0};
  img_t mask_normalize[1] = {0};
  const char* fname = 0;
  _chdir("D:/pub/bin/iris/testPic");
  fname = "S2409R04.jpg";

  if (0) {
    img_t im[1] = {0};
    imread(fname, 3, 1, im);
    DrawIris(im->h, im->w, im->tt.data, im->s, im->c, iris, CV_RGB(255, 0, 0), 1, 8, 0);
    imshow(im);
    cvWaitKey(-1);
    return 0;
  }

  imread(fname, 1, 1, gry);
  ROI_col = gry->w;
  //v_normalize = Daugmannormalized(&nrr, &wrr, Iris);
  get_y(ROI_col, iris->u, z_top);
  get_y(ROI_col, iris->d, z_bot);
  imclone(gry, mask_iris);
  iris_detect_show(mask_iris, z_top, z_bot, iris->n, iris->w);
  Daugmannormalized(iris->n, iris->w, gry, v_normalize);
  Daugmannormalized(iris->n, iris->w, mask_iris, mask_normalize);

  if (0) {
    //imwrite2(buf12, v_normalize);
    //imwrite2(buf12, mask_normalize);
  }
  else {
    imshow(gry);
    imshow(mask_iris);
    imshow(v_normalize);
    imshow(mask_normalize);
    cvWaitKey(-1);
  }

  imfree(gry);
  imfree(mask_iris);
  imfree(v_normalize);
  imfree(mask_normalize);
  return 0;
}
