
#include "ui/window.inl"
int test_dib()
{
  img_t dib[1] = {0};
  img_t im[1] = {0};
  dib_loadfile("C:\\11.bmp", dib);
  dib2img(dib, 1, 1, im);
  //imshow(im);cvWaitKey(-1);
  imfree(dib);
  imfree(im);
  return 0;
}
