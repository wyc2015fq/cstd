#include "img/imgio.inl"
int test_brush(ctrl_t* c)
{
  SYSIO;
  //IRECT rc = c->rc, rcLeft;
  int left_w = 200;
  double t1 = 0;
  static int inited = 0;
  spanv_t sp[1] = {0};
  bool flip_y = 0;
  if (1) {
    spanv_ellipseR(sp, im, flip_y, NULL, iRECT2(50, 450, 150, 120));
    spanv_gradient(sp, im, flip_y, 50, 450, 200, 450, ColorBlue, ColorGreen);
  }
  {
    IRECT rect1 = iRECT2(10, 10, 200, 200);
    IRECT rect2 = iRECT2(210, 10, 200, 200);
    IRECT rect3 = iRECT2(410, 10, 200, 200);
    img_t image[1] = {0};
    double m33[] = {0.5f, 0.0f, 0.1f, 0.5f, 0.0f, 0.0f};
    IRECT rcsrc = iRECT2(10, 10, 50, 50);
    texture_t tex[1] = {0};
    if (NULL == image->tt.data) {
      sys_chdir("E:/pub/bin/img2");
      imread("luffy.jpg", 3, 1, image);
    }
    BMPINITIM(tex, image);
    spanv_textureR(im, NULL, rect1, tex, NULL, NULL, 0, 0, _rgba(1, 0, 1, 0.1), WrapModeClamp); //完整图案填充
    spanv_textureR(im, NULL, rect2, tex, &rcsrc, NULL, 0, 0, _rgba(1, 0, 1, 0.1), WrapModeTile); //从源图截取一部分填充
    spanv_textureR(im, NULL, rect3, tex, NULL, m33, 0, 0, _rgba(1, 0, 1, 0.1), WrapModeTile); //缩小0。5倍填充
    //imfree(im);
  }
  spanv_free(sp);
  return 0;
}

