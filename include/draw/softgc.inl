
#include "cmath.h"
#include "geo/geo.inl"
#include "font/font.inl"
#include "scancell.inl"
#include "brush.inl"
#include "scanline.inl"
//#include "region.inl"
#include "cmdgc.inl"
#include "tridraw.inl"

typedef void (*blender_type)(uchar* p, const uchar* s, unsigned width, const uchar* covers);

typedef struct {
  scanline sl[1];
  BOOL flip_y;
  uchar* covers;
  COLOR* colors;
  uchar m_gamma[256];
  img_t im[1];
  brush_t brush[1];
  brush_t pen[1];
  brush_t* br;
  PenStyle penstyle[1];
  float matrix[6];
  int textAlign;
  
  //Path path[1];
  float* zbuf;
  int swap_rb;
  FPOINT* stroke_buf;
  blender_type blender;
}softgc;

void softgc_setsize(scanline* s, IRECT rc, int n) {
  softgc* ss = (softgc*)(s->x);
  int w = RCW(&rc);
  MYREALLOC(ss->covers, w);
  MYREALLOC(ss->colors, w);
  MEMSET(ss->colors, 0, w);
}
void softgc_addspans(scanline* s, int b, int e, int y, const uchar* covers) {
  softgc* ss = (softgc*)(s->x);
  int len = e - b;
  img_t* im = s->im;
  COLOR* pix_x = (COLOR*)(im->tt.data + (ss->flip_y ? (im->h - y - 1) : y) * im->s + b * im->c);
  //memset(ss->colors, 0, len*sizeof(COLOR));
  ss->br->fillline(ss->br, ss->colors, b, e, y);
  ss->blender((uchar*)pix_x, (uchar*)ss->colors, len, covers);
}
void softgc_addspan(scanline* s, int b, int e, int y, int cover) {
  softgc* ss = (softgc*)(s->x);
  if(b>e) {
    //printf("adsfasdf\n");
  }
  cover = ((uint)cover)&0xff;
  if (ss->m_gamma) {
    cover = ss->m_gamma[cover];
  }
  memset(ss->covers, cover, e - b);
  softgc_addspans(s, b, e, y, ss->covers);
}
void softgc_free(softgc* ss) {
  FREE(ss->covers);
  FREE(ss->colors);
  FREE(ss->stroke_buf);
  //PathFree(ss->path);
  memset(ss, 0, sizeof(*ss));
}
void scanline_setSmoothingMode(softgc* ss, SmoothingMode smoothingMode) {
  scanline* s = ss->sl;
  //blender_type blender[];
  //ss->smoothingMode = SmoothingModeDefault;
  int cn = ss->im->c==3 ? 3 : ss->im->c==1 ? 1 : 4;
  switch (smoothingMode)
  {
  case QualityModeInvalid:
  case QualityModeLow:
  case SmoothingModeNone:
    ss->blender = cn==3 ? blender_rgb_bin : cn==1 ? blender_gray_bin : blender_rgba_bin;
    break;
  default:
  case QualityModeDefault:
  case QualityModeHigh:
  case SmoothingModeAntiAlias:
    ss->blender = cn==3 ? blender_rgb : cn==1 ? blender_gray : blender_rgba;
    break;
  }
}
int softgc_reset(softgc* ss) {
  int i;
  scanline* s = ss->sl;
  //ss->path->Count = 0;
  for (i=0; i<256; ++i) {
    ss->m_gamma[i] = i;
  }
  brush_set_solid(ss->brush, _rgb(0, 0, 0));
  brush_set_solid(ss->pen, _rgb(0, 0, 0));
  ss->flip_y = false;
  ss->br = ss->brush;
  iRectSet(s->clip, 0, 0, ss->im->w, ss->im->h);
  s->pclip = s->clip;
  s->x = ss;
  s->im = ss->im;
  PenStyle_Init(ss->penstyle);
  M23Set(ss->matrix);
  s->setsize = softgc_setsize;
  s->addspan = softgc_addspan;
  s->addspans = softgc_addspans;
  scanline_setSmoothingMode(ss, SmoothingModeAntiAlias);
  ss->brush->wrapX = ss->brush->wrapY = WrapModeDefault;
  return 0;
}
int scanline_set_path_stroke(scanline* sl, const FPOINT* Points, const BYTE* Types, int Count, PenStyle* penstyle) {
  PATH_LOOP_BEGIN;
  scanline_set_poly_dash_stroke(sl, Points + pos, end - pos, closed, penstyle);
  PATH_LOOP_END;
  return 0;
}
int scanline_set_path_stroke2(scanline* sl, const Path* path, PenStyle* penstyle) {
  Path flat_path[1] = {0};
  int ret = 0;
  if (!PathIsFlatten(path)) {
    PathFlatten(path, 0.01, flat_path);
    path = flat_path;
  }
  ret = scanline_set_path_stroke(sl, path->Points, path->Types, path->Count, penstyle);
  PathFree(flat_path);
  return ret;
}
int spanv_set_path(spanv_t* sp, img_t* im, const IRECT* pclip, const FPOINT* Points, const BYTE* Types, int Count, BOOL fill_even_odd) {
  int* plen = NULL;
  int nlen = GetPathLens(Types, Count, NULL);
  MYREALLOC(plen, nlen);
  nlen = GetPathLens(Types, Count, plen);
  spanv_set_poly(sp, im, NULL, Points, plen, nlen, fill_even_odd);
  FREE(plen);
  return 0;
}
int spanv_set_path2(spanv_t* sp, img_t* im, const IRECT* pclip, const Path* path, BOOL fill_even_odd) {
  Path flat_path[1] = {0};
  int ret = 0;
  if (!PathIsFlatten(path)) {
    PathFlatten(path, 0.01, flat_path);
    path = flat_path;
  }
  ret = spanv_set_path(sp, im, pclip, path->Points, path->Types, path->Count, fill_even_odd);
  PathFree(flat_path);
  return ret;
}
int scanline_set_path(scanline* sl, const FPOINT* Points, const BYTE* Types, int Count, BOOL fill_even_odd) {
  int* plen = NULL;
  int nlen = GetPathLens2(Types, Count, &plen);
  scanline_set_poly(sl, Points, plen, nlen, fill_even_odd);
  FREE(plen);
  return 0;
}
int scanline_set_path2(scanline* sl, const Path* path, BOOL fill_even_odd) {
  Path flat_path[1] = {0};
  int ret = 0;
  if (!PathIsFlatten(path)) {
    PathFlatten(path, 0.01, flat_path);
    path = flat_path;
  }
  ret = scanline_set_path(sl, path->Points, path->Types, path->Count, fill_even_odd);
  PathFree(flat_path);
  return ret;
}

#if 0
int region_brush(region_t* re, img_t* im, bool flip_y, const brush_t* br)
{
  int y, i;
  IRECT clip = iRectClip(&re->extents, 0, 0, im->w, im->h);
  uchar* covers = (uchar*)pmalloc(RCW(&clip));
  memset(covers, 255, RCW(&clip));
  ASSERT(4 == im->c);
  for (i = 0; i < re->numRects; ++i) {
    IRECT rc = iRectInter(re->rects[i], clip);
    for (y = rc.t; y < rc.b; ++y) {
      COLOR* pix_x = (COLOR*)(im->tt.data + (flip_y ? (im->h - y - 1) : y) * im->s + rc.l * im->c);
      br->fillline(br, pix_x, rc.t, rc.b, y, covers);
    }
  }
  pfree(covers);
  return 0;
}
int spanv_solid(spanv_t* sp, img_t* im, bool flip_y, COLOR clr)
{
  brush_t br[1] = {0};
  brush_set_solid(br, clr);
  spanv_brush(sp, im, flip_y, br);
  return 0;
}
int region_solid(region_t* re, img_t* im, bool flip_y, COLOR clr)
{
  brush_t br[1] = {0};
  brush_set_solid(br, clr);
  region_brush(re, im, flip_y, br);
  return 0;
}
int spanv_hatch(spanv_t* sp, img_t* im, bool flip_y, int fill_area_x, int fill_area_y, COLOR forecol, COLOR backcol, const uchar* hatch_data)
{
  brush_t br[1] = {0};
  brush_set_hatch(br, fill_area_x, fill_area_y, forecol, backcol, hatch_data);
  spanv_brush(sp, im, flip_y, br);
  return 0;
}
int region_hatch(region_t* re, img_t* im, bool flip_y, int fill_area_x, int fill_area_y, COLOR forecol, COLOR backcol, const uchar* hatch_data)
{
  brush_t br[1] = {0};
  brush_set_hatch(br, fill_area_x, fill_area_y, forecol, backcol, hatch_data);
  region_brush(re, im, flip_y, br);
  return 0;
}
int spanv_lut_gradient(spanv_t* sp, img_t* im, bool flip_y, double x1, double y1, double x2, double y2, const COLOR* clrs)
{
  brush_t br[1] = {0};
  brush_set_lut_gradient(br, x1, y1, x2, y2, clrs);
  spanv_brush(sp, im, flip_y, br);
  return 0;
}
CC_INLINE int spanv_gradient(spanv_t* sp, img_t* im, bool flip_y, double x1, double y1, double x2, double y2, COLOR clr1, COLOR clr2)
{
  COLOR clrs[256];
  fill_color_array(256, clrs, clr1, clr2);
  return spanv_lut_gradient(sp, im, flip_y, x1, y1, x2, y2, clrs);
}
CC_INLINE int spanv_gradient2(spanv_t* sp, img_t* im, bool flip_y, double x1, double y1, double x2, double y2, int m, const COLOR* clrs, const float* pos)
{
  COLOR clrs2[256];
  fill_color_array2(256, clrs2, m, clrs, pos);
  spanv_lut_gradient(sp, im, flip_y, x1, y1, x2, y2, clrs2);
  return 0;
}
#endif
int draw_textureR(img_t* im, const IRECT* pclip, IRECT drc, const texture_t* tex1, const IRECT* prcSrc, double* m23, int alphamode, int flip_y, COLOR bgclr, WrapMode wrapX, WrapMode wrapY)
{
  int x, y;
  double m23_[] = {1, 0, 0, 1, 0, 0};
  IRECT rcSrc_ = {0, 0, tex1->w, tex1->h};
  IRECT clip = iRectClip(pclip, 0, 0, im->w, im->h);
  IRECT rect = iRectInter(clip, drc);
  prcSrc = prcSrc ? prcSrc : &rcSrc_;
  if (m23) {
    MEMCPY(m23_, m23, 6);
  }
  ASSERT(4 == im->c);
  if (RCH(&rect) > 0 && RCW(&rect) > 0 && RCW(prcSrc) > 0 && RCH(prcSrc) > 0) {
    int sw = RCW(prcSrc), sh = RCH(prcSrc);
    const uchar* srcpix = tex1->data + prcSrc->t * tex1->s + prcSrc->l * 3;
    uchar* img = im->tt.data;
    int step = im->s;
    int im23[6];
    uchar clr1[4] = {0};
    uchar* pclr = (uchar*)&bgclr;
    struct pixfun_t* fun = get_pixfun(tex1->fmt);
    if (NULL==fun) {
      return 0;
    }
    //int bpp = PixelFormatBpp(tex1->fmt);
    if (!m23_invert(m23_)) {
      return 0;
    }
    for (x = 0; x < 6; ++x) {
      im23[x] = (int)(m23_[x] * (1 << 16));
    }
    {
      for (y = rect.t; y < rect.b; ++y) {
        uchar* ptr = img + (flip_y ? (im->h - y - 1) : y) * step;
        uchar* ptr_x = (uchar*)(ptr + rect.l * 4);
        for (x = rect.l; x < rect.r; ++x, ptr_x += 4) {
          int xsrc = (x * im23[0] + y * im23[2] + im23[4]) >> 16;
          int ysrc = (x * im23[1] + y * im23[3] + im23[5]) >> 16;
          xsrc = WrapPix1D(xsrc, sw, wrapX);
          ysrc = WrapPix1D(ysrc, sh, wrapY);
          fun->getpixel((COLOR*)clr1, tex1->data + ysrc*tex1->s, xsrc, NULL);
          if (clr1[3]==255) {
            ptr_x[0] = clr1[0];
            ptr_x[1] = clr1[1];
            ptr_x[2] = clr1[2];
            ptr_x[3] = clr1[3];
          }
          else {
            PIXBLENDA3(ptr_x, pclr[3]);
          }
        }
      }
    }
  }
  return 0;
}
int draw_textureR1(img_t* im, const IRECT* pclip, IRECT drc, const texture_t* tex1, const IRECT* prcSrc, int alphamode, int flip_y, COLOR clr)
{
  int x, y;
  IRECT rcSrc = {0, 0, tex1->w, tex1->h};
  IRECT clip = iRectClip(pclip, 0, 0, im->w, im->h);
  IRECT rect = iRectInter(clip, drc);
  prcSrc = prcSrc ? prcSrc : &rcSrc;
  ASSERT(4 == im->c);
  if (RCH(&rect) > 0 && RCW(&rect) > 0 && RCW(&rcSrc) > 0 && RCH(&rcSrc) > 0) {
    int* xoff = NULL, * yoff = NULL;
    char* buf = NULL;
    int h = im->h;
    uchar* img = im->tt.data;
    int step = im->s;
    uchar* pclr = (uchar*)&clr;
    int bpp = PixelFormatBpp(tex1->fmt);
    multialloc(&buf, &xoff, RCW(&rect)*sizeof(int), &yoff, RCH(&rect)*sizeof(int), NULL);
    for (x = rect.l; x < rect.r; ++x) {
      xoff[x - rect.l] = (int)((x - drc.l + 0.5) * RCW(&rcSrc)/ RCW(&drc));
    }
    for (y = rect.t; y < rect.b; ++y) {
      yoff[y - rect.t] = (int)((y - drc.t + 0.5) * RCH(&rcSrc)/ RCH(&drc));
    }
    switch (bpp) {
    case 1:
      if (alphamode) {
        for (y = rect.t; y < rect.b; ++y) {
          uchar* ptr_x = img + (flip_y ? (h - y - 1) : y) * step + rect.l * 4;
          uchar* psrc = tex1->data + yoff[y - rect.t] * tex1->s;
          uchar* bit = psrc;
          for (x = rect.l; x < rect.r; ++x, ptr_x += 4) {
            int k = xoff[x - rect.l];
            if ((bit[k >> 3] >> (7 - (k & 7))) & 1) {
              PIXBLENDA3(ptr_x, 255);
            }
          }
        }
      }
      else {
        for (y = rect.t; y < rect.b; ++y) {
          uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
          uint32* ptr_x = (uint32*)(ptr + rect.l * 4);
          uchar* psrc = tex1->data + yoff[y - rect.t] * tex1->s;
          uchar* bit = psrc;
          for (x = rect.l; x < rect.r; ++x) {
            int k = xoff[x - rect.l];
            if ((bit[k >> 3] >> (7 - (k & 7))) & 1) {
              *ptr_x++ = 0xffffffff;
            }
          }
        }
      }
      break;
    case 8:
      if (alphamode) {
        for (y = rect.t; y < rect.b; ++y) {
          uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
          uchar* ptr_x = ptr + rect.l * 4;
          uchar* psrc = tex1->data + yoff[y - rect.t] * tex1->s;
          for (x = rect.l; x < rect.r; ++x, ptr_x += 4) {
            uchar alpha = psrc[xoff[x - rect.l]];
            PIXBLENDA3(ptr_x, alpha);
          }
        }
      }
      else {
        for (y = rect.t; y < rect.b; ++y) {
          uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
          uint32* ptr_x = (uint32*)(ptr + rect.l * 4);
          uchar* psrc = tex1->data + yoff[y - rect.t] * tex1->s;
          for (x = rect.l; x < rect.r; ++x) {
            uchar gry = psrc[xoff[x - rect.l]];
            *ptr_x++ = _RGB(gry, gry, gry);
          }
        }
      }
      break;
    case 24:
      if (alphamode) {
        for (y = rect.t; y < rect.b; ++y) {
          uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
          uchar* ptr_x = (uchar*)(ptr + rect.l * 4);
          uchar* psrc = (uchar*)(tex1->data + yoff[y - rect.t] * tex1->s);
          for (x = rect.l; x < rect.r; ++x, ptr_x += 4) {
            uchar* clr1 = psrc + 3 * xoff[x - rect.l];
            uchar alpha = clr1[0];
            PIXBLENDA3(ptr_x, alpha);
          }
        }
      }
      else {
        //show_bitmap("asdf", tex1->h, tex1->w, tex1->data, tex1->s, PF_32bppARGB, 1, 1);waitkey(-1);
        for (y = rect.t; y < rect.b; ++y) {
          uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
          uint32* ptr_x = (uint32*)(ptr + rect.l * 4);
          uchar* psrc = (uchar*)(tex1->data + yoff[y - rect.t] * tex1->s);
          for (x = rect.l; x < rect.r; ++x) {
            uchar* clr1 = psrc + 3 * xoff[x - rect.l];
            *ptr_x++ = _RGB(clr1[2], clr1[1], clr1[0]);
          }
        }
      }
      break;
    case 32:
      if (alphamode) {
        for (y = rect.t; y < rect.b; ++y) {
          uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
          uchar* ptr_x = (uchar*)(ptr + rect.l * 4);
          uint32* psrc = (uint32*)(tex1->data + yoff[y - rect.t] * tex1->s);
          for (x = rect.l; x < rect.r; ++x, ptr_x += 4) {
            uint32 clr1 = psrc[xoff[x - rect.l]];
            uchar alpha = GetAV(clr1);
            PIXBLENDA3(ptr_x, alpha);
          }
        }
      }
      else {
        //show_bitmap("asdf", tex1->h, tex1->w, tex1->data, tex1->s, PF_32bppARGB, 1, 1);waitkey(-1);
        for (y = rect.t; y < rect.b; ++y) {
          uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
          uchar* ptr_x = (uchar*)(ptr + rect.l * 4);
          uint32* psrc = (uint32*)(tex1->data + yoff[y - rect.t] * tex1->s);
          for (x = rect.l; x < rect.r; ++x, ptr_x += 4) {
            const uchar* pclr = (const uchar*)&psrc[xoff[x - rect.l]];
            PIXBLENDA3(ptr_x, pclr[3]);
          }
        }
      }
      break;
    default:
      ASSERT(0);
      break;
    }
    FREE(buf);
  }
  return 0;
}
int spanv_ellipse(spanv_t* sp, img_t* im, bool flip_y, const IRECT* pclip, BOOL m_cw, double m_x, double m_y, double m_rx, double m_ry)
{
  FPOINT pt[64];
  int n = vcgen_ellipse(pt, countof(pt), m_cw, m_x, m_y, m_rx, m_ry);
  return spanv_set_poly(sp, im, pclip, pt, &n, 1, false);
}
int spanv_ellipseR(spanv_t* sp, img_t* im, bool flip_y, const IRECT* pclip, IRECT rc)
{
  return spanv_ellipse(sp, im, flip_y, pclip, 0, RCX(&rc) * .5, RCY(&rc) * .5, RCW(&rc) * .5, RCH(&rc) * .5);
}

int draw_rasterize_text1(img_t* im, const IRECT* pclip, brush_t* br, int flip_y,
                        const PrimRectUV* prcuv, int nrcuv, const bitmap_t* tex1, int alphamode, int nvtx, ImVertex* vtx) {
  ASSERT(0);

#if 0
  if (nvtx > 0 && vtx) {
    int i;
    for (i = 0; i < nvtx; ++i) {
      int x = (int)vtx[i].pos.x;
      int y = (int)vtx[i].pos.y;
      //br->inity(br, y);
      //fun->inity(br, y);
      //GETCLR_INIT(y);
      //GETCLR(x, y);
      //vtx[i].col = (COLOR)br->clrs[0];
    }
  }
  
  if (nrcuv > 0 && prcuv && tex1) {
    IMARGDEFTT;
    int i, j, x, y;
    void* buf = NULL;
    int* xoff = NULL;
    int* yoff = NULL;
    COLOR* row = NULL;
    uchar* covers = NULL;
    IRECT clip = {0, 0, w, h}, rect;
    if (pclip) {
      clip = iRectInter(clip, *pclip);
    }
    for (i = 0; i < nrcuv; ++i) {
      IRECT drc = iRECT((int)prcuv[i].a.x, (int)prcuv[i].a.y, (int)prcuv[i].c.x, (int)prcuv[i].c.y);
      int dw = RCW(&drc), dh = RCH(&drc);
      ImVec2 uv_a = prcuv[i].uv_a;
      ImVec2 uv_c = prcuv[i].uv_c;
      float uv_w = (uv_c.x - uv_a.x) / dw;
      float uv_h = (uv_c.y - uv_a.y) / dh;
      rect = iRectInter(clip, drc);
      multialloc(&buf, &xoff, dw*sizeof(int), &yoff, dh*sizeof(int), &row, dw*sizeof(COLOR), &covers, dw, NULL);
      for (j = 0; j < dw; ++j) {
        xoff[j] = (int)((uv_a.x + uv_w * j) * tex1->w);
      }
      for (j = 0; j < dh; ++j) {
        yoff[j] = (int)((uv_a.y + uv_h * j) * tex1->h);
      }
      memset(covers, 255, dw);
      if (RCH(&rect) > 0 && RCW(&rect) > 0) {
        switch (tex1->bpp) {
        case 1:
          if (alphamode) {
            for (y = rect.t; y < rect.b; ++y) {
              uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
              uchar* pix_x = (ptr + rect.l * 4);
              uchar* psrc = tex1->data + yoff[y - drc.t] * tex1->s;
              uchar* bit = psrc;
              uchar* pclr = (uchar*)row;
              br->fillline(br, row, rect.l, rect.r, y);
              for (x = rect.l; x < rect.r; ++x, pix_x+=4, pclr+=4) {
                int k = xoff[x - drc.l];
                if ((bit[k >> 3] >> (7 - (k & 7))) & 1) {
                  PIXBLENDA4(pix_x, 255);
                }
              }
            }
          } else {
            for (y = rect.t; y < rect.b; ++y) {
              uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
              uint32* pix_x = (uint32*)(ptr + rect.l * 4);
              uchar* psrc = tex1->data + yoff[y - drc.t] * tex1->s;
              uchar* bit = psrc;
              for (x = rect.l; x < rect.r; ++x) {
                int k = xoff[x - drc.l];
                if ((bit[k >> 3] >> (7 - (k & 7))) & 1) {
                  *pix_x++ = 0xffffffff;
                }
              }
            }
          }
          break;
        case 8:
          if (alphamode) {
            for (y = rect.t; y < rect.b; ++y) {
              uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
              uchar* pix_x = ptr + rect.l * 4;
              uchar* psrc = tex1->data + yoff[y - drc.t] * tex1->s;
              uchar* pclr = (uchar*)row;
              br->fillline(br, row, rect.l, rect.r, y, covers);
              for (x = rect.l; x < rect.r; ++x, pix_x += 4, pclr+=4) {
                uchar alpha = psrc[xoff[x - drc.l]];
                PIXBLENDA4(pix_x, alpha);
              }
            }
          } else {
            for (y = rect.t; y < rect.b; ++y) {
              uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
              uint32* pix_x = (uint32*)(ptr + rect.l * 4);
              uchar* psrc = tex1->data + yoff[y - drc.t] * tex1->s;
              for (x = rect.l; x < rect.r; ++x) {
                uchar gry = psrc[xoff[x - drc.l]];
                *pix_x++ = _RGB(gry, gry, gry);
              }
            }
          }
          break;
        case 24:
          if (alphamode) {
            for (y = rect.t; y < rect.b; ++y) {
              uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
              uchar* pix_x = (uchar*)(ptr + rect.l * 4);
              uchar* psrc = (uchar*)(tex1->data + yoff[y - drc.t] * tex1->s);
              uchar* pclr = (uchar*)row;
              br->fillline(br, row, rect.l, rect.r, y, covers);
              for (x = rect.l; x < rect.r; ++x, pix_x += 4, pclr += 4) {
                uchar* clr1 = psrc+3*xoff[x - drc.l];
                uchar alpha = clr1[0];
                PIXBLENDA4(pix_x, alpha);
              }
            }
          } else {
            //show_bitmap("asdf", tex1->h, tex1->w, tex1->data, tex1->s, PF_32bppARGB, 1, 1);waitkey(-1);
            for (y = rect.t; y < rect.b; ++y) {
              uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
              uint32* pix_x = (uint32*)(ptr + rect.l * 4);
              uchar* psrc = (uchar*)(tex1->data + yoff[y - drc.t] * tex1->s);
              for (x = rect.l; x < rect.r; ++x) {
                uchar* clr1 = psrc+3*xoff[x - drc.l];
                *pix_x++ = _RGB(clr1[0], clr1[1], clr1[2]);
              }
            }
          }
          break;
        case 32:
          if (alphamode) {
            for (y = rect.t; y < rect.b; ++y) {
              uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
              uchar* pix_x = (uchar*)(ptr + rect.l * 4);
              uint32* psrc = (uint32*)(tex1->data + yoff[y - drc.t] * tex1->s);
              uchar* pclr = (uchar*)row;
              br->fillline(br, row, rect.l, rect.r, y, covers);
              for (x = rect.l; x < rect.r; ++x, pix_x += 4, pclr += 4) {
                uint32 clr1 = psrc[xoff[x - drc.l]];
                uchar alpha = GetAV(clr1);
                PIXBLENDA4(pix_x, alpha);
              }
            }
          } else {
            //show_bitmap("asdf", tex1->h, tex1->w, tex1->data, tex1->s, PF_32bppARGB, 1, 1);waitkey(-1);
            for (y = rect.t; y < rect.b; ++y) {
              uchar* ptr = img + (flip_y ? (h - y - 1) : y) * step;
              uchar* pix_x = (uchar*)(ptr + rect.l * 4);
              uint32* psrc = (uint32*)(tex1->data + yoff[y - drc.t] * tex1->s);
              for (x = rect.l; x < rect.r; ++x, pix_x += 4) {
                const uchar* pclr = (const uchar*)&psrc[xoff[x - drc.l]];
                uchar alpha = pclr[3];
                PIXBLENDA4(pix_x, alpha);
              }
            }
          }
          break;
        default:
          ASSERT(0);
          break;
        }
      }
    }
    FREE(buf);
  }
#endif
  return 0;
}

int draw_rasterize1(img_t* im, const IRECT* pclip, brush_t* br, scancell* s, bool flip_y,
                   int opt, const FPOINT* pt0, const int* lens, int n, const PrimRectUV* prcuv, int nrcuv, const bitmap_t* tex1, int alphamode, int nvtx, ImVertex* vtx, spanv_t* sg) {
  
#if 0
  if (nrcuv > 0 && prcuv && tex1) {
    draw_rasterize_text1(im, pclip, br, flip_y, prcuv, nrcuv, tex1, alphamode, nvtx, vtx);
    return 0;
  }
  
  if (n > 0 && im && (im->h * im->w * im->c) > 0) {
    //static spanv_t sg[1] = {0};
    spanv_set_poly(sg, im, pclip, pt0, lens, n);
    spanv_brush(sg, im, flip_y, br);
    //spanv_free(sg);

    draw2d_poly_brush(im, pclip, flip_y, pt0, lens, n, br, flip_y);
  }
#endif
  return 0;
}
void PrimRectUV_RenderChar(PrimRectUV* g, const texfont_t* font, float size, ImVec2 pos, unsigned short c)
{
  const font_glyph_t* glyph;
  if (c == ' ' || c == '\t' || c == '\n' || c == '\r') { // Match behavior of RenderText(), those 4 codepoints are hard-coded.
    return;
  }
  glyph = texfont_FindGlyph(font, c);
  if (glyph) {
    ImVec2 pos_tl, pos_br;
    float scale = (size > 0.0f) ? (size / font->size) : 1.0f;
    pos.x = (float)(int)pos.x + font->DisplayOffset.x;
    pos.y = (float)(int)pos.y + font->DisplayOffset.y;
    pos_tl = fVec2(pos.x + glyph->X0 * scale, pos.y + glyph->Y0 * scale);
    pos_br = fVec2(pos.x + glyph->X1 * scale, pos.y + glyph->Y1 * scale);
    PrimRectUV_set(g, pos_tl, pos_br, fVec2(glyph->U0, glyph->V0), fVec2(glyph->U1, glyph->V1));
  }
}
int PrimRectUV_RenderText(PrimRectUV* g, const texfont_t* font, float size, ImVec2 pos, FRECT clip_rect, const char* text_begin, const char* text_end, float wrap_width, bool cpu_fine_clip)
{
  PrimRectUV* g0 = g;
  float x, y;
  float scale;
  float line_height;
  bool word_wrap_enabled;
  const char* word_wrap_eol = NULL;
  const char* s;
  if (!text_end) {
    text_end = text_begin + strlen(text_begin);
  }
  // Align to be pixel perfect
  pos.x = (float)(int)pos.x + font->DisplayOffset.x;
  pos.y = (float)(int)pos.y + font->DisplayOffset.y;
  x = pos.x;
  y = pos.y;
  if (y > clip_rect.b) {
    return 0;
  }
  scale = (size > 0.0f) ? (size / font->size) : 1.0f;
  line_height = font->size * scale;
  word_wrap_enabled = (wrap_width > 0.0f);
  // Skip non-visible lines
  s = text_begin;
  if (!word_wrap_enabled && y + line_height < clip_rect.t) {
    while (s < text_end && *s != '\n') { // Fast-forward to next line
      s++;
    }
  }
  {
    // Reserve vertices for remaining worse case (over-reserving is useful and easily amortized)
    while (s < text_end) {
      unsigned int c;
      float char_width;
      const font_glyph_t* glyph;
      if (word_wrap_enabled) {
        // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
        if (!word_wrap_eol) {
          word_wrap_eol = texfont_CalcWordWrapPositionA(font, scale, s, text_end, wrap_width - (x - pos.x));
          if (word_wrap_eol == s) { // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
            word_wrap_eol++; // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
          }
        }
        if (s >= word_wrap_eol) {
          x = pos.x;
          y += line_height;
          word_wrap_eol = NULL;
          // Wrapping skips upcoming blanks
          while (s < text_end) {
            const char c = *s;
            if (ImCharIsSpace(c)) {
              s++;
            }
            else if (c == '\n') {
              s++;
              break;
            }
            else {
              break;
            }
          }
          continue;
        }
      }
      // Decode and advance source
      c = (unsigned int) * s;
      if (c < 0x80) {
        s += 1;
      }
      else {
        s += TextCharFromUtf8(&c, s, text_end);
        if (c == 0) {
          break;
        }
      }
      if (c < 32) {
        if (c == '\n') {
          x = pos.x;
          y += line_height;
          if (y > clip_rect.b) {
            break;
          }
          if (!word_wrap_enabled && y + line_height < clip_rect.t)
            while (s < text_end && *s != '\n') { // Fast-forward to next line
              s++;
            }
          continue;
        }
        if (c == '\r') {
          continue;
        }
      }
      char_width = 0.0f;
	  glyph = texfont_FindGlyph(font, (unsigned short)c);
      if (glyph) {
        char_width = glyph->XAdvance * scale;
        // Arbitrarily assume that both space and tabs are empty glyphs as an optimization
        if (c != ' ' && c != '\t') {
          // We don't do a second finer clipping test on the Y axis as we've already skipped anything before clip_rect.t and exit once we pass clip_rect.b
          float x1 = x + glyph->X0 * scale;
          float x2 = x + glyph->X1 * scale;
          float y1 = y + glyph->Y0 * scale;
          float y2 = y + glyph->Y1 * scale;
          if (x1 <= clip_rect.r && x2 >= clip_rect.l) {
            // Render a character
            float u1 = glyph->U0;
            float v1 = glyph->V0;
            float u2 = glyph->U1;
            float v2 = glyph->V1;
            // CPU side clipping used to fit text in their frame when the frame is too small. Only does clipping for axis aligned quads.
            if (cpu_fine_clip) {
              if (x1 < clip_rect.l) {
                u1 = u1 + (1.0f - (x2 - clip_rect.l) / (x2 - x1)) * (u2 - u1);
                x1 = clip_rect.l;
              }
              if (y1 < clip_rect.t) {
                v1 = v1 + (1.0f - (y2 - clip_rect.t) / (y2 - y1)) * (v2 - v1);
                y1 = clip_rect.t;
              }
              if (x2 > clip_rect.r) {
                u2 = u1 + ((clip_rect.r - x1) / (x2 - x1)) * (u2 - u1);
                x2 = clip_rect.r;
              }
              if (y2 > clip_rect.b) {
                v2 = v1 + ((clip_rect.b - y1) / (y2 - y1)) * (v2 - v1);
                y2 = clip_rect.b;
              }
              if (y1 >= y2) {
                x += char_width;
                continue;
              }
            }
            // We are NOT calling PrimRectUV() here because non-inlined causes too much overhead in a debug build.
            // Inlined here:
            {
              PrimRectUV_set(g++, fVec2(x1, y1), fVec2(x2, y2), fVec2(u1, v1), fVec2(u2, v2));
            }
          }
        }
      }
      x += char_width;
    }
  }
  // Give back unused vertices
  return g - g0;
}


#ifdef _WIN32
//#define soft_color(clr)   ((g->swap_rb) ? SWAPRB(clr) : (clr))
#define soft_color(clr)   (clr)
#else
#define soft_color(clr)   SWAPRB(clr)
#endif

int cmdgc_init(gc_t* g) {
  static gcfun_t cmdgc_fun = {
#define GCFUNDEF(RET, NAME, ARG)   cmd##NAME,
    //GCFUNDEF_DEF(GCFUNDEF)
#undef GCFUNDEF
    0
  };
  g->fun = &cmdgc_fun;
  return 0;
}

int softgc_init(softgc* g, int swap_rb)
{
  g->swap_rb = swap_rb;
  softgc_reset(g);
  return 0;
}

CC_INLINE int softgc_rander_text1(softgc* g, const PrimRectUV* prcuv, int nrcuv, texture_t* tex)
{
  //draw_rasterize(g->im, g->clip, g->brush, g->s, g->flip_y, 0, NULL, 0, 0, prcuv, nrcuv, tex, 1, 0, 0, g->sp);
  //draw_rasterize_text(g->im, &g->clip, g->brush, g->flip_y, prcuv, nrcuv, tex, 1, 0, NULL);
  return 0;
}
CC_INLINE int softgc_stroke(softgc* g, const FPOINT* pt0, int ptlen, int closed)
{
  return 0;
}

CC_INLINE int softgc_dash_stroke(softgc* g, const FPOINT* pt, int ptlen, int closed, int m_dash_start, int m_num_dashes, const float* m_dashes)
{
  FPOINT pt0[1000];
  int lens[1000];
  int i, k = 0, n = vcgen_dash(pt, ptlen, closed, m_dashes, m_num_dashes, m_dash_start, countof(pt0), countof(lens), pt0, lens);
  
  for (i = 0; i < n; i++) {
    softgc_stroke(g, pt0 + k, lens[i], 0);
    k += lens[i];
  }
  
  return 0;
}

#define SOFTGC_ARG     softgc* sg = (softgc*)g->x
CC_INLINE void sgSolid(gc_t* g, int opt, COLOR clr)
{
  SOFTGC_ARG;
  clr = soft_color(clr);
  if (GcOptFill&opt) {
    brush_set_solid(sg->brush, clr);
  }
  if (GcOptStroke&opt) {
    brush_set_solid(sg->pen, clr);
  }
}
CC_INLINE void sgHatch(gc_t* g, int opt, HatchStyle hatchstyle, COLOR forecol, COLOR backcol, int x, int y)
{
  SOFTGC_ARG;
  forecol = soft_color(forecol);
  backcol = soft_color(backcol);
  if (GcOptFill&opt) {
    brush_set_hatch(sg->brush, x, y, forecol, backcol, g_hatch_datas[hatchstyle]);
  }
  if (GcOptStroke&opt) {
    brush_set_hatch(sg->pen, x, y, forecol, backcol, g_hatch_datas[hatchstyle]);
  }
}

//用于填充绘图的渐变对象（线性或放射性）
CC_INLINE void sgGradient(gc_t* g, int opt, float x0, float y0, float x1, float y1, const float* blendPositions, const COLOR* presetColors, int count)
{
  SOFTGC_ARG;
  if (0 == count) {
    return ;
  }
  else if (1 == count) {
    gcSolid(g, opt, presetColors[0]);
  }
  else {
    if (GcOptFill&opt) {
      brush_set_gradient2(sg->brush, x0, y0, x1, y1, count, presetColors, blendPositions);
    }
    if (GcOptStroke&opt) {
      brush_set_gradient2(sg->pen, x0, y0, x1, y1, count, presetColors, blendPositions);
    }
  }
}
CC_INLINE void sgRadial(gc_t* g, int opt, float x1, float y1, float rx, float ry, const float* blendPositions, const COLOR* presetColors, int count)
{
  SOFTGC_ARG;
  if (GcOptFill&opt) {
    brush_set_radial2(sg->brush, x1, y1, rx, ry, count, presetColors, blendPositions);
  }
  if (GcOptStroke&opt) {
    brush_set_radial2(sg->pen, x1, y1, rx, ry, count, presetColors, blendPositions);
  }
}
CC_INLINE void sgPathRadial(gc_t* g, int opt, const FPOINT* Points, const BYTE* Types, int Count, const COLOR* clrs, const FPOINT* ptcenter)
{
  SOFTGC_ARG;
  if (GcOptFill&opt) {
    brush_set_path_radial(sg->brush, Points, Types, Count, clrs, ptcenter);
  }
  if (GcOptStroke&opt) {
    brush_set_path_radial(sg->pen, Points, Types, Count, clrs, ptcenter);
  }
}
CC_INLINE void sgPathGradient(gc_t* g, int opt, const FPOINT* Points, const BYTE* Types, int Count, const COLOR* clrs, int nclrs, const FPOINT* ptcenter, COLOR clrcenter)
{
  SOFTGC_ARG;
  if (GcOptFill&opt) {
    brush_set_path_gradient(sg->brush, Points, Types, Count, clrs, nclrs, ptcenter, clrcenter);
  }
  if (GcOptStroke&opt) {
    brush_set_path_gradient(sg->pen, Points, Types, Count, clrs, nclrs, ptcenter, clrcenter);
  }
}
CC_INLINE void sgPattern(gc_t* g, int opt, const texture_t* tex, const IRECT* prcSrc, COLOR bgclr, const COLOR* palette, const float* m23)
{
  SOFTGC_ARG;
  if (GcOptFill&opt) {
    brush_set_texture(sg->brush, tex, prcSrc, bgclr, palette, m23);
  }
  if (GcOptStroke&opt) {
    brush_set_texture(sg->pen, tex, prcSrc, bgclr, palette, m23);
  }
}

void sgClip(gc_t* g) {
//softgc
}
CC_INLINE void sgLineDash(gc_t* g, float dashPhase, const float* dashPattern, int numdash)
{
  SOFTGC_ARG;
  PenStyleSetLineDash(sg->penstyle, dashPhase, dashPattern, numdash);
}
CC_INLINE void sgWrapMode(gc_t* g, WrapMode wrapX, WrapMode wrapY)
{
  SOFTGC_ARG;
  sg->brush->wrapX = wrapX;
  sg->brush->wrapY = wrapY;
}
CC_INLINE void sgLineWidth(gc_t* g, float lineWidth)
{
  SOFTGC_ARG;
  sg->penstyle->lineWidth = lineWidth;
}
CC_INLINE void gcDashStyle(gc_t* g, DashStyle dashStyle)
{
  SOFTGC_ARG;
  sg->penstyle->dashStyle = dashStyle;
  sg->penstyle->numdash = 0;
  switch(dashStyle) {
  case DashStyleDash:
    break;
	default:break;
  }
  //gcLineDash();
}
CC_INLINE void sgPenAlignment(gc_t* g, PenAlignment penAlignment)
{
  SOFTGC_ARG;
  sg->penstyle->penAlignment = penAlignment;
}
CC_INLINE void sgInnerJoin(gc_t* g, InnerJoin innerJoin)
{
  SOFTGC_ARG;
  sg->penstyle->innerJoin = innerJoin;
}
CC_INLINE void sgLineJoin(gc_t* g, LineJoin lineJoin)
{
  SOFTGC_ARG;
  sg->penstyle->lineJoin = lineJoin;
  sg->penstyle->innerJoin = lineJoin;
}
CC_INLINE void sgLineCap(gc_t* g, LineCap startCap, LineCap endCap)
{
  SOFTGC_ARG;
  sg->penstyle->startCap = startCap;
  sg->penstyle->endCap = endCap;
}
CC_INLINE void sgMiterLimit(gc_t* g, float miterLimit)
{
  SOFTGC_ARG;
  sg->penstyle->miterLimit = miterLimit;
}
CC_INLINE void sgRect(gc_t* g, int opt, float x, float y, float w, float h)
{
  SOFTGC_ARG;
  FPOINT pt[4];
  int n = vcgen_rect(pt, 0, x, y, x + w, y + h);
  if (GcOptFill&opt) {
    sg->br = sg->brush;
    scanline_set_poly(sg->sl, pt, &n, 1, false);
  } 
  if (GcOptStroke&opt) {
    sg->br = sg->pen;
    scanline_set_poly_dash_stroke(sg->sl, pt, n, 1, sg->penstyle);
    sg->br = sg->brush;
  }
}
CC_INLINE void sgEllipse(gc_t* g, int opt, float x, float y, float w, float h)
{
  SOFTGC_ARG;
  FPOINT pt[64];
  float rx = w/2, ry = h/2;
  int n = vcgen_ellipse(pt, countof(pt), 1, x+rx, y+ry, rx, ry);
  
  if (GcOptFill&opt) {
    sg->br = sg->brush;
    scanline_set_poly(sg->sl, pt, &n, true, 0!=(GcOptEO&opt));
  }
  if (GcOptStroke&opt) {
    sg->br = sg->pen;
    scanline_set_poly_dash_stroke(sg->sl, pt, n, true, sg->penstyle);
    sg->br = sg->brush;
  }
}
CC_INLINE void sgPath(gc_t* g, int opt, const FPOINT* Points, const BYTE* Types, int Count) {
  SOFTGC_ARG;
  Path path[1] = {0};
  path->Points = (FPOINT*)Points;
  path->Types = (BYTE*)Types;
  path->Count = Count;
  
  if (GcOptFill&opt) {
    sg->br = sg->brush;
    scanline_set_path2(sg->sl, path, (GcOptEO&opt)!=0);
  }
  if (GcOptStroke&opt) {
    sg->br = sg->pen;
    scanline_set_path_stroke2(sg->sl, path, sg->penstyle);
    sg->br = sg->brush;
  }
}
CC_INLINE void sgClipRect(gc_t* g, const IRECT* prc)
{
  SOFTGC_ARG;
  scanline_cliprect(sg->sl, prc);
}
CC_INLINE void sgTextAlign(gc_t* g, int textAlign)
{
  SOFTGC_ARG;
  sg->textAlign = textAlign;
}
CC_INLINE void sgText(gc_t* g, int opt, const char* text, const char* text_end, font_t* font, float size, float x, float y, float w, float h, int formatFlags)
{
  SOFTGC_ARG;
  scanline_set_string(sg->sl, opt, text, text_end, font, size, x, y, w, h, formatFlags);
}
CC_INLINE void sgPrimRectUV(gc_t* g, const PrimRectUV* pr, int npr, texture_t* tex)
{
  SOFTGC_ARG;
  if (tex) {
    ASSERT(0);
    //draw_rasterize(sg->im, sg->clip, sg->brush, sg->s, sg->flip_y, 0, NULL, 0, 0, pr, npr, tex, 1, 0, NULL, sg->sp);
  }
}
CC_INLINE void sgPrimitive(gc_t* g, info3d_t* info) {
  ASSERT(0);
}
CC_INLINE void sgQuadImage(gc_t* g, const ImVertex* vtx4, const texture_t* tex) {
  ASSERT(0);
}
CC_INLINE void sgRectImage(gc_t* g, const ImVertex* vtx, const texture_t* tex) {
  SOFTGC_ARG;
  PrimRectUV uv[1];
  SETPT(uv->a, vtx[0].pos.x, vtx[0].pos.y);
  SETPT(uv->c, vtx[1].pos.x, vtx[1].pos.y);
  uv->uv_a = vtx[0].uv;
  uv->uv_c = vtx[1].uv;
  uv->clr = vtx[0].col;
  if (tex && tex->w>0) {
    IRECT rcs = iRECT(uv->uv_a.x*tex->w, uv->uv_a.y*tex->h, uv->uv_c.x*tex->w, uv->uv_c.y*tex->h);
    IRECT rcd = iRECT(uv->a.x, uv->a.y, uv->c.x, uv->c.y);
    IRECT clip = scanline_clip(sg->sl, rcd.l, rcd.t, RCW(&rcd), RCH(&rcd));
    draw_textureR1(sg->sl->im, &clip, rcd, tex, &rcs, 0, true, 0);
  }
}

CC_INLINE void sgSetDirty(gc_t* g, IRECT rc, BOOL reset) {
  SOFTGC_ARG;
  if (reset) {
    sg->sl->pre_dirty = sg->sl->cur_dirty;
    sg->sl->cur_dirty = rc;
  } else {
    sg->sl->cur_dirty = iRectUnion(sg->sl->cur_dirty, rc);
  }
}
int gc_set_softgc(gc_t* g, softgc* sg) {
  static gcfun_t softgc_fun = {
#define GCFUNDEF(RET, NAME, ARG)   sg##NAME,
    GCFUNDEF_DEF(GCFUNDEF)
#undef GCFUNDEF
  };
  g->x = sg;
  g->fun = &softgc_fun;
  return 0;
}


int memset_color(int n, uchar* ptr, int cn, COLOR clr) {
  uchar* ptr_end = ptr + cn*n - cn;
  memcpy(ptr, &clr, cn);
  for (; ptr!=ptr_end; ++ptr) {
    ptr[cn] = *ptr;
  }
  return 0;
}
int imclear(img_t* im, const IRECT* pclip, COLOR clr)
{
  IRECT clip = iRectClip(pclip, 0, 0, im->w, im->h);
  int y, w;
  uchar* ptr1;
  w = RCW(&clip);
  ptr1 = im->tt.data + clip.t * im->s + clip.l * im->c;
  memset_color(w, ptr1, im->c, clr);
  for (y = clip.t + 1; y < clip.b; ++y) {
    uchar* pix_x = im->tt.data + y * im->s + clip.l * im->c;
    memcpy(pix_x, ptr1, w * im->c);
  }
  return 0;
}
////////////////////////////////////////////////////////////////////////

#define VA_ARG_EQ(T, A) T A = VA_ARG(T)

int softgc_run(softgc* sp, const char* va, const char* vb) {
  int i;
  //imclear(sp->sp->im, &drv->rcDirty, soft_color(clrbg));
  //sp->clip = clip;
  //softgc_clear(sp, clrbg);
  for (; va < vb;) {
    DrawCmd type = (DrawCmd)VA_ARG(int);
    switch (type) {
    case ImDrawCmd_Solid: {
      VA_ARG_EQ(int, fill);
      VA_ARG_EQ(COLOR, clr);
      clr = soft_color(clr);
      brush_set_solid(sp->brush, clr);
    }
    break;
    case ImDrawCmd_Gradient: {
      VA_ARG_EQ(int, fill);
      VA_ARG_EQ(float, x0);
      VA_ARG_EQ(float, y0);
      VA_ARG_EQ(float, x1);
      VA_ARG_EQ(float, y1);
      VA_ARG_EQ(int, count);
      COLOR blend[100];
      float positions[100];
      ASSERT(count < 100);
      for (i = 0; i < count; ++i) {
        positions[i] = VA_ARG(float);
        blend[i] = VA_ARG(COLOR);
        blend[i] = soft_color(blend[i]);
      }
      brush_set_gradient2(sp->brush, x0, y0, x1, y1, count, blend, positions);
    }
    break;
    case ImDrawCmd_Pattern:
      ASSERT(0);
      break;
    case ImDrawCmd_LineWidth:
      sp->penstyle->lineWidth = VA_ARG(float);
      break;
    case ImDrawCmd_MiterLimit:
      sp->penstyle->miterLimit = VA_ARG(float);
      break;
    case ImDrawCmd_ClipRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      ASSERT(0);
    }
    break;
    case ImDrawCmd_TextAlign:
      sp->textAlign = VA_ARG(int);
      break;
    case ImDrawCmd_Primitive:
      {
        VA_ARG_EQ(info3d_t*, info);
        //softgc_primitive(sp, info);
      }
      break;
    case ImDrawCmd_QuadImage:
      ASSERT(0);
      break;
    case ImDrawCmd_RectImage: {
      texture_t* tex = VA_ARGN(texture_t, 1);
      ImVertex* vtx = VA_ARGN(ImVertex, 2);
      PrimRectUV uv[1];
      SETPT(uv->a, vtx[0].pos.x, vtx[0].pos.y);
      SETPT(uv->c, vtx[1].pos.x, vtx[1].pos.y);
      uv->uv_a = vtx[0].uv;
      uv->uv_c = vtx[1].uv;
      uv->clr = vtx[0].col;
    }
    break;
    case ImDrawCmd_Text: {
      VA_ARG_EQ(int, fill);
      VA_ARG_EQ(int, textlen);
      VA_ARG_EQ(texfont_t*, font);
      VA_ARG_EQ(float, show_size);
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      VA_ARG_EQ(int, formatFlags);
      const char* text = (va += textlen) - textlen;
      if (font) {
        //int nuv;
        PrimRectUV* uv = NULL;
        ImVec2 sz;
        FRECT rc = fRECT2(x, y, w, h);
        MYREALLOC(uv, textlen);
        sz = texfont_CalcTextSizeA(font, show_size, w, w, text, text + textlen, NULL);
        fRectAlign(rc, sz.x, sz.y, formatFlags, &rc);
        PrimRectUV_RenderText(uv, font, show_size, fVec2(rc.l, rc.t), rc, text, text + textlen, w, TRUE);
        ASSERT(0);
        //softgc_rander_text(sp, uv, nuv, font->tex);
        //scanline_set_text(sp->sl, &sp->clip, uv, 1, font->tex);
        FREE(uv);
      }
    }
    break;
    case ImDrawCmd_PrimRectUV: {
      VA_ARG_EQ(int, npr);
      VA_ARG_EQ(texture_t*, tex);
      int sz = npr * sizeof(PrimRectUV);
      const PrimRectUV* pr = (const PrimRectUV*)((va += sz) - sz);
      if (tex) {
        ASSERT(0);
        //draw_rasterize(sp->im, sp->clip, sp->brush, sp->s, sp->flip_y, 0, NULL, 0, 0, pr, npr, tex, 1, 0, NULL, sp->sp);
      }
    }
    break;
    default:
      type = type;
      ASSERT(0);
      break;
    }
  }
  return 0;
}

int trirun(ImDrawList* g, const char* va, const char* vb)
{
  int i;
  g->anti_aliased = true;
  PathReset(g->path);
  triresetClip(g);
  for (; va < vb;) {
    DrawCmd type = (DrawCmd)VA_ARG(int);
    switch (type) {
    case ImDrawCmd_Solid: {
      char fill = VA_ARG(char);
      COLOR clr = VA_ARG(COLOR);
      if (fill) {
        g->fillColor = clr;
      }
      else {
        g->lineColor = clr;
      }
      brush_set_solid(fill ? g->brush : g->pen, clr);
    }
    break;
    case ImDrawCmd_Gradient: {
      char fill = VA_ARG(char);
      FPOINT pt1, pt2;
      int count;
      COLOR blend[100];
      float positions[100];
      pt1.x = VA_ARG(float);
      pt1.y = VA_ARG(float);
      pt2.x = VA_ARG(float);
      pt2.y = VA_ARG(float);
      count = VA_ARG(int);
      ASSERT(count < 100);
      for (i = 0; i < count; ++i) {
        positions[i] = VA_ARG(float);
        blend[i] = VA_ARG(COLOR);
      }
      brush_set_gradient2(fill ? g->brush : g->pen, pt1.x, pt1.y, pt2.x, pt2.y, count, blend, positions);
    }
    break;
    case ImDrawCmd_Pattern:
      ASSERT(0);
      break;
    case ImDrawCmd_LineWidth:
      g->lineWidth = VA_ARG(float);
      break;
    case ImDrawCmd_MiterLimit: {
      g->miterLimit = VA_ARG(float);
    }
    break;
    case ImDrawCmd_ClipRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
    }
    break;
    case ImDrawCmd_TextAlign:
      g->textAlign = VA_ARG(int);
    break;
    case ImDrawCmd_QuadImage: {
      texture_t* tex = VA_ARGN(texture_t, 1);
      ImVertex* vtx = VA_ARGN(ImVertex, 4);
      triAddCmd(g);
      g->tex = tex;
      g->vtxtype = 1;
      //triPrimQuadUV(g, vtx[0].pos, vtx[1].pos, vtx[2].pos, vtx[3].pos, vtx[0].uv, vtx[1].uv, vtx[2].uv, vtx[3].uv, 0);
      triAddCmd(g);
      g->vtxtype = 0;
    }
    break;
    case ImDrawCmd_RectImage: {
      texture_t* tex = VA_ARGN(texture_t, 1);
      ImVertex* vtx = VA_ARGN(ImVertex, 2);
      triAddCmd(g);
      g->tex = tex;
      g->vtxtype = 1;
      //triPrimRectUV(g, vtx[0].pos, vtx[1].pos, vtx[0].uv, vtx[1].uv, 0);
      triAddCmd(g);
      g->vtxtype = 0;
    }
    break;
    case ImDrawCmd_Text: {
      VA_ARG_EQ(int, fill);
      VA_ARG_EQ(int, textlen);
      VA_ARG_EQ(texfont_t*, font);
      VA_ARG_EQ(float, show_size);
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      VA_ARG_EQ(int, formatFlags);
      const char* text = VA_ARGN(char, textlen);
      if (font) {
        ImVec2 sz;
        FRECT rc = fRECT2(x, y, w, h);
        if (font->tex != g->tex) {
          triAddCmd(g);
          g->tex = font->tex;
        }
        sz = texfont_CalcTextSizeA(font, show_size, w, w, text, text + textlen, NULL);
        fRectAlign(rc, sz.x, sz.y, formatFlags, &rc);
        triRenderText(g, font, show_size, fVec2(rc.l, rc.t), g->fillColor, rc, text, text + textlen, w, TRUE);
        triAddCmd(g);
      }
      //WORD* wtext = NULL;
      //int wlen = my_mb2wc(&wtext, text, textlen);
      //FREE(wtext);
    }
    break;
    default:
      ASSERT(0);
      break;
    }
  }
  triAddCmd(g);
  return 0;
}
///////////////////////////////////////////////////////////////////

#define UICOLORDEF_DEF(COLORDEF) \
  COLORDEF(Col_Text,0.90f,0.90f,0.90f,1.00f)\
  COLORDEF(Col_TextDisabled,0.60f,0.60f,0.60f,1.00f)\
  COLORDEF(Col_WindowBg,0.00f,0.00f,0.00f,0.70f)\
  COLORDEF(Col_ChildWindowBg,0.00f,0.00f,0.00f,0.00f)\
  COLORDEF(Col_PopupBg,0.05f,0.05f,0.10f,0.90f)\
  COLORDEF(Col_Border,0.70f,0.70f,0.70f,0.65f)\
  COLORDEF(Col_BorderShadow,0.00f,0.00f,0.00f,0.00f)\
  COLORDEF(Col_FrameBg,0.80f,0.80f,0.80f,0.30f)\
  COLORDEF(Col_FrameBgHovered,0.90f,0.80f,0.80f,0.40f)\
  COLORDEF(Col_FrameBgActive,0.90f,0.65f,0.65f,0.45f)\
  COLORDEF(Col_TitleBg,0.27f,0.27f,0.54f,0.83f)\
  COLORDEF(Col_TitleBgCollapsed,0.40f,0.40f,0.80f,0.20f)\
  COLORDEF(Col_TitleBgActive,0.32f,0.32f,0.63f,0.87f)\
  COLORDEF(Col_MenuBarBg,0.40f,0.40f,0.55f,0.80f)\
  COLORDEF(Col_ScrollbarBg,0.20f,0.25f,0.30f,0.60f)\
  COLORDEF(Col_ScrollbarGrab,0.40f,0.40f,0.80f,0.30f)\
  COLORDEF(Col_ScrollbarGrabHovered,0.40f,0.40f,0.80f,0.40f)\
  COLORDEF(Col_ScrollbarGrabActive,0.80f,0.50f,0.50f,0.40f)\
  COLORDEF(Col_ComboBg,0.20f,0.20f,0.20f,0.99f)\
  COLORDEF(Col_CheckMark,0.90f,0.90f,0.90f,0.50f)\
  COLORDEF(Col_SliderGrab,1.00f,1.00f,1.00f,0.30f)\
  COLORDEF(Col_SliderGrabHovered,0.80f,0.50f,0.50f,0.50f)\
  COLORDEF(Col_SliderGrabActive,0.80f,0.50f,0.50f,1.00f)\
  COLORDEF(Col_LayoutSplit,1.00f,1.00f,1.00f,0.30f)\
  COLORDEF(Col_LayoutSplitHovered,1.0f,1.0f,1.0f,0.60f)\
  COLORDEF(Col_LayoutSplitActive,1.0f,1.0f,1.0f,0.90f)\
  COLORDEF(Col_Button,0.67f,0.40f,0.40f,0.60f)\
  COLORDEF(Col_ButtonHovered,0.67f,0.40f,0.40f,1.00f)\
  COLORDEF(Col_ButtonActive,0.80f,0.50f,0.50f,1.00f)\
  COLORDEF(Col_Header,0.40f,0.40f,0.90f,0.45f)\
  COLORDEF(Col_HeaderHovered,0.45f,0.45f,0.90f,0.80f)\
  COLORDEF(Col_HeaderActive,0.53f,0.53f,0.87f,0.80f)\
  COLORDEF(Col_Column,0.50f,0.50f,0.50f,1.00f)\
  COLORDEF(Col_ColumnHovered,0.70f,0.60f,0.60f,1.00f)\
  COLORDEF(Col_ColumnActive,0.90f,0.70f,0.70f,1.00f)\
  COLORDEF(Col_ResizeGrip,1.00f,1.00f,1.00f,0.30f)\
  COLORDEF(Col_ResizeGripHovered,1.00f,1.00f,1.00f,0.60f)\
  COLORDEF(Col_ResizeGripActive,1.00f,1.00f,1.00f,0.90f)\
  COLORDEF(Col_CloseButton,0.50f,0.50f,0.90f,0.50f)\
  COLORDEF(Col_CloseButtonHovered,0.70f,0.70f,0.90f,0.60f)\
  COLORDEF(Col_CloseButtonActive,0.70f,0.70f,0.70f,1.00f)\
  COLORDEF(Col_PlotLines,1.00f,1.00f,1.00f,1.00f)\
  COLORDEF(Col_PlotLinesHovered,0.90f,0.70f,0.00f,1.00f)\
  COLORDEF(Col_PlotHistogram,0.90f,0.70f,0.00f,1.00f)\
  COLORDEF(Col_PlotHistogramHovered,1.00f,0.60f,0.00f,1.00f)\
  COLORDEF(Col_TextSelectedBg,0.00f,0.00f,1.00f,0.35f)\
  COLORDEF(Col_ModalWindowDarkening,0.20f,0.20f,0.20f,0.35f)
enum GuiCol_ {
#define COLORDEF(id, r, g, b, a) id = _rgba(r, g, b, a),
  UICOLORDEF_DEF(COLORDEF)
#undef COLORDEF
  ImGuiCol_COUNT
};
static COLOR ColorMul(COLOR clr, double a)
{
  double r = GetRV(clr) * a, g = GetGV(clr) * a, b = GetBV(clr) * a;
  return _RGBA(MIN(r, 255), MIN(g, 255), MIN(b, 255), GetAV(clr));
}
#define gcEdgeRect1(g, rc, fill_color, crBorder, b) gcEdgeRect(g, rc, fill_color, crBorder, crBorder, crBorder, crBorder, b)
static IRECT gcEdgeRect(gc_t* g, IRECT rc, COLOR fill_color, COLOR crL, COLOR crT, COLOR crR, COLOR crB, int b)
{
  int x = rc.l, y = rc.t, w = RCW(&rc), h = RCH(&rc);
  b = MIN(b, w / 2);
  b = MIN(b, h / 2);
  if (GetAV(crT)) {
    gcSolidRect(g, x, y, w, b, crT);
    y += b, h -= b;
  }
  if (GetAV(crB)) {
    gcSolidRect(g, x, y + h - b, w, b, crB);
    h -= b;
  }
  if (GetAV(crL)) {
    gcSolidRect(g, x, y, b, h, crL);
    x += b, w -= b;
  }
  if (GetAV(crR)) {
    gcSolidRect(g, x + w - b, y, b, h, crR);
    w -= b;
  }
  if (GetAV(fill_color)) {
    gcSolidRect(g, x, y, w, h, fill_color);
  }
  return iRECT2(x, y, w, h);
}
static IRECT gcEdgeRect2(gc_t* g, IRECT rc, int b, COLOR fill_color, const COLOR* crL, const COLOR* crT, const COLOR* crR, const COLOR* crB)
{
  int i;
  for (i = 0; i < b; ++i) {
    rc = gcEdgeRect(g, rc, i == (b - 1) ? fill_color : 0, crL ? crL[i] : 0, crT ? crT[i] : 0, crR ? crR[i] : 0, crB ? crB[i] : 0, 1);
  }
  return rc;
}

#define BORDERSTYLEDEF_DEF(BORDERSTYLEDEF) \
  BORDERSTYLEDEF(BDS_SUNKEN, CLR_3DSHADOW, CLR_3DHILIGHT, CLR_3DDKSHADOW, CLR_3DLIGHT, 0, 0 ) \
  BORDERSTYLEDEF(BDS_LOW, CLR_3DSHADOW, CLR_3DHILIGHT, 0, 0, 0, 0) \
  BORDERSTYLEDEF(BDS_UP, CLR_3DHILIGHT, CLR_3DSHADOW, 0, 0, 0, 0) \
  BORDERSTYLEDEF(BDS_HIGH, CLR_3DHILIGHT, CLR_3DDKSHADOW, CLR_3DLIGHT, CLR_3DSHADOW, 0, 0) \
  BORDERSTYLEDEF(BDS_HIGH2, CLR_3DLIGHT, CLR_3DDKSHADOW, CLR_3DHILIGHT, CLR_3DSHADOW, 0, 0) \
  BORDERSTYLEDEF(BDS_SIMPLE, CLR_3DDKSHADOW, CLR_3DDKSHADOW, 0, 0, 0, 0) \
  BORDERSTYLEDEF(BDS_SIMPLE_DOT, CLR_3DDKSHADOW, CLR_3DDKSHADOW, 0, 0, 0, 0) \
  BORDERSTYLEDEF(BDS_FLAT, CLR_3DDKSHADOW, CLR_3DDKSHADOW, CLR_3DDKSHADOW, CLR_3DDKSHADOW, 0, 0) \
  BORDERSTYLEDEF(BDS_FRAME, CLR_3DHILIGHT, CLR_3DDKSHADOW, CLR_3DFACE, CLR_3DFACE, CLR_3DDKSHADOW, CLR_3DHILIGHT) \
  BORDERSTYLEDEF(BDS_LIGHT_FRAME, CLR_3DHILIGHT, CLR_3DSHADOW, CLR_3DFACE, CLR_3DFACE, CLR_3DSHADOW, CLR_3DHILIGHT) \
  BORDERSTYLEDEF(BDS_THIN_FRAME, CLR_3DHILIGHT, CLR_3DDKSHADOW, CLR_3DDKSHADOW, CLR_3DHILIGHT, 0, 0) \
  BORDERSTYLEDEF(BDS_THIN_LIGHT_FRAME, CLR_3DHILIGHT, CLR_3DSHADOW, CLR_3DSHADOW, CLR_3DHILIGHT, 0, 0) \
  BORDERSTYLEDEF(BDS_ETCHED, CLR_3DSHADOW, CLR_3DHILIGHT, CLR_3DHILIGHT, CLR_3DSHADOW, 0, 0) \
  BORDERSTYLEDEF(BDS_ETCHED2, CLR_3DSHADOW, CLR_3DHILIGHT, CLR_3DDKSHADOW, CLR_3DDKSHADOW, CLR_3DHILIGHT, CLR_3DSHADOW)
typedef enum {
#define BORDERSTYLEDEF(a, b0, c0, b1, c1, b2, c2) a,
  BORDERSTYLEDEF_DEF(BORDERSTYLEDEF)
#undef BORDERSTYLEDEF
} BorderStyle;

// ControlBar styles
#define CBRS_ALIGN_LEFT 0x1000L
#define CBRS_ALIGN_TOP 0x2000L
#define CBRS_ALIGN_RIGHT 0x4000L
#define CBRS_ALIGN_BOTTOM 0x8000L
#define CBRS_ALIGN_ANY 0xF000L
#define CBRS_BORDER_LEFT 0x0100L
#define CBRS_BORDER_TOP 0x0200L
#define CBRS_BORDER_RIGHT 0x0400L
#define CBRS_BORDER_BOTTOM 0x0800L
#define CBRS_BORDER_ANY 0x0F00L
#define CBRS_TOOLTIPS 0x0010L
#define CBRS_FLYBY 0x0020L
#define CBRS_FLOAT_MULTI 0x0040L
#define CBRS_BORDER_3D 0x0080L
#define CBRS_HIDE_INPLACE 0x0008L
#define CBRS_SIZE_DYNAMIC 0x0004L
#define CBRS_SIZE_FIXED 0x0002L
#define CBRS_FLOATING 0x0001L
#define CBRS_GRIPPER 0x00400000L
#define CBRS_ORIENT_HORZ (CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM)
#define CBRS_ORIENT_VERT (CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT)
#define CBRS_ORIENT_ANY (CBRS_ORIENT_HORZ|CBRS_ORIENT_VERT)
#define CBRS_ALL 0x0040FFFFL
// the CBRS_ style is made up of an alignment style and a draw border style
// the alignment styles are mutually exclusive
// the draw border styles may be combined
#define CBRS_NOALIGN 0x00000000L
#define CBRS_LEFT (CBRS_ALIGN_LEFT|CBRS_BORDER_RIGHT)
#define CBRS_TOP (CBRS_ALIGN_TOP|CBRS_BORDER_BOTTOM)
#define CBRS_RIGHT (CBRS_ALIGN_RIGHT|CBRS_BORDER_LEFT)
#define CBRS_BOTTOM (CBRS_ALIGN_BOTTOM|CBRS_BORDER_TOP)
enum BorderMark {
  BDM_L = 0x0001,
    BDM_T = 0x0002,
    BDM_R = 0x0004,
    BDM_B = 0x0008,
    BDM_ALL = BDM_L | BDM_T | BDM_R | BDM_B,
    BDM_ROUND_TOP = 0x0010,
    BDM_ROUND_BOTTOM = 0x0020
};


#define MinMax( a, b, _min, _max) (_min=MIN(a, b), _max=MAX(a, b))
#define RCINFLATE4(RC, L, T, R, B)   ( (RC)->l -= L,(RC)->t -= T,(RC)->r += R,(RC)->b += B)
#define RCINFLATE(rc, lr, tb)  RCINFLATE4(rc, lr, tb, lr, tb)
#define RCINFLATER(rc, rc2) RCINFLATE4(rc, (rc2)->l,(rc2)->t,(rc2)->r,(rc2)->b)
#define RCOFFSET4(rc, x, y) ( (rc)->l += x,(rc)->t += y,(rc)->r += x,(rc)->b += y)
#define RCDEFLATE(RC, X, Y) RCINFLATE(RC, -(X), -(Y))
#define RCDEFLATE4(rc, l, t, r, b) ((rc)->l += l,(rc)->t += t,(rc)->r -= r,(rc)->b -= b)
#define RCDEFLATER(rc, rc2) RCDEFLATE4(rc, (rc2)->l,(rc2)->t,(rc2)->r,(rc2)->b)

CC_INLINE IRECT gcEdgeRectSys(gc_t* g, IRECT rc, BorderStyle m_border_style, uint flag, COLOR clr)
{
#define BORDERSTYLEDEF(a, b0, c0, b1, c1, b2, c2) if (a == m_border_style) { \
    COLOR clrLT[]= { myGetSysColor(b0), myGetSysColor(b1), myGetSysColor(b2) }, clrRB[]= { myGetSysColor(c0), myGetSysColor(c1), myGetSysColor(c2) }; \
    int nlr=countof(clrLT)-(!b1)-(!b2), nrb=countof(clrRB)-(!c1)-(!c2); \
    rc = gcEdgeRect2(g, rc, nlr, clr, flag&BDM_L?clrLT:0, flag&BDM_T ? clrLT:0, flag&BDM_R ? clrRB : 0, flag&BDM_B ? clrRB : 0); }
  BORDERSTYLEDEF_DEF(BORDERSTYLEDEF);
#undef BORDERSTYLEDEF
  return rc;
}
CC_INLINE int gcDrawGripper(gc_t* g, IRECT rc, int n, int b, int d, int isv)
{
  if (isv) {
    int i = 0, r = rc.b;
    
    for (; i < n && rc.t < r; ++i) {
      rc.b = rc.t + b;
      gcEdgeRectSys(g, rc, BDS_UP, BDM_ALL, 0);
      rc.t += b + d;
    }
  }
  else {
    int i = 0, r = rc.r;
    
    for (; i < n && rc.l < r; ++i) {
      rc.r = rc.l + b;
      gcEdgeRectSys(g, rc, BDS_UP, BDM_ALL, 0);
      rc.l += b + d;
    }
  }
  
  return 0;
}

CC_INLINE int gcDrawMenuimg(gc_t* g, IRECT rc, IMAGES_IDS id, COLOR clr)
{
  static const uchar menuimg_9x324[] = {
#include "draw/menuimg_9x324.txt"
  };
  static texture_t menuimg_tex[1] = {0};
  enum { www = 324, cx = MENUIMAGES_W, cy = MENUIMAGES_H, MAXid = countof(menuimg_9x324) * 8 / (cx * cy), bl = countof(menuimg_9x324) * 8 / (cy)};
  PrimRectUV pr[1] = {0};
  int fmt = TF_VCENTER|TF_CENTER;
  if (NULL==menuimg_tex->data) {

  }
  //imdraw_bit(im, pclip, rc, DT_VCENTER | DT_CENTER, cx, cy, menuimg_9x324 + (id * cx) / 8, (bl+7)>>3, (id * cx) & 7, 0, 1, clr);
  iRectAlign(rc, cx, cy, fmt, &rc);
  pr->a = fVec2(rc.l, rc.t);
  pr->c = fVec2(rc.r, rc.b);
  pr->uv_a = fVec2(id * cx * 1. / www, 0);
  pr->uv_c = fVec2((id + 1) * cx * 1. / www, 1.f);
  //imuiCreateImage(&img_id, "menuimg_9x324", cy, www, (bl + 7) >> 3, 1, menuimg_9x324);
  //gcPrimRectUV(g, pr, 1, img_id);
  return 0;
}

// 画直方图
CC_INLINE int gcDrawHist(gc_t* g, IRECT rect, const uchar* f, int hdims)
{
  int w = RCW(&rect), h = RCH(&rect);
  double bin_w;
  int i;
  bin_w = (double) w / hdims;
  
  for (i = 0; i < hdims; ++i) {
    double val = (f[ i ] * h);
    COLOR color = CC_RGB(255, 255, 0);   //(hsv2rgb(i*180.f/hdims);
    IRECT rc = iRECT((int)(i * bin_w), h, (int)((i + 1) * bin_w), (int)(h - val));
    gcSolidBrush(g, color);
    gcFillRect2(g, rc);
  }
  
  return 0;
}

CC_INLINE int gcDrawHisthsv(gc_t* g, IRECT rect, const int* hist, int hdims)
{
  int w = RCW(&rect), h = RCH(&rect);
  int i, bin_w = w / hdims;
  //int maxid = 0;
  //float ss = 0.f;
  
  for (i = 0; i < hdims; i++) {
    int val = ROUND((hist[ i ]) * h / 255);
    COLOR color = hsv2rgb(i * 180.f / hdims);
    IRECT rc = iRECT(i * bin_w, h, (i + 1) * bin_w, h - val);
    gcSolidBrush(g, color);
    gcFillRect2(g, rc);
  }
  
  return 0;
}

