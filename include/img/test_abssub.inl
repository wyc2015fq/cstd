
//int findfiles()
int test_findfile()
{
  char* dir = "D:/www/gif/nawthatznasty.tumblr.com/gif/ttt/*.*";
  findfiles_t ff[1] = {0};
  //MEM_CHECK_BEGIN();
  findfiles_push(ff, strdup(dir));
  ff->spec = ".jpg|.gif";

  for (; ff->filesLen < 1000;) {
    if (!findfiles_next(ff, 15)) {
      break;
    }
  }

  findfiles_close(ff);
  //MEM_CHECK_END()
  return 0;
}


enum {NN = 10000};
int ga[NN];
int gb[NN];
#define MYIABS(a)  (((a)^((a)>>31))+(int)(((unsigned int)(a))>>31))
int test_abssub()
{
  int i, j, d;
  int* a = ga;
  int* b = gb;
  srand(time(0));

  for (i = 0; i < NN; ++i) {
    a[i] = rand();
    b[i] = rand();
  }

  {
    utime_start(_start_time);
    d = 0;

    for (j = 0; j < 10000; ++j) {
      for (i = 0; i < NN; ++i) {
        d += a[i] > b[i] ? a[i] - b[i] : b[i] - a[i];
      }
    }

    printf("%d %f\n", d, utime_elapsed(_start_time));
  }

  {
    utime_start(_start_time);
    d = 0;

    for (j = 0; j < 10000; ++j) {
      for (i = 0; i < NN; ++i) {
        int c = a[i] - b[i];
        d += IABS(c);
      }
    }

    printf("%d %f\n", d, utime_elapsed(_start_time));
  }

  {
    utime_start(_start_time);
    d = 0;

    for (j = 0; j < 10000; ++j) {
      for (i = 0; i < NN; ++i) {
        int c = a[i] - b[i];
        d += MYIABS(c);
      }
    }

    printf("%d %f\n", d, utime_elapsed(_start_time));
  }

  return 0;
}
