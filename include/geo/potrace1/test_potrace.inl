
#include "cstd.h"

#define HAVE_STRING_H  1
#define MKBITMAP  "MKBITMAP"
#define VERSION  "VERSION"
#define POTRACE  "POTRACE"

#include "curve.inl"
#include "decompose.inl"
#include "flate.inl"
#include "getopt.inl"
#include "getopt1.inl"
#include "lzw.inl"
#include "potracelib.inl"
#include "render.inl"
#include "trace.inl"
#include "bitmap_io.inl"
#if 0
#include "backend_pdf.inl"
#include "mkbitmap.inl"
#include "greymap.inl"
#include "backend_eps.inl"
#include "backend_gimp.inl"
#include "backend_pgm.inl"
#include "backend_xfig.inl"
#include "main.inl"
#include "backend_svg.inl"
#endif
#include "potracelib_demo.inl"

int test_potrace() {
  potracelib_demo();
  return 0;
}