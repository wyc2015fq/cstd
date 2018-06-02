
#define POTRACE ""
#define VERSION ""

#include "getopt.inl"
#include "getopt1.inl"
#include "bitmap_io.inl"
#include "trans.inl"
#include "backend_xfig.inl"
#include "backend_svg.inl"
#include "decompose.inl"
#include "curve.inl"
#include "bbox.inl"
#include "trace.inl"
#include "main.inl"
#include "potracelib.inl"
#include "progress_bar.inl"

int test_potrace() {
  char* argv[] = {
    "adsf",
    "E:/code/c/Graphics/计算几何/矢量化/potrace/CR8tracer/head-orig3.bmp",
    "-o",
    "E:/code/c/Graphics/计算几何/矢量化/potrace/CR8tracer/head-orig3.svg",
    "-s",
  };
  test_potrace_main(countof(argv), argv);
  return 0;
}
