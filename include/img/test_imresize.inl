
#include "cv/imgwarp.inl"


int test_imresize()
{
  img_t im[3] = {0};
  int ai = 4, bi = ai;
  //enum {ah = 1100, aw = 1100, ai = 3, bh = ah, bw = aw, bi = ai};
  IRECT arc, brc;
  ISIZE asz, bsz;
  img_t* im1 = im + 1;
  char ch = 0;
  int bww = 550, bww1 = 0;
  const char* name = " test_imresize ";
  _chdir("D:/bak/pub/pic");
  //imread("lena512.jpg", ai, 1, im);

  imshow2(name, im);
  cvCreateTrackbar("³ß´ç", name, &bww, 5500, 0);

  for (; 'q' != ch;) {
    //imread("cast_0003.jpg", ai, 1, im);
    imsetsize(im1, bww, bww, bi, 1);

    asz = iSIZE(im->w, im->h);
    bsz = iSIZE(im1->w, im1->h);
    arc = iRECT(0, 0, im->w, im->h);
    brc = iRECT(0, 0, im1->w, im1->h);
    iRectIn(&arc, 0);
    //brc = iRectIn(brc, 100);
    {
      utime_start(_start_time);
      //imresize_rect(im->h, im->w, im->tt.data, im->s, im->c, arc, im1->h, im1->w, im1->tt.data, im1->s, im1->c, brc, CC_INTER_LINEAR);
      //imresize_nn(im->h, im->w, im->tt.data, im->s, im->c, im1->h, im1->w, im1->tt.data, im1->s, im1->c);
      //imresize_bl(im->h, im->w, im->tt.data, im->s, im->c, im1->h, im1->w, im1->tt.data, im1->s, im1->c);
      //icvResize_NN_8u(im->tt.data, im->s, asz, im[1].tt.data, im[1].s, bsz, ai);
      //icvResize_Area_8u(im->tt.data, im->s, asz, im[1].tt.data, im[1].s, bsz, ai);
      //icvResize_Bilinear_8u(im->tt.data, im->s, asz, im[1].tt.data, im[1].s, bsz, ai);
      //imdraw_image(im1, brc, im, arc);
      printf("%f\n", utime_elapsed(_start_time));
    }
    imshow2(name, im1);
    ch = cvWaitKey(50);
  }

  freeims(im, 2);
  return 0;
}
