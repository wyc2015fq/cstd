
int test_scancell()
{
  scancell s[1] = {0};
  int i;
  DPOINT pt[10];
  int lens[10];
  rand_darr(&pt->x, countof(pt) * 2, 50, 1500);
  lens[0] = countof(pt);
  {
    utime_start(_start_time);

    for (i = 0; i < 10; ++i) {
      scancell_set(s, pt, lens, 1, 100, 100, 1300, 1300);
      scancell_free(s);
    }

    printf("%f\n", utime_elapsed(_start_time));
  }
  return 0;
}
