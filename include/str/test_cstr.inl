
int test_cstr() {
  double t1;
  char buf[256];
  if (1) {
    utime_start(aaa);
    printf("asdfaf\n");
    sys_sleep(100);
    printf("asdfaf\n");
    t1 = utime_elapsed(aaa);
    t1 = 100.001;
    snprintf(buf, 256, "%f", (float)t1);
    //cstr_nprintf(buf, 256, "%I32f", (float)t1);
    cstr_nprintf(buf, 256, "%f", t1);
    puts(buf);
  }
  return 0;
}
