typedef struct brush_t brush_t;
struct brush_t {
  int type;
  COLOR alpha_mask;
  uchar alpha_lut[256];
  COLOR clrs[256];
  int args[16];
  img_t texture[1];
  int inited;
};
/////////////////////////////////////////////////////////////////////////////////////////
CC_INLINE int brush_init(brush_t* br)
{
  int i;
  if (0 == br->inited) {
    br->inited = 1;
    br->alpha_mask = _rgba(1, 1, 1, 1);
    for (i = 0; i < 256; ++i) {
      br->alpha_lut[i] = i;
    }
  }
  return 0;
}
#define XBRUSH_PUTPIX_INIT(br) int inited = brush_init(br)
CC_INLINE int brush_setaa(brush_t* br, int isaa)
{
  int i;
  XBRUSH_PUTPIX_INIT(br);
  if (isaa) {
    for (i = 0; i < 256; ++i) {
      br->alpha_lut[i] = i;
    }
  }
  else {
    for (i = 0; i < 256; ++i) {
      br->alpha_lut[i] = i < 128 ? 0 : 255;
    }
  }
  return 0;
}
CC_INLINE int brush_set_solid(brush_t* br, COLOR clr)
{
  XBRUSH_PUTPIX_INIT(br, solid);
  br->clrs[0] = clr;
  return 0;
}
CC_INLINE int brush_set_hatch(brush_t* br, int fill_area_x, int fill_area_y, COLOR forecol, COLOR backcol, const uchar* hatch_data)
{
  XBRUSH_PUTPIX_INIT(br, hatch);
  int i = 0;
  br->clrs[i++] = forecol;
  br->clrs[i++] = backcol;
  i = 0;
  br->args[i++] = fill_area_x;
  br->args[i++] = fill_area_y;
  br->args[i++] = (int)hatch_data;
  return 0;
}
CC_INLINE int brush_set_lut_gradient(brush_t* br, double x1, double y1, double x2, double y2, const COLOR* clrs)
{
  XBRUSH_PUTPIX_INIT(br);
  int alpha0 = 255;
  double ds = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
  double dx = (x2 - x1) / ds;
  double dy = (y2 - y1) / ds;
  int it = -(int)((x1 * dx + y1 * dy) * 256);
  int ix = (int)(dx * 256 * 256);
  int iy = (int)(dy * 256 * 256);
  int n1 = MAX(1, (int)(ds + 0.0001));
  br->args[0] = ix, br->args[1] = iy, br->args[2] = it;
  if (br->clrs != clrs) {
    MEMCPY(br->clrs, clrs, 256);
  }
  br->type = br_lut_gradient;
  return 0;
}
CC_INLINE int brush_set_gradient(brush_t* br, double x1, double y1, double x2, double y2, COLOR clr1, COLOR clr2)
{
  XBRUSH_PUTPIX_INIT(br);
  fill_color_array(256, br->clrs, clr1, clr2);
  brush_set_lut_gradient(br, x1, y1, x2, y2, br->clrs);
  return 0;
}
CC_INLINE int brush_set_gradient2(brush_t* br, double x1, double y1, double x2, double y2, int m, const COLOR* clrs, const double* pos)
{
  XBRUSH_PUTPIX_INIT(br);
  fill_color_array2(256, br->clrs, m, clrs, pos);
  brush_set_lut_gradient(br, x1, y1, x2, y2, br->clrs);
  return 0;
}
CC_INLINE int brush_set_colormatrix(brush_t* br, const double* A)
{
  XBRUSH_PUTPIX_INIT(br);
  int iA[12], i;
  for (i = 0; i < 12; ++i) {
    br->args[i] = iA[i] = F2FIXN(A[i], 8);
  }
  br->type = br_colormatrix;
  return 0;
}
CC_INLINE int brush_set_lut_radial(brush_t* br, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  XBRUSH_PUTPIX_INIT(br);
  int ix1 = F2FIXN(x1, 0), iy1 = F2FIXN(y1, 0), invrx = F2FIXN(1 / rx, 16), invry = F2FIXN(1 / ry, 16);
  br->args[0] = ix1, br->args[1] = iy1, br->args[2] = invrx, br->args[3] = invry;
  MEMCPY(br->clrs, clrs, 256);
  br->type = br_lut_radial;
  return 0;
}
CC_INLINE int brush_set_radial(brush_t* br, double x1, double y1, double rx, double ry, COLOR clr1, COLOR clr2)
{
  XBRUSH_PUTPIX_INIT(br);
  fill_color_array(256, br->clrs, clr1, clr2);
  brush_set_lut_radial(br, x1, y1, rx, ry, br->clrs);
  return 0;
}
CC_INLINE int brush_set_lut_swirled(brush_t* br, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  XBRUSH_PUTPIX_INIT(br);
  int ix1 = F2FIXN(x1, 8), iy1 = F2FIXN(y1, 8), invrx = F2FIXN(rx, 8), invry = F2FIXN(ry, 8);
  br->args[0] = ix1, br->args[1] = iy1, br->args[2] = invrx, br->args[3] = invry;
  MEMCPY(br->clrs, clrs, 256);
  br->type = br_lut_swirled;
  return 0;
}
CC_INLINE int brush_set_lut_charicature(brush_t* br, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  XBRUSH_PUTPIX_INIT(br);
  int ix1 = F2FIXN(x1, 8), iy1 = F2FIXN(y1, 8), invrx = F2FIXN(rx, 8), invry = F2FIXN(ry, 8);
  br->args[0] = ix1, br->args[1] = iy1, br->args[2] = invrx, br->args[3] = invry;
  MEMCPY(br->clrs, clrs, 256);
  br->type = br_lut_charicature;
  return 0;
}
CC_INLINE int brush_set_lut_diamond(brush_t* br, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  XBRUSH_PUTPIX_INIT(br);
  int ix1 = F2FIXN(x1, 8), iy1 = F2FIXN(y1, 8), invrx = F2FIXN(1 / rx, 8), invry = F2FIXN(1 / ry, 8);
  br->args[0] = ix1, br->args[1] = iy1, br->args[2] = invrx, br->args[3] = invry;
  MEMCPY(br->clrs, clrs, 256);
  br->type = br_lut_diamond;
  return 0;
}
CC_INLINE int brush_set_texture(brush_t* br, int wrap_mode_x, int wrap_mode_y, int h, int w, const uchar* data, int al, int ai)
{
  XBRUSH_PUTPIX_INIT(br);
  IMINIT(br->texture, h, w, data, al, ai, 1);
  br->args[0] = wrap_mode_x;
  br->args[1] = wrap_mode_y;
  br->type = br_texture;
  return 0;
}
CC_INLINE int brush_set_gouraud(brush_t* br, double x1, double y1, double x2, double y2, double x3, double y3, COLOR c1, COLOR c2, COLOR c3)
{
  XBRUSH_PUTPIX_INIT(br);
  double x[12], A[9] = {0}, b[12];
  V9SET(A, x1, x2, x3, y1, y2, y3, 1, 1, 1);
  V12SET(b, GetRV(c1), GetRV(c2), GetRV(c3), GetGV(c1), GetGV(c2), GetGV(c3), GetBV(c1), GetBV(c2), GetBV(c3), GetAV(c1), GetAV(c2), GetAV(c3));
  slove3n(A, b, x, 4);
  brush_set_colormatrix(br, x);
  return 0;
}