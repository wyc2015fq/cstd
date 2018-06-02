
int test_testwindow() {
  static window_t win[10] = {0};
  static ctrl_t cc[10] = {0};
  static int inited = 0;
  sysio_t* io = sys_getio();
  int i = 0;
  if (!inited) {
    inited = 1;
    win[0].rc = iRECT2(10, 10, 200, 200);
  }
  if (window_begin(&win[0], "test_testwindow", 0)) {
    static int open = 0;
    win[0].client->sz = flexboxlay(win[0].client->rc, countof(cc), cc, 4, flex_cstart|flex_istart|flex_wrap);
    if (button_ctrl(cc + i++, "adfasdf")) {
      open = !open;
    }
    if (button_ctrl(cc + i++, "adfasdf 1")) {
      open = !open;
    }
    if (button_ctrl(cc + i++, "adfasdf 2")) {
      open = !open;
    }
    if (ISHOT(cc + i-1)) {
      tooltip_ctrl("adfasdfaef");
    }
    if (open) {
      //test_timewindow();
    }
    window_end();
  }
  return 0;
}