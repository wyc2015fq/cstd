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
//#define IPOL_C0
#define IPOL_T0
//#define IPOL_T1
// apply global override
#ifndef SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
#undef INVERSE_W
#endif
#ifndef SOFTWARE_DRIVER_2_SUBTEXEL
#undef SUBTEXEL
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
// swap integer with xor
#define swap_xor(a, b)   (a ^= b, b ^= a, a ^= b)
// IBurningShader
void CTRTextureWire2_renderLine(IBurningShader* s, const s4DVertex* a, const s4DVertex* b)
{
  int pitch0 = s->RenderTarget->Size.w << VIDEO_SAMPLE_GRANULARITY;
  int pitch1 = s->RenderTarget->Size.w << 2;
  int aposx = (int) a->Pos[0];
  int aposy = (int) a->Pos[1];
  int bposx = (int) b->Pos[0];
  int bposy = (int) b->Pos[1];
  int dx = bposx - aposx;
  int dy = bposy - aposy;
  int c;
  int m;
  int d = 0;
  int run;
  tVideoSample* dst;
#ifdef USE_ZBUFFER
  fp24* z;
#endif
  int xInc0 = 1 << VIDEO_SAMPLE_GRANULARITY;
  int yInc0 = pitch0;
  int xInc1 = 4;
  int yInc1 = pitch1;
  tVideoSample color;
#ifdef IPOL_Z
  f32 slopeZ, dataZ;
#endif
#ifdef IPOL_W
  fp24 slopeW, dataW;
#endif
#ifdef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
  tFixPoint r0, g0, b0;
  getSample_color_rgb(&r0, &g0, &b0, a->Color[0]);
  color = fix_to_color_rgb(r0, g0, b0);
#else
  color = (tVideoSample) 0xFFFFFFFF;
#endif
  if (dx < 0) {
    xInc0 = - (1 << VIDEO_SAMPLE_GRANULARITY);
    xInc1 = -4;
    dx = -dx;
  }
  if (dy > dx) {
    swap_xor(dx, dy);
    swap_xor(xInc0, yInc0);
    swap_xor(xInc1, yInc1);
  }
  if (0 == dx) {
    return;
  }
  dst = (tVideoSample*)((u8*)(tVideoSample*)s->RenderTarget->Data + (aposy * pitch0) + (aposx << VIDEO_SAMPLE_GRANULARITY));
#ifdef USE_ZBUFFER
  z = (fp24*)((u8*)(fp24*)(s->DepthBuffer->Data + aposy * pitch1) + (aposx << 2));
#endif
  c = dx << 1;
  m = dy << 1;
#ifdef IPOL_Z
  slopeZ = (b->Pos[2] - a->Pos[2]) / (f32)(dx);
  dataZ = a->Pos[2];
#endif
#ifdef IPOL_W
  slopeW = (b->Pos[3] - a->Pos[3]) / (f32)(dx);
  dataW = a->Pos[3];
#endif
  run = dx;
  while (run) {
#ifdef CMP_Z
    if (*z >= dataZ)
#endif
#ifdef CMP_W
      if (dataW >= *z)
#endif
      {
#ifdef WRITE_Z
        *z = dataZ;
#endif
#ifdef WRITE_W
        *z = dataW;
#endif
        *dst = color;
      }
    dst = (tVideoSample*)((u8*) dst + xInc0);    // x += xInc
#ifdef IPOL_Z
    z = (fp24*)((u8*) z + xInc1);
#endif
#ifdef IPOL_W
    z = (fp24*)((u8*) z + xInc1);
#endif
    d += m;
    if (d > dx) {
      dst = (tVideoSample*)((u8*) dst + yInc0);    // y += yInc
#ifdef IPOL_Z
      z = (fp24*)((u8*) z + yInc1);
#endif
#ifdef IPOL_W
      z = (fp24*)((u8*) z + yInc1);
#endif
      d -= c;
    }
    run -= 1;
#ifdef IPOL_Z
    dataZ += slopeZ;
#endif
#ifdef IPOL_W
    dataW += slopeW;
#endif
  }
}
void CTRTextureWire2_drawTriangle(IBurningShader* s, const s4DVertex* a, const s4DVertex* b, const s4DVertex* c)
{
  //sScanLineData line;
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
  CTRTextureWire2_renderLine(s, a, b);
  CTRTextureWire2_renderLine(s, b, c);
  CTRTextureWire2_renderLine(s, a, c);
}
void CTRTextureWire2_drawLine(IBurningShader* s, const s4DVertex* a, const s4DVertex* b)
{
  // query access to TexMaps
  // sort on height, y
  if (a->Pos[1] > b->Pos[1]) {
    swapVertexPointer(&a, &b);
  }
  CTRTextureWire2_renderLine(s, a, b);
}

