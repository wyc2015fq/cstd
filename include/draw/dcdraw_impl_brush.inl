{
  const uchar* pclr = 0;
  const uchar* pmask = (const uchar*)& dc->alpha_mask;
  uchar _a = 0;

  if (0 >= im->h * im->w * im->c) {
    return 0;
  }

  switch (dc->brtype) {
  case br_solid: {
    pclr = (const uchar*)(dc->clrs);
#define GET_PIX(pclr, x, y)  {}
#include "dcdraw_impl.inl"
  }
  break;

  case br_lut_gradient: {
    int d, ix = dc->args[0], iy = dc->args[1], it = dc->args[2];
#define GET_PIX(pclr, x, y)  { d = (it + ((x * ix + y * iy)>>8)); pclr = (const uchar*)(dc->clrs+BOUND(d, 0, 255)); }
#include "dcdraw_impl.inl"
  }
  break;

  case br_lut_radial: {
    int d, ix1 = dc->args[0], iy1 = dc->args[1], invrx = dc->args[2], invry = dc->args[3];
#define GET_PIX(pclr, x, y)  { d = fixmag(((x)-ix1)*invrx, ((y)-iy1)*invry)>>8; pclr = (const uchar*)(dc->clrs+BOUND(d, 0, 255)); }
#include "dcdraw_impl.inl"
  }
  break;

  case br_colormatrix: {
    const int* iA = dc->args;
    COLOR clr;
    pclr = (const uchar*)&clr;
#define GET_PIX(pclr, x, y)  { int r = ((iA[0]) * x + (iA[1]) * y + iA[2])>>8, g = ((iA[3]) * x + (iA[4]) * y + iA[5])>>8,  \
    b = ((iA[6]) * x + (iA[7]) * y + iA[8])>>8, a = ((iA[9]) * x + (iA[10]) * y + iA[11])>>8; clr=BOUND_RGBA(r, g, b, a); }
#include "dcdraw_impl.inl"
  }
  break;

  case br_lut_swirled: {
    int ix1 = dc->args[0], iy1 = dc->args[1], invrx = dc->args[2], invry = dc->args[3];
    double R = invrx / 256.;
#define GET_PIX(pclr, x, y)  { double fi = atan((y-ix1/256.)/(x-ix1/256.)); int r = fixmag( y-iy1/256, x-ix1/256), X = (int)(fabs(r*cos(fi+r/8))), \
    Y = (int)(fabs(r*sin(fi+r/8))), d = fixmag(X, Y); pclr = (const uchar*)(dc->clrs+BOUND(d, 0, 255)); }
#include "dcdraw_impl.inl"
  }
  break;

  case br_lut_charicature: {
    int ix1 = dc->args[0], iy1 = dc->args[1], invrx = dc->args[2], invry = dc->args[3];
    double R = invrx / 256.;
#define GET_PIX(pclr, x, y)  { double fi = atan((y-ix1/256.)/(x-ix1/256.)); \
  int r = fixmag( y-iy1/256, x-ix1/256); \
  int X = (int)(sqrt(r*R*2)*cos(fi)); \
  int Y = (int)(fabs(sqrt(r*R*2)*sin(fi))); \
  int d = fixmag(X, Y); pclr = (const uchar*)(dc->clrs+BOUND(d, 0, 255)); }

#include "dcdraw_impl.inl"
  }
  break;

  case br_texture: {
    const img_t* tex = dc->texture;
    const uchar* data = dc->texture->tt.data;
    int* xoff = MALLOC(int, im->w);
    int* yoff = MALLOC(int, im->h);
    uchar backclr[4] = {0, 0, 0, 0};
    uchar bclr[4] = {255, 255, 255, 255};
    wrap_index(xoff, im->w, tex->w, 0, tex->c, dc->args[0]);
    wrap_index(yoff, im->h, tex->h, 0, tex->s, dc->args[1]);

    if (wrap_mode_clamp == dc->args[0] || wrap_mode_clamp == dc->args[1]) {
#define GETTEXCLR1(x, y)  ((xoff[x]>=0 && yoff[y]>=0) ? data + xoff[x] + yoff[y] : backclr)

      switch (tex->c) {
      case 4: {
#define GET_PIX(pclr, x, y)  {pclr = GETTEXCLR1(x, y);}
#include "dcdraw_impl.inl"
      }
      break;

      case 3: {
#define GET_PIX(pclr, x, y)  {pclr = GETTEXCLR1(x, y); bclr[0]=pclr[0], bclr[1]=pclr[1], bclr[2]=pclr[2]; pclr = bclr;}
#include "dcdraw_impl.inl"
      }
      break;

      case 2: {
#define GET_PIX(pclr, x, y)  {pclr = GETTEXCLR1(x, y); bclr[0]=bclr[1]=bclr[2]=pclr[0], bclr[3]=pclr[1]; pclr = bclr;}
#include "dcdraw_impl.inl"
      }
      break;

      case 1: {
#define GET_PIX(pclr, x, y)  {pclr = GETTEXCLR1(x, y); bclr[0]=bclr[1]=bclr[2]=pclr[0]; pclr = bclr;}
#include "dcdraw_impl.inl"
      }
      break;

      default:
        ASSERT(0);
        break;
      }

#undef GETTEXCLR1
    }
    else {
#define GETTEXCLR1(x, y)  (data + xoff[x] + yoff[y])

      switch (tex->c) {
      case 4: {
#define GET_PIX(pclr, x, y)  {pclr = GETTEXCLR1(x, y);}
#include "dcdraw_impl.inl"
      }
      break;

      case 3: {
#define GET_PIX(pclr, x, y)  {pclr = GETTEXCLR1(x, y); bclr[0]=pclr[0], bclr[1]=pclr[1], bclr[2]=pclr[2]; pclr = bclr;}
#include "dcdraw_impl.inl"
      }
      break;

      case 2: {
#define GET_PIX(pclr, x, y)  {pclr = GETTEXCLR1(x, y); bclr[0]=bclr[1]=bclr[2]=pclr[0], bclr[3]=pclr[1]; pclr = bclr;}
#include "dcdraw_impl.inl"
      }
      break;

      case 1: {
#define GET_PIX(pclr, x, y)  {pclr = GETTEXCLR1(x, y); bclr[0]=bclr[1]=bclr[2]=pclr[0]; pclr = bclr;}
#include "dcdraw_impl.inl"
      }
      break;

      default:
        ASSERT(0);
        break;
      }

#undef GETTEXCLR1
    }

    FREE(yoff);
    FREE(xoff);
  }
  break;

  default:
    ASSERT(0);
    break;
  }
}

