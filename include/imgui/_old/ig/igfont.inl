
//-----------------------------------------------------------------------------
// ImDrawList
//-----------------------------------------------------------------------------
static const ImVec4 GNullClipRect = {-8192.0f, -8192.0f, +8192.0f, +8192.0f}; // Large values that are easy to encode in a few bits+shift
void ImDrawList_Clear(ImDrawList* g)
{
  myImGui_Free(g->CmdBuffer, g->CmdBuffer_Size);
  myImGui_Free(g->IdxBuffer, g->IdxBuffer_Size);
  myImGui_Free(g->VtxBuffer, g->VtxBuffer_Size);

  g->_VtxCurrentIdx = 0;
  g->_VtxWritePtr = NULL;
  g->_IdxWritePtr = NULL;
  myImGui_Free(g->_ClipRectStack, g->_ClipRectStack_Size);
  myImGui_Free(g->_TextureIdStack, g->_TextureIdStack_Size);
  myImGui_Free(g->_Path, g->_Path_Size);
  g->_ChannelsCurrent = 0;
  g->_ChannelsCount = 1;
  // NB: Do not clear channels so our allocations are re-used after the first frame.
}
void ImDrawList_ClearFreeMemory(ImDrawList* g)
{
  int i;
  ImDrawList_Clear(g);

  for (i = 0; i < g->_Channels_Size; i++) {
    if (i == 0) {
      memset(&g->_Channels[0], 0, sizeof(g->_Channels[0]));  // channel 0 is a copy of g->CmdBuffer/IdxBuffer, don't destruct again
    }

    myImGui_Free(g->_Channels[i].CmdBuffer, g->_Channels[i].CmdBuffer_Size);
    myImGui_Free(g->_Channels[i].IdxBuffer, g->_Channels[i].IdxBuffer_Size);
  }
  
  myImGui_Free(g->_Channels, g->_Channels_Size);
}
IM_INLINE void ImDrawCmd_init(ImDrawCmd* cmd) {
  cmd->ElemCount = 0;
  cmd->ClipRect.x = cmd->ClipRect.y = -8192.0f;
  cmd->ClipRect.z = cmd->ClipRect.w = +8192.0f;
  cmd->TextureId = NULL;
  cmd->UserCallback = NULL;
  cmd->UserCallbackData = NULL;
}
// Use macros because C++ is a terrible language, we want guaranteed inline, no code in header, and no overhead in Debug mode
#define GetCurrentClipRect(g) (g->_ClipRectStack_Size ? g->_ClipRectStack[g->_ClipRectStack_Size-1] : GNullClipRect)
#define GetCurrentTextureId(g) (g->_TextureIdStack_Size ? g->_TextureIdStack[g->_TextureIdStack_Size-1] : NULL)
ImDrawCmd* ImDrawList_AddDrawCmd(ImDrawList* g)
{
  ImDrawCmd* draw_cmd = myImGui_AllocAdd(g->CmdBuffer, g->CmdBuffer_Size, 1);
  ImDrawCmd_init(draw_cmd);
  draw_cmd->ClipRect = GetCurrentClipRect(g);
  draw_cmd->TextureId = GetCurrentTextureId(g);
  IM_ASSERT(draw_cmd->ClipRect.x <= draw_cmd->ClipRect.z && draw_cmd->ClipRect.y <= draw_cmd->ClipRect.w);
  return draw_cmd;
}
void ImDrawList_AddCallback(ImDrawList* g, ImDrawCallback callback, void* callback_data)
{
  ImDrawCmd* current_cmd = g->CmdBuffer_Size ? (g->CmdBuffer + g->CmdBuffer_Size-1) : NULL;

  if (!current_cmd || current_cmd->ElemCount != 0 || current_cmd->UserCallback != NULL) {
    current_cmd = ImDrawList_AddDrawCmd(g);
  }

  current_cmd->UserCallback = callback;
  current_cmd->UserCallbackData = callback_data;
  ImDrawList_AddDrawCmd(g); // Force a new command after us (see comment below)
}
// Our scheme may appears a bit unusual, basically we want the most-common calls AddLine AddRect etc. to not have to perform any check so we always have a command ready in the stack.
// The cost of figuring out if a new command has to be added or if we can merge is paid in those Update** functions only.
void ImDrawList_UpdateClipRect(ImDrawList* g)
{
  // If current command is used with different settings we need to add a new command
  const ImVec4 curr_clip_rect = GetCurrentClipRect(g);
  ImDrawCmd* curr_cmd = g->CmdBuffer_Size > 0 ? (g->CmdBuffer + g->CmdBuffer_Size - 1) : NULL;
  ImDrawCmd* prev_cmd;

  if (!curr_cmd || (curr_cmd->ElemCount != 0 && memcmp(&curr_cmd->ClipRect, &curr_clip_rect, sizeof(ImVec4)) != 0) || curr_cmd->UserCallback != NULL) {
    ImDrawList_AddDrawCmd(g);
    return;
  }

  // Try to merge with previous command if it matches, else use current command
  prev_cmd = g->CmdBuffer_Size > 1 ? curr_cmd - 1 : NULL;

  if (curr_cmd->ElemCount == 0 && prev_cmd && memcmp(&prev_cmd->ClipRect, &curr_clip_rect, sizeof(ImVec4)) == 0 && prev_cmd->TextureId == GetCurrentTextureId(g) && prev_cmd->UserCallback == NULL) {
    g->CmdBuffer_Size--;
  }
  else {
    curr_cmd->ClipRect = curr_clip_rect;
  }
}
void ImDrawList_UpdateTextureID(ImDrawList* g)
{
  // If current command is used with different settings we need to add a new command
  const ImTextureID curr_texture_id = GetCurrentTextureId(g);
  ImDrawCmd* curr_cmd = g->CmdBuffer_Size ? (g->CmdBuffer+g->CmdBuffer_Size-1) : NULL;
  ImDrawCmd* prev_cmd;
  ImVec4 tmp;

  if (!curr_cmd || (curr_cmd->ElemCount != 0 && curr_cmd->TextureId != curr_texture_id) || curr_cmd->UserCallback != NULL) {
    ImDrawList_AddDrawCmd(g);
    return;
  }

  // Try to merge with previous command if it matches, else use current command
  prev_cmd = g->CmdBuffer_Size > 1 ? curr_cmd - 1 : NULL;
  tmp = GetCurrentClipRect(g);
  if (prev_cmd && prev_cmd->TextureId == curr_texture_id && memcmp(&prev_cmd->ClipRect, &tmp, sizeof(ImVec4)) == 0 && prev_cmd->UserCallback == NULL) {
    g->CmdBuffer_Size--;
  }
  else {
    curr_cmd->TextureId = curr_texture_id;
  }
}
#undef GetCurrentClipRect
#undef GetCurrentTextureId
// Render-level scissoring. This is passed down to your render function but not used for CPU-side coarse clipping. Prefer using higher-level ImGui_PushClipRect(g) to affect logic (hit-testing and widget culling)
void ImDrawList_PushClipRect(ImDrawList* g, ImVec2 cr_min, ImVec2 cr_max, BOOL intersect_with_current_clip_rect)
{
  ImVec4 cr = fVec4(cr_min.x, cr_min.y, cr_max.x, cr_max.y);

  if (intersect_with_current_clip_rect && g->_ClipRectStack_Size) {
    ImVec4 current = g->_ClipRectStack[g->_ClipRectStack_Size - 1];

    if (cr.x < current.x) {
      cr.x = current.x;
    }

    if (cr.y < current.y) {
      cr.y = current.y;
    }

    if (cr.z > current.z) {
      cr.z = current.z;
    }

    if (cr.w > current.w) {
      cr.w = current.w;
    }
  }

  cr.z = ImMax(cr.x, cr.z);
  cr.w = ImMax(cr.y, cr.w);
  g->_ClipRectStack[g->_ClipRectStack_Size++] = cr;
  ImDrawList_UpdateClipRect(g);
}
void ImDrawList_PushClipRectFullScreen(ImDrawList* g)
{
  ImDrawList_PushClipRect(g, fVec2(GNullClipRect.x, GNullClipRect.y), fVec2(GNullClipRect.z, GNullClipRect.w), 0);
  //PushClipRect(GetVisibleRect(g)); // FIXME-OPT: This would be more correct but we're not supposed to access ImGuiContext from here?
}
void ImDrawList_PopClipRect(ImDrawList* g)
{
  IM_ASSERT(g->_ClipRectStack_Size > 0);
  g->_ClipRectStack_Size--;
  ImDrawList_UpdateClipRect(g);
}
void ImDrawList_PushTextureID(ImDrawList* g, ImTextureID texture_id)
{
  g->_TextureIdStack[g->_TextureIdStack_Size++] = texture_id;
  ImDrawList_UpdateTextureID(g);
}
void ImDrawList_PopTextureID(ImDrawList* g)
{
  IM_ASSERT(g->_TextureIdStack_Size > 0);
  g->_TextureIdStack_Size--;
  ImDrawList_UpdateTextureID(g);
}
void ImDrawList_ChannelsSplit(ImDrawList* g, int channels_count)
{
  int i, old_channels_count, n;
  IM_ASSERT(g->_ChannelsCurrent == 0 && g->_ChannelsCount == 1);
  old_channels_count = g->_Channels_Size;

  if (old_channels_count < channels_count) {
    n = channels_count - old_channels_count;
    myImGui_AllocAdd(g->_Channels, g->_Channels_Size, n);
  }

  g->_ChannelsCount = channels_count;
  // g->_Channels[] (24 bytes each) hold storage that we'll swap with this->_CmdBuffer/_IdxBuffer
  // The content of g->_Channels[0] at this point doesn't matter. We clear it to make state tidy in a debugger but we don't strictly need to.
  // When we switch to the next channel, we'll copy _CmdBuffer/_IdxBuffer into g->_Channels[0] and then g->_Channels[1] into _CmdBuffer/_IdxBuffer
  memset(&g->_Channels[0], 0, sizeof(ImDrawChannel));

  for (i = 1; i < channels_count; i++) {
    if (i >= old_channels_count) {
      memset(g->_Channels+i, 0, sizeof(*g->_Channels));
    }
    else {
      myImGui_Free(g->_Channels[i].CmdBuffer, g->_Channels[i].CmdBuffer_Size);
      myImGui_Free(g->_Channels[i].IdxBuffer, g->_Channels[i].IdxBuffer_Size);
    }

    if (g->_Channels[i].CmdBuffer_Size == 0) {
      ImDrawCmd* draw_cmd = myImGui_AllocAdd(g->_Channels[i].CmdBuffer, g->_Channels[i].CmdBuffer_Size, 1);
      ImDrawCmd_init(draw_cmd);
      draw_cmd->ClipRect = g->_ClipRectStack[g->_ClipRectStack_Size-1];
      draw_cmd->TextureId = g->_TextureIdStack[g->_TextureIdStack_Size-1];
    }
  }
}
void ImDrawList_ChannelsSetCurrent(ImDrawList* g, int idx)
{
  IM_ASSERT(idx >= 0 && idx < g->_ChannelsCount);
  IM_ASSERT(g->_ChannelsCurrent >= 0 && g->_ChannelsCurrent < g->_ChannelsCount);
  
  IM_ASSERT(0);
#if 0
  if (g->_ChannelsCurrent == idx) {
    return;
  }
  memcpy(&g->_Channels[g->_ChannelsCurrent].CmdBuffer, &g->CmdBuffer, sizeof(g->CmdBuffer)); // copy 12 bytes, four times
  memcpy(&g->_Channels[g->_ChannelsCurrent].IdxBuffer, &IdxBuffer, sizeof(IdxBuffer));
  g->_ChannelsCurrent = idx;
  memcpy(&g->CmdBuffer, &g->_Channels[g->_ChannelsCurrent].CmdBuffer, sizeof(g->CmdBuffer));
  memcpy(&IdxBuffer, &g->_Channels[g->_ChannelsCurrent].IdxBuffer, sizeof(IdxBuffer));
  _IdxWritePtr = IdxBuffer.Data + IdxBuffer_Size;
#endif
}
void ImDrawList_ChannelsMerge(ImDrawList* g)
{
  int i, new_cmd_buffer_count, new_idx_buffer_count, sz;
  ImDrawCmd* cmd_write;
  ImDrawIdx* _IdxWritePtr;

  // Note that we never use or rely on channels_Size because it is merely a buffer that we never shrink back to 0 to keep all sub-buffers ready for use.
  if (g->_ChannelsCount <= 1) {
    return;
  }

  ImDrawList_ChannelsSetCurrent(g, 0);

  if (g->CmdBuffer_Size && g->CmdBuffer[g->CmdBuffer_Size-1].ElemCount == 0) {
    g->CmdBuffer_Size--;
  }

  new_cmd_buffer_count = 0, new_idx_buffer_count = 0;

  for (i = 1; i < g->_ChannelsCount; i++) {
    ImDrawChannel* ch = g->_Channels+i;

    if (ch->CmdBuffer_Size && ch->CmdBuffer[ch->CmdBuffer_Size-1].ElemCount == 0) {
      ch->CmdBuffer_Size--;
    }

    new_cmd_buffer_count += ch->CmdBuffer_Size;
    new_idx_buffer_count += ch->IdxBuffer_Size;
  }

  cmd_write = myImGui_AllocAdd(g->CmdBuffer, g->CmdBuffer_Size, new_cmd_buffer_count);
  _IdxWritePtr = myImGui_AllocAdd(g->IdxBuffer, g->IdxBuffer_Size, new_idx_buffer_count);

  for (i = 1; i < g->_ChannelsCount; i++) {
    ImDrawChannel* ch = g->_Channels+i;

    if (sz = ch->CmdBuffer_Size) {
      memcpy(cmd_write, ch->CmdBuffer, sz * sizeof(ImDrawCmd));
      cmd_write += sz;
    }

    if (sz = ch->IdxBuffer_Size) {
      memcpy(_IdxWritePtr, ch->IdxBuffer, sz * sizeof(ImDrawIdx));
      _IdxWritePtr += sz;
    }
  }

  ImDrawList_AddDrawCmd(g);
  g->_ChannelsCount = 1;
}
// NB: this can be called with negative count for removing primitives (as long as the result does not underflow)
void ImDrawList_PrimReserve(ImDrawList* g, int idx_count, int vtx_count)
{
  ImDrawCmd* draw_cmd = g->CmdBuffer+g->CmdBuffer_Size - 1;
  draw_cmd->ElemCount += idx_count;
  g->_VtxWritePtr = myImGui_AllocAdd(g->VtxBuffer, g->VtxBuffer_Size, vtx_count);
  g->_IdxWritePtr = myImGui_AllocAdd(g->IdxBuffer, g->IdxBuffer_Size, idx_count);
}
// Fully unrolled with inline call to keep our debug builds decently fast.
void ImDrawList_PrimRect(ImDrawList* g, ImVec2 a, ImVec2 c, ImU32 col)
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
  ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
  _IdxWritePtr[0] = idx;
  _IdxWritePtr[1] = (ImDrawIdx)(idx + 1);
  _IdxWritePtr[2] = (ImDrawIdx)(idx + 2);
  _IdxWritePtr[3] = idx;
  _IdxWritePtr[4] = (ImDrawIdx)(idx + 2);
  _IdxWritePtr[5] = (ImDrawIdx)(idx + 3);
  _VtxWritePtr[0].pos = a;
  _VtxWritePtr[0].uv = uv_a;
  _VtxWritePtr[0].col = col;
  _VtxWritePtr[1].pos = b;
  _VtxWritePtr[1].uv = uv_b;
  _VtxWritePtr[1].col = col;
  _VtxWritePtr[2].pos = c;
  _VtxWritePtr[2].uv = uv_c;
  _VtxWritePtr[2].col = col;
  _VtxWritePtr[3].pos = d;
  _VtxWritePtr[3].uv = uv_d;
  _VtxWritePtr[3].col = col;
  _VtxWritePtr += 4;
  _VtxCurrentIdx += 4;
  _IdxWritePtr += 6;
}
void ImDrawList_PrimQuadUV(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
{
  ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
  _IdxWritePtr[0] = idx;
  _IdxWritePtr[1] = (ImDrawIdx)(idx + 1);
  _IdxWritePtr[2] = (ImDrawIdx)(idx + 2);
  _IdxWritePtr[3] = idx;
  _IdxWritePtr[4] = (ImDrawIdx)(idx + 2);
  _IdxWritePtr[5] = (ImDrawIdx)(idx + 3);
  _VtxWritePtr[0].pos = a;
  _VtxWritePtr[0].uv = uv_a;
  _VtxWritePtr[0].col = col;
  _VtxWritePtr[1].pos = b;
  _VtxWritePtr[1].uv = uv_b;
  _VtxWritePtr[1].col = col;
  _VtxWritePtr[2].pos = c;
  _VtxWritePtr[2].uv = uv_c;
  _VtxWritePtr[2].col = col;
  _VtxWritePtr[3].pos = d;
  _VtxWritePtr[3].uv = uv_d;
  _VtxWritePtr[3].col = col;
  _VtxWritePtr += 4;
  _VtxCurrentIdx += 4;
  _IdxWritePtr += 6;
}
// TODO: Thickness anti-aliased lines cap are missing their AA fringe.
void ImDrawList_AddPolyline(ImDrawList* g, const ImVec2* points, const int points_count, ImU32 col, BOOL closed, float thickness, BOOL anti_aliased)
{
  int i;

  if (points_count < 2) {
    return;
  }

  const ImVec2 uv = GImGui->FontTexUvWhitePixel;
  anti_aliased &= GImGui->Style.AntiAliasedLines;
  //if (ImGui_GetIO(g).KeyCtrl) anti_aliased = false; // Debug
  int count = points_count;

  if (!closed) {
    count = points_count - 1;
  }

  const BOOL thick_line = thickness > 1.0f;

  if (anti_aliased) {
    // Anti-aliased stroke
    const float AA_SIZE = 1.0f;
    const ImU32 col_trans = col & 0x00ffffff;
    const int idx_count = thick_line ? count * 18 : count * 12;
    const int vtx_count = thick_line ? points_count * 4 : points_count * 3;
    PrimReserve(idx_count, vtx_count);
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
      unsigned int idx1 = _VtxCurrentIdx;

      for (int i1 = 0; i1 < count; i1++) {
        const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
        unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : idx1 + 3;
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
        _IdxWritePtr[0] = (ImDrawIdx)(idx2 + 0);
        _IdxWritePtr[1] = (ImDrawIdx)(idx1 + 0);
        _IdxWritePtr[2] = (ImDrawIdx)(idx1 + 2);
        _IdxWritePtr[3] = (ImDrawIdx)(idx1 + 2);
        _IdxWritePtr[4] = (ImDrawIdx)(idx2 + 2);
        _IdxWritePtr[5] = (ImDrawIdx)(idx2 + 0);
        _IdxWritePtr[6] = (ImDrawIdx)(idx2 + 1);
        _IdxWritePtr[7] = (ImDrawIdx)(idx1 + 1);
        _IdxWritePtr[8] = (ImDrawIdx)(idx1 + 0);
        _IdxWritePtr[9] = (ImDrawIdx)(idx1 + 0);
        _IdxWritePtr[10] = (ImDrawIdx)(idx2 + 0);
        _IdxWritePtr[11] = (ImDrawIdx)(idx2 + 1);
        _IdxWritePtr += 12;
        idx1 = idx2;
      }

      // Add vertexes
      for (i = 0; i < points_count; i++) {
        _VtxWritePtr[0].pos = points[i];
        _VtxWritePtr[0].uv = uv;
        _VtxWritePtr[0].col = col;
        _VtxWritePtr[1].pos = temp_points[i * 2 + 0];
        _VtxWritePtr[1].uv = uv;
        _VtxWritePtr[1].col = col_trans;
        _VtxWritePtr[2].pos = temp_points[i * 2 + 1];
        _VtxWritePtr[2].uv = uv;
        _VtxWritePtr[2].col = col_trans;
        _VtxWritePtr += 3;
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
      unsigned int idx1 = _VtxCurrentIdx;

      for (int i1 = 0; i1 < count; i1++) {
        const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
        unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : idx1 + 4;
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
        _IdxWritePtr[0] = (ImDrawIdx)(idx2 + 1);
        _IdxWritePtr[1] = (ImDrawIdx)(idx1 + 1);
        _IdxWritePtr[2] = (ImDrawIdx)(idx1 + 2);
        _IdxWritePtr[3] = (ImDrawIdx)(idx1 + 2);
        _IdxWritePtr[4] = (ImDrawIdx)(idx2 + 2);
        _IdxWritePtr[5] = (ImDrawIdx)(idx2 + 1);
        _IdxWritePtr[6] = (ImDrawIdx)(idx2 + 1);
        _IdxWritePtr[7] = (ImDrawIdx)(idx1 + 1);
        _IdxWritePtr[8] = (ImDrawIdx)(idx1 + 0);
        _IdxWritePtr[9] = (ImDrawIdx)(idx1 + 0);
        _IdxWritePtr[10] = (ImDrawIdx)(idx2 + 0);
        _IdxWritePtr[11] = (ImDrawIdx)(idx2 + 1);
        _IdxWritePtr[12] = (ImDrawIdx)(idx2 + 2);
        _IdxWritePtr[13] = (ImDrawIdx)(idx1 + 2);
        _IdxWritePtr[14] = (ImDrawIdx)(idx1 + 3);
        _IdxWritePtr[15] = (ImDrawIdx)(idx1 + 3);
        _IdxWritePtr[16] = (ImDrawIdx)(idx2 + 3);
        _IdxWritePtr[17] = (ImDrawIdx)(idx2 + 2);
        _IdxWritePtr += 18;
        idx1 = idx2;
      }

      // Add vertexes
      for (i = 0; i < points_count; i++) {
        _VtxWritePtr[0].pos = temp_points[i * 4 + 0];
        _VtxWritePtr[0].uv = uv;
        _VtxWritePtr[0].col = col_trans;
        _VtxWritePtr[1].pos = temp_points[i * 4 + 1];
        _VtxWritePtr[1].uv = uv;
        _VtxWritePtr[1].col = col;
        _VtxWritePtr[2].pos = temp_points[i * 4 + 2];
        _VtxWritePtr[2].uv = uv;
        _VtxWritePtr[2].col = col;
        _VtxWritePtr[3].pos = temp_points[i * 4 + 3];
        _VtxWritePtr[3].uv = uv;
        _VtxWritePtr[3].col = col_trans;
        _VtxWritePtr += 4;
      }
    }

    _VtxCurrentIdx += (ImDrawIdx)vtx_count;
  }
  else {
    // Non Anti-aliased Stroke
    const int idx_count = count * 6;
    const int vtx_count = count * 4; // FIXME-OPT: Not sharing edges
    PrimReserve(idx_count, vtx_count);

    for (int i1 = 0; i1 < count; i1++) {
      const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
      const ImVec2& p1 = points[i1];
      const ImVec2& p2 = points[i2];
      ImVec2 diff = p2 - p1;
      diff *= ImInvLength(diff, 1.0f);
      const float dx = diff.x * (thickness * 0.5f);
      const float dy = diff.y * (thickness * 0.5f);
      _VtxWritePtr[0].pos.x = p1.x + dy;
      _VtxWritePtr[0].pos.y = p1.y - dx;
      _VtxWritePtr[0].uv = uv;
      _VtxWritePtr[0].col = col;
      _VtxWritePtr[1].pos.x = p2.x + dy;
      _VtxWritePtr[1].pos.y = p2.y - dx;
      _VtxWritePtr[1].uv = uv;
      _VtxWritePtr[1].col = col;
      _VtxWritePtr[2].pos.x = p2.x - dy;
      _VtxWritePtr[2].pos.y = p2.y + dx;
      _VtxWritePtr[2].uv = uv;
      _VtxWritePtr[2].col = col;
      _VtxWritePtr[3].pos.x = p1.x - dy;
      _VtxWritePtr[3].pos.y = p1.y + dx;
      _VtxWritePtr[3].uv = uv;
      _VtxWritePtr[3].col = col;
      _VtxWritePtr += 4;
      _IdxWritePtr[0] = (ImDrawIdx)(_VtxCurrentIdx);
      _IdxWritePtr[1] = (ImDrawIdx)(_VtxCurrentIdx + 1);
      _IdxWritePtr[2] = (ImDrawIdx)(_VtxCurrentIdx + 2);
      _IdxWritePtr[3] = (ImDrawIdx)(_VtxCurrentIdx);
      _IdxWritePtr[4] = (ImDrawIdx)(_VtxCurrentIdx + 2);
      _IdxWritePtr[5] = (ImDrawIdx)(_VtxCurrentIdx + 3);
      _IdxWritePtr += 6;
      _VtxCurrentIdx += 4;
    }
  }
}
void ImDrawList_AddConvexPolyFilled(ImDrawList* g, const ImVec2* points, const int points_count, ImU32 col, BOOL anti_aliased)
{
  int i, i0, i1;
  const ImVec2 uv = GImGui->FontTexUvWhitePixel;
  anti_aliased &= GImGui->Style.AntiAliasedShapes;

  //if (ImGui_GetIO(g).KeyCtrl) anti_aliased = false; // Debug
  if (anti_aliased) {
    // Anti-aliased Fill
    const float AA_SIZE = 1.0f;
    const ImU32 col_trans = col & 0x00ffffff;
    const int idx_count = (points_count - 2) * 3 + points_count * 6;
    const int vtx_count = (points_count * 2);
    PrimReserve(idx_count, vtx_count);
    // Add indexes for fill
    unsigned int vtx_inner_idx = _VtxCurrentIdx;
    unsigned int vtx_outer_idx = _VtxCurrentIdx + 1;

    for (i = 2; i < points_count; i++) {
      _IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx);
      _IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx + ((i - 1) << 1));
      _IdxWritePtr[2] = (ImDrawIdx)(vtx_inner_idx + (i << 1));
      _IdxWritePtr += 3;
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
      _VtxWritePtr[0].pos = (points[i1] - dm);
      _VtxWritePtr[0].uv = uv;
      _VtxWritePtr[0].col = col; // Inner
      _VtxWritePtr[1].pos = (points[i1] + dm);
      _VtxWritePtr[1].uv = uv;
      _VtxWritePtr[1].col = col_trans; // Outer
      _VtxWritePtr += 2;
      // Add indexes for fringes
      _IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx + (i1 << 1));
      _IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx + (i0 << 1));
      _IdxWritePtr[2] = (ImDrawIdx)(vtx_outer_idx + (i0 << 1));
      _IdxWritePtr[3] = (ImDrawIdx)(vtx_outer_idx + (i0 << 1));
      _IdxWritePtr[4] = (ImDrawIdx)(vtx_outer_idx + (i1 << 1));
      _IdxWritePtr[5] = (ImDrawIdx)(vtx_inner_idx + (i1 << 1));
      _IdxWritePtr += 6;
    }

    _VtxCurrentIdx += (ImDrawIdx)vtx_count;
  }
  else {
    // Non Anti-aliased Fill
    const int idx_count = (points_count - 2) * 3;
    const int vtx_count = points_count;
    PrimReserve(idx_count, vtx_count);

    for (i = 0; i < vtx_count; i++) {
      _VtxWritePtr[0].pos = points[i];
      _VtxWritePtr[0].uv = uv;
      _VtxWritePtr[0].col = col;
      _VtxWritePtr++;
    }

    for (i = 2; i < points_count; i++) {
      _IdxWritePtr[0] = (ImDrawIdx)(_VtxCurrentIdx);
      _IdxWritePtr[1] = (ImDrawIdx)(_VtxCurrentIdx + i - 1);
      _IdxWritePtr[2] = (ImDrawIdx)(_VtxCurrentIdx + i);
      _IdxWritePtr += 3;
    }

    _VtxCurrentIdx += (ImDrawIdx)vtx_count;
  }
}
void ImDrawList_PathArcToFast(ImDrawList* g, const ImVec2& centre, float radius, int amin, int amax)
{
  int i;
  static ImVec2 circle_vtx[12];
  static BOOL circle_vtx_builds = false;
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
    _Path.push_back(centre);
  }
  else {
    _Path.reserve(_Path_Size + (amax - amin + 1));

    for (int a = amin; a <= amax; a++) {
      const ImVec2& c = circle_vtx[a % circle_vtx_count];
      _Path.push_back(fVec2(centre.x + c.x * radius, centre.y + c.y * radius));
    }
  }
}
void ImDrawList_PathArcTo(ImDrawList* g, const ImVec2& centre, float radius, float amin, float amax, int num_segments)
{
  int i;

  if (radius == 0.0f) {
    _Path.push_back(centre);
  }

  _Path.reserve(_Path_Size + (num_segments + 1));

  for (i = 0; i <= num_segments; i++) {
    const float a = amin + ((float)i / (float)num_segments) * (amax - amin);
    _Path.push_back(fVec2(centre.x + cosf(a) * radius, centre.y + sinf(a) * radius));
  }
}
static void PathBezierToCasteljau(ImDrawList* g, ImVector<ImVec2>* path, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tess_tol, int level)
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
  ImVec2 p1 = _Path.back(g);

  if (num_segments == 0) {
    // Auto-tessellated
    PathBezierToCasteljau(&_Path, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, GImGui->Style.CurveTessellationTol, 0);
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
      _Path.push_back(fVec2(w1 * p1.x + w2 * p2.x + w3 * p3.x + w4 * p4.x, w1 * p1.y + w2 * p2.y + w3 * p3.y + w4 * p4.y));
    }
  }
}
void ImDrawList_PathRect(ImDrawList* g, const ImVec2& a, const ImVec2& b, float rounding, int rounding_corners)
{
  float r = rounding;
  r = ImMin(r, fabsf(b.x - a.x) * (((rounding_corners & (1 | 2)) == (1 | 2)) || ((rounding_corners & (4 | 8)) == (4 | 8)) ? 0.5f : 1.0f) - 1.0f);
  r = ImMin(r, fabsf(b.y - a.y) * (((rounding_corners & (1 | 8)) == (1 | 8)) || ((rounding_corners & (2 | 4)) == (2 | 4)) ? 0.5f : 1.0f) - 1.0f);

  if (r <= 0.0f || rounding_corners == 0) {
    PathLineTo(a);
    PathLineTo(fVec2(b.x, a.y));
    PathLineTo(b);
    PathLineTo(fVec2(a.x, b.y));
  }
  else {
    const float r0 = (rounding_corners & 1) ? r : 0.0f;
    const float r1 = (rounding_corners & 2) ? r : 0.0f;
    const float r2 = (rounding_corners & 4) ? r : 0.0f;
    const float r3 = (rounding_corners & 8) ? r : 0.0f;
    PathArcToFast(fVec2(a.x + r0, a.y + r0), r0, 6, 9);
    PathArcToFast(fVec2(b.x - r1, a.y + r1), r1, 9, 12);
    PathArcToFast(fVec2(b.x - r2, b.y - r2), r2, 0, 3);
    PathArcToFast(fVec2(a.x + r3, b.y - r3), r3, 3, 6);
  }
}
void ImDrawList_AddLine(ImDrawList* g, const ImVec2& a, const ImVec2& b, ImU32 col, float thickness)
{
  if ((col >> 24) == 0) {
    return;
  }

  PathLineTo(a + fVec2(0.5f, 0.5f));
  PathLineTo(b + fVec2(0.5f, 0.5f));
  PathStroke(col, false, thickness);
}
// a: upper-left, b: lower-right. we don't render 1 px sized rectangles properly.
void ImDrawList_AddRect(ImDrawList* g, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners, float thickness)
{
  if ((col >> 24) == 0) {
    return;
  }

  PathRect(a + fVec2(0.5f, 0.5f), b - fVec2(0.5f, 0.5f), rounding, rounding_corners);
  PathStroke(col, true, thickness);
}
void ImDrawList_AddRectFilled(ImDrawList* g, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners)
{
  if ((col >> 24) == 0) {
    return;
  }

  if (rounding > 0.0f) {
    PathRect(a, b, rounding, rounding_corners);
    PathFill(col);
  }
  else {
    PrimReserve(6, 4);
    PrimRect(a, b, col);
  }
}
void ImDrawList_AddRectFilledMultiColor(ImDrawList* g, const ImVec2& a, const ImVec2& c, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left)
{
  if (((col_upr_left | col_upr_right | col_bot_right | col_bot_left) >> 24) == 0) {
    return;
  }

  const ImVec2 uv = GImGui->FontTexUvWhitePixel;
  PrimReserve(6, 4);
  PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx));
  PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx + 1));
  PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx + 2));
  PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx));
  PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx + 2));
  PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx + 3));
  PrimWriteVtx(a, uv, col_upr_left);
  PrimWriteVtx(fVec2(c.x, a.y), uv, col_upr_right);
  PrimWriteVtx(c, uv, col_bot_right);
  PrimWriteVtx(fVec2(a.x, c.y), uv, col_bot_left);
}
void ImDrawList_AddQuad(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness)
{
  if ((col >> 24) == 0) {
    return;
  }

  PathLineTo(a);
  PathLineTo(b);
  PathLineTo(c);
  PathLineTo(d);
  PathStroke(col, true, thickness);
}
void ImDrawList_AddQuadFilled(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col)
{
  if ((col >> 24) == 0) {
    return;
  }

  PathLineTo(a);
  PathLineTo(b);
  PathLineTo(c);
  PathLineTo(d);
  PathFill(col);
}
void ImDrawList_AddTriangle(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness)
{
  if ((col >> 24) == 0) {
    return;
  }

  PathLineTo(a);
  PathLineTo(b);
  PathLineTo(c);
  PathStroke(col, true, thickness);
}
void ImDrawList_AddTriangleFilled(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col)
{
  if ((col >> 24) == 0) {
    return;
  }

  PathLineTo(a);
  PathLineTo(b);
  PathLineTo(c);
  PathFill(col);
}
void ImDrawList_AddCircle(ImDrawList* g, const ImVec2& centre, float radius, ImU32 col, int num_segments, float thickness)
{
  if ((col >> 24) == 0) {
    return;
  }

  const float a_max = IM_PI * 2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
  PathArcTo(centre, radius - 0.5f, 0.0f, a_max, num_segments);
  PathStroke(col, true, thickness);
}
void ImDrawList_AddCircleFilled(ImDrawList* g, const ImVec2& centre, float radius, ImU32 col, int num_segments)
{
  if ((col >> 24) == 0) {
    return;
  }

  const float a_max = IM_PI * 2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
  PathArcTo(centre, radius, 0.0f, a_max, num_segments);
  PathFill(col);
}
void ImDrawList_AddBezierCurve(ImDrawList* g, const ImVec2& pos0, const ImVec2& cp0, const ImVec2& cp1, const ImVec2& pos1, ImU32 col, float thickness, int num_segments)
{
  if ((col >> 24) == 0) {
    return;
  }

  PathLineTo(pos0);
  PathBezierCurveTo(cp0, cp1, pos1, num_segments);
  PathStroke(col, false, thickness);
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

  // Note: This is one of the few instance of breaking the encapsulation of ImDrawList, as we pull this from ImGui state, but it is just SO useful.
  // Might just move Font/FontSize to ImDrawList?
  if (font == NULL) {
    font = GImGui->Font;
  }

  if (font_size == 0.0f) {
    font_size = GImGui->FontSize;
  }

  IM_ASSERT(font->ContainerAtlas->TexID == g->_TextureIdStack.back(g)); // Use high-level ImGui_PushFont(g) or low-level ImDrawList_PushTextureId(g) to change font.
  ImVec4 clip_rect = g->_ClipRectStack.back(g);

  if (cpu_fine_clip_rect) {
    clip_rect.x = ImMax(clip_rect.x, cpu_fine_clip_rect->x);
    clip_rect.y = ImMax(clip_rect.y, cpu_fine_clip_rect->y);
    clip_rect.z = ImMin(clip_rect.z, cpu_fine_clip_rect->z);
    clip_rect.w = ImMin(clip_rect.w, cpu_fine_clip_rect->w);
  }

  font->RenderText(this, font_size, pos, col, clip_rect, text_begin, text_end, wrap_width, cpu_fine_clip_rect != NULL);
}
void ImDrawList_AddText(ImDrawList* g, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end)
{
  AddText(GImGui->Font, GImGui->FontSize, pos, col, text_begin, text_end);
}
void ImDrawList_AddImage(ImDrawList* g, ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv0, const ImVec2& uv1, ImU32 col)
{
  if ((col >> 24) == 0) {
    return;
  }

  // FIXME-OPT: This is wasting draw calls.
  const BOOL push_texture_id = g->_TextureIdStack.empty(g) || user_texture_id != g->_TextureIdStack.back(g);

  if (push_texture_id) {
    PushTextureID(user_texture_id);
  }

  PrimReserve(6, 4);
  PrimRectUV(a, b, uv0, uv1, col);

  if (push_texture_id) {
    PopTextureID(g);
  }
}
//-----------------------------------------------------------------------------
// ImDrawData
//-----------------------------------------------------------------------------
// For backward compatibility: convert all buffers from indexed to de-indexed, in case you cannot render indexed. Note: this is slow and most likely a waste of resources. Always prefer indexed rendering!
void ImDrawData_DeIndexAllBuffers(ImDrawList* g)
{
  int i;
  ImVector<ImDrawVert> new_vtx_buffer;
  TotalVtxCount = TotalIdxCount = 0;

  for (i = 0; i < CmdListsCount; i++) {
    ImDrawList* cmd_list = CmdLists[i];

    if (cmd_list->IdxBuffer.empty(g)) {
      continue;
    }

    new_vtx_buffer.resize(cmd_list->IdxBuffer_Size);

    for (int j = 0; j < cmd_list->IdxBuffer_Size; j++) {
      new_vtx_buffer[j] = cmd_list->VtxBuffer[cmd_list->IdxBuffer[j]];
    }

    cmd_list->VtxBuffer.swap(new_vtx_buffer);
    cmd_list->IdxBuffer myImGui_Free(g);
    TotalVtxCount += cmd_list->VtxBuffer_Size;
  }
}
// Helper to scale the ClipRect field of each ImDrawCmd. Use if your final output buffer is at a different scale than ImGui expects, or if there is a difference between your window resolution and framebuffer resolution.
void ImDrawData_ScaleClipRects(ImDrawList* g, const ImVec2& scale)
{
  int i;

  for (i = 0; i < CmdListsCount; i++) {
    ImDrawList* cmd_list = CmdLists[i];

    for (int cmd_i = 0; cmd_i < cmd_list->g->CmdBuffer_Size; cmd_i++) {
      ImDrawCmd* cmd = &cmd_list->g->CmdBuffer[cmd_i];
      cmd->ClipRect = fVec4(cmd->ClipRect.x * scale.x, cmd->ClipRect.y * scale.y, cmd->ClipRect.z * scale.x, cmd->ClipRect.w * scale.y);
    }
  }
}