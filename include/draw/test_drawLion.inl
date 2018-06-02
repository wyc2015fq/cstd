int test_DrawLion(HDDC hDC, int ch)
{
  DPOINT* pt;
  int len = 0;
  // Parse the lion and then detect its bounding
  // box and arrange polygons orientations (make all polygons
  // oriented clockwise or counterclockwise)
  int npaths = 0;
  DWORD clr = 0;
  DWORD lens[200];
  DWORD clrs[200];
  DPOINT pts[200][100];
  static double sc = 1;
  int len1;
  //char g_lion[10000];
  char* g_lion = loadfile("lion.txt", &len1);
  const char* ptr = g_lion;

  //savedata_11("lion.txt", g_lion, strlen(g_lion), 0);
  //savedata2txt("lion.txt");
  switch (ch) {
  case '+':
    sc = BOUND(sc * 1.1, 0.1, 10);
    break;

  case '-':
    sc = BOUND(sc / 1.1, 0.1, 10);
    break;
  };

  while (*ptr) {
    if (*ptr != 'M' && isalnum(*ptr)) {
      unsigned c = 0;
      sscanf(ptr, "%x", &c);
      clr = c | 0xff000000;

      // New color. Every new color creates new path in the path object.
      while (*ptr && *ptr != '\n') {
        ptr++;
      }

      if (*ptr == '\n') {
        ptr++;
      }
    }
    else {
      double x = 0.0;
      double y = 0.0;

      while (*ptr && *ptr != '\n') {
        int c = *ptr;

        while (*ptr && !isdigit(*ptr)) {
          ptr++;
        }

        x = atof(ptr);

        while (*ptr && isdigit(*ptr)) {
          ptr++;
        }

        while (*ptr && !isdigit(*ptr)) {
          ptr++;
        }

        y = atof(ptr);

        if (c == 'M') {
          len = 0;
          pt = pts[npaths];
        }

        pt[len].x = (sc * x), pt[len].y = (sc * y);
        len++;

        if (len >= 2 && pt[len - 2].x == pt[len - 1].x && pt[len - 2].y == pt[len - 1].y) {
          --len;
        }

        while (*ptr && isdigit(*ptr)) {
          ptr++;
        }

        while (*ptr && *ptr != '\n' && !isalpha(*ptr)) {
          ptr++;
        }
      }

      if (*ptr == '\n') {
        ptr++;
      }

      if (pt->x == pt[len - 1].x && pt->y == pt[len - 1].y) {
        --len;
      }

      lens[npaths] = len;
      clrs[npaths] = clr;
      npaths++;
      //ScanFill(hDC, pt, len, clr);
    }
  }

  if (1) {
    utime_start(_start_time);
    {
      int i;

      for (i = 0; i < npaths; ++i) {
        pt = pts[i];
        clr = clrs[i];
        len = lens[i];
        //ScanFill(hDC, pt, len, clr);
        //FillPolyCell1(hDC, pt, len, clr);
        drawaa_strokepoly(hDC, pt, len, 1, clr, sc);
      }
    }
    printf("%.6f ", utime_elapsed(_start_time));
  }

  free(g_lion);
  return npaths;
}

