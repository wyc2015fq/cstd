// compile flag for this file
#undef USE_ZBUFFER
#undef IPOL_Z
#undef CMP_Z
#undef WRITE_Z
#undef IPOL_W
#undef CMP_W
#undef WRITE_W
#undef SUBTEXEL
#undef INVERSE_W
#undef IPOL_C0
#undef IPOL_T0
#undef IPOL_T1
// define render case
#define SUBTEXEL
#define INVERSE_W
#define USE_ZBUFFER
#define IPOL_W
#define CMP_W
#define WRITE_W
#define IPOL_C0
#define IPOL_T0
//#define IPOL_T1
// apply global override
#ifndef SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
#undef INVERSE_W
#endif
#ifndef SOFTWARE_DRIVER_2_SUBTEXEL
#undef SUBTEXEL
#endif
#ifndef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
#undef IPOL_C0
#endif
#if !defined ( SOFTWARE_DRIVER_2_USE_WBUFFER ) && defined ( USE_ZBUFFER )
#ifndef SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
#undef IPOL_W
#endif
#define IPOL_Z
#ifdef CMP_W
#undef CMP_W
#define CMP_Z
#endif
#ifdef WRITE_W
#undef WRITE_W
#define WRITE_Z
#endif
#endif
/*!
*/
void CTRTextureGouraudAlpha2_scanline_bilinear(IBurningShader* s)
{
  sScanLineData* line = s->line;
  sScanConvertData* scan = s->scan;
  tVideoSample* dst;
#ifdef USE_ZBUFFER
  fp24* z;
#endif
  s32 xStart;
  s32 xEnd;
  s32 dx;
#ifdef SUBTEXEL
  f32 subPixel;
#endif
#ifdef IPOL_Z
  f32 slopeZ;
#endif
#ifdef IPOL_W
  fp24 slopeW;
#endif
#ifdef IPOL_C0
  sVec4 slopeC[MATERIAL_MAX_COLORS];
#endif
#ifdef IPOL_T0
  sVec2 slopeT[BURNING_MATERIAL_MAX_TEXTURES];
#endif
  f32 invDeltaX;
#ifdef INVERSE_W
  f32 inversew;
#endif
#ifdef BURNINGVIDEO_RENDERER_FAST
  u32 dIndex = (line->y & 3) << 2;
#else
  tFixPoint a0;
  tFixPoint r0, g0, b0;
#endif
#ifdef IPOL_C0
  tFixPoint r1, g1, b1;
  tFixPoint r2, g2, b2;
#endif
  s32 i;
  // apply top-left fill-convention, left
  xStart = ceil32(line->x[0]);
  xEnd = ceil32(line->x[1]) - 1;
  dx = xEnd - xStart;
  if (dx < 0) {
    return;
  }
  // slopes
  invDeltaX = reciprocal_approxim(line->x[1] - line->x[0]);
#ifdef IPOL_Z
  slopeZ = (line->z[1] - line->z[0]) * invDeltaX;
#endif
#ifdef IPOL_W
  slopeW = (line->w[1] - line->w[0]) * invDeltaX;
#endif
#ifdef IPOL_C0
  VOP2a_4(slopeC[0], = , line->c[0][1], -, line->c[0][0], invDeltaX);
#endif
#ifdef IPOL_T0
  VOP2a_2(slopeT[0], = , line->t[0][1], -, line->t[0][0], invDeltaX);
#endif
#ifdef IPOL_T1
  VOP2a_2(slopeT[1], = , line->t[1][1], -, line->t[1][0], invDeltaX);
#endif
#ifdef SUBTEXEL
  subPixel = ((f32) xStart) - line->x[0];
#ifdef IPOL_Z
  line->z[0] += slopeZ * subPixel;
#endif
#ifdef IPOL_W
  line->w[0] += slopeW * subPixel;
#endif
#ifdef IPOL_C0
  VOP1a_4(line->c[0][0], += , slopeC[0], *, subPixel);
#endif
#ifdef IPOL_T0
  VOP1a_2(line->t[0][0], += , slopeT[0], *, subPixel);
#endif
#ifdef IPOL_T1
  VOP1a_2(line->t[1][0], += , slopeT[1], *, subPixel);
#endif
#endif
  dst = (tVideoSample*)(s->RenderTarget->Data + line->y * s->RenderTarget->Pitch) + xStart;
#ifdef USE_ZBUFFER
  z = (fp24*)(s->DepthBuffer->Data + line->y * s->DepthBuffer->Pitch) + xStart;
#endif
  for (i = 0; i <= dx; ++i) {
#ifdef CMP_Z
    if (line->z[0] < z[i])
#endif
#ifdef CMP_W
      if (line->w[0] >= z[i])
#endif
      {
#ifdef BURNINGVIDEO_RENDERER_FAST
        const tFixPointu d = dithermask [ dIndex | (i) & 3 ];
#ifdef INVERSE_W
        inversew = fix_inverse32(line->w[0]);
        u32 argb = getTexel_plain(&s->IT[0],  d + tofix(line->t[0][0][0], inversew),
            d + tofix(line->t[0][0][1], inversew)
                                 );
#else
        u32 argb = getTexel_plain(&s->IT[0],  d + tofix(line->t[0][0][0]),
            d + tofix(line->t[0][0][1])
                                 );
#endif
        const u32 alpha = (argb >> 24);
        if (alpha > s->AlphaRef) {
#ifdef WRITE_Z
          z[i] = line->z[0];
#endif
#ifdef WRITE_W
          z[i] = line->w[0];
#endif
          dst[i] = PixelBlend32(dst[i], argb, alpha);
        }
#else
#ifdef INVERSE_W
        inversew = fix_inverse32(line->w[0]);
        getSample_texture_argb(&a0, &r0, &g0, &b0,
            &s->IT[0],
            tofix(line->t[0][0][0], inversew),
            tofix(line->t[0][0][1], inversew)
                              );
#else
        getSample_texture_argb(&a0, &r0, &g0, &b0,
            &s->IT[0],
            tofix(line->t[0][0][0]),
            tofix(line->t[0][0][1])
                              );
#endif
        if ((tFixPointu) a0 > s->AlphaRef) {
#ifdef WRITE_Z
          z[i] = line->z[0];
#endif
#ifdef WRITE_W
          z[i] = line->w[0];
#endif
#ifdef INVERSE_W
          getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], inversew);
#else
          getSample_color_rgb(&r2, &g2, &b2, line->c[0][0]);
#endif
          r0 = imulFix(r0, r2);
          g0 = imulFix(g0, g2);
          b0 = imulFix(b0, b2);
          color_to_fix_rgb(&r1, &g1, &b1, dst[i]);
          a0 >>= 8;
          r2 = r1 + imulFix(a0, r0 - r1);
          g2 = g1 + imulFix(a0, g0 - g1);
          b2 = b1 + imulFix(a0, b0 - b1);
          dst[i] = fix4_to_color_argb(a0, r2, g2, b2);
          /*
                dst[i] = PixelBlend32 ( dst[i],
                            fix_to_color ( r0,g0, b0 ),
                            fixPointu_to_u32 ( a0 )
                          );
          */
          /*
                getSample_color ( r2, g2, b2, line->c[0][0], inversew * COLOR_MAX );
                color_to_fix ( r1, g1, b1, dst[i] );
                r2 = r0 + imulFix ( a0, r1 - r0 );
                g2 = g0 + imulFix ( a0, g1 - g0 );
                b2 = b0 + imulFix ( a0, b1 - b0 );
                dst[i] = fix_to_color ( r2, g2, b2 );
          */
        }
#endif
      }
#ifdef IPOL_Z
    line->z[0] += slopeZ;
#endif
#ifdef IPOL_W
    line->w[0] += slopeW;
#endif
#ifdef IPOL_C0
    VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
#ifdef IPOL_T0
    VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_T1
    VOP1_2(line->t[1][0], += , slopeT[1]);
#endif
  }
}
void CTRTextureGouraudAlpha2_drawTriangle(IBurningShader* s, const s4DVertex* a, const s4DVertex* b, const s4DVertex* c)
{
  sScanLineData* line = s->line;
  sScanConvertData* scan = s->scan;
  f32 ca, ba, cb;
  f32 temp[4];
  s32 yStart;
  s32 yEnd;
#ifdef SUBTEXEL
  f32 subPixel;
#endif
  // sort on height, y
  if (F32_A_GREATER_B(a->Pos[1], b->Pos[1])) {
    swapVertexPointer(&a, &b);
  }
  if (F32_A_GREATER_B(b->Pos[1], c->Pos[1])) {
    swapVertexPointer(&b, &c);
  }
  if (F32_A_GREATER_B(a->Pos[1], b->Pos[1])) {
    swapVertexPointer(&a, &b);
  }
  ca = c->Pos[1] - a->Pos[1];
  ba = b->Pos[1] - a->Pos[1];
  cb = c->Pos[1] - b->Pos[1];
  // calculate delta y of the edges
  scan->invDeltaY[0] = reciprocal(ca);
  scan->invDeltaY[1] = reciprocal(ba);
  scan->invDeltaY[2] = reciprocal(cb);
  if (F32_LOWER_EQUAL_0(scan->invDeltaY[0])) {
    return;
  }
  // find if the major edge is left or right aligned
  temp[0] = a->Pos[0] - c->Pos[0];
  temp[1] = -ca;
  temp[2] = b->Pos[0] - a->Pos[0];
  temp[3] = ba;
  scan->left = (temp[0] * temp[3] - temp[1] * temp[2]) > 0.f ? 0 : 1;
  scan->right = 1 - scan->left;
  // calculate slopes for the major edge
  scan->slopeX[0] = (c->Pos[0] - a->Pos[0]) * scan->invDeltaY[0];
  scan->x[0] = a->Pos[0];
#ifdef IPOL_Z
  scan->slopeZ[0] = (c->Pos[2] - a->Pos[2]) * scan->invDeltaY[0];
  scan->z[0] = a->Pos[2];
#endif
#ifdef IPOL_W
  scan->slopeW[0] = (c->Pos[3] - a->Pos[3]) * scan->invDeltaY[0];
  scan->w[0] = a->Pos[3];
#endif
#ifdef IPOL_C0
  VOP2a_4(scan->slopeC[0][0], = , c->Color[0], -, a->Color[0], scan->invDeltaY[0]);
  VOP1_4(scan->c[0][0], = , a->Color[0]);
#endif
#ifdef IPOL_T0
  VOP2a_2(scan->slopeT[0][0], = , c->Tex[0], -, a->Tex[0], scan->invDeltaY[0]);
  VOP1_2(scan->t[0][0], = , a->Tex[0]);
#endif
#ifdef IPOL_T1
  VOP2a_2(scan->slopeT[1][0], = , c->Tex[1], -, a->Tex[1], scan->invDeltaY[0]);
  VOP1_2(scan->t[1][0], = , a->Tex[1]);
#endif
  // top left fill convention y run
  // rasterize upper sub-triangle
  if ((f32) 0.0 != scan->invDeltaY[1]) {
    // calculate slopes for top edge
    scan->slopeX[1] = (b->Pos[0] - a->Pos[0]) * scan->invDeltaY[1];
    scan->x[1] = a->Pos[0];
#ifdef IPOL_Z
    scan->slopeZ[1] = (b->Pos[2] - a->Pos[2]) * scan->invDeltaY[1];
    scan->z[1] = a->Pos[2];
#endif
#ifdef IPOL_W
    scan->slopeW[1] = (b->Pos[3] - a->Pos[3]) * scan->invDeltaY[1];
    scan->w[1] = a->Pos[3];
#endif
#ifdef IPOL_C0
    VOP2a_4(scan->slopeC[0][1], = , b->Color[0], -, a->Color[0], scan->invDeltaY[1]);
    VOP1_4(scan->c[0][1], = , a->Color[0]);
#endif
#ifdef IPOL_T0
    VOP2a_2(scan->slopeT[0][1], = , b->Tex[0], -, a->Tex[0], scan->invDeltaY[1]);
    VOP1_2(scan->t[0][1], = , a->Tex[0]);
#endif
#ifdef IPOL_T1
    VOP2a_2(scan->slopeT[1][1], = , b->Tex[1], -, a->Tex[1], scan->invDeltaY[1]);
    VOP1_2(scan->t[1][1], = , a->Tex[1]);
#endif
    // apply top-left fill convention, top part
    yStart = ceil32(a->Pos[1]);
    yEnd = ceil32(b->Pos[1]) - 1;
#ifdef SUBTEXEL
    subPixel = ((f32) yStart) - a->Pos[1];
    // correct to pixel center
    scan->x[0] += scan->slopeX[0] * subPixel;
    scan->x[1] += scan->slopeX[1] * subPixel;
#ifdef IPOL_Z
    scan->z[0] += scan->slopeZ[0] * subPixel;
    scan->z[1] += scan->slopeZ[1] * subPixel;
#endif
#ifdef IPOL_W
    scan->w[0] += scan->slopeW[0] * subPixel;
    scan->w[1] += scan->slopeW[1] * subPixel;
#endif
#ifdef IPOL_C0
    VOP1a_4(scan->c[0][0], += , scan->slopeC[0][0], *, subPixel);
    VOP1a_4(scan->c[0][1], += , scan->slopeC[0][1], *, subPixel);
#endif
#ifdef IPOL_T0
    VOP1a_2(scan->t[0][0], += , scan->slopeT[0][0], *, subPixel);
    VOP1a_2(scan->t[0][1], += , scan->slopeT[0][1], *, subPixel);
#endif
#ifdef IPOL_T1
    VOP1a_2(scan->t[1][0], += , scan->slopeT[1][0], *, subPixel);
    VOP1a_2(scan->t[1][1], += , scan->slopeT[1][1], *, subPixel);
#endif
#endif
    // rasterize the edge scanlines
    for (line->y = yStart; line->y <= yEnd; ++line->y) {
      line->x[scan->left] = scan->x[0];
      line->x[scan->right] = scan->x[1];
#ifdef IPOL_Z
      line->z[scan->left] = scan->z[0];
      line->z[scan->right] = scan->z[1];
#endif
#ifdef IPOL_W
      line->w[scan->left] = scan->w[0];
      line->w[scan->right] = scan->w[1];
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][scan->left], = , scan->c[0][0]);
      VOP1_4(line->c[0][scan->right], = , scan->c[0][1]);
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][scan->left], = , scan->t[0][0]);
      VOP1_2(line->t[0][scan->right], = , scan->t[0][1]);
#endif
#ifdef IPOL_T1
      VOP1_2(line->t[1][scan->left], = , scan->t[1][0]);
      VOP1_2(line->t[1][scan->right], = , scan->t[1][1]);
#endif
      // render a scanline
      CTRTextureGouraudAlpha2_scanline_bilinear(s);
      scan->x[0] += scan->slopeX[0];
      scan->x[1] += scan->slopeX[1];
#ifdef IPOL_Z
      scan->z[0] += scan->slopeZ[0];
      scan->z[1] += scan->slopeZ[1];
#endif
#ifdef IPOL_W
      scan->w[0] += scan->slopeW[0];
      scan->w[1] += scan->slopeW[1];
#endif
#ifdef IPOL_C0
      VOP1_4(scan->c[0][0], += , scan->slopeC[0][0]);
      VOP1_4(scan->c[0][1], += , scan->slopeC[0][1]);
#endif
#ifdef IPOL_T0
      VOP1_2(scan->t[0][0], += , scan->slopeT[0][0]);
      VOP1_2(scan->t[0][1], += , scan->slopeT[0][1]);
#endif
#ifdef IPOL_T1
      VOP1_2(scan->t[1][0], += , scan->slopeT[1][0]);
      VOP1_2(scan->t[1][1], += , scan->slopeT[1][1]);
#endif
    }
  }
  // rasterize lower sub-triangle
  if ((f32) 0.0 != scan->invDeltaY[2]) {
    // advance to middle point
    if ((f32) 0.0 != scan->invDeltaY[1]) {
      temp[0] = b->Pos[1] - a->Pos[1];  // dy
      scan->x[0] = a->Pos[0] + scan->slopeX[0] * temp[0];
#ifdef IPOL_Z
      scan->z[0] = a->Pos[2] + scan->slopeZ[0] * temp[0];
#endif
#ifdef IPOL_W
      scan->w[0] = a->Pos[3] + scan->slopeW[0] * temp[0];
#endif
#ifdef IPOL_C0
      VOP2b_4(scan->c[0][0], = , a->Color[0], +, scan->slopeC[0][0], temp[0]);
#endif
#ifdef IPOL_T0
      VOP2b_2(scan->t[0][0], = , a->Tex[0], +, scan->slopeT[0][0], temp[0]);
#endif
#ifdef IPOL_T1
      VOP2b_2(scan->t[1][0], = , a->Tex[1], +, scan->slopeT[1][0], temp[0]);
#endif
    }
    // calculate slopes for bottom edge
    scan->slopeX[1] = (c->Pos[0] - b->Pos[0]) * scan->invDeltaY[2];
    scan->x[1] = b->Pos[0];
#ifdef IPOL_Z
    scan->slopeZ[1] = (c->Pos[2] - b->Pos[2]) * scan->invDeltaY[2];
    scan->z[1] = b->Pos[2];
#endif
#ifdef IPOL_W
    scan->slopeW[1] = (c->Pos[3] - b->Pos[3]) * scan->invDeltaY[2];
    scan->w[1] = b->Pos[3];
#endif
#ifdef IPOL_C0
    VOP2a_4(scan->slopeC[0][1], = , c->Color[0], -, b->Color[0], scan->invDeltaY[2]);
    VOP1_4(scan->c[0][1], = , b->Color[0]);
#endif
#ifdef IPOL_T0
    VOP2a_2(scan->slopeT[0][1], = , c->Tex[0], -, b->Tex[0], scan->invDeltaY[2]);
    VOP1_2(scan->t[0][1], = , b->Tex[0]);
#endif
#ifdef IPOL_T1
    VOP2a_2(scan->slopeT[1][1], = , c->Tex[1], -, b->Tex[1], scan->invDeltaY[2]);
    VOP1_2(scan->t[1][1], = , b->Tex[1]);
#endif
    // apply top-left fill convention, top part
    yStart = ceil32(b->Pos[1]);
    yEnd = ceil32(c->Pos[1]) - 1;
#ifdef SUBTEXEL
    subPixel = ((f32) yStart) - b->Pos[1];
    // correct to pixel center
    scan->x[0] += scan->slopeX[0] * subPixel;
    scan->x[1] += scan->slopeX[1] * subPixel;
#ifdef IPOL_Z
    scan->z[0] += scan->slopeZ[0] * subPixel;
    scan->z[1] += scan->slopeZ[1] * subPixel;
#endif
#ifdef IPOL_W
    scan->w[0] += scan->slopeW[0] * subPixel;
    scan->w[1] += scan->slopeW[1] * subPixel;
#endif
#ifdef IPOL_C0
    VOP1a_4(scan->c[0][0], += , scan->slopeC[0][0], *, subPixel);
    VOP1a_4(scan->c[0][1], += , scan->slopeC[0][1], *, subPixel);
#endif
#ifdef IPOL_T0
    VOP1a_2(scan->t[0][0], += , scan->slopeT[0][0], *, subPixel);
    VOP1a_2(scan->t[0][1], += , scan->slopeT[0][1], *, subPixel);
#endif
#ifdef IPOL_T1
    VOP1a_2(scan->t[1][0], += , scan->slopeT[1][0], *, subPixel);
    VOP1a_2(scan->t[1][1], += , scan->slopeT[1][1], *, subPixel);
#endif
#endif
    // rasterize the edge scanlines
    for (line->y = yStart; line->y <= yEnd; ++line->y) {
      line->x[scan->left] = scan->x[0];
      line->x[scan->right] = scan->x[1];
#ifdef IPOL_Z
      line->z[scan->left] = scan->z[0];
      line->z[scan->right] = scan->z[1];
#endif
#ifdef IPOL_W
      line->w[scan->left] = scan->w[0];
      line->w[scan->right] = scan->w[1];
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][scan->left], = , scan->c[0][0]);
      VOP1_4(line->c[0][scan->right], = , scan->c[0][1]);
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][scan->left], = , scan->t[0][0]);
      VOP1_2(line->t[0][scan->right], = , scan->t[0][1]);
#endif
#ifdef IPOL_T1
      VOP1_2(line->t[1][scan->left], = , scan->t[1][0]);
      VOP1_2(line->t[1][scan->right], = , scan->t[1][1]);
#endif
      // render a scanline
      CTRTextureGouraudAlpha2_scanline_bilinear(s);
      scan->x[0] += scan->slopeX[0];
      scan->x[1] += scan->slopeX[1];
#ifdef IPOL_Z
      scan->z[0] += scan->slopeZ[0];
      scan->z[1] += scan->slopeZ[1];
#endif
#ifdef IPOL_W
      scan->w[0] += scan->slopeW[0];
      scan->w[1] += scan->slopeW[1];
#endif
#ifdef IPOL_C0
      VOP1_4(scan->c[0][0], += , scan->slopeC[0][0]);
      VOP1_4(scan->c[0][1], += , scan->slopeC[0][1]);
#endif
#ifdef IPOL_T0
      VOP1_2(scan->t[0][0], += , scan->slopeT[0][0]);
      VOP1_2(scan->t[0][1], += , scan->slopeT[0][1]);
#endif
#ifdef IPOL_T1
      VOP1_2(scan->t[1][0], += , scan->slopeT[1][0]);
      VOP1_2(scan->t[1][1], += , scan->slopeT[1][1]);
#endif
    }
  }
}
/*!
*/
void CTRTextureGouraudAlpha2_setParam(IBurningShader* s, u32 index, f32 value)
{
#ifdef BURNINGVIDEO_RENDERER_FAST
  s->AlphaRef = floor32(value * 256.f);
#else
  s->AlphaRef = u32_to_fixPoint(floor32(value * 256.f));
#endif
}

