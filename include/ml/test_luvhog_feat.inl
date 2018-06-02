
int test_luvhog_feat()
{
  img_t im[1] = {0};
  img_t im2[1] = {0};
  _chdir("D:/pub/pic");
  imread("lena512.jpg", 3, 1, im);
  //imsetsize(im, 240, 320, 3, 1);
  //imresize_bili(im0, im);
  //imread("bar__.jpg", 3, 1, im);
  //imread("//192.168.1.7/aflw/AFLW¹éÒ»»¯ÈËÁ³¿â/left_face/imface_0_22.5/1.bmp", 3, 1, im);

  if (1) {
    int ch = 0;

    for (; 'q' != ch;) {
      imsetsize(im2, im->h / sell_size, im->w / sell_size, 10, 1);
      {
        utime_start(_start_time);
        luvhog_feat(im->h, im->w, im->tt.data, im->s, im->c, im2->tt.data, im2->s, im2->c);
        printf("%f\n", utime_elapsed(_start_time));
      }
      {
        static int aa = 0;

        if (!aa) {
          aa = 1;
          fprintf_arr("1.txt", "%3I8u %3I8u %3I8u %3I8u %3I8u %3I8u %3I8u %3I8u %3I8u %3I8u\n", im2->h * im2->w, im2->tt.data, im2->h * im2->w * 10 * 60);
        }
      }
      //imshow2(name, im2);
      imshow(im);
      ch = cvWaitKey(50);
      imfree(im2);
    }
  }

  imfree(im);
  imfree(im2);
  return 0;
}
