
int test_euler_distance()
{
  double aaa[128];
  double* ccc;
  double* ddd = 0;
  double mind = 0;
  int i, j, k = 1000000;
  ccc = MALLOC(double, 128 * k);
  ddd = MALLOC(double, k);
  mind = 100000000;
  srand(0);

  for (i = 0; i < 128; ++i) {
    aaa[i] = rand();
  }

  for (i = 0; i < 128 * k; ++i) {
    ccc[i] = rand();
  }

  {
    utime_start(_start_time);

    for (i = 0; i < k; ++i) {
      double* cc = ccc + i * 128;
      ddd[i] = 0;

      for (j = 0; j < 128; ++j) {
        ddd[i] += (cc[j] - aaa[j]) * (cc[j] - aaa[j]);
      }
    }

    printf("%f\n", utime_elapsed(_start_time));
  }

  pfree(ccc);
  pfree(ddd);
  return 0;
}
