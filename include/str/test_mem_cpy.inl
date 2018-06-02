char* StrCopy1(char* dest, const char* source)
{
  char* pd = dest;
  char* pe, *ps = (char*)source;
  int ext, size;
  while (*ps ++);
  size = ps - source;
  ext = size & 3;
  ps = (char*)source;
  pe = ps + (size & 0xfffffffc);
  for (; ps < pe; pd += 4, ps += 4) {
    *(long*)pd = *(long*)ps;
  }
  for (; ext > 0; ext --) {
    *pd ++ = *ps ++;
  }
  return dest;
}
char* StrCopy2(char* dest, const char* source)
{
  char* pd = dest;
  int s = source - dest;
  if ((unsigned)pd & 1) {
    *pd = *source;
    if (*pd == 0) {
      return dest;
    }
    pd ++;
  }
  while (true) {
    if (*(pd + s) == 0) {
      break;
    }
    *(short*)pd = *(short*)(pd + s);
    if (*(unsigned short*)pd <= 255) {
      return dest;
    }
    pd += 2;
  }
  *pd = 0;
  return dest;
}
void* mem_cpy(void* dest, const void* source, int size)
{
  char* pd = dest;
  const char* pe, *ps;
  int ext;
  ps = (char*)source;
  ext = size & 3;
  pe = ps + (size & 0xfffffffc);
  for (; ps < pe; pd += 4, ps += 4) {
    *(long*)pd = *(long*)ps;
  }
  for (; ext > 0; ext --) {
    *pd ++ = *ps ++;
  }
  return dest;
}
size_t strlen1(const char* str)
{
  const char* p = str;
  while (*p) {
    p ++;
  }
  return (size_t)(p - str);
}
size_t strlen2(const char* str)
{
  unsigned v;
  const char* p = str;
  while (*p && ((unsigned)p & 3)) {
    p ++;
  }
  if (*p == 0) {
    return (size_t)(p - str);
  }
  for (v = 0; !v; p += 4) {
    v = (*(unsigned*)p - 0x01010101) & 0x80808080;
    if (v) {
      v &= ~*(unsigned*)p;
    }
  }
  for (; (v & 0xff) == 0; p ++) {
    v >>= 8;
  }
  return (size_t)(p - str - 4);
}
int test_mem_cpy()
{
  int i;
  enum {nn = 1 << 20, tt = 100};
  uchar* ss = MALLOC(uchar, nn);
  uchar* dd = MALLOC(uchar, nn);
  double t;
  {
    utime_start(_start_time);
    for (i = 0; i < tt; ++i) {
      mem_cpy(dd, ss, nn);
    }
    t = utime_elapsed(_start_time);
    printf("%.3f\n", t);
  }
  {
    utime_start(_start_time);
    for (i = 0; i < tt; ++i) {
      memcpy(dd, ss, nn);
    }
    t = utime_elapsed(_start_time);
    printf("%.3f\n", t);
  }
  FREE(ss);
  FREE(dd);
  return 0;
}

