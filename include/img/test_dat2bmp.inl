

#include "bmp_save.inl"

static char* argopt_getstr(int argn, char** argc, const char* opt, char* buf, int maxlen) {
  int i;
  for (i=0; i<argn-1; ++i) {
    if(0==stricmp(opt, argc[i])) {
      strncpy(buf, argc[i+1], maxlen);
      return buf;
    }
  }
  return NULL;
}

static int argopt_getint(int argn, char** argc, const char* opt, int default_value) {
  char buf[32];
  if (argopt_getstr(argn, argc, opt, buf, 32)) {
    default_value = atoi(buf);
  }
  return default_value;
}

//#include "img/imgio.inl"

#if 0

#include "D:\code\cstd\include\img\bmp_save.inl"
save_double_to_bmp("C://aaa.bmp", tmpTable, 32, 32, 3);
#endif

#include "cstd.h"
#include "cfile.h"
int test_dat2bmp(int argn, char** argc) {
  buf_t bf[1] = {0};
  char fn[256] = "C:\\tmpTable_double_1024x3.dat";
  char buf[256];
  int width = argopt_getint(argn, argc, "-width", 32);
  int cn = argopt_getint(argn, argc, "-cn", 3);
  argopt_getstr(argn, argc, "-file", fn, countof(fn));
  if (buf_load(fn, bf)) {
    double* pd = (double*)bf->data;
    int len = bf->len/sizeof(double)/cn;
    int height = len/width;
    _snprintf(buf, 256, "%s.bmp", fn);
    save_double_to_bmp(buf, pd, width, height, 3);
  }

  return 0;
}
