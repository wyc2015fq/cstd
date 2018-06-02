
int test_ctrl(ctrl_t* c) {
  static ctrl_t cc[10] = {0};
  IRECT rc = c->rc, rcLeft;
  int i = 0;
  static int64 _start_time = sys_utime_counter();
  int64 _start_time1 = sys_utime_counter();
  static char str1[256] = "asdf";
  iRectCutL(&rc, 250, 0, &rcLeft);
  color_background_ctrl2(c->rc);
  flexboxlay(rcLeft, countof(cc), cc, 2, flex_column);
  fmtlabel_ctrl(cc + i++, "time1 %5.2fms", utime_elapsed2(_start_time, _start_time1));
  button_ctrl(cc + i++, "°´Å¥");
  edit_ctrl(cc + i++, "edit1", str1, countof(str1));
  _start_time = _start_time1;
  return 0;
}