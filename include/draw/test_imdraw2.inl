
int test_imdraw2()
{
  //BitMap bitmap = LoadBmpFile( "rgb.bmp" );
  int h = 512, w = 512, ai = 3;
  int al = w * ai;
  // n 循环次数
  // lw 线宽
  int i, n = 1, lw = 22, k = w + 100;
  int k2 = k / 2, k4 = k / 4;
  uchar* A;
  const char* name = "result";
  char ch = 0;
  uchar hi[ 30 ] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  A = (uchar*) malloc(al * w);
  srand((unsigned int) time(NULL));
  memset(A, 0, al * w);
  cvNamedWindow(name, 0);

  DrawHist(h, w, A, al, ai, hi, 30);

  while (ch != 'q') {
    for (i = 0; i < n; ++i) {
      IPOINT a, b;
      IPOINT center;
      int axes_height, axes_width;
      a.x = rand() % k;
      a.y = rand() % k;
      b.x = rand() % k;
      b.y = rand() % k;
      center.x = (a.x + b.x) / 2;
      center.y = (a.y + b.y) / 2;
      axes_width = rand() % k4;
      axes_height = rand() % k4;

      if (a.x < 0) {
        b.x = -b.x;
      }

      if (a.y < 0) {
        b.y = -b.y;
      }

#if 0
      DrawLine(h, w, A, al, ai, a, b, DEFULT_COLOR, lw, 8, 0);
      DrawRectangle(h, w, A, al, ai, a, b, DEFULT_COLOR, lw, 8, 0);
      DrawCircle(h, w, A, al, ai, center, axes_width, DEFULT_COLOR, lw, 8, 0);
      DrawEllipse(h, w, A, al, ai, center, axes_height, axes_width, 0, 0, 360, DEFULT_COLOR, lw, 8, 0);
#endif

    }

    //SaveBmpFile( "result.bmp", bitmap );
    cvShowImage(name, h, w, A, al, ai);
    ch = cvWaitKey(10);
  }

  cvDestroyWindow(name);
  free(A);
  return 0;
}