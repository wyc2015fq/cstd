{
  char* img0;
  char* img;
  int step;
  int x, y;
  int nbd;
  int deltas[16];
  char* tmp_buf = MALLOC(char, (size.w + 2) * (size.h + 2));
  /* make zero borders */
  step = size.w + 2;
  img0 = tmp_buf;
  img = tmp_buf + step + 1;
  nbd = 2;
  
  /* initialize local state */
  CC_INIT_3X3_DELTAS(deltas, step, 1);
  memcpy(deltas + 8, deltas, 8 * sizeof(deltas[0]));

  // Initializes scanner structure.
  // Prepare image for scanning (clear borders and convert all pixels to 0-1.
  // CC_ERROR(CC_StsUnsupportedFormat, "[Start]FindContours support only 8uC1 images");
  mem_fill_border(size.h, size.w, src, srcstep, (size.h + 2), (size.w + 2), tmp_buf, step, 1, 1, 1, 0, BD_CONSTANT);

  // converts all pixels to 0 or 1
  cvThreshold_8u(img, step, img, step, size, 0, 1, CC_THRESH_BINARY);

  for (y = 0; y < size.h; ++y, img += step) {
    int prev = 0;

    for (x = 0; x < size.w; ++x) {
      int p = img[x];
      if (p != prev) {
        int is_hole = 0;
        IPOINT pt;
        char* ptr;

        if (!(prev == 0 && p == 1)) {   /* if not external contour */
          /* check hole */
          if (p != 0 || prev < 1) {
            prev = p;
            continue;
          }

          is_hole = 1;
        }

        /* find contour parent */
        // initialize header
        {
          ptr = img + x - is_hole;
          pt.x = x - is_hole, pt.y = y;
          iVC_add(vc, pt.x, pt.y);
          //icvFetchContourEx(img + x - is_hole, step, vc, nbd, is_hole);
#include "icvFetchContourEx.inl"
          iVC_end(vc);

          if (is_hole) {
            vc->cc[vc->l - 1].flag |= CC_SEQ_FLAG_HOLE;
          }

          /* change nbd */
          nbd = (nbd + 1) & 127;
          nbd += nbd == 0 ? 3 : 0;
        }

        prev = p;
      }
    }
  }

  FREE(tmp_buf);
}
