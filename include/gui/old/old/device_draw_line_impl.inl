{
  int x, y, rem = 0;

  if (x1 == x2 && y1 == y2) {
    DEVICE_PIXEL(x1, y1, c);
  }
  else if (x1 == x2) {
    int inc = (y1 <= y2) ? 1 : -1;

    for (y = y1; y != y2; y += inc) {
      DEVICE_PIXEL(x1, y, c);
    }

    DEVICE_PIXEL(x2, y2, c);
  }
  else if (y1 == y2) {
    int inc = (x1 <= x2) ? 1 : -1;

    for (x = x1; x != x2; x += inc) {
      DEVICE_PIXEL(x, y1, c);
    }

    DEVICE_PIXEL(x2, y2, c);
  }
  else {
    int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
    int dy = (y1 < y2) ? y2 - y1 : y1 - y2;

    if (dx >= dy) {
      if (x2 < x1) {
        x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
      }

      for (x = x1, y = y1; x <= x2; x++) {
        DEVICE_PIXEL(x, y, c);
        rem += dy;

        if (rem >= dx) {
          rem -= dx;
          y += (y2 >= y1) ? 1 : -1;
          DEVICE_PIXEL(x, y, c);
        }
      }

      DEVICE_PIXEL(x2, y2, c);
    }
    else {
      if (y2 < y1) {
        x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
      }

      for (x = x1, y = y1; y <= y2; y++) {
        DEVICE_PIXEL(x, y, c);
        rem += dx;

        if (rem >= dy) {
          rem -= dy;
          x += (x2 >= x1) ? 1 : -1;
          DEVICE_PIXEL(x, y, c);
        }
      }

      DEVICE_PIXEL(x2, y2, c);
    }
  }
}
#undef DEVICE_PIXEL
