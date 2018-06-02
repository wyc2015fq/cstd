
#include "cstd.h"
#include "imgui/imgui.inl"
//#include "imgui/ctrl.inl"

int test_nnui()
{
#if 0
  window_host_t host[2] = {0};
  ctrl_t cc[10] = {0};
  double time1 = 0;
  int pos = 0, tpos = 0;
  sysio_t* io = sys_getio();
  font_t fo[1] = {0};
  io->font = fo;
  host_open(host, 200, 200, 800, 600);
  host_create(io, host, "asdf");
  soft_drv_Init(host, 0, 0);
  font_init_memraster(fo, "songti", songti_12);
  for (; sys_waitio(100) && host_begin(host);) {
    utime_start(_start_time);
    host_end(host, _rgb(1, 1, 1));
    time1 = utime_elapsed(_start_time);
  }
#endif
  return 0;
}
