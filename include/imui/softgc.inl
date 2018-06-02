#ifndef _SOFTGC_INL_
#define _SOFTGC_INL_
#define VA_ARG_EQ(T, A) T A = VA_ARG(T)
int dpath_run(dpath_t* path, int type, const char* va, const char** pva)
{
  const char* va0 = va;
  int ret = 1;
  switch (type) {
  case ImDrawCmd_rect: {
    VA_ARG_EQ(float, x);
    VA_ARG_EQ(float, y);
    VA_ARG_EQ(float, w);
    VA_ARG_EQ(float, h);
    dpath_rect(path, x, y, w, h);
  }
  break;
  case ImDrawCmd_resetPath:
    dpath_reset(path);
    break;
  case ImDrawCmd_beginPath:
    dpath_reset(path);
    break;
  case ImDrawCmd_closePath:
    dpath_close(path);
    break;
  case ImDrawCmd_moveTo: {
    VA_ARG_EQ(float, x);
    VA_ARG_EQ(float, y);
    dpath_lineto(path, x, y);
  }
  break;
  case ImDrawCmd_lineTo: {
    VA_ARG_EQ(float, x);
    VA_ARG_EQ(float, y);
    dpath_lineto(path, x, y);
  }
  break;
  case ImDrawCmd_polygons: {
    VA_ARG_EQ(int, n);
    VA_ARG_EQ(int, c);
    VA_ARG_EQ(int, type);
    VA_ARG_EQ(int, step);
    int* ln = VA_ARGN(int, c);
    int* flag = VA_ARGN(int, c);
    DPOINT* pt = VA_ARGN(DPOINT, n);
    dpath_addpts2(path, pt, n, ln, flag, c);
  }
  break;
  case ImDrawCmd_arcTo: {
    VA_ARG_EQ(float, x1);
    VA_ARG_EQ(float, y1);
    VA_ARG_EQ(float, x2);
    VA_ARG_EQ(float, y2);
    VA_ARG_EQ(float, radius);
    ASSERT(0);
    //g->x = x2, g->y = y2;
  }
  break;
  case ImDrawCmd_arc: {
    VA_ARG_EQ(float, x);
    VA_ARG_EQ(float, y);
    VA_ARG_EQ(float, rx);
    VA_ARG_EQ(float, ry);
    VA_ARG_EQ(float, startAngle);
    VA_ARG_EQ(float, endAngle);
    startAngle *= RADIAN;
    endAngle *= RADIAN;
    dpath_arc(path, x, y, rx, ry, startAngle, endAngle);
    //dpath_lineto(path, x + rx * cos(endAngle), y + ry * sin(endAngle));
  }
  break;
  case ImDrawCmd_quadraticCurveTo: {
    VA_ARG_EQ(float, cpx);
    VA_ARG_EQ(float, cpy);
    VA_ARG_EQ(float, x);
    VA_ARG_EQ(float, y);
    ASSERT(0);
    //g->x = x, g->y = y;
  }
  break;
  case ImDrawCmd_bezierCurveTo: {
    VA_ARG_EQ(float, cpx1);
    VA_ARG_EQ(float, cpy1);
    VA_ARG_EQ(float, cpx2);
    VA_ARG_EQ(float, cpy2);
    VA_ARG_EQ(float, x);
    VA_ARG_EQ(float, y);
    ASSERT(0);
    //g->x = x, g->y = y;
  }
  break;
  case ImDrawCmd_scale: {
    VA_ARG_EQ(float, scalewidth);
    VA_ARG_EQ(float, scaleheight);
    ASSERT(0);
  }
  break;
  case ImDrawCmd_rotate: {
    VA_ARG_EQ(float, angle);
    ASSERT(0);
  }
  break;
  case ImDrawCmd_translate: {
    VA_ARG_EQ(float, x);
    VA_ARG_EQ(float, y);
    affine_translation(path->matrix, x, y);
  }
  break;
  case ImDrawCmd_transform: {
    VA_ARG_EQ(float, a);
    VA_ARG_EQ(float, b);
    VA_ARG_EQ(float, c);
    VA_ARG_EQ(float, d);
    VA_ARG_EQ(float, e);
    VA_ARG_EQ(float, f);
    ASSERT(0);
  }
  break;
  case ImDrawCmd_setTransform: {
    VA_ARG_EQ(float, a);
    VA_ARG_EQ(float, b);
    VA_ARG_EQ(float, c);
    VA_ARG_EQ(float, d);
    VA_ARG_EQ(float, e);
    VA_ARG_EQ(float, f);
    ASSERT(0);
  }
  break;
  default:
    ret = 0;
    break;
  }
  *pva = va;
  return ret;
}
int softgc_run(softgc* g, const char* va, const char* vb, IRECT rcDirty)
{
  int i;
  *g->clip = rcDirty;
  dpath_reset(g->path);
  affine_init(g->path->matrix);
  for (; va < vb;) {
    int type = VA_ARG(char);
    if (dpath_run(g->path, type, va, &va)) {
      continue;
    }
    switch (type) {
    case ImDrawCmd_styleColor: {
      char fill = VA_ARG(char);
      COLOR clr = VA_ARG(COLOR);
      clr = soft_color(clr);
      brush_set_solid(fill ? g->brush : g->pen, clr);
    }
    break;
    case ImDrawCmd_styleGradient: {
      char fill = VA_ARG(char);
      FPOINT pt1, pt2;
      int count;
      COLOR blend[100];
      double positions[100];
      pt1.x = VA_ARG(float);
      pt1.y = VA_ARG(float);
      pt2.x = VA_ARG(float);
      pt2.y = VA_ARG(float);
      count = VA_ARG(int);
      ASSERT(count < 100);
      for (i = 0; i < count; ++i) {
        positions[i] = VA_ARG(float);
        blend[i] = VA_ARG(COLOR);
        blend[i] = soft_color(blend[i]);
      }
      brush_set_gradient2(fill ? g->brush : g->pen, pt1.x, pt1.y, pt2.x, pt2.y, count, blend, positions);
    }
    break;
    case ImDrawCmd_stylePattern:
      ASSERT(0);
      break;
    case ImDrawCmd_lineWidth:
      g->lineWidth = VA_ARG(float);
      break;
    case ImDrawCmd_lineStyle:
      g->lineStyle = VA_ARG(int);
      break;
    case ImDrawCmd_miterLimit:
      g->miterLimit = VA_ARG(float);
      break;
    case ImDrawCmd_fillRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      DPOINT pt[4];
      int n = vcgen_rect(pt, 0, 4, x, y, x + w, y + h);
      int flag = 1;
      softgc_rander(g, 1, 0, pt, &n, &flag, 1, n);
    }
    break;
    case ImDrawCmd_strokeRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      DPOINT pt[4];
      int n = vcgen_rect(pt, 0, 4, x, y, x + w, y + h);
      int flag = 1;
      softgc_rander(g, 0, 1, pt, &n, &flag, 1, n);
    }
    break;
    case ImDrawCmd_clearRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      ASSERT(0);
    }
    break;
    case ImDrawCmd_clip:
      ASSERT(0);
      break;
    case ImDrawCmd_clipRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      if (w < 0 || h < 0) {
        *g->clip = rcDirty;
      }
      else {
        *g->clip = iRectInter(rcDirty, iRECT2((int)x, (int)y, (int)w, (int)h));
      }
    }
    break;
    case ImDrawCmd_fill:
      softgc_rander(g, 1, 0, g->path->pt, g->path->ln, g->path->flag, g->path->c, g->path->n);
      break;
    case ImDrawCmd_stroke:
      softgc_rander(g, 0, 1, g->path->pt, g->path->ln, g->path->flag, g->path->c, g->path->n);
      break;
    case ImDrawCmd_textAlign: {
      VA_ARG_EQ(char, textAlign);
      g->textAlign = textAlign;
    }
    break;
    case ImDrawCmd_quadImage:
      ASSERT(0);
      break;
    case ImDrawCmd_rectImage: {
      texture_t* tex = VA_ARGN(texture_t, 1);
      ImVertex* vtx = VA_ARGN(ImVertex, 2);
      uint32* tex32 = (uint32*)tex->tex;
      PrimRectUV uv[1];
      uv->a = vtx[0].pos;
      uv->c = vtx[1].pos;
      uv->uv_a = vtx[0].uv;
      uv->uv_c = vtx[1].uv;
      uv->clr = vtx[0].col;
      softgc_rander_texture(g, uv, 1, tex->h, tex->w, tex32);
    }
    break;
    case ImDrawCmd_text: {
      VA_ARG_EQ(char, fill);
      VA_ARG_EQ(int, textlen);
      const char* text = (va += textlen) - textlen;
      VA_ARG_EQ(texfont_t*, font);
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      VA_ARG_EQ(int, formatFlags);
      PrimRectUV* uv = NULL;
      int nuv;
      if (font) {
        ImVec2 sz;
        FRECT rc = fRECT2(x, y, w, h);
        MYREALLOC(uv, textlen);
        sz = texfont_CalcTextSizeA(font, font->FontSize, w, w, text, text + textlen, NULL);
        fRectAlign(rc, sz.x, sz.y, formatFlags, &rc);
        nuv = PrimRectUV_RenderText(uv, font, font->FontSize, fVec2(rc.l, rc.t), rc, text, text + textlen, w, TRUE);
        softgc_rander_text(g, uv, nuv, font);
        FREE(uv);
      }
    }
    break;
    case ImDrawCmd_primRectUV: {
      VA_ARG_EQ(int, npr);
      VA_ARG_EQ(texture_t*, tex);
      int sz = npr * sizeof(PrimRectUV);
      const PrimRectUV* pr = (const PrimRectUV*)((va += sz) - sz);
      if (tex) {
        bitmap_t bmp[1] = {0};
        BITMAP_INIT(bmp, img->height, img->width, img->step, img->bpp, (uchar*)img->image);
        draw_rasterize(g->im, g->clip, g->brush, g->s, g->flip_y, 0, NULL, 0, 0, pr, npr, tex, 0, NULL);
      }
    }
    break;
    default:
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
  dpath_reset(g->path);
  affine_init(g->path->matrix);
  triresetClip(g);
  for (; va < vb;) {
    int type = VA_ARG(char);
    if (dpath_run(g->path, type, va, &va)) {
      continue;
    }
    switch (type) {
    case ImDrawCmd_styleColor: {
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
    case ImDrawCmd_styleGradient: {
      char fill = VA_ARG(char);
      FPOINT pt1, pt2;
      int count;
      COLOR blend[100];
      double positions[100];
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
    case ImDrawCmd_stylePattern:
      ASSERT(0);
      break;
    case ImDrawCmd_lineWidth:
      g->lineWidth = VA_ARG(float);
      break;
    case ImDrawCmd_lineStyle:
      g->lineStyle = VA_ARG(int);
      break;
    case ImDrawCmd_miterLimit: {
      g->miterLimit = VA_ARG(float);
    }
    break;
#define VA_ARG_EQ(T, A) T A = VA_ARG(T)
    case ImDrawCmd_fillRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      int off = g->VtxBuffer_Size;
      triPrimRect(g, fVec2(x, y), fVec2(x + w, y + h), g->fillColor);
      draw_rasterize(NULL, NULL, g->brush, NULL, 0, 0, NULL, 0, 0, 0, 0, 0, g->VtxBuffer_Size - off, g->VtxBuffer + off);
    }
    break;
    case ImDrawCmd_strokeRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      DPOINT pt[4];
      int n = vcgen_rect(pt, 0, 4, x, y, x + w, y + h);
      int off = g->VtxBuffer_Size;
      triPolyline(g, pt, n, g->lineColor, true, g->lineWidth, g->anti_aliased);
      draw_rasterize(NULL, NULL, g->pen, NULL, 0, 0, NULL, 0, 0, 0, 0, 0, g->VtxBuffer_Size - off, g->VtxBuffer + off);
    }
    break;
    case ImDrawCmd_clearRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      ASSERT(0);
    }
    break;
    case ImDrawCmd_clip:
      ASSERT(0);
      break;
    case ImDrawCmd_clipRect: {
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      if (w > 0 && h > 0) {
        ImVec4 ClipRect = fVec4(x, y, x + w, y + h);
        if (0 != memcmp(&ClipRect, &g->ClipRect, sizeof(g->ClipRect))) {
          triAddCmd(g);
          g->ClipRect = ClipRect;
        }
      }
      else {
        triresetClip(g);
      }
    }
    break;
    case ImDrawCmd_fill: {
      int off = g->VtxBuffer_Size;
      triConvexPolyFilled(g, g->path->pt, g->path->n, g->fillColor, g->anti_aliased);
      draw_rasterize(NULL, NULL, g->brush, NULL, 0, 0, NULL, 0, 0, 0, 0, 0, g->VtxBuffer_Size - off, g->VtxBuffer + off);
    }
    break;
    case ImDrawCmd_stroke: {
      int off = g->VtxBuffer_Size, pos = 0;
      for (i = 0; pos < g->path->n; ++i) {
        int len = i < g->path->c ? g->path->ln[i] : (g->path->n - pos);
        triPolyline(g, g->path->pt + pos, len, g->lineColor, i < g->path->c, g->lineWidth, g->anti_aliased);
        pos += len;
      }
      draw_rasterize(NULL, NULL, g->pen, NULL, 0, 0, NULL, 0, 0, 0, 0, 0, g->VtxBuffer_Size - off, g->VtxBuffer + off);
    }
    break;
    case ImDrawCmd_textAlign: {
      VA_ARG_EQ(char, textAlign);
      g->textAlign = textAlign;
    }
    break;
    case ImDrawCmd_quadImage: {
      texture_t* tex = VA_ARGN(texture_t, 1);
      ImVertex* vtx = VA_ARGN(ImVertex, 4);
      triAddCmd(g);
      g->TextureId = tex->tex;
      g->vtxtype = 1;
      triPrimQuadUV(g, vtx[0].pos, vtx[1].pos, vtx[2].pos, vtx[3].pos, vtx[0].uv, vtx[1].uv, vtx[2].uv, vtx[3].uv, 0);
      triAddCmd(g);
      g->vtxtype = 0;
    }
    break;
    case ImDrawCmd_rectImage: {
      texture_t* tex = VA_ARGN(texture_t, 1);
      ImVertex* vtx = VA_ARGN(ImVertex, 2);
      triAddCmd(g);
      g->TextureId = tex->tex;
      g->vtxtype = 1;
      triPrimRectUV(g, vtx[0].pos, vtx[1].pos, vtx[0].uv, vtx[1].uv, 0);
      triAddCmd(g);
      g->vtxtype = 0;
    }
    break;
    case ImDrawCmd_text: {
      VA_ARG_EQ(char, fill);
      VA_ARG_EQ(int, textlen);
      const char* text = VA_ARGN(char, textlen);
      VA_ARG_EQ(texfont_t*, font);
      VA_ARG_EQ(float, x);
      VA_ARG_EQ(float, y);
      VA_ARG_EQ(float, w);
      VA_ARG_EQ(float, h);
      VA_ARG_EQ(int, formatFlags);
      if (NULL==font) {

      }
      if (font) {
        ImVec2 sz;
        FRECT rc = fRECT2(x, y, w, h);
        if (font != g->font) {
          triAddCmd(g);
          g->font = font;
        }
        sz = texfont_CalcTextSizeA(font, font->FontSize, w, w, text, text + textlen, NULL);
        fRectAlign(rc, sz.x, sz.y, formatFlags, &rc);
        triRenderText(g, font, font->FontSize, fVec2(rc.l, rc.t), g->fillColor, rc, text, text + textlen, w, TRUE);
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
#endif // _SOFTGC_INL_

