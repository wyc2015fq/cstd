{
  const uchar* pclr=0;
  const uchar* pmask=(const uchar*)&br->alpha_mask;
  uchar _a=0;
  if (0>=im->h*im->w*im->c) {
    return 0;
  }
  
  switch (br->type) {
  case br_solid:
    {
      pclr = (const uchar*)(br->clrs);
#define GETCLR_INIT(y)
#define GETCLR(x, y)  {}
#include "imdrawaa_poly_case_cn.inl"
    }
    break;
  case br_lut_gradient:
    {
      int d, ix = br->args[0], iy = br->args[1], it = br->args[2];
#define GETCLR_INIT(y)
#define GETCLR(x, y)  { d = (it + ((x * ix + y * iy)>>8)); pclr = (const uchar*)(br->clrs+BOUND(d, 0, 255)); }
#include "imdrawaa_poly_case_cn.inl"
    }
    break;
  case br_lut_radial:
    {
      int d, ix1 = br->args[0], iy1 = br->args[1], invrx = br->args[2], invry = br->args[3];
#define GETCLR_INIT(y)
#define GETCLR(x, y)  { d = fixmag(((x)-ix1)*invrx, ((y)-iy1)*invry)>>8; pclr = (const uchar*)(br->clrs+BOUND(d, 0, 255)); }
#include "imdrawaa_poly_case_cn.inl"
    }
    break;
  case br_colormatrix:
    {
      const int* iA = br->args;
      COLOR clr;
      pclr = (const uchar*)&clr;
#define GETCLR_INIT(y)  int ry = (iA[1]) * y + iA[2], gy = (iA[4]) * y + iA[5], by = (iA[7]) * y + iA[8], ay = (iA[10]) * y + iA[11];
#define GETCLR(x, y)  { int r = ((iA[0]) * x + ry)>>8, g = ((iA[3]) * x + gy)>>8,  \
  b = ((iA[6]) * x + by)>>8, a = ((iA[9]) * x + ay)>>8; clr=BOUND_RGBA(r, g, b, a); }
#include "imdrawaa_poly_case_cn.inl"
    }
    break;
  case br_lut_swirled:
    {
      int ix1 = br->args[0], iy1 = br->args[1], invrx = br->args[2], invry = br->args[3];
      double R = invrx/256.;
#define GETCLR_INIT(y)
#define GETCLR(x, y)  { double fi = atan((y-ix1/256.)/(x-ix1/256.)); int r = fixmag( y-iy1/256, x-ix1/256), X = (int)(fabs(r*cos(fi+r/8))), \
      Y = (int)(fabs(r*sin(fi+r/8))), d = fixmag(X, Y); pclr = (const uchar*)(br->clrs+BOUND(d, 0, 255)); }
#include "imdrawaa_poly_case_cn.inl"
    }
    break;
  case br_lut_charicature:
    {
      int ix1 = br->args[0], iy1 = br->args[1], invrx = br->args[2], invry = br->args[3];
      double R = invrx/256.;
#define GETCLR_INIT(y)
#define GETCLR(x, y)  { double fi = atan((y-ix1/256.)/(x-ix1/256.)); \
      int r = fixmag( y-iy1/256, x-ix1/256); \
      int X = (int)(sqrt(r*R*2)*cos(fi)); \
      int Y = (int)(fabs(sqrt(r*R*2)*sin(fi))); \
      int d = fixmag(X, Y); pclr = (const uchar*)(br->clrs+BOUND(d, 0, 255)); }

#include "imdrawaa_poly_case_cn.inl"
    }
    break;
  case br_texture:
    {
      const img_t* tex = br->texture;
      const uchar* data = br->texture->tt.data;
      int* xoff = MALLOC(int, im->w);
      int* yoff = MALLOC(int, im->h);
      uchar backclr[4] = {0, 0, 0, 0};
      uchar bclr[4] = {255, 255, 255, 255};
      wrap_index(xoff, im->w, tex->w, 0, tex->c, br->args[0]);
      wrap_index(yoff, im->h, tex->h, 0, tex->s, br->args[1]);
      if (wrap_mode_clamp==br->args[0] || wrap_mode_clamp==br->args[1]) {
#define GETTEXCLR1(x, y)  ((xoff[x]>=0 && yoff[y]>=0) ? data + xoff[x] + yoff[y] : backclr)
        switch (tex->c) {
        case 4:
#define GETCLR_INIT(y)
#define GETCLR(x, y)  {pclr = GETTEXCLR1(x, y);}
#include "imdrawaa_poly_case_cn.inl"
          break;
        case 3:
#define GETCLR_INIT(y)
#define GETCLR(x, y)  {pclr = GETTEXCLR1(x, y); bclr[0]=pclr[0], bclr[1]=pclr[1], bclr[2]=pclr[2]; pclr = bclr;}
#include "imdrawaa_poly_case_cn.inl"
          break;
        case 2:
#define GETCLR_INIT(y)
#define GETCLR(x, y)  {pclr = GETTEXCLR1(x, y); bclr[0]=bclr[1]=bclr[2]=pclr[0], bclr[3]=pclr[1]; pclr = bclr;}
#include "imdrawaa_poly_case_cn.inl"
          break;
        case 1:
#define GETCLR_INIT(y)
#define GETCLR(x, y)  {pclr = GETTEXCLR1(x, y); bclr[0]=bclr[1]=bclr[2]=pclr[0]; pclr = bclr;}
#include "imdrawaa_poly_case_cn.inl"
          break;
        default:
          ASSERT(0);
          break;
        }
#undef GETTEXCLR1
      } else {
#define GETTEXCLR1(x, y)  (data + xoff[x] + yoff[y])
        switch (tex->c) {
        case 4:
#define GETCLR_INIT(y)
#define GETCLR(x, y)  {pclr = GETTEXCLR1(x, y);}
#include "imdrawaa_poly_case_cn.inl"
          break;
        case 3:
#define GETCLR_INIT(y)
#define GETCLR(x, y)  {pclr = GETTEXCLR1(x, y); bclr[0]=pclr[0], bclr[1]=pclr[1], bclr[2]=pclr[2]; pclr = bclr;}
#include "imdrawaa_poly_case_cn.inl"
          break;
        case 2:
#define GETCLR_INIT(y)
#define GETCLR(x, y)  {pclr = GETTEXCLR1(x, y); bclr[0]=bclr[1]=bclr[2]=pclr[0], bclr[3]=pclr[1]; pclr = bclr;}
#include "imdrawaa_poly_case_cn.inl"
          break;
        case 1:
#define GETCLR_INIT(y)
#define GETCLR(x, y)  {pclr = GETTEXCLR1(x, y); bclr[0]=bclr[1]=bclr[2]=pclr[0]; pclr = bclr;}
#include "imdrawaa_poly_case_cn.inl"
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
    //ASSERT(0);
    break;
  }
}
#undef SCANCELL_SET
