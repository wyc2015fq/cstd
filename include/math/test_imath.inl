int test_imath()
{
  int i, j;
  double err, abserr;
  err = abserr = 0;
  for (i = 0; i < 360; ++i) {
    double a = sin360[i] * 1. / (1 << 16);
    double b = sin(i * PI / 180.);
    abserr += fabs(a - b);
    err += (a - b);
  }
  printf("sin abserr=%f err=%f\n", abserr, err);
  for (i = 0; i < 360; ++i) {
    double a = cos360[i] * 1. / (1 << 16);
    double b = cos(i * PI / 180.);
    abserr += fabs(a - b);
    err += (a - b);
  }
  printf("cos abserr=%f err=%f\n", abserr, err);
  for (i = -256; i < 256; ++i) {
    for (j = -256; j < 256; ++j) {
      double a = fixatan2(i, j);
      double b = atan2(i, j) * 180 / PI;
      abserr += fabs(a - b);
      err += (a - b);
      //printf("%3d %3d %5.1f %5.1f  %s\n", i, j, a, b, fabs(a-b)>1 ? "------" : "");
    }
  }
  printf("atan2 abserr=%f err=%f\n", abserr, err);
  return 0;
}

