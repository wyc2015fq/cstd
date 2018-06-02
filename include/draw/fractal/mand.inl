
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "macro.h"
#include "fractal.h"

void DrawMand(IDC* pDC)
{
  int i, j;
  int red, green, blue, times;
  long double zx, zy, zxs, zys;
  BOOL inset;

  double x, y;
  double x0, y0;
  int detail;
  double x1, y1;
  double width, height;
  //double ltx,lty,rbx,rby;
  //int ltX,ltY;

  x1 = -2.5;
  y1 = -2;
  width = 4.0;
  height = 4.0;

  x0 = x1;
  y0 = y1;
  detail = 100;

  for (j = 0; j < pDC->h; j++) {
    y = y0 + ((double)(j)) * height / pDC->h;

    for (i = 0; i < pDC->w; i++) {
      x = x0 + ((double) i) * width / pDC->w;
      zx = 0;
      zy = 0;
      inset = TRUE;
      times = 0;

      while (inset && times < detail) {
        times++;
        zxs = zx * zx;
        zys = zy * zy;
        zy = 2 * zx * zy + y;
        zx = zxs - zys + x;

        if (zxs + zys >= 4.0) {
          inset = FALSE;
        }
      }

      if (inset) {
        SetPixel(pDC, i, j, 0, 0, 0);
      }
      else {
        red = (times + 100) % 200 + 50;
        green = (times + red) % 200 + 50;
        blue = (red + green) % 200 + 50;
        SetPixel(pDC, i, j, red, green, blue);
      }
    }
  }
}
//---------------------------------------------------------------------------
