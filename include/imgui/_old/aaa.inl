// Fully unrolled with inline call to keep our debug builds decently fast.
void ImDrawList_PrimRect(ImDrawList* g, const ImVec2& a, const ImVec2& c, ImU32 col)
{
  ImVec2 b = fVec2(c.x, a.y), d = fVec2(a.x, c.y), uv = (GImGui->FontTexUvWhitePixel);
  ImDrawIdx idx = (ImDrawIdx)g->_VtxCurrentIdx;
  g->_IdxWritePtr[0] = idx;
  g->_IdxWritePtr[1] = (ImDrawIdx)(idx + 1);
  g->_IdxWritePtr[2] = (ImDrawIdx)(idx + 2);
  g->_IdxWritePtr[3] = idx;
  g->_IdxWritePtr[4] = (ImDrawIdx)(idx + 2);
  g->_IdxWritePtr[5] = (ImDrawIdx)(idx + 3);
  g->_VtxWritePtr[0].pos = a;
  g->_VtxWritePtr[0].uv = uv;
  g->_VtxWritePtr[0].col = col;
  g->_VtxWritePtr[1].pos = b;
  g->_VtxWritePtr[1].uv = uv;
  g->_VtxWritePtr[1].col = col;
  g->_VtxWritePtr[2].pos = c;
  g->_VtxWritePtr[2].uv = uv;
  g->_VtxWritePtr[2].col = col;
  g->_VtxWritePtr[3].pos = d;
  g->_VtxWritePtr[3].uv = uv;
  g->_VtxWritePtr[3].col = col;
  g->_VtxWritePtr += 4;
  g->_VtxCurrentIdx += 4;
  g->_IdxWritePtr += 6;
}
void ImDrawList_PrimRectUV(ImDrawList* g, const ImVec2& a, const ImVec2& c, const ImVec2& uv_a, const ImVec2& uv_c, ImU32 col)
{
  ImVec2 b = fVec2(c.x, a.y), d = fVec2(a.x, c.y), uv_b = fVec2(uv_c.x, uv_a.y), uv_d = fVec2(uv_a.x, uv_c.y);
  ImDrawIdx idx = (ImDrawIdx)g->_VtxCurrentIdx;
  g->_IdxWritePtr[0] = idx;
  g->_IdxWritePtr[1] = (ImDrawIdx)(idx + 1);
  g->_IdxWritePtr[2] = (ImDrawIdx)(idx + 2);
  g->_IdxWritePtr[3] = idx;
  g->_IdxWritePtr[4] = (ImDrawIdx)(idx + 2);
  g->_IdxWritePtr[5] = (ImDrawIdx)(idx + 3);
  g->_VtxWritePtr[0].pos = a;
  g->_VtxWritePtr[0].uv = uv_a;
  g->_VtxWritePtr[0].col = col;
  g->_VtxWritePtr[1].pos = b;
  g->_VtxWritePtr[1].uv = uv_b;
  g->_VtxWritePtr[1].col = col;
  g->_VtxWritePtr[2].pos = c;
  g->_VtxWritePtr[2].uv = uv_c;
  g->_VtxWritePtr[2].col = col;
  g->_VtxWritePtr[3].pos = d;
  g->_VtxWritePtr[3].uv = uv_d;
  g->_VtxWritePtr[3].col = col;
  g->_VtxWritePtr += 4;
  g->_VtxCurrentIdx += 4;
  g->_IdxWritePtr += 6;
}
void ImDrawList_PrimQuadUV(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
{
  ImDrawIdx idx = (ImDrawIdx)g->_VtxCurrentIdx;
  g->_IdxWritePtr[0] = idx;
  g->_IdxWritePtr[1] = (ImDrawIdx)(idx + 1);
  g->_IdxWritePtr[2] = (ImDrawIdx)(idx + 2);
  g->_IdxWritePtr[3] = idx;
  g->_IdxWritePtr[4] = (ImDrawIdx)(idx + 2);
  g->_IdxWritePtr[5] = (ImDrawIdx)(idx + 3);
  g->_VtxWritePtr[0].pos = a;
  g->_VtxWritePtr[0].uv = uv_a;
  g->_VtxWritePtr[0].col = col;
  g->_VtxWritePtr[1].pos = b;
  g->_VtxWritePtr[1].uv = uv_b;
  g->_VtxWritePtr[1].col = col;
  g->_VtxWritePtr[2].pos = c;
  g->_VtxWritePtr[2].uv = uv_c;
  g->_VtxWritePtr[2].col = col;
  g->_VtxWritePtr[3].pos = d;
  g->_VtxWritePtr[3].uv = uv_d;
  g->_VtxWritePtr[3].col = col;
  g->_VtxWritePtr += 4;
  g->_VtxCurrentIdx += 4;
  g->_IdxWritePtr += 6;
}
// TODO: Thickness anti-aliased lines cap are missing their AA fringe.
void ImDrawList_AddPolyline(ImDrawList* g, const ImVec2* points, const int points_count, ImU32 col, bool closed, float thickness, bool anti_aliased)
{
  int i;
  if (points_count < 2) {
    return;
  }
  const ImVec2 uv = GImGui->FontTexUvWhitePixel;
  anti_aliased &= GImGui->Style.AntiAliasedLines;
  //if (ImGui_GetIO().KeyCtrl) anti_aliased = false; // Debug
  int count = points_count;
  if (!closed) {
    count = points_count - 1;
  }
  const bool thick_line = thickness > 1.0f;
  if (anti_aliased) {
    // Anti-aliased stroke
    const float AA_SIZE = 1.0f;
    const ImU32 col_trans = col & 0x00ffffff;
    const int idx_count = thick_line ? count * 18 : count * 12;
    const int vtx_count = thick_line ? points_count * 4 : points_count * 3;
    ImDrawList_PrimReserve(g, idx_count, vtx_count);
    // Temporary buffer
    ImVec2* temp_normals = (ImVec2*)alloca(points_count * (thick_line ? 5 : 3) * sizeof(ImVec2));
    ImVec2* temp_points = temp_normals + points_count;
    for (int i1 = 0; i1 < count; i1++) {
      const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
      ImVec2 diff = points[i2] - points[i1];
      diff *= ImInvLength(diff, 1.0f);
      temp_normals[i1].x = diff.y;
      temp_normals[i1].y = -diff.x;
    }
    if (!closed) {
      temp_normals[points_count - 1] = temp_normals[points_count - 2];
    }
    if (!thick_line) {
      if (!closed) {
        temp_points[0] = points[0] + temp_normals[0] * AA_SIZE;
        temp_points[1] = points[0] - temp_normals[0] * AA_SIZE;
        temp_points[(points_count - 1) * 2 + 0] = points[points_count - 1] + temp_normals[points_count - 1] * AA_SIZE;
        temp_points[(points_count - 1) * 2 + 1] = points[points_count - 1] - temp_normals[points_count - 1] * AA_SIZE;
      }
      // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
      unsigned int idx1 = g->_VtxCurrentIdx;
      for (int i1 = 0; i1 < count; i1++) {
        const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
        unsigned int idx2 = (i1 + 1) == points_count ? g->_VtxCurrentIdx : idx1 + 3;
        // Average normals
        ImVec2 dm = (temp_normals[i1] + temp_normals[i2]) * 0.5f;
        float dmr2 = dm.x * dm.x + dm.y * dm.y;
        if (dmr2 > 0.000001f) {
          float scale = 1.0f / dmr2;
          if (scale > 100.0f) {
            scale = 100.0f;
          }
          dm *= scale;
        }
        dm *= AA_SIZE;
        temp_points[i2 * 2 + 0] = points[i2] + dm;
        temp_points[i2 * 2 + 1] = points[i2] - dm;
        // Add indexes
        g->_IdxWritePtr[0] = (ImDrawIdx)(idx2 + 0);
        g->_IdxWritePtr[1] = (ImDrawIdx)(idx1 + 0);
        g->_IdxWritePtr[2] = (ImDrawIdx)(idx1 + 2);
        g->_IdxWritePtr[3] = (ImDrawIdx)(idx1 + 2);
        g->_IdxWritePtr[4] = (ImDrawIdx)(idx2 + 2);
        g->_IdxWritePtr[5] = (ImDrawIdx)(idx2 + 0);
        g->_IdxWritePtr[6] = (ImDrawIdx)(idx2 + 1);
        g->_IdxWritePtr[7] = (ImDrawIdx)(idx1 + 1);
        g->_IdxWritePtr[8] = (ImDrawIdx)(idx1 + 0);
        g->_IdxWritePtr[9] = (ImDrawIdx)(idx1 + 0);
        g->_IdxWritePtr[10] = (ImDrawIdx)(idx2 + 0);
        g->_IdxWritePtr[11] = (ImDrawIdx)(idx2 + 1);
        g->_IdxWritePtr += 12;
        idx1 = idx2;
      }
      // Add vertexes
      for (i = 0; i < points_count; i++) {
        g->_VtxWritePtr[0].pos = points[i];
        g->_VtxWritePtr[0].uv = uv;
        g->_VtxWritePtr[0].col = col;
        g->_VtxWritePtr[1].pos = temp_points[i * 2 + 0];
        g->_VtxWritePtr[1].uv = uv;
        g->_VtxWritePtr[1].col = col_trans;
        g->_VtxWritePtr[2].pos = temp_points[i * 2 + 1];
        g->_VtxWritePtr[2].uv = uv;
        g->_VtxWritePtr[2].col = col_trans;
        g->_VtxWritePtr += 3;
      }
    }
    else {
      const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;
      if (!closed) {
        temp_points[0] = points[0] + temp_normals[0] * (half_inner_thickness + AA_SIZE);
        temp_points[1] = points[0] + temp_normals[0] * (half_inner_thickness);
        temp_points[2] = points[0] - temp_normals[0] * (half_inner_thickness);
        temp_points[3] = points[0] - temp_normals[0] * (half_inner_thickness + AA_SIZE);
        temp_points[(points_count - 1) * 4 + 0] = points[points_count - 1] + temp_normals[points_count - 1] * (half_inner_thickness + AA_SIZE);
        temp_points[(points_count - 1) * 4 + 1] = points[points_count - 1] + temp_normals[points_count - 1] * (half_inner_thickness);
        temp_points[(points_count - 1) * 4 + 2] = points[points_count - 1] - temp_normals[points_count - 1] * (half_inner_thickness);
        temp_points[(points_count - 1) * 4 + 3] = points[points_count - 1] - temp_normals[points_count - 1] * (half_inner_thickness + AA_SIZE);
      }
      // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
      unsigned int idx1 = g->_VtxCurrentIdx;
      for (int i1 = 0; i1 < count; i1++) {
        const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
        unsigned int idx2 = (i1 + 1) == points_count ? g->_VtxCurrentIdx : idx1 + 4;
        // Average normals
        ImVec2 dm = (temp_normals[i1] + temp_normals[i2]) * 0.5f;
        float dmr2 = dm.x * dm.x + dm.y * dm.y;
        if (dmr2 > 0.000001f) {
          float scale = 1.0f / dmr2;
          if (scale > 100.0f) {
            scale = 100.0f;
          }
          dm *= scale;
        }
        ImVec2 dm_out = dm * (half_inner_thickness + AA_SIZE);
        ImVec2 dm_in = dm * half_inner_thickness;
        temp_points[i2 * 4 + 0] = points[i2] + dm_out;
        temp_points[i2 * 4 + 1] = points[i2] + dm_in;
        temp_points[i2 * 4 + 2] = points[i2] - dm_in;
        temp_points[i2 * 4 + 3] = points[i2] - dm_out;
        // Add indexes
        g->_IdxWritePtr[0] = (ImDrawIdx)(idx2 + 1);
        g->_IdxWritePtr[1] = (ImDrawIdx)(idx1 + 1);
        g->_IdxWritePtr[2] = (ImDrawIdx)(idx1 + 2);
        g->_IdxWritePtr[3] = (ImDrawIdx)(idx1 + 2);
        g->_IdxWritePtr[4] = (ImDrawIdx)(idx2 + 2);
        g->_IdxWritePtr[5] = (ImDrawIdx)(idx2 + 1);
        g->_IdxWritePtr[6] = (ImDrawIdx)(idx2 + 1);
        g->_IdxWritePtr[7] = (ImDrawIdx)(idx1 + 1);
        g->_IdxWritePtr[8] = (ImDrawIdx)(idx1 + 0);
        g->_IdxWritePtr[9] = (ImDrawIdx)(idx1 + 0);
        g->_IdxWritePtr[10] = (ImDrawIdx)(idx2 + 0);
        g->_IdxWritePtr[11] = (ImDrawIdx)(idx2 + 1);
        g->_IdxWritePtr[12] = (ImDrawIdx)(idx2 + 2);
        g->_IdxWritePtr[13] = (ImDrawIdx)(idx1 + 2);
        g->_IdxWritePtr[14] = (ImDrawIdx)(idx1 + 3);
        g->_IdxWritePtr[15] = (ImDrawIdx)(idx1 + 3);
        g->_IdxWritePtr[16] = (ImDrawIdx)(idx2 + 3);
        g->_IdxWritePtr[17] = (ImDrawIdx)(idx2 + 2);
        g->_IdxWritePtr += 18;
        idx1 = idx2;
      }
      // Add vertexes
      for (i = 0; i < points_count; i++) {
        g->_VtxWritePtr[0].pos = temp_points[i * 4 + 0];
        g->_VtxWritePtr[0].uv = uv;
        g->_VtxWritePtr[0].col = col_trans;
        g->_VtxWritePtr[1].pos = temp_points[i * 4 + 1];
        g->_VtxWritePtr[1].uv = uv;
        g->_VtxWritePtr[1].col = col;
        g->_VtxWritePtr[2].pos = temp_points[i * 4 + 2];
        g->_VtxWritePtr[2].uv = uv;
        g->_VtxWritePtr[2].col = col;
        g->_VtxWritePtr[3].pos = temp_points[i * 4 + 3];
        g->_VtxWritePtr[3].uv = uv;
        g->_VtxWritePtr[3].col = col_trans;
        g->_VtxWritePtr += 4;
      }
    }
    g->_VtxCurrentIdx += (ImDrawIdx)vtx_count;
  }
  else {
    // Non Anti-aliased Stroke
    const int idx_count = count * 6;
    const int vtx_count = count * 4; // FIXME-OPT: Not sharing edges
    ImDrawList_PrimReserve(g, idx_count, vtx_count);
    for (int i1 = 0; i1 < count; i1++) {
      const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
      const ImVec2& p1 = points[i1];
      const ImVec2& p2 = points[i2];
      ImVec2 diff = p2 - p1;
      diff *= ImInvLength(diff, 1.0f);
      const float dx = diff.x * (thickness * 0.5f);
      const float dy = diff.y * (thickness * 0.5f);
      g->_VtxWritePtr[0].pos.x = p1.x + dy;
      g->_VtxWritePtr[0].pos.y = p1.y - dx;
      g->_VtxWritePtr[0].uv = uv;
      g->_VtxWritePtr[0].col = col;
      g->_VtxWritePtr[1].pos.x = p2.x + dy;
      g->_VtxWritePtr[1].pos.y = p2.y - dx;
      g->_VtxWritePtr[1].uv = uv;
      g->_VtxWritePtr[1].col = col;
      g->_VtxWritePtr[2].pos.x = p2.x - dy;
      g->_VtxWritePtr[2].pos.y = p2.y + dx;
      g->_VtxWritePtr[2].uv = uv;
      g->_VtxWritePtr[2].col = col;
      g->_VtxWritePtr[3].pos.x = p1.x - dy;
      g->_VtxWritePtr[3].pos.y = p1.y + dx;
      g->_VtxWritePtr[3].uv = uv;
      g->_VtxWritePtr[3].col = col;
      g->_VtxWritePtr += 4;
      g->_IdxWritePtr[0] = (ImDrawIdx)(g->_VtxCurrentIdx);
      g->_IdxWritePtr[1] = (ImDrawIdx)(g->_VtxCurrentIdx + 1);
      g->_IdxWritePtr[2] = (ImDrawIdx)(g->_VtxCurrentIdx + 2);
      g->_IdxWritePtr[3] = (ImDrawIdx)(g->_VtxCurrentIdx);
      g->_IdxWritePtr[4] = (ImDrawIdx)(g->_VtxCurrentIdx + 2);
      g->_IdxWritePtr[5] = (ImDrawIdx)(g->_VtxCurrentIdx + 3);
      g->_IdxWritePtr += 6;
      g->_VtxCurrentIdx += 4;
    }
  }
}
void ImDrawList_AddConvexPolyFilled(ImDrawList* g, const ImVec2* points, const int points_count, ImU32 col, bool anti_aliased)
{
  int i, i0, i1;
  const ImVec2 uv = GImGui->FontTexUvWhitePixel;
  anti_aliased &= GImGui->Style.AntiAliasedShapes;
  //if (ImGui_GetIO().KeyCtrl) anti_aliased = false; // Debug
  if (anti_aliased) {
    // Anti-aliased Fill
    const float AA_SIZE = 1.0f;
    const ImU32 col_trans = col & 0x00ffffff;
    const int idx_count = (points_count - 2) * 3 + points_count * 6;
    const int vtx_count = (points_count * 2);
    ImDrawList_PrimReserve(g, idx_count, vtx_count);
    // Add indexes for fill
    unsigned int vtx_inner_idx = g->_VtxCurrentIdx;
    unsigned int vtx_outer_idx = g->_VtxCurrentIdx + 1;
    for (i = 2; i < points_count; i++) {
      g->_IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx);
      g->_IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx + ((i - 1) << 1));
      g->_IdxWritePtr[2] = (ImDrawIdx)(vtx_inner_idx + (i << 1));
      g->_IdxWritePtr += 3;
    }
    // Compute normals
    ImVec2* temp_normals = (ImVec2*)alloca(points_count * sizeof(ImVec2));
    for (i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++) {
      const ImVec2& p0 = points[i0];
      const ImVec2& p1 = points[i1];
      ImVec2 diff = p1 - p0;
      diff *= ImInvLength(diff, 1.0f);
      temp_normals[i0].x = diff.y;
      temp_normals[i0].y = -diff.x;
    }
    for (i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++) {
      // Average normals
      const ImVec2& n0 = temp_normals[i0];
      const ImVec2& n1 = temp_normals[i1];
      ImVec2 dm = (n0 + n1) * 0.5f;
      float dmr2 = dm.x * dm.x + dm.y * dm.y;
      if (dmr2 > 0.000001f) {
        float scale = 1.0f / dmr2;
        if (scale > 100.0f) {
          scale = 100.0f;
        }
        dm *= scale;
      }
      dm *= AA_SIZE * 0.5f;
      // Add vertices
      g->_VtxWritePtr[0].pos = (points[i1] - dm);
      g->_VtxWritePtr[0].uv = uv;
      g->_VtxWritePtr[0].col = col; // Inner
      g->_VtxWritePtr[1].pos = (points[i1] + dm);
      g->_VtxWritePtr[1].uv = uv;
      g->_VtxWritePtr[1].col = col_trans; // Outer
      g->_VtxWritePtr += 2;
      // Add indexes for fringes
      g->_IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx + (i1 << 1));
      g->_IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx + (i0 << 1));
      g->_IdxWritePtr[2] = (ImDrawIdx)(vtx_outer_idx + (i0 << 1));
      g->_IdxWritePtr[3] = (ImDrawIdx)(vtx_outer_idx + (i0 << 1));
      g->_IdxWritePtr[4] = (ImDrawIdx)(vtx_outer_idx + (i1 << 1));
      g->_IdxWritePtr[5] = (ImDrawIdx)(vtx_inner_idx + (i1 << 1));
      g->_IdxWritePtr += 6;
    }
    g->_VtxCurrentIdx += (ImDrawIdx)vtx_count;
  }
  else {
    // Non Anti-aliased Fill
    const int idx_count = (points_count - 2) * 3;
    const int vtx_count = points_count;
    ImDrawList_PrimReserve(g, idx_count, vtx_count);
    for (i = 0; i < vtx_count; i++) {
      g->_VtxWritePtr[0].pos = points[i];
      g->_VtxWritePtr[0].uv = uv;
      g->_VtxWritePtr[0].col = col;
      g->_VtxWritePtr++;
    }
    for (i = 2; i < points_count; i++) {
      g->_IdxWritePtr[0] = (ImDrawIdx)(g->_VtxCurrentIdx);
      g->_IdxWritePtr[1] = (ImDrawIdx)(g->_VtxCurrentIdx + i - 1);
      g->_IdxWritePtr[2] = (ImDrawIdx)(g->_VtxCurrentIdx + i);
      g->_IdxWritePtr += 3;
    }
    g->_VtxCurrentIdx += (ImDrawIdx)vtx_count;
  }
}
void ImDrawList_PathArcToFast(ImDrawList* g, const ImVec2& centre, float radius, int amin, int amax)
{
  int i;
  static ImVec2 circle_vtx[12];
  static bool circle_vtx_builds = false;
  const int circle_vtx_count = IM_ARRAYSIZE(circle_vtx);
  if (!circle_vtx_builds) {
    for (i = 0; i < circle_vtx_count; i++) {
      const float a = ((float)i / (float)circle_vtx_count) * 2 * IM_PI;
      circle_vtx[i].x = cosf(a);
      circle_vtx[i].y = sinf(a);
    }
    circle_vtx_builds = true;
  }
  if (amin > amax) {
    return;
  }
  if (radius == 0.0f) {
    g->_Path.push_back(centre);
  }
  else {
    g->_Path.reserve(g->_Path.Size + (amax - amin + 1));
    for (int a = amin; a <= amax; a++) {
      const ImVec2& c = circle_vtx[a % circle_vtx_count];
      g->_Path.push_back(fVec2(centre.x + c.x * radius, centre.y + c.y * radius));
    }
  }
}
void ImDrawList_PathArcTo(ImDrawList* g, const ImVec2& centre, float radius, float amin, float amax, int num_segments)
{
  int i;
  if (radius == 0.0f) {
    g->_Path.push_back(centre);
  }
  g->_Path.reserve(g->_Path.Size + (num_segments + 1));
  for (i = 0; i <= num_segments; i++) {
    const float a = amin + ((float)i / (float)num_segments) * (amax - amin);
    g->_Path.push_back(fVec2(centre.x + cosf(a) * radius, centre.y + sinf(a) * radius));
  }
}
static void PathBezierToCasteljau(ImVector<ImVec2>* path, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tess_tol, int level)
{
  float dx = x4 - x1;
  float dy = y4 - y1;
  float d2 = ((x2 - x4) * dy - (y2 - y4) * dx);
  float d3 = ((x3 - x4) * dy - (y3 - y4) * dx);
  d2 = (d2 >= 0) ? d2 : -d2;
  d3 = (d3 >= 0) ? d3 : -d3;
  if ((d2 + d3) * (d2 + d3) < tess_tol * (dx * dx + dy * dy)) {
    path->push_back(fVec2(x4, y4));
  }
  else if (level < 10) {
    float x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
    float x23 = (x2 + x3) * 0.5f, y23 = (y2 + y3) * 0.5f;
    float x34 = (x3 + x4) * 0.5f, y34 = (y3 + y4) * 0.5f;
    float x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
    float x234 = (x23 + x34) * 0.5f, y234 = (y23 + y34) * 0.5f;
    float x1234 = (x123 + x234) * 0.5f, y1234 = (y123 + y234) * 0.5f;
    PathBezierToCasteljau(path, x1, y1, x12, y12, x123, y123, x1234, y1234, tess_tol, level + 1);
    PathBezierToCasteljau(path, x1234, y1234, x234, y234, x34, y34, x4, y4, tess_tol, level + 1);
  }
}
void ImDrawList_PathBezierCurveTo(ImDrawList* g, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, int num_segments)
{
  ImVec2 p1 = g->_Path.back();
  if (num_segments == 0) {
    // Auto-tessellated
    PathBezierToCasteljau(&g->_Path, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, GImGui->Style.CurveTessellationTol, 0);
  }
  else {
    float t_step = 1.0f / (float)num_segments;
    for (int i_step = 1; i_step <= num_segments; i_step++) {
      float t = t_step * i_step;
      float u = 1.0f - t;
      float w1 = u * u * u;
      float w2 = 3 * u * u * t;
      float w3 = 3 * u * t * t;
      float w4 = t * t * t;
      g->_Path.push_back(fVec2(w1 * p1.x + w2 * p2.x + w3 * p3.x + w4 * p4.x, w1 * p1.y + w2 * p2.y + w3 * p3.y + w4 * p4.y));
    }
  }
}
void ImDrawList_PathRect(ImDrawList* g, const ImVec2& a, const ImVec2& b, float rounding, int rounding_corners)
{
  float r = rounding;
  float rx = fabsf(b.x - a.x) * (((rounding_corners & (1 | 2)) == (1 | 2)) || ((rounding_corners & (4 | 8)) == (4 | 8)) ? 0.5f : 1.0f) - 1.0f;
  float ry = fabsf(b.y - a.y) * (((rounding_corners & (1 | 8)) == (1 | 8)) || ((rounding_corners & (2 | 4)) == (2 | 4)) ? 0.5f : 1.0f) - 1.0f;
  r = ImMin(r, rx);
  r = ImMin(r, ry);
  if (r <= 0.0f || rounding_corners == 0) {
    ImDrawList_PathLineTo(g, a);
    ImDrawList_PathLineTo(g, fVec2(b.x, a.y));
    ImDrawList_PathLineTo(g, b);
    ImDrawList_PathLineTo(g, fVec2(a.x, b.y));
  }
  else {
    const float r0 = (rounding_corners & 1) ? r : 0.0f;
    const float r1 = (rounding_corners & 2) ? r : 0.0f;
    const float r2 = (rounding_corners & 4) ? r : 0.0f;
    const float r3 = (rounding_corners & 8) ? r : 0.0f;
    ImDrawList_PathArcToFast(g, fVec2(a.x + r0, a.y + r0), r0, 6, 9);
    ImDrawList_PathArcToFast(g, fVec2(b.x - r1, a.y + r1), r1, 9, 12);
    ImDrawList_PathArcToFast(g, fVec2(b.x - r2, b.y - r2), r2, 0, 3);
    ImDrawList_PathArcToFast(g, fVec2(a.x + r3, b.y - r3), r3, 3, 6);
  }
}
void ImDrawList_AddLine(ImDrawList* g, const ImVec2& a, const ImVec2& b, ImU32 col, float thickness)
{
  if ((col >> 24) == 0) {
    return;
  }
  ImDrawList_PathLineTo(g, a + fVec2(0.5f, 0.5f));
  ImDrawList_PathLineTo(g, b + fVec2(0.5f, 0.5f));
  ImDrawList_PathStroke(g, col, false, thickness);
}
// a: upper-left, b: lower-right. we don't render 1 px sized rectangles properly.
void ImDrawList_AddRect(ImDrawList* g, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners, float thickness)
{
  if ((col >> 24) == 0) {
    return;
  }
  ImDrawList_PathRect(g, a + fVec2(0.5f, 0.5f), b - fVec2(0.5f, 0.5f), rounding, rounding_corners);
  ImDrawList_PathStroke(g, col, true, thickness);
}
void ImDrawList_AddRectFilled(ImDrawList* g, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners)
{
  if ((col >> 24) == 0) {
    return;
  }
  if (rounding > 0.0f) {
    ImDrawList_PathRect(g, a, b, rounding, rounding_corners);
    ImDrawList_PathFill(g, col);
  }
  else {
    ImDrawList_PrimReserve(g, 6, 4);
    ImDrawList_PrimRect(g, a, b, col);
  }
}
void ImDrawList_AddRectFilledMultiColor(ImDrawList* g, const ImVec2& a, const ImVec2& c, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left)
{
  if (((col_upr_left | col_upr_right | col_bot_right | col_bot_left) >> 24) == 0) {
    return;
  }
  const ImVec2 uv = GImGui->FontTexUvWhitePixel;
  ImDrawList_PrimReserve(g, 6, 4);
  ImDrawList_PrimWriteIdx(g, (ImDrawIdx)(g->_VtxCurrentIdx));
  ImDrawList_PrimWriteIdx(g, (ImDrawIdx)(g->_VtxCurrentIdx + 1));
  ImDrawList_PrimWriteIdx(g, (ImDrawIdx)(g->_VtxCurrentIdx + 2));
  ImDrawList_PrimWriteIdx(g, (ImDrawIdx)(g->_VtxCurrentIdx));
  ImDrawList_PrimWriteIdx(g, (ImDrawIdx)(g->_VtxCurrentIdx + 2));
  ImDrawList_PrimWriteIdx(g, (ImDrawIdx)(g->_VtxCurrentIdx + 3));
  ImDrawList_PrimWriteVtx(g, a, uv, col_upr_left);
  ImDrawList_PrimWriteVtx(g, fVec2(c.x, a.y), uv, col_upr_right);
  ImDrawList_PrimWriteVtx(g, c, uv, col_bot_right);
  ImDrawList_PrimWriteVtx(g, fVec2(a.x, c.y), uv, col_bot_left);
}
void ImDrawList_AddQuad(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness)
{
  if ((col >> 24) == 0) {
    return;
  }
  ImDrawList_PathLineTo(g, a);
  ImDrawList_PathLineTo(g, b);
  ImDrawList_PathLineTo(g, c);
  ImDrawList_PathLineTo(g, d);
  ImDrawList_PathStroke(g, col, true, thickness);
}
void ImDrawList_AddQuadFilled(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col)
{
  if ((col >> 24) == 0) {
    return;
  }
  ImDrawList_PathLineTo(g, a);
  ImDrawList_PathLineTo(g, b);
  ImDrawList_PathLineTo(g, c);
  ImDrawList_PathLineTo(g, d);
  ImDrawList_PathFill(g, col);
}
void ImDrawList_AddTriangle(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness)
{
  if ((col >> 24) == 0) {
    return;
  }
  ImDrawList_PathLineTo(g, a);
  ImDrawList_PathLineTo(g, b);
  ImDrawList_PathLineTo(g, c);
  ImDrawList_PathStroke(g, col, true, thickness);
}
void ImDrawList_AddTriangleFilled(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col)
{
  if ((col >> 24) == 0) {
    return;
  }
  ImDrawList_PathLineTo(g, a);
  ImDrawList_PathLineTo(g, b);
  ImDrawList_PathLineTo(g, c);
  ImDrawList_PathFill(g, col);
}
void ImDrawList_AddCircle(ImDrawList* g, const ImVec2& centre, float radius, ImU32 col, int num_segments, float thickness)
{
  if ((col >> 24) == 0) {
    return;
  }
  const float a_max = IM_PI * 2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
  ImDrawList_PathArcTo(g, centre, radius - 0.5f, 0.0f, a_max, num_segments);
  ImDrawList_PathStroke(g, col, true, thickness);
}
void ImDrawList_AddCircleFilled(ImDrawList* g, const ImVec2& centre, float radius, ImU32 col, int num_segments)
{
  if ((col >> 24) == 0) {
    return;
  }
  const float a_max = IM_PI * 2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
  ImDrawList_PathArcTo(g, centre, radius, 0.0f, a_max, num_segments);
  ImDrawList_PathFill(g, col);
}
void ImDrawList_AddBezierCurve(ImDrawList* g, const ImVec2& pos0, const ImVec2& cp0, const ImVec2& cp1, const ImVec2& pos1, ImU32 col, float thickness, int num_segments)
{
  if ((col >> 24) == 0) {
    return;
  }
  ImDrawList_PathLineTo(g, pos0);
  ImDrawList_PathBezierCurveTo(g, cp0, cp1, pos1, num_segments);
  ImDrawList_PathStroke(g, col, false, thickness);
}
void ImDrawList_AddText(ImDrawList* g, const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
{
  if ((col >> 24) == 0) {
    return;
  }
  if (text_end == NULL) {
    text_end = text_begin + strlen(text_begin);
  }
  if (text_begin == text_end) {
    return;
  }
  // Note: This is one of the few instance of breaking the encapsulation of ImDrawList, as we pull g from ImGui state, but it is just SO useful.
  // Might just move Font/FontSize to ImDrawList?
  if (font == NULL) {
    font = GImGui->Font;
  }
  if (font_size == 0.0f) {
    font_size = GImGui->FontSize;
  }
  IM_ASSERT(font->ContainerAtlas->TexID == g->_TextureIdStack.back()); // Use high-level ImGui_PushFont() or low-level ImDrawList_PushTextureId() to change font.
  ImVec4 clip_rect = g->_ClipRectStack.back();
  if (cpu_fine_clip_rect) {
    clip_rect.x = ImMax(clip_rect.x, cpu_fine_clip_rect->x);
    clip_rect.y = ImMax(clip_rect.y, cpu_fine_clip_rect->y);
    clip_rect.z = ImMin(clip_rect.z, cpu_fine_clip_rect->z);
    clip_rect.w = ImMin(clip_rect.w, cpu_fine_clip_rect->w);
  }
  ImFont_RenderText(font, g, font_size, pos, col, clip_rect, text_begin, text_end, wrap_width, cpu_fine_clip_rect != NULL);
}
void ImDrawList_AddText(ImDrawList* g, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end)
{
  ImDrawList_AddText(g, GImGui->Font, GImGui->FontSize, pos, col, text_begin, text_end);
}
void ImDrawList_AddImage(ImDrawList* g, ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv0, const ImVec2& uv1, ImU32 col)
{
  if ((col >> 24) == 0) {
    return;
  }
  // FIXME-OPT: This is wasting draw calls.
  const bool push_texture_id = g->_TextureIdStack.empty() || user_texture_id != g->_TextureIdStack.back();
  if (push_texture_id) {
    ImDrawList_PushTextureID(g, user_texture_id);
  }
  ImDrawList_PrimReserve(g, 6, 4);
  ImDrawList_PrimRectUV(g, a, b, uv0, uv1, col);
  if (push_texture_id) {
    ImDrawList_PopTextureID(g);
  }
}

