
int test_mem_fill_border()
{
  img_t im[1] = {0};
  img_t im2[1] = {0};
  _chdir("D:/pub/pic");
  //imread("lena512.jpg", 3, 1, im);
  imread("bar__.jpg", 4, 1, im);
  imsetsize(im2, im->h, im->w, 1, 1);

  if (1) {
    int c[10] = {0xffffffff, 0};
    const char* name = "im2";
    enum {ww = 500, };
    int offx = ww, offy = ww, ch = 0, type = BD_CONSTANT;
    imsetsize(im2, ww, ww, im->c, 1);
    cvNamedWindow(name, 1);
    cvCreateTrackbar("type", name, &type, 3, 0);
    cvCreateTrackbar("xx", name, &offx, im2->w * 3, 0);
    cvCreateTrackbar("yy", name, &offy, im2->h * 3, 0);

    for (; 'q' != ch;) {
      imsetsize(im2, 500, 500, im->c, 1);
      {
        utime_start(_start_time);
        mem_fill_border(im->h, im->w, im->tt.data, im->s, im2->h, im2->w, im2->tt.data, im2->s, im->c, offx - im2->w, offy - im2->h, c, type);
        printf("%f\n", utime_elapsed(_start_time));
      }
      imshow2(name, im2);
      imshow(im);
      ch = cvWaitKey(50);
      imfree(im2);
    }
  }

  imfree(im);
  imfree(im2);
  return 0;
}