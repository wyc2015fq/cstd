//生成太极图完整代码
int imdraw_taichi(int h, int w, uchar* img, int step, int cn, int x, int y, double r)
{
  COLOR crBlack = _RGB(0, 0, 0);
  COLOR crWhite = _RGB(255, 255, 255);
  double r1 = r, r2 = r * 0.5, r3 = r * 0.1;
  DPOINT pt[128];
  int n = 0;
  imdrawaa_ellipse(h, w, img, step, cn, x, y, r1, r1, crWhite, crBlack, 1);
  n += vcgen_arc(pt + n, countof(pt), 0, x, y, r1, r1, PI, 0);
  n += vcgen_arc(pt + n, countof(pt), 1, x + r2, y, r2, r2, 0, PI);
  n += vcgen_arc(pt + n, countof(pt), 0, x - r2, y, r2, r2, 0, PI);
  imdrawaa_poly(h, w, img, step, cn, pt, n, 1, crBlack, 0, 0);
  imdrawaa_ellipse(h, w, img, step, cn, x - r2, y, r3, r3, crWhite, 0, 0);
  imdrawaa_ellipse(h, w, img, step, cn, x + r2, y, r3, r3, crBlack, 0, 0);
  return 0;
}
