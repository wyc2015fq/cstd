
// int xc, yc, r;
if (!isaa)
{
  int xc = 0, yc = 0, r = 0;
  COLOR crColor = 0;
  setvar(p[0], &xc, 'i', &args);
  setvar(p[0], &yc, 'i', &args);
  setvar(p[0], &r , 'i', &args);
  setvar(p[0], &crColor, 'i', &args);
  // wline Ïß¿í 0 ±íÊ¾Ìî³ä
  //CC_INLINE int imdraw_circle(img_t* im, int xc, int yc, int r, COLOR solidfill, COLOR solidline, int wline)
  {
    int iwline = (int)wline;
    int mapbuf[10240] = {0};
    int* map1 = mapbuf;
    int* map2 = mapbuf + r + iwline + 1;
    int i, j, r1 = r, r2 = r1 - iwline;
    uchar* img = im->tt.data;
    ASSERT(r + r < 4096);
    r2 = BOUND(r2, 0, r1);
    draw_circle_map(r1, map1);
    draw_circle_map(r2, map2);

    if (solidline) {
      for (i = r2 + 1; i <= r1; ++i) {
        ICC_PUT_POINT(xc, yc - i, solidline);
        ICC_PUT_POINT(xc, yc + i, solidline);
      }

      for (i = 0; i <= r1; ++i) {
        for (j = map2[i] + 1; j <= map1[i]; ++j) {
          ICC_PUT_POINT(xc + j, yc - i, solidline);
          ICC_PUT_POINT(xc + j, yc + i, solidline);
          ICC_PUT_POINT(xc - j, yc - i, solidline);
          ICC_PUT_POINT(xc - j, yc + i, solidline);
        }
      }
    }

    if (solidfill) {
      for (i = 0; i <= r2; ++i) {
        for (j = xc - map2[i]; j <= xc + map2[i]; ++j) {
          ICC_PUT_POINT(j, yc - i, solidfill);
          ICC_PUT_POINT(j, yc + i, solidfill);
        }
      }
    }
  }
}