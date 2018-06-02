
int test_imsubref()
{
  img_t im[1] = {0};
  img_t im2[1] = {0};
  IRECT rc = {60, 80, 180, 220};
  _chdir("D:\\pub\\bin\\imgproc\\match\\");
  imread("1.bmp", 3, 1, im);
  imsubref(im, rc, im2);
  imwrite2("a1.bmp", im2);
  imfree(im);
  return 0;
}

