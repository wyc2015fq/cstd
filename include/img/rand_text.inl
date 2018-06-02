int randint(int minv, int maxv)
{
  int t;

  if (minv > maxv) {
    CC_SWAP(minv, maxv, t);
  }

  if (minv == maxv) {
    return minv;
  }

  t = minv + rand() % (maxv - minv);
  return t;
}
int rand_init()
{
  static int inited = 0;

  if (!inited) {
    inited = 1;
    srand((unsigned)time(NULL));
  }

  return 0;
}
char* rand_text(char* buf, int buflen)
{
  static const char ss[] = "0123456789 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ +-*/()[]{}";
  int i;
  rand_init();

  for (i = 0; i < buflen - 1; i++) {
    buf[i] = ss[rand() % sizeof(ss)];
  }

  buf[i] = 0;
  return buf;
}
char* rand_chinese(char* buf, int buflen)
{
  int iRange1 = 0xf7 - 0xb0;
  int iRange2 = 0xfe - 0xa1;
  int i;
  rand_init();

  for (i = 0; i < buflen - 2; i += 2) {
    buf[i + 0] = rand() % iRange1 + 0xb0;
    buf[i + 1] = rand() % iRange2 + 0xa1;
  }

  buf[i] = 0;
  return buf;
}
int test_rand_chinese()
{
  int i;
  char buf[256];

  for (i = 0; i < 10; ++i) {
    printf("%s\n", rand_chinese(buf, 80));
    printf("%s\n", rand_text(buf, 80));
  }

  return 0;
}

