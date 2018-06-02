
#define IMPIX(im, x, y)  *((DWORD*)((im)->data+(y)*(im)->s+(x)*(im)->c))

int imglass(img_t* im, img_t* im2)
{
  int x, y;

  for (x = 1; x < im->w; x++) {
    for (y = 1; y < im->h; y++) {
      COLORREF piex1;
      double f = RAND_MAX;
      int k = (int)(24 * (rand() / f));
      int dx = x + k % 15;
      int dy = y + k % 15;

      if (dx >= im->w) {
        dx = im->w - 1;
      }

      if (dy >= im->h) {
        dy = im->h - 1;
      }

      piex1 = IMPIX(im2, dx, dy);
      IMPIX(im2, x, y) = piex1;
    }
  }

  return 0;
}

