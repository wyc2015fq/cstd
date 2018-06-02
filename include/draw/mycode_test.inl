#include "cstd.h"
int on_btn_reg(int x)
{
  return 0;
}
static char test_membuf[8 * _Mb] = {0};
enum {nn = 100};
void* ptrs[nn] = {0};
int test_memmalloc()
{
  int i, j;
  uint seed = 110;
  MEM_INIT(test_membuf);
  for (i = 0; i < nn; ++i) {
    int k = 1 + myrand32(&seed) % 100;
    ptrs[i] = pmalloc(k);
    //memset(ptrs[i], 0, k);
  }
  for (j = 0; j < 1; ++j) {
    for (i = 0; i < nn; ++i) {
      int k = nn + myrand32(&seed) % 200;
      ptrs[i] = prealloc(ptrs[i], k);
    }
  }
  for (i = 0; i < nn; ++i) {
    pfree(ptrs[nn - 1 - i]);
    ptrs[nn - 1 - i] = NULL;
  }
  return 0;
}
#include "easydraw/easydraw.inl"
//#include "imgui/imgui.inl"
//#include "font/font.inl"
//#include "imgui/ctrl.inl"
//#include "imgui/ctrl.inl"
//#include "imgui/soft.inl"
#include "easyui.inl"
#include "easydraw_impl.inl"
static int label_ctrl(ctrl_t* c, const char* buf)
{
  SYSIO;
  IRECT rc = c->rc;
  const int fmtFlag = TF_VCENTER | TF_LEFT;
  //
  if (1) {
    ISIZE sz = measureText(buf, NULL, io->font, fmtFlag, &c->rc);
    c->sz = iSIZE(sz.w + 6, sz.h + 6);
  }
  if (1) {
    gcSolidText(g, rc, buf, NULL, io->font, fmtFlag, Color_Black);
  }
  return 0;
}
int myframe(int h, int w, void* img, int step, int cn, int t, int out_h, int out_w, void* out_buf, int out_step, int out_cn)
{
  //printf("asdfasdf\n");
  if (0) {
    utime_start(_start_time);
    test_memmalloc();
    printf("%f\n", utime_elapsed(_start_time));
  }
  return 0;
}
#include "sys_am437x.inl"

