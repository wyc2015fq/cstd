#include "cstd.h"
#include "str.h"
int test_printf()
{
  char buf1[50];
  char buf2[50];
  int i;
  int ii;
  double dd = 1.1782467989306318;
  double dds[] = { 1.1782467989306318,
    1.74574363303785,
    1.5394399887434922,
    1.4424933164485718,
    1.3230054875474884,
    1.826171380329253,
    1.7587448993949626,
    1.6884480090052061,
    1.1605459406219221,
 1.7920360208245392,
  };
  int buflen = countof(buf1);
  int64 aa64 = 999999999999999lu;
  {
    str_t str = STR1("aaaaaaaaaaaaaaaaa");
    cstr_nprintf(buf1, buflen, "%#s", str.l, str.s);
    printf("%30s\n", buf1);
  }
  dd = 123456789.123456789;
  ii = 123456789;
  //cstr_nprintf(buf2, buflen, "%08d %10s %c", ii, "aaa", 'c');
  dd = dds[0] * 355000000000000000000000000000000000000000000000000.;
  dd = 10000000000000.0000000000000001;
  cstr_nprintf(buf2, buflen, "%08I64d %010s %s", aa64, "aaaaa", "aaaaa");
  _snprintf(buf1, buflen, "%0.6e", dd);
  cstr_nprintf(buf2, buflen, "%0.6e", dd);
  cstr_nprintf(buf2, buflen, "%08d %010s %s", ii, "aaaaa", "aaaaa");
  cstr_nprintf(buf2, buflen, "%f", dd);
  for (i = 0; i < countof(dds); ++i) {
    dd = dds[i];
    _snprintf(buf1, buflen, "%-.60g", dd);
    cstr_nprintf(buf2, buflen, "%-.60g", dd);
    //db2str(buf2, buflen, dd, 10, 0, 64, LEFT);
    printf("%30s %30s\n", buf1, buf2);
  }
  for (i = 0; i < 10; ++i) {
    dd = dds[i] * 355000000000000000000000000000000000000000000000000.;
    _snprintf(buf1, buflen, "%0.16e", dd);
    cstr_nprintf(buf2, buflen, "%0.16e", dd);
    //db2str(buf2, buflen, dd, 10, 0, 64, LEFT);
    printf("%30s %30s\n", buf1, buf2);
  }
  for (i = 0; i < 10; ++i) {
    dd = dds[i] / 355000000000000000000000000000000000000000000000000.;
    memset(buf1, countof(buf1), 0);
    _snprintf(buf1, buflen-1, "%-0.64f", dd);
    cstr_nprintf(buf2, buflen-1, "%-0.64f", dd);
    //db2str(buf2, buflen, dd, 10, 0, 64, LEFT);
    printf("%30s %30s\n", buf1, buf2);
  }
  for (i = 0; i < 10; ++i) {
    ii = (int)(dds[i]*100000000);
    _snprintf(buf1, buflen, "%08d %10s %c", ii, "aaa", 'c');
    cstr_nprintf(buf2, buflen, "%08d %10s %c", ii, "aaa", 'c');
    //db2str(buf2, buflen, dd, 10, 0, 64, LEFT);
    printf("%30s %30s\n", buf1, buf2);
  }
  dd = 123456789.123456789;
#if 0
  {
    utime_start(_start_time);
    for (i = 0; i < 10000; ++i) {
      _snprintf(buf1, buflen, "%f %d %X %s", dd, ii, ii, "aaaaaaaaaaaaaaaaa");
    }
    printf("%f\n", utime_elapsed(_start_time));
  }
  {
    utime_start(_start_time);
    for (i = 0; i < 10000; ++i) {
      cstr_nprintf(buf1, buflen, "%I64f %d %X %s", dd, ii, ii, "aaaaaaaaaaaaaaaaa");
    }
    printf("%f\n", utime_elapsed(_start_time));
  }
#endif
  printf("%30s %30s\n", buf1, buf2);
  //cstr_nprintf(buf2, buflen, "%f, %f", dd, ff);
  return 0;
}

