
// È¥¹â°ß
int test_glassRemoval()
{
  const char* fname = "D://pub//bin//iris//detect//S2001L01.BMP";
  img_t im[1] = {0};
  img_t im2[1] = {0};
  imread(fname, 1, 1, im);

  //char buf[256];
  //{utime_start(_start_time);
  glassRemoval(im, im2);
  //printf("%f\n", utime_elapsed(_start_time));}

  //imshow(im2); cvWaitKey(-1);
  imfree(im);
  return 0;
}
