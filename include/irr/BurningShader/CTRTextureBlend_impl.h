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
void CTRTextureBlend_fragment_dst_color_src_alpha(IBurningShader* s)
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
  f32 iw =  FIX_POINT_F32_MUL;
  tFixPoint a0, r0, g0, b0;
  tFixPoint     r1, g1, b1;
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
  switch (s->ZCompare) {
  case 1:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] >= z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_argb(&a0, &r0, &g0, &b0, &s->IT[0], tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        color_to_fix_rgb(&r1, &g1, &b1, dst[i]);
        dst[i] = fix_to_color_rgb(clampfix_maxcolor(imulFix_tex2(r0, r1)), clampfix_maxcolor(imulFix_tex2(g0, g1)),
            clampfix_maxcolor(imulFix_tex2(b0, b1)));
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  case 2:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] == z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_argb(&a0, &r0, &g0, &b0,
            &s->IT[0],
            tofix(line->t[0][0][0], iw),
            tofix(line->t[0][0][1], iw)
                              );
        color_to_fix_rgb(&r1, &g1, &b1, dst[i]);
        dst[i] = fix_to_color_rgb(clampfix_maxcolor(imulFix_tex2(r0, r1)),
            clampfix_maxcolor(imulFix_tex2(g0, g1)),
            clampfix_maxcolor(imulFix_tex2(b0, b1))
                                 );
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  } // zcompare
}
/*!
*/
void CTRTextureBlend_fragment_src_color_src_alpha(IBurningShader* s)
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
  f32 iw =  FIX_POINT_F32_MUL;
  tFixPoint a0, r0, g0, b0;
  tFixPoint     r1, g1, b1;
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
  switch (s->ZCompare) {
  case 1:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] >= z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_argb(&a0, &r0, &g0, &b0, &s->IT[0],  tofix(line->t[0][0][0], iw),  tofix(line->t[0][0][1], iw));
        color_to_fix_rgb(&r1, &g1, &b1, dst[i]);
        //    u32 check = imulFix_tex1( r0, r1 );
        dst[i] = fix_to_color_rgb(clampfix_maxcolor(imulFix_tex1(r0, r1) + imulFix_tex1(r1, a0)),
            clampfix_maxcolor(imulFix_tex1(g0, g1) + imulFix_tex1(g1, a0)),
            clampfix_maxcolor(imulFix_tex1(b0, b1) + imulFix_tex1(b1, a0))
                                 );
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  case 2:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] == z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_argb(&a0, &r0, &g0, &b0,
            &s->IT[0],
            tofix(line->t[0][0][0], iw),
            tofix(line->t[0][0][1], iw)
                              );
        color_to_fix_rgb(&r1, &g1, &b1, dst[i]);
        dst[i] = fix_to_color_rgb(clampfix_maxcolor(imulFix_tex2(r0, r1)),
            clampfix_maxcolor(imulFix_tex2(g0, g1)),
            clampfix_maxcolor(imulFix_tex2(b0, b1))
                                 );
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  } // zcompare
}
/*!
*/
void CTRTextureBlend_fragment_one_one_minus_src_alpha(IBurningShader* s)
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
  f32 iw = FIX_POINT_F32_MUL;
  tFixPoint a0, r0, g0, b0;
  tFixPoint  r1, g1, b1;
#ifdef IPOL_C0
  tFixPoint  r2, g2, b2;
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
  switch (s->ZCompare) {
  case 1:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] >= z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_argb(&a0, &r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        a0 = FIX_POINT_ONE - a0;
        color_to_fix1_rgb(&r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
        getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
        dst[i] = fix_to_color_rgb(imulFix(r0 + imulFix(r1, a0), r2), imulFix(g0 + imulFix(g1, a0), g2),
            imulFix(b0 + imulFix(b1, a0), b2));
#else
        dst[i] = fix_to_color_rgb(r0 + imulFix(r1, a0),
            g0 + imulFix(g1, a0),
            b0 + imulFix(b1, a0)
                                 );
#endif
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  case 2:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] == z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_argb(&a0, &r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        a0 = FIX_POINT_ONE - a0;
        color_to_fix1_rgb(&r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
        getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
        dst[i] = fix_to_color_rgb(imulFix(r0 + imulFix(r1, a0), r2),
            imulFix(g0 + imulFix(g1, a0), g2),
            imulFix(b0 + imulFix(b1, a0), b2)
                                 );
#else
        dst[i] = fix_to_color_rgb(r0 + imulFix(r1, a0),
            g0 + imulFix(g1, a0),
            b0 + imulFix(b1, a0)
                                 );
#endif
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  } // zcompare
}
/*!
*/
void CTRTextureBlend_fragment_one_minus_dst_alpha_one(IBurningShader* s)
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
  f32 iw = FIX_POINT_F32_MUL;
  tFixPoint r0, g0, b0;
  tFixPoint a1, r1, g1, b1;
#ifdef IPOL_C0
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
  switch (s->ZCompare) {
  case 1:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] >= z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_rgb(&r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        color_to_fix1_argb(&a1, &r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
        getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
        a1 = FIX_POINT_ONE - a1;
        dst[i] = fix_to_color_rgb(imulFix(imulFix(r0, a1) + r1, r2),
            imulFix(imulFix(g0, a1) + g1, g2),
            imulFix(imulFix(b0, a1) + b1, b2)
                                 );
#else
        dst[i] = fix_to_color_rgb(imulFix(r0, a1) + r0,
            imulFix(g0, a1) + g0,
            imulFix(b0, a1) + b0
                                 );
#endif
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  case 2:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] == z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_rgb(&r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        color_to_fix1_argb(&a1, &r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
        getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
        a1 = FIX_POINT_ONE - a1;
        dst[i] = fix_to_color_rgb(imulFix(imulFix(r0, a1) + r1, r2),
            imulFix(imulFix(g0, a1) + g1, g2),
            imulFix(imulFix(b0, a1) + b1, b2)
                                 );
#else
        dst[i] = fix_to_color_rgb(imulFix(r0, a1) + r0,
            imulFix(g0, a1) + g0,
            imulFix(b0, a1) + b0
                                 );
#endif
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  } // zcompare
}
/*!
*/
void CTRTextureBlend_fragment_src_alpha_one(IBurningShader* s)
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
  f32 iw = FIX_POINT_F32_MUL;
  tFixPoint a0, r0, g0, b0;
  tFixPoint r1, g1, b1;
#ifdef IPOL_C0
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
  switch (s->ZCompare) {
  case 1:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] >= z[i])
#endif
      {
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_argb(&a0, &r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        if (a0 > 0) {
          a0 >>= 8;
          color_to_fix_rgb(&r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
          getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
          dst[i] = fix4_to_color_argb(a0,
              clampfix_maxcolor(imulFix(r0, a0) + r1),
              clampfix_maxcolor(imulFix(g0, a0) + g1),
              clampfix_maxcolor(imulFix(b0, a0) + b1)
                                     );
          /*
              a0 >>= 8;
              dst[i] = fix4_to_color ( a0,
                          imulFix ( imulFix ( r0, a0 ) + r1, r2 ),
                          imulFix ( imulFix ( g0, a0 ) + g1, g2 ),
                          imulFix ( imulFix ( b0, a0 ) + b1, b2 )
                        );
          */
#else
          dst[i] = fix4_to_color_argb(a0,
              clampfix_maxcolor(imulFix(r0, a0) + r1),
              clampfix_maxcolor(imulFix(g0, a0) + g1),
              clampfix_maxcolor(imulFix(b0, a0) + b1)
                                     );
#endif
#ifdef WRITE_W
          //z[i] = line->w[0];
#endif
        }
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  case 2:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] == z[i])
#endif
      {
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_argb(&a0, &r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        if (a0 > 0) {
          a0 >>= 8;
          color_to_fix_rgb(&r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
          getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
          dst[i] = fix4_to_color_argb(a0,
              clampfix_maxcolor(imulFix(imulFix(r0, a0) + r1, r2)),
              clampfix_maxcolor(imulFix(imulFix(g0, a0) + g1, g2)),
              clampfix_maxcolor(imulFix(imulFix(b0, a0) + b1, b2))
                                     );
          /*
              a0 >>= 8;
              dst[i] = fix4_to_color ( a0,
                          imulFix ( imulFix ( r0, a0 ) + r1, r2 ),
                          imulFix ( imulFix ( g0, a0 ) + g1, g2 ),
                          imulFix ( imulFix ( b0, a0 ) + b1, b2 )
                        );
          */
#else
          dst[i] = fix4_to_color_argb(a0,
              clampfix_maxcolor(imulFix(r0, a0) + r1),
              clampfix_maxcolor(imulFix(g0, a0) + g1),
              clampfix_maxcolor(imulFix(b0, a0) + b1)
                                     );
#endif
#ifdef WRITE_W
          z[i] = line->w[0];
#endif
        }
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  } // zcompare
}
/*!
*/
void CTRTextureBlend_fragment_dst_color_one_minus_dst_alpha(IBurningShader* s)
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
  f32 iw = FIX_POINT_F32_MUL;
  tFixPoint r0, g0, b0;
  tFixPoint a1, r1, g1, b1;
#ifdef IPOL_C0
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
  switch (s->ZCompare) {
  case 1:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] >= z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_rgb(&r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        color_to_fix1_argb(&a1, &r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
        getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
        a1 = FIX_POINT_ONE - a1;
        dst[i] = fix_to_color_rgb(imulFix(imulFix(r1, r0 + a1), r2),
            imulFix(imulFix(g1, g0 + a1), g2),
            imulFix(imulFix(b1, b0 + a1), b2)
                                 );
#else
        dst[i] = fix_to_color_rgb(imulFix(r1, r0 + a1),
            imulFix(g1, g0 + a1),
            imulFix(b1, b0 + a1)
                                 );
#endif
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  case 2:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] == z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_rgb(&r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        color_to_fix1_argb(&a1, &r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
        getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
        a1 = FIX_POINT_ONE - a1;
        dst[i] = fix_to_color_rgb(imulFix(imulFix(r1, r0 + a1), r2),
            imulFix(imulFix(g1, g0 + a1), g2),
            imulFix(imulFix(b1, b0 + a1), b2)
                                 );
#else
        dst[i] = fix_to_color_rgb(imulFix(r1, r0 + a1),
            imulFix(g1, g0 + a1),
            imulFix(b1, b0 + a1)
                                 );
#endif
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  } // zcompare
}
/*!
*/
void CTRTextureBlend_fragment_dst_color_zero(IBurningShader* s)
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
  f32 iw = FIX_POINT_F32_MUL;
  tFixPoint r0, g0, b0;
  tFixPoint r1, g1, b1;
#ifdef IPOL_C0
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
  switch (s->ZCompare) {
  case 1:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] >= z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_rgb(&r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        color_to_fix1_rgb(&r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
        getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
        dst[i] = fix_to_color_rgb(imulFix(imulFix(r0, r1), r2),
            imulFix(imulFix(g0, g1), g2),
            imulFix(imulFix(b0, b1), b2));
#else
        dst[i] = fix_to_color_rgb(imulFix(r0, r1),
            imulFix(g0, g1),
            imulFix(b0, b1)
                                 );
#endif
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  case 2:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] == z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_rgb(&r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        color_to_fix1_rgb(&r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
        getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
        dst[i] = fix_to_color_rgb(imulFix(imulFix(r0, r1), r2),
            imulFix(imulFix(g0, g1), g2),
            imulFix(imulFix(b0, b1), b2)
                                 );
#else
        dst[i] = fix_to_color_rgb(imulFix(r0, r1),
            imulFix(g0, g1),
            imulFix(b0, b1)
                                 );
#endif
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  } // zcompare
}
/*!
*/
void CTRTextureBlend_fragment_dst_color_one(IBurningShader* s)
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
  f32 iw = FIX_POINT_F32_MUL;
  tFixPoint r0, g0, b0;
  tFixPoint r1, g1, b1;
#ifdef IPOL_C0
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
  switch (s->ZCompare) {
  case 1:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] >= z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_rgb(&r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        color_to_fix_rgb(&r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
        getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
        dst[i] = fix_to_color_rgb(clampfix_maxcolor(imulFix_tex1(r0, r1) + r1),
            clampfix_maxcolor(imulFix_tex1(g0, g1) + g1),
            clampfix_maxcolor(imulFix_tex1(b0, b1) + b1)
                                 );
#else
        dst[i] = fix_to_color_rgb(clampfix_maxcolor(imulFix_tex1(r0, r1) + r1),
            clampfix_maxcolor(imulFix_tex1(g0, g1) + g1),
            clampfix_maxcolor(imulFix_tex1(b0, b1) + b1)
                                 );
#endif
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  case 2:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] == z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_rgb(&r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        color_to_fix_rgb(&r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
        getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
        dst[i] = fix_to_color_rgb(clampfix_maxcolor(imulFix_tex1(r0, r1) + r1),
            clampfix_maxcolor(imulFix_tex1(g0, g1) + g1),
            clampfix_maxcolor(imulFix_tex1(b0, b1) + b1)
                                 );
#else
        dst[i] = fix_to_color_rgb(clampfix_maxcolor(imulFix_tex1(r0, r1) + r1),
            clampfix_maxcolor(imulFix_tex1(g0, g1) + g1),
            clampfix_maxcolor(imulFix_tex1(b0, b1) + b1)
                                 );
#endif
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  } // zcompare
}
/*!
*/
void CTRTextureBlend_fragment_zero_one_minus_scr_color(IBurningShader* s)
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
  f32 iw = FIX_POINT_F32_MUL;
  tFixPoint r0, g0, b0;
  tFixPoint r1, g1, b1;
#ifdef IPOL_C0
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
  switch (s->ZCompare) {
  case 1:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] >= z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_rgb(&r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        color_to_fix1_rgb(&r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
        getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
        dst[i] = fix_to_color_rgb(imulFix(FIX_POINT_ONE - r0, r1),
            imulFix(FIX_POINT_ONE - g0, g1),
            imulFix(FIX_POINT_ONE - b0, b1)
                                 );
#else
        dst[i] = fix_to_color_rgb(imulFix(FIX_POINT_ONE - r0, r1),
            imulFix(FIX_POINT_ONE - g0, g1),
            imulFix(FIX_POINT_ONE - b0, b1)
                                 );
#endif
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  case 2:
    for (i = 0; i <= dx; ++i) {
#ifdef CMP_W
      if (line->w[0] == z[i])
#endif
      {
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
#ifdef INVERSE_W
        iw = fix_inverse32(line->w[0]);
#endif
        getSample_texture_rgb(&r0, &g0, &b0, s->IT + 0, tofix(line->t[0][0][0], iw), tofix(line->t[0][0][1], iw));
        color_to_fix1_rgb(&r1, &g1, &b1, dst[i]);
#ifdef IPOL_C0
        getSample_color_rgb2(&r2, &g2, &b2, line->c[0][0], iw);
        dst[i] = fix_to_color_rgb(imulFix(FIX_POINT_ONE - r0, r1),
            imulFix(FIX_POINT_ONE - g0, g1),
            imulFix(FIX_POINT_ONE - b0, b1)
                                 );
#else
        dst[i] = fix_to_color_rgb(imulFix(FIX_POINT_ONE - r0, r1),
            imulFix(FIX_POINT_ONE - g0, g1),
            imulFix(FIX_POINT_ONE - b0, b1)
                                 );
#endif
      }
#ifdef IPOL_W
      line->w[0] += slopeW;
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
    }
    break;
  } // zcompare
}
void CTRTextureBlend_drawTriangle(IBurningShader* s, const s4DVertex* a, const s4DVertex* b, const s4DVertex* c)
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
  if (0 == s->fragmentShader) {
    return;
  }
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
      (s->fragmentShader)(s);
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
      (s->fragmentShader)(s);
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
void CTRTextureBlend_setParam(IBurningShader* s, u32 index, f32 value)
{
  u8 showname = 0;
  E_BLEND_FACTOR srcFact, dstFact;
  E_MODULATE_FUNC modulate;
  u32 alphaSrc;
  static const c8* n[] = {
    "gl_zero",
    "gl_one",
    "gl_dst_color",
    "gl_one_minus_dst_color",
    "gl_src_color",
    "gl_one_minus_src_color",
    "gl_src_alpha",
    "gl_one_minus_src_alpha",
    "gl_dst_alpha",
    "gl_one_minus_dst_alpha",
    "gl_src_alpha_saturate"
  };
  static E_BLEND_FACTOR lsrcFact = EBF_ZERO;
  static E_BLEND_FACTOR ldstFact = EBF_ZERO;
  unpack_textureBlendFunc(&srcFact, &dstFact, &modulate, &alphaSrc, value);
  s->fragmentShader = 0;
  if (srcFact == EBF_DST_COLOR && dstFact == EBF_ZERO) {
    s->fragmentShader = &CTRTextureBlend_fragment_dst_color_zero;
  }
  else if (srcFact == EBF_DST_COLOR && dstFact == EBF_ONE) {
    s->fragmentShader = &CTRTextureBlend_fragment_dst_color_one;
  }
  else if (srcFact == EBF_DST_COLOR && dstFact == EBF_SRC_ALPHA) {
    s->fragmentShader = &CTRTextureBlend_fragment_dst_color_src_alpha;
  }
  else if (srcFact == EBF_DST_COLOR && dstFact == EBF_ONE_MINUS_DST_ALPHA) {
    s->fragmentShader = &CTRTextureBlend_fragment_dst_color_one_minus_dst_alpha;
  }
  else if (srcFact == EBF_ZERO && dstFact == EBF_ONE_MINUS_SRC_COLOR) {
    s->fragmentShader = &CTRTextureBlend_fragment_zero_one_minus_scr_color;
  }
  else if (srcFact == EBF_ONE && dstFact == EBF_ONE_MINUS_SRC_ALPHA) {
    s->fragmentShader = &CTRTextureBlend_fragment_one_one_minus_src_alpha;
  }
  else if (srcFact == EBF_ONE_MINUS_DST_ALPHA && dstFact == EBF_ONE) {
    s->fragmentShader = &CTRTextureBlend_fragment_one_minus_dst_alpha_one;
  }
  else if (srcFact == EBF_SRC_ALPHA && dstFact == EBF_ONE) {
    s->fragmentShader = &CTRTextureBlend_fragment_src_alpha_one;
  }
  else if (srcFact == EBF_SRC_COLOR && dstFact == EBF_SRC_ALPHA) {
    s->fragmentShader = &CTRTextureBlend_fragment_src_color_src_alpha;
  }
  else {
    showname = 1;
    s->fragmentShader = &CTRTextureBlend_fragment_dst_color_zero;
  }
  if (showname && (lsrcFact != srcFact || ldstFact != dstFact)) {
    llogprintf(ELL_INFORMATION, "missing shader: %s %s", n[srcFact], n[dstFact]);
    lsrcFact = srcFact;
    ldstFact = dstFact;
  }
}
