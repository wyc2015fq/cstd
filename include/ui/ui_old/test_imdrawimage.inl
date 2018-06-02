
#include "cv/imgwarp.inl"

int test_imdrawimage()
{
  img_t im[2] = {0};
  enum {ah = 1000, aw = 1000, ai = 3, bh = ah, bw = aw, bi = ai};
  IRECT arc, brc;
  img_t* im1 = im + 1;
  imsetsize(im, ah, aw, ai, 1);
  imsetsize(im1, bh, bw, bi, 1);
  _chdir("D:/pub/pic");
  imread("lena512.jpg", ai, 1, im);
  {
    int i;
    ISIZE asz, bsz;
    DPOINT pt[64];
    int n = vcgen_ellipse(pt, countof(pt), 0, aw / 2, ah / 2, aw / 3, ah / 3);
    asz = iSIZE(im->w, im->h);
    bsz = iSIZE(im[1].w, im[1].h);
    arc = iRECT(0, 0, im->w, im->h);
    brc = iRECT(0, 0, im[1].w, im[1].h);
    imdraw_image(im1, brc, im, arc);

    for (i = 0; i < 10; ++i) {
      utime_start(_start_time);
      imdraw_image(im1, brc, im, arc);
      //imdrawaa_poly_gradient(im1, pt, &n, 1, 100, 100, 600, 300, rgb(1,0,0), rgb(0,1,0));
      //imdrawaa_poly_radial(im1, pt, &n, 1, aw/2, ah/2, 100, 300, rgb(1,0,0), rgb(0,1,0));
      //imdrawaa_poly_gouraud(im1, pt, &n, 1, aw/2, 0, 0, 500, 700, 600, rgb(1,0,0), rgb(0,1,0), rgb(0,0,1));
      //imdraw_rect(im1, brc, rgba(0, 1, 0, 0.05), rgba(1, 0, 0, 0.05), 0);
      //imdraw_fill(im1, rgba(0, 1, 0, 0.05));
      //imdraw_rect(im+1, brc, rgb(0,1,0), rgb(1,0,0), 0);
      //imdraw_hatch(im+1, brc, rgba(1,0,0, 0.15), rgba(0,1,0, 0.05), 2);
      //imdraw_hatch(im+1, brc, rgb(1,0,0), rgb(0,1,0), 1);
      //imdraw_gradient_charicature(im+1, brc, aw/2, ah/2, 400, rgb(1,0,0), rgb(0,1,0));
      //imdraw_rect_gradient_impl(im+1, brc, 100, 100, 800, 600, rgb(1,0,0), rgb(0,1,0));
      //icvResize_NN_8u(im->tt.data, im->s, asz, im[1].tt.data, im[1].s, bsz, ai);
      //icvResize_Bilinear_8u(im->tt.data, im->s, asz, im[1].tt.data, im[1].s, bsz, ai);
      //icvResize_Area_8u(im->tt.data, im->s, asz, im[1].tt.data, im[1].s, bsz, ai);
      //icvResize_AreaFast_8u(im->tt.data, im->s, asz, im[1].tt.data, im[1].s, bsz, ai);
      //cvResize_8u(im->tt.data, im->s, asz, im[1].tt.data, im[1].s, bsz, ai, CV_INTER_LINEAR);
      //imresize(im, ah, aw, im+1);
      //memcpy(im[1].tt.data, im->tt.data, ah*aw*ai);
      printf("%f\n", utime_elapsed(_start_time));
    }
  }

  cvNamedWindow("test_imdrawimage", 1);
  imshow2("test_imdrawimage", im1);
  cvWaitKey(-1);
  freeims(im, 2);
  return 0;
}