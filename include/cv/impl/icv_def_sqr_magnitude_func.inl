
{
  int i;

  for (i = 0; i <= len - 4; i += 4) {
    magtype x0 = (magtype)xarr[i], y0 = (magtype)yarr[i];
    magtype x1 = (magtype)xarr[i + 1], y1 = (magtype)yarr[i + 1];

    x0 = x0 * x0 + y0 * y0;
    x1 = x1 * x1 + y1 * y1;
    magarr[i] = x0;
    magarr[i + 1] = x1;
    x0 = (magtype)xarr[i + 2], y0 = (magtype)yarr[i + 2];
    x1 = (magtype)xarr[i + 3], y1 = (magtype)yarr[i + 3];
    x0 = x0 * x0 + y0 * y0;
    x1 = x1 * x1 + y1 * y1;
    magarr[i + 2] = x0;
    magarr[i + 3] = x1;
  }

  for (; i < len; i++) {
    magtype x0 = (magtype)xarr[i], y0 = (magtype)yarr[i];
    magarr[i] = x0 * x0 + y0 * y0;
  }

}
