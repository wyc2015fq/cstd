int test_charsetchecker()
{
  char buf[256];
  int i;
  for (i = 0; i < 100; i += 2) {
    randstr_cn(buf, 50);
    printf("%s\n", buf);
  }
  return 0;
}

