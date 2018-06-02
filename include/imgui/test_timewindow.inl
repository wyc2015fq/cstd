
int test_timewindow(int n, double time1) {
  sysio_t* io = sys_getio();
  static window_t win[10] = {0};
  static ctrl_t cc[10] = {0};
  static int inited = 0;
  int i, j;
  if (!inited) {
    inited = 1;
    for (j=0; j<countof(win); ++j) {
      win[j].rc = iRECT2(j*20, j*20, 200, 200);
      _snprintf(win[j].text, 16, "win %d", j);
    }
  }
  i = 0;
  n = BOUND(n, 0, countof(win));
  for (j=0; j<n; ++j) {
    char buf[256];
    _snprintf(buf, 256, "win %d", j);
    if (window_begin(&win[j], buf, 0)) {
      fmtlabel_ctrl(cc + i++, "time1 %5.2f ms %5.2f frame", 1000 * time1, 1. / time1);
      win[j].client->sz = flexboxlay(win[j].client->rc, 1, cc+i-1, 0, flex_column);
      window_end();
    }
  }
  switch (io->msg) {
  case MSG_KEYDOWN:
    if (' '==io->last_key) {
      window_print(io->cur_host->root, 0);
    }
    break;
  }
  return 0;
}