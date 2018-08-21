{
  register unsigned int r, g, b;
  register unsigned int rinc = 0, ginc = 0, binc = 0;

  switch (zb->cn) {
  case 4: {
    enum {PSZB = 4};
    typedef uint PIXEL;
#define RGB_TO_PIXEL  RGB_TO_PIXEL32
#include "zline_impl.inl"
  }
  break;

  case 3: {
    enum {PSZB = 3};
    typedef uchar PIXEL;
#define TGL_FEATURE_RENDER_BITS 24
#define RGB_TO_PIXEL  RGB_TO_PIXEL24
#include "zline_impl.inl"
  }
  break;

  case 2: {
    enum {PSZB = 2};
    typedef ushort PIXEL;
#define RGB_TO_PIXEL  RGB_TO_PIXEL16
#include "zline_impl.inl"
  }
  break;
  }
}
#undef INTERP_Z
#undef INTERP_RGB

