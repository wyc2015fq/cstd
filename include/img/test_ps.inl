

#include "img\imgopt.inl"
#include "ps\ps.inl"

int test_ps()
{
  img_t im[1] = {0};
  img_t im2[1] = {0};
  int ch;
  int tt = 9;
  const char* fn;
  _chdir("D:/pub/pic");
  fn = "6.jpg";
  fn = "meinv01.png";
  imread(fn, 3, 1, im);
  imclone(im, im2);
  imshow(im);

  for (;;) {
    utime_start(_start_time);

    switch (tt) {
    case 0:
      if (im->h) {
        MSRCR(im->h, im->w, im->data, im2->data, im2->s, im->c);
      }

      break;

    case 1:
      if (im->h) {
        fragment(im->h, im->w, im->data, im->s, im2->data, im2->s, im->c);
      }

      break;

    case 2:
      if (im->h) {
        imread("22.jpg", 1, 1, im);
        CLAHE(im->h, im->w, im->data, 0, 255, 10, 10, 30, 100);
      }

      break;

    case 3:
      if (im->h) {
        FCPixelOilPaint(im->h, im->w, im->data, im->s, im->c, 7);
        //FCPixelColorTone(im->h, im->w, im->data, im->s, im->c, PCL_RGBA(254,168,33,0xff));
        //FCPixelNoisify(im->h, im->w, im->data, im->s, im->c, 30, 0);
      }

      break;

    case 4:
      if (im->h) {
        //FCPixelOilPaint(im->h, im->w, im->data, im->s, im->c, 7);
        //FCPixelColorTone(im->h, im->w, im->data, im->s, im->c, PCL_RGBA(254,168,33,0xff));
        //FCPixelNoisify(im->h, im->w, im->data, im->s, im->c, 30, 0);
        FCPixelSplash(im->h, im->w, im->data, im->s, im->c, 10);
      }

      break;

    case 5:
      if (im->h) {
        WellneradaptiveThreshold2(im->h, im->w, im->data, im->s, 35, 15);
      }

      break;

    case 6:
      if (im->h) {
        FillHole(im->h, im->w, im->data, im->s, 0);
      }

      break;

    case 7:
      if (im->h) {
        double cc;
        cc = GetColorCastFactor(im->h, im->w, im->data, im->s);
      }

      break;

    case 8:
      if (im->h) {
        BEEPS(im->h, im->w, im->data, im->s, im2->data, im2->s, im->c);
      }

      break;

    case 9:
      if (im->h) {
        ctmf(im->h, im->w, im->data, im->s, im2->data, im2->s, im->c, 10, 1 << 16);
      }

      break;
    }

    printf("%f\n", utime_elapsed(_start_time));

    imshow(im);
    imshow(im2);
    ch = cvWaitKey(100);
  }

  imfree(im);
  imfree(im2);

  return 0;
}
