
int test_strlist_ctrl(ctrl_t* c)
{
  sysio_t* io = sys_getio();
  static const char* strs[] = {"第一列", "bbbb", "ccccc", "dddd"};
  static vstr_t sv[1] = {0};
  static int width[countof(strs)] = {0};
  static IPOINT pos = {0};
  static int sel = -1;
  static int hotid = -1;
  static ctrl_t cc[10] = {0};
  char buf[256];
  int i, n = 1000;
  if (sv->n != n) {
    strv_setsize(sv, n);
    for (i = 0; i < n; ++i) {
      _snprintf(buf, 256, "第%d行|aaaa %d|bbbb %d|cccc %d", i, i, i, i);
      str_setstr(sv->v + i, buf, -1);
    }
  }
  if (1) {
    i = 0;
    c->sz = flexboxlay(c->rc, countof(cc), cc, 0, flex_column);
    fmtlabel_ctrl(cc + i++, "list selected %d hot %d x=%d y=%d", sel, hotid, pos.x, pos.y);
    listheader(cc + i++, countof(strs), strs, width);
    cc[i].rc.b = c->rc.b;
    if (strlist_ctrl(cc + i++, sv, countof(strs), width, &pos, &sel, &hotid)) {
      printf("selected %d \n", sel);
    }
    if (ISHOT(sv) && hotid >= 0 && hotid < sv->n) {
      tooltip_ctrl(sv->v[hotid].s);
    }
  }
  return 0;
}
