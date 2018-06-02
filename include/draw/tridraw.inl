
/////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
  int ElemCount;
  ImVec4 ClipRect; // Clipping rectangle (x1, y1, x2, y2)
  texture_t* tex;
  int vtxtype; // 0 color 1 tex
} drawcmd_t;
static void drawcmd_init(drawcmd_t* cmd)
{
  cmd->ElemCount = 0;
  cmd->ClipRect.x = cmd->ClipRect.y = -8192.0f;
  cmd->ClipRect.z = cmd->ClipRect.w = +8192.0f;
  cmd->tex = 0;
}
typedef unsigned short ImDrawIdx;
typedef struct {
  ImVertex* VtxBuffer;
  int VtxBuffer_Size;
  ImDrawIdx* IdxBuffer;
  int IdxBuffer_Size;
  int Idx_off;
  drawcmd_t* CmdBuffer;
  int CmdBuffer_Size;
  int _ChannelsCurrent;
  int _ChannelsCount;
  //
  Path path[1];
  int textAlign;
  COLOR fillColor;
  COLOR lineColor;
  float lineWidth;
  float miterLimit;
  int lineStyle;
  bool anti_aliased;
  ImVec4 ClipRect;
  texture_t* tex;
  brush_t brush[1];
  brush_t pen[1];
  int vtxtype;
  spanv_t sp[1];
} ImDrawList;
/////////////////////////////////////////////////////////////////////////////////
#define ImVec2_mul_s(lhs, rhs) fVec2(lhs.x * rhs, lhs.y * rhs)
#define ImVec2_div_s(lhs, rhs) fVec2(lhs.x / rhs, lhs.y / rhs)
#define ImVec2_add_s(lhs, rhs) fVec2(lhs.x + rhs.x, lhs.y + rhs.y)
#define ImVec2_sub_s(lhs, rhs) fVec2(lhs.x - rhs.x, lhs.y - rhs.y)
#define ImVec2_mul(lhs, rhs) fVec2(lhs.x * rhs.x, lhs.y * rhs.y)
#define ImVec2_div(lhs, rhs) fVec2(lhs.x / rhs.x, lhs.y / rhs.y)
#define ImVec2_add_to(lhs, rhs) (lhs.x += rhs.x, lhs.y += rhs.y)
#define ImVec2_sub_to(lhs, rhs) (lhs.x -= rhs.x, lhs.y -= rhs.y, lsh)
#define ImVec2_mul_s_to(lhs, rhs) (lhs.x *= rhs, lhs.y *= rhs, lsh)
#define ImVec2_div_s_to(lhs, rhs) (lhs.x /= rhs, lhs.y /= rhs, lsh)
#define ImVec4_sub(lhs, rhs) fVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w)
#define ImVec2_Min(lhs, rhs) fVec2(ImMin(lhs.x, rhs.x), ImMin(lhs.y, rhs.y))
#define ImVec2_Max(lhs, rhs) fVec2(ImMax(lhs.x, rhs.x), ImMax(lhs.y, rhs.y))
#define ImVec2_Clamp(f, mn, mx) fVec2(ImClamp(f.x, mn.x, mx.x), ImClamp(f.y, mn.y, mx.y))
/////////////////////////////////////////////////////////////////////////////////
#define ImMin(a, b) ((a)<(b)?(a):(b))
#define ImMax(a, b) ((a)>(b)?(a):(b))
#define ImClamp(v, mn, mx) (((v) < (mn)) ? (mn) : ((v) > (mx)) ? (mx) : (v))
//-----------------------------------------------------------------------------
CC_INLINE float ImSaturate(float f)
{
  return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f;
}
CC_INLINE float ImLerp(float a, float b, float t)
{
  return a + (b - a) * t;
}
#define ImLerp2(a, b, t) fVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y)
#define LengthSqr(lhs) (lhs.x * lhs.x + lhs.y * lhs.y)
#define ImLengthSqr(lhs) (lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z + lhs.w * lhs.w)
CC_INLINE float ImInvLength(ImVec2 lhs, float fail_value)
{
  float d = lhs.x * lhs.x + lhs.y * lhs.y;
  if (d > 0.0f) {
    return 1.0f / sqrtf(d);
  }
  return fail_value;
}
CC_INLINE float ImFloor(float f)
{
  return (float)(int)f;
}
CC_INLINE ImVec2 ImFloorV2(ImVec2 v)
{
  return fVec2((float)(int)v.x, (float)(int)v.y);
}
CC_INLINE float ImDistSqr(ImVec2 a, ImVec2 b)
{
  return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}
float ImTriangleArea(ImVec2 p1, ImVec2 p2, ImVec2 p3)
{
  float a = (float)fabs((p2.x * p1.y - p1.x * p2.y) + (p3.x * p2.y - p2.x * p3.y) + (p1.x * p3.y - p3.x * p1.y)) / 2;
  return a;
}
CC_INLINE int ImUpperPowerOfTwo(int v)
{
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
  ImVec2 FontTexUvWhitePixel;
  float CurveTessellationTol;
} xxx;
static xxx GImGui[1] = {0};
#define IDL_BEG(nIdx, nVtx) { ImDrawIdx _idx = (ImDrawIdx)g->VtxBuffer_Size; \
    ImDrawIdx* _IdxWritePtr_0 = (ImDrawIdx*)myAllocPush(g->IdxBuffer, g->IdxBuffer_Size, nIdx), *_IdxWritePtr_00 = _IdxWritePtr_0, * _IdxWritePtr_1=_IdxWritePtr_0+nIdx;\
    ImVertex* _VtxWritePtr_0 = (ImVertex*)myAllocPush(g->VtxBuffer, g->VtxBuffer_Size, nVtx), *_VtxWritePtr_00 = _VtxWritePtr_0, * _VtxWritePtr_1=_VtxWritePtr_0+nVtx;
#define IDL_IDX1(a) (*_IdxWritePtr_0++ = (ImDrawIdx)(_idx + (a)))
#define IDL_IDX3(a, b, c) (IDL_IDX1(a), IDL_IDX1(b), IDL_IDX1(c))
#define IDL_VTX(PT, UV, COL) (SETPT(_VtxWritePtr_0->pos, (PT).x, (PT).y), _VtxWritePtr_0->uv = (UV), _VtxWritePtr_0->col = (COL), ++_VtxWritePtr_0)
#define IDL_VTX2(X, Y, UVX, UVY, COL) (_VtxWritePtr_0->pos.x = (X), _VtxWritePtr_0->pos.y = (Y), _VtxWritePtr_0->uv.x = (UVX), _VtxWritePtr_0->uv.y = (UVY), _VtxWritePtr_0->col = (COL), ++_VtxWritePtr_0)
#define IDL_END() ASSERT(_IdxWritePtr_0<=_IdxWritePtr_1 && _VtxWritePtr_0<=_VtxWritePtr_1); }
// Fully unrolled with inline call to keep our debug builds decently fast.
void triPrimQuadUVMultiColor(ImDrawList* g, ImVec2 a, ImVec2 b, ImVec2 c, ImVec2 d, ImVec2 uv_a, ImVec2 uv_b, ImVec2 uv_c, ImVec2 uv_d, COLOR col_a, COLOR col_b, COLOR col_c, COLOR col_d)
{
  //if (((col_a | col_b | col_c | col_d) >> 24) == 0) { return; } else
  {
    IDL_BEG(6, 4);
    IDL_IDX3(0, 1, 2);
    IDL_IDX3(0, 2, 3);
    IDL_VTX(a, uv_a, col_a);
    IDL_VTX(b, uv_b, col_b);
    IDL_VTX(c, uv_c, col_c);
    IDL_VTX(d, uv_d, col_d);
    IDL_END();
  }
}
void triPrimQuadUV(ImDrawList* g, ImVec2 a, ImVec2 b, ImVec2 c, ImVec2 d, ImVec2 uv_a, ImVec2 uv_b, ImVec2 uv_c, ImVec2 uv_d, COLOR col)
{
  triPrimQuadUVMultiColor(g, a, b, c, d, uv_a, uv_b, uv_c, uv_d, col, col, col, col);
}
void triPrimRectMultiColor(ImDrawList* g, ImVec2 a, ImVec2 c, COLOR col_a, COLOR col_b, COLOR col_c, COLOR col_d)
{
  ImVec2 b = fVec2(c.x, a.y), d = fVec2(a.x, c.y);
  const ImVec2 uv = GImGui->FontTexUvWhitePixel;
  triPrimQuadUVMultiColor(g, a, b, c, d, uv, uv, uv, uv, col_a, col_b, col_c, col_d);
}
void triPrimRectUV(ImDrawList* g, ImVec2 a, ImVec2 c, ImVec2 uv_a, ImVec2 uv_c, COLOR col)
{
  ImVec2 b = fVec2(c.x, a.y), d = fVec2(a.x, c.y), uv_b = fVec2(uv_c.x, uv_a.y), uv_d = fVec2(uv_a.x, uv_c.y);
  triPrimQuadUV(g, a, b, c, d, uv_a, uv_b, uv_c, uv_d, col);
}
void triPrimRect(ImDrawList* g, ImVec2 a, ImVec2 c, COLOR col)
{
  triPrimRectUV(g, a, c, GImGui->FontTexUvWhitePixel, GImGui->FontTexUvWhitePixel, col);
}
// TODO: Thickness anti-aliased lines cap are missing their AA fringe.
void triPolyline(ImDrawList* g, const FPOINT* points, const int points_count, COLOR col, BOOL closed, float thickness, BOOL anti_aliased)
{
  int i, i1, count, idx1;
  ImVec2 uv;
  const BOOL thick_line = thickness > 1.0f;
  int _VtxCurrentIdx = 0;
  ImVec2 diff;
  float invlen;
  if (points_count < 2) {
    return;
  }
  uv = GImGui->FontTexUvWhitePixel;
  //anti_aliased &= GImGui->Style.AntiAliasedLines;
  //if (imuiGetIO(g).KeyCtrl) anti_aliased = false; // Debug
  count = points_count;
  if (!closed) {
    count = points_count - 1;
  }
  if (anti_aliased) {
    // Anti-aliased stroke
    const float AA_SIZE = 1.0f;
    const COLOR col_trans = col & 0x00ffffff;
    const int idx_count = thick_line ? count * 18 : count * 12;
    const int vtx_count = thick_line ? points_count * 4 : points_count * 3;
    ImVec2* temp_normals;
    ImVec2* temp_points;
    ImVec2 dm;
    float dmr2;
    ImVec2 dm_out;
    ImVec2 dm_in;
    IDL_BEG(idx_count, vtx_count);
    // Temporary buffer
    temp_normals = (ImVec2*)alloca(points_count * (thick_line ? 5 : 3) * sizeof(ImVec2));
    temp_points = temp_normals + points_count;
    for (i1 = 0; i1 < count; i1++) {
      const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
      diff.x = (float)(points[i2].x - points[i1].x);
      diff.y = (float)(points[i2].y - points[i1].y);
      invlen = ImInvLength(diff, 1.0f);
      diff.x *= invlen;
      diff.y *= invlen;
      temp_normals[i1].x = diff.y;
      temp_normals[i1].y = -diff.x;
    }
    if (!closed) {
      temp_normals[points_count - 1] = temp_normals[points_count - 2];
    }
    if (!thick_line) {
      if (!closed) {
        temp_points[0].x = points[0].x + temp_normals[0].x * AA_SIZE;
        temp_points[0].y = points[0].y + temp_normals[0].y * AA_SIZE;
        temp_points[1].x = points[0].x - temp_normals[0].x * AA_SIZE;
        temp_points[1].y = points[0].y - temp_normals[0].y * AA_SIZE;
        temp_points[(points_count - 1) * 2 + 0].x = points[points_count - 1].x + temp_normals[points_count - 1].x * AA_SIZE;
        temp_points[(points_count - 1) * 2 + 0].y = points[points_count - 1].y + temp_normals[points_count - 1].y * AA_SIZE;
        temp_points[(points_count - 1) * 2 + 1].x = points[points_count - 1].x - temp_normals[points_count - 1].x * AA_SIZE;
        temp_points[(points_count - 1) * 2 + 1].y = points[points_count - 1].y - temp_normals[points_count - 1].y * AA_SIZE;
      }
      // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
      idx1 = _VtxCurrentIdx;
      for (i1 = 0; i1 < count; i1++) {
        const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
        unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : idx1 + 3;
        // Average normals
        dm.x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
        dm.y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
        dmr2 = dm.x * dm.x + dm.y * dm.y;
        if (dmr2 > 0.000001f) {
          float scale = 1.0f / dmr2;
          if (scale > 100.0f) {
            scale = 100.0f;
          }
          dm.x *= scale;
          dm.y *= scale;
        }
        dm.x *= AA_SIZE;
        dm.y *= AA_SIZE;
        temp_points[i2 * 2 + 0].x = points[i2].x + dm.x;
        temp_points[i2 * 2 + 0].y = points[i2].y + dm.y;
        temp_points[i2 * 2 + 1].x = points[i2].x - dm.x;
        temp_points[i2 * 2 + 1].y = points[i2].y - dm.y;
        // Add indexes
        IDL_IDX3(idx2 + 0, idx1 + 0, idx1 + 2);
        IDL_IDX3(idx1 + 2, idx2 + 2, idx2 + 0);
        IDL_IDX3(idx2 + 1, idx1 + 1, idx1 + 0);
        IDL_IDX3(idx1 + 0, idx2 + 0, idx2 + 1);
        idx1 = idx2;
      }
      // Add vertexes
      for (i = 0; i < points_count; i++) {
        ImVec2 pt = {points[i].x, points[i].y};
        IDL_VTX(pt, uv, col);
        IDL_VTX(temp_points[i * 2 + 0], uv, col_trans);
        IDL_VTX(temp_points[i * 2 + 1], uv, col_trans);
      }
    }
    else {
      const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;
      if (!closed) {
        temp_points[0].x = points[0].x + temp_normals[0].x * (half_inner_thickness + AA_SIZE);
        temp_points[0].y = points[0].y + temp_normals[0].y * (half_inner_thickness + AA_SIZE);
        temp_points[1].x = points[0].x + temp_normals[0].x * (half_inner_thickness);
        temp_points[1].y = points[0].y + temp_normals[0].y * (half_inner_thickness);
        temp_points[2].x = points[0].x - temp_normals[0].x * (half_inner_thickness);
        temp_points[2].y = points[0].y - temp_normals[0].y * (half_inner_thickness);
        temp_points[3].x = points[0].x - temp_normals[0].x * (half_inner_thickness + AA_SIZE);
        temp_points[3].y = points[0].y - temp_normals[0].y * (half_inner_thickness + AA_SIZE);
        temp_points[(points_count - 1) * 4 + 0].x = points[points_count - 1].x + temp_normals[points_count - 1].x * (half_inner_thickness + AA_SIZE);
        temp_points[(points_count - 1) * 4 + 0].y = points[points_count - 1].y + temp_normals[points_count - 1].y * (half_inner_thickness + AA_SIZE);
        temp_points[(points_count - 1) * 4 + 1].x = points[points_count - 1].x + temp_normals[points_count - 1].x * (half_inner_thickness);
        temp_points[(points_count - 1) * 4 + 1].y = points[points_count - 1].y + temp_normals[points_count - 1].y * (half_inner_thickness);
        temp_points[(points_count - 1) * 4 + 2].x = points[points_count - 1].x - temp_normals[points_count - 1].x * (half_inner_thickness);
        temp_points[(points_count - 1) * 4 + 2].y = points[points_count - 1].y - temp_normals[points_count - 1].y * (half_inner_thickness);
        temp_points[(points_count - 1) * 4 + 3].x = points[points_count - 1].x - temp_normals[points_count - 1].x * (half_inner_thickness + AA_SIZE);
        temp_points[(points_count - 1) * 4 + 3].y = points[points_count - 1].y - temp_normals[points_count - 1].y * (half_inner_thickness + AA_SIZE);
      }
      // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
      idx1 = _VtxCurrentIdx;
      for (i1 = 0; i1 < count; i1++) {
        const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
        unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : idx1 + 4;
        // Average normals
        ImVec2 dm;
        float dmr2;
        dm.x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
        dm.y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
        dmr2 = dm.x * dm.x + dm.y * dm.y;
        if (dmr2 > 0.000001f) {
          float scale = 1.0f / dmr2;
          if (scale > 100.0f) {
            scale = 100.0f;
          }
          dm.x *= scale;
          dm.y *= scale;
        }
        dm_out.x = dm.x * (half_inner_thickness + AA_SIZE);
        dm_out.y = dm.y * (half_inner_thickness + AA_SIZE);
        dm_in.x = dm.x * half_inner_thickness;
        dm_in.y = dm.y * half_inner_thickness;
        temp_points[i2 * 4 + 0].x = points[i2].x + dm_out.x;
        temp_points[i2 * 4 + 0].y = points[i2].y + dm_out.y;
        temp_points[i2 * 4 + 1].x = points[i2].x + dm_in.x;
        temp_points[i2 * 4 + 1].y = points[i2].y + dm_in.y;
        temp_points[i2 * 4 + 2].x = points[i2].x - dm_in.x;
        temp_points[i2 * 4 + 2].y = points[i2].y - dm_in.y;
        temp_points[i2 * 4 + 3].x = points[i2].x - dm_out.x;
        temp_points[i2 * 4 + 3].y = points[i2].y - dm_out.y;
        // Add indexes
        IDL_IDX3(idx2 + 1, idx1 + 1, idx1 + 2);
        IDL_IDX3(idx1 + 2, idx2 + 2, idx2 + 1);
        IDL_IDX3(idx2 + 1, idx1 + 1, idx1 + 0);
        IDL_IDX3(idx1 + 0, idx2 + 0, idx2 + 1);
        IDL_IDX3(idx2 + 2, idx1 + 2, idx1 + 3);
        IDL_IDX3(idx1 + 3, idx2 + 3, idx2 + 2);
        idx1 = idx2;
      }
      // Add vertexes
      for (i = 0; i < points_count; i++) {
        IDL_VTX(temp_points[i * 4 + 0], uv, col_trans);
        IDL_VTX(temp_points[i * 4 + 1], uv, col);
        IDL_VTX(temp_points[i * 4 + 2], uv, col);
        IDL_VTX(temp_points[i * 4 + 3], uv, col_trans);
      }
    }
    IDL_END();
  }
  else {
    // Non Anti-aliased Stroke
    const int idx_count = count * 6;
    const int vtx_count = count * 4; // FIXME-OPT: Not sharing edges
    IDL_BEG(idx_count, vtx_count);
    for (i1 = 0; i1 < count; i1++) {
      const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
      const FPOINT* p1 = &points[i1];
      const FPOINT* p2 = &points[i2];
      ImVec2 diff;
      float dx, dy;
      diff.x = p2->x - p1->x;
      diff.y = p2->y - p1->y;
      invlen = ImInvLength(diff, 1.0f);
      diff.x *= invlen;
      diff.y *= invlen;
      dx = diff.x * (thickness * 0.5f);
      dy = diff.y * (thickness * 0.5f);
      IDL_IDX3(0, 1, 2);
      IDL_IDX3(0, 2, 3);
      IDL_VTX(fVec2(p1->x + dy, p1->y - dx), uv, col);
      IDL_VTX(fVec2(p2->x + dy, p2->y - dx), uv, col);
      IDL_VTX(fVec2(p2->x - dy, p2->y + dx), uv, col);
      IDL_VTX(fVec2(p1->x - dy, p1->y + dx), uv, col);
    }
    IDL_END();
  }
}
void triConvexPolyFilled(ImDrawList* g, const FPOINT* points, const int points_count, COLOR col, bool anti_aliased)
{
  int i, i0, i1;
  const ImVec2 uv = GImGui->FontTexUvWhitePixel;
  //if (imuiGetIO().KeyCtrl) anti_aliased = false; // Debug
  if (points_count <= 0) {
    return ;
  }
  if (anti_aliased) {
    // Anti-aliased Fill
    const float AA_SIZE = 1.0f;
    const COLOR col_trans = col & 0x00ffffff;
    const int idx_count = (points_count - 2) * 3 + points_count * 6;
    const int vtx_count = (points_count * 2);
    enum { vtx_inner_idx = 0, vtx_outer_idx = 1 };
    ImVec2* temp_normals;
    IDL_BEG(idx_count, vtx_count);
    // Add indexes for fill
    for (i = 2; i < points_count; i++) {
      IDL_IDX3(vtx_inner_idx, vtx_inner_idx + ((i - 1) << 1), vtx_inner_idx + (i << 1));
    }
    // Compute normals
    temp_normals = (ImVec2*)alloca(points_count * sizeof(ImVec2));
    for (i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++) {
      ImVec2 p0 = {points[i0].x, points[i0].y};
      ImVec2 p1 = {points[i1].x, points[i1].y};
      ImVec2 diff;
      float invlen;
      diff.x = p1.x - p0.x;
      diff.y = p1.y - p0.y;
      invlen = ImInvLength(diff, 1.0f);
      diff.x *= invlen;
      diff.y *= invlen;
      temp_normals[i0].x = diff.y;
      temp_normals[i0].y = -diff.x;
    }
    for (i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++) {
      // Average normals
      ImVec2 n0 = temp_normals[i0];
      ImVec2 n1 = temp_normals[i1];
      ImVec2 dm, p1, p2;
      float dmr2;
      dm.x = (n0.x + n1.x) * 0.5f;
      dm.y = (n0.y + n1.y) * 0.5f;
      dmr2 = dm.x * dm.x + dm.y * dm.y;
      if (dmr2 > 0.000001f) {
        float scale = 1.0f / dmr2;
        if (scale > 100.0f) {
          scale = 100.0f;
        }
        dm.x *= scale;
        dm.y *= scale;
      }
      dm.x *= AA_SIZE * 0.5f;
      dm.y *= AA_SIZE * 0.5f;
      // Add vertices
      p1.x = points[i1].x - dm.x;
      p1.y = points[i1].y - dm.y;
      p2.x = points[i1].x + dm.x;
      p2.y = points[i1].y + dm.y;
      IDL_VTX(p1, uv, col); // Inner
      IDL_VTX(p2, uv, col_trans); // Outer
      // Add indexes for fringes
      IDL_IDX3(vtx_inner_idx + (i1 << 1), vtx_inner_idx + (i0 << 1), vtx_outer_idx + (i0 << 1));
      IDL_IDX3(vtx_outer_idx + (i0 << 1), vtx_outer_idx + (i1 << 1), vtx_inner_idx + (i1 << 1));
    }
    IDL_END();
  }
  else {
    // Non Anti-aliased Fill
    const int idx_count = (points_count - 2) * 3;
    const int vtx_count = points_count;
    IDL_BEG(idx_count, vtx_count);
    for (i = 0; i < vtx_count; i++) {
      ImVec2 pt = {points[i].x, points[i].y};
      IDL_VTX(pt, uv, col);
    }
    for (i = 2; i < points_count; i++) {
      IDL_IDX3(0, i - 1, i);
    }
    IDL_END();
  }
}
void triRenderChar(ImDrawList* g, const texfont_t* font, float size, ImVec2 pos, COLOR col, unsigned short c)
{
  const font_glyph_t* glyph;
  if (c == ' ' || c == '\t' || c == '\n' || c == '\r') { // Match behavior of RenderText(), those 4 codepoints are hard-coded.
    return;
  }
  glyph = texfont_FindGlyph(font, c);
  if (glyph) {
    ImVec2 pos_tl, pos_br;
    float scale = (size >= 0.0f) ? (size / font->size) : 1.0f;
    pos.x = (float)(int)pos.x + font->DisplayOffset.x;
    pos.y = (float)(int)pos.y + font->DisplayOffset.y;
    pos_tl = fVec2(pos.x + glyph->X0 * scale, pos.y + glyph->Y0 * scale);
    pos_br = fVec2(pos.x + glyph->X1 * scale, pos.y + glyph->Y1 * scale);
    triPrimRectUV(g, pos_tl, pos_br, fVec2(glyph->U0, glyph->V0), fVec2(glyph->U1, glyph->V1), col);
  }
}
void triRenderText(ImDrawList* g, const texfont_t* font, float size, ImVec2 pos, COLOR col, FRECT clip_rect, const char* text_begin, const char* text_end, float wrap_width, bool cpu_fine_clip)
{
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
    return;
  }
  scale = size / font->size;
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
    const int vtx_count_max = (int)(text_end - s) * 4;
    const int idx_count_max = (int)(text_end - s) * 6;
    const int idx_expected_size = g->IdxBuffer_Size + idx_count_max;
    unsigned int vtx_current_idx = 0;
    IDL_BEG(idx_count_max, vtx_count_max);
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
          if (!word_wrap_enabled && y + line_height < clip_rect.t) {
            while (s < text_end && *s != '\n') { // Fast-forward to next line
              s++;
            }
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
              IDL_IDX3(vtx_current_idx, vtx_current_idx + 1, vtx_current_idx + 2);
              IDL_IDX3(vtx_current_idx, vtx_current_idx + 2, vtx_current_idx + 3);
              IDL_VTX2(x1, y1, u1, v1, col);
              IDL_VTX2(x2, y1, u2, v1, col);
              IDL_VTX2(x2, y2, u2, v2, col);
              IDL_VTX2(x1, y2, u1, v2, col);
              vtx_current_idx += 4;
            }
          }
        }
      }
      x += char_width;
    }
    IDL_END();
  }
  // Give back unused vertices
}
void triresetClip(ImDrawList* g)
{
  g->ClipRect.x = g->ClipRect.y = -8192.0f;
  g->ClipRect.z = g->ClipRect.w = +8192.0f;
}
void triAddCmd(ImDrawList* g)
{
  int ElemCount = g->IdxBuffer_Size - g->Idx_off;
  if (ElemCount > 0) {
    drawcmd_t* cmd = (drawcmd_t*)myAllocPush(g->CmdBuffer, g->CmdBuffer_Size, 1);
    drawcmd_init(cmd);
    cmd->vtxtype = g->vtxtype;
    cmd->tex = g->tex;
    cmd->ClipRect = g->ClipRect;
    cmd->ElemCount = ElemCount;
    g->Idx_off = g->IdxBuffer_Size;
  }
  g->tex = NULL;
  //triresetClip(g);
}
void triClear(ImDrawList* g)
{
  g->CmdBuffer_Size = 0;
  g->VtxBuffer_Size = 0;
  g->IdxBuffer_Size = 0;
  g->Idx_off = 0;
}
void triFree(ImDrawList* g)
{
  myFree(g->CmdBuffer, g->CmdBuffer_Size);
  myFree(g->VtxBuffer, g->VtxBuffer_Size);
  myFree(g->IdxBuffer, g->IdxBuffer_Size);
  memset(g, 0, sizeof(*g));
}
#undef VA_ARG_EQ

