

int test_levelOM()
{
  const char* fn;
  img_t im[1] = {0};
  uchar ft[IRIS_FEATLEN] = {0};
  uchar fim[IRIS_H * IRIS_W];
  _chdir("D:/code/cstd/src/iris/iris-OM");
  fn = "1.bmp";
  imread(fn, 1, 1, im);
  ASSERT(30 == im->h && 360 == im->w);
  //{utime_start(_start_time);
  levelOM(im, fim, ft);
  //printf("%f\n", utime_elapsed(_start_time));}
  imshow(im);
  cvWaitKey(-1);
  return 0;
}
