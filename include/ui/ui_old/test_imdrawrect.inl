
CC_INLINE int imdraw_circle_block2(img_t* im, int x0, int y0, double r, int shift, int n, const COLOR* clrs)
{
  int i;

  for (i = 0; i < n; ++i, r -= 1) {
    COLOR clr = clrs[i];
    double x = 0, y = r, d = 3 - 2 * r;

    while (x <= y) {
      IRECT rc;
#define round11(x)   ((int)(x))
#define putpix(x, y)    rc = iRECT((x0)+(round11(x))*10, (y0)+(round11(y))*10, (x0)+((round11(x))+1)*10, (y0)+((round11(y))+1)*10), imdraw_rect(im, rc, clr, 0, 0);
      putpix(x, y);
      putpix(-x, y);
      putpix(x, -y);
      putpix(-x, -y);
      putpix(y, x);
      putpix(-y, x);
      putpix(y, -x);
      putpix(-y, -x);

      if (d < 0) {
        d += 4 * x + 6;
      }
      //ажвст╫ Tel:13728706096 QQ:765425020
      else {
        d += 4 * (x - y) + 10;
        y -= 1;
      }

      x += 1;
    }
  }

  return 0;
}

CC_INLINE int imdraw_circle_block(img_t* im, int x0, int y0, double r, int shift, int n, const COLOR* clrs)
{
  IRECT rc;
  double x, y;

  for (y = 0; y < r; y += 1) {
    for (x = 0; x < r; x += 1) {
      double rr = sqrt(x * x + y * y);

      if (rr < r) {
        int i = (int)(r - rr);
        COLOR clr = clrs[i];
        putpix(x, y);
        putpix(-x, y);
        putpix(x, -y);
        putpix(-x, -y);
      }
    }
  }

  return 0;
}

int test_draw_border(img_t* im, IRECT rc, const uistyle_t* drawer)
{
  const char* name[] = {
#define BORDERSTYLEDEF(a, b0, c0, b1, c1, b2, c2)  #a ,
#include "draw/borderstyledef.txt"
#undef BORDERSTYLEDEF
  };
  enum {nn = countof(name), col = 8};
  IRECT rc1[nn];
  IRECT rc2;
  int i;

  imdraw_groupbox(im, rc, "test_draw_border", 10, drawer);
  rc = iRectIn(rc, 20);

  iRectMatrix(rc, (nn + col - 1) / col, col, 2, 2, nn, rc1);

  for (i = 0; i < nn; ++i) {
    iRectCutT(&rc1[i], -16, 0, &rc2);
    //imdraw_rect(im, rc1[i], rgb(1,0,0), rgb(0,0,0), 1);
    imdraw_border_syscolor(im, rc1[i], i, BDM_ALL);
    //imdraw_border_syscolor(im, rc1[i], i, BDM_L | BDM_T);
    imdraw_text(im, rc2, name[i], -1, drawer->font, rgb(0, 0, 0), 0, TT_VCENTER | TT_CENTER);
  }

  return 0;
}

int test_colortable(img_t* im, IRECT rc, const uistyle_t* drawer)
{
  enum {nn = countof(g_ColTable) - 1, col = 10};
  IRECT rc1[nn];
  IRECT rc2;
  int i;

  imdraw_groupbox(im, rc, "test_colortable", 10, drawer);
  rc = iRectIn(rc, 20);

  iRectMatrix(rc, (nn + col - 1) / col, col, 2, 2, nn, rc1);

  for (i = 0; i < nn; ++i) {
    iRectCutT(&rc1[i], -12, 0, &rc2);
    //imdraw_rect(im, rc1[i], rgb(1,0,0), rgb(0,0,0), 1);
    imdraw_rect(im, rc1[i], g_ColTable[i].colVal, 0, 0);
    //imdraw_border_syscolor(im, rc1[i], i, BDM_L | BDM_T);
    imdraw_text(im, rc2, g_ColTable[i].szName, -1, drawer->font, rgb(0, 0, 0), 0, TT_VCENTER | TT_CENTER);
  }

  return 0;
}