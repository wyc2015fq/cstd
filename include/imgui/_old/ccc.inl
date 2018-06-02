//-----------------------------------------------------------------------------
// ImFont
//-----------------------------------------------------------------------------
void ImFont_Init(ImFont* g)
{
  memset(g, 0, sizeof(*g));
  g->Scale = 1.0f;
  g->FallbackChar = (ImWchar)'?';
  ImFont_Clear(g);
}
// Invalidate active font so that the user gets a clear crash instead of a dangling pointer.
// If you want to delete fonts you need to do it between Render() and NewFrame().
// FIXME-CLEANUP
/*
ImGuiContext& g = *GImGui;
if (g.Font == this)
g.Font = NULL;
*/
void ImFont_Clear(ImFont* g)
{
  g->FontSize = 0.0f;
  g->DisplayOffset = fVec2(0.0f, 1.0f);
  g->ConfigData = NULL;
  g->ConfigDataCount = 0;
  g->Ascent = g->Descent = 0.0f;
  g->ContainerAtlas = NULL;
  g->Glyphs.clear();
  g->FallbackGlyph = NULL;
  g->FallbackXAdvance = 0.0f;
  g->IndexXAdvance.clear();
  g->IndexLookup.clear();
}
void ImFont_BuildLookupTable(ImFont* g)
{
  int i;
  int max_codepoint = 0;
  for (i = 0; i != g->Glyphs.Size; i++) {
    max_codepoint = ImMax(max_codepoint, (int)g->Glyphs[i].Codepoint);
  }
  IM_ASSERT(g->Glyphs.Size < 0xFFFF); // -1 is reserved
  g->IndexXAdvance.clear();
  g->IndexLookup.clear();
  ImFont_GrowIndex(g, max_codepoint + 1);
  for (i = 0; i < g->Glyphs.Size; i++) {
    int codepoint = (int)g->Glyphs[i].Codepoint;
    g->IndexXAdvance[codepoint] = g->Glyphs[i].XAdvance;
    g->IndexLookup[codepoint] = (unsigned short)i;
  }
  // Create a glyph to handle TAB
  // FIXME: Needs proper TAB handling but it needs to be contextualized (or we could arbitrary say that each string starts at "column 0" ?)
  if (ImFont_FindGlyph(g, (unsigned short)' ')) {
    if (g->Glyphs.back().Codepoint != '\t') { // So we can call this function multiple times
      g->Glyphs.resize(g->Glyphs.Size + 1);
    }
    ImFontGlyph& tab_glyph = g->Glyphs.back();
    tab_glyph = *ImFont_FindGlyph(g, (unsigned short)' ');
    tab_glyph.Codepoint = '\t';
    tab_glyph.XAdvance *= 4;
    g->IndexXAdvance[(int)tab_glyph.Codepoint] = (float)tab_glyph.XAdvance;
    g->IndexLookup[(int)tab_glyph.Codepoint] = (unsigned short)(g->Glyphs.Size - 1);
  }
  g->FallbackGlyph = NULL;
  g->FallbackGlyph = ImFont_FindGlyph(g, g->FallbackChar);
  g->FallbackXAdvance = g->FallbackGlyph ? g->FallbackGlyph->XAdvance : 0.0f;
  for (i = 0; i < max_codepoint + 1; i++) {
    if (g->IndexXAdvance[i] < 0.0f) {
      g->IndexXAdvance[i] = g->FallbackXAdvance;
    }
  }
}
void ImFont_SetFallbackChar(ImFont* g, ImWchar c)
{
  g->FallbackChar = c;
  ImFont_BuildLookupTable(g);
}
void ImFont_GrowIndex(ImFont* g, int new_size)
{
  int i;
  IM_ASSERT(g->IndexXAdvance.Size == g->IndexLookup.Size);
  int old_size = g->IndexLookup.Size;
  if (new_size <= old_size) {
    return;
  }
  g->IndexXAdvance.resize(new_size);
  g->IndexLookup.resize(new_size);
  for (i = old_size; i < new_size; i++) {
    g->IndexXAdvance[i] = -1.0f;
    g->IndexLookup[i] = (unsigned short) - 1;
  }
}
void ImFont_AddRemapChar(ImFont* g, ImWchar dst, ImWchar src, bool overwrite_dst)
{
  IM_ASSERT(g->IndexLookup.Size > 0); // Currently this can only be called AFTER the font has been built, aka after calling ImFontAtlas_GetTexDataAs*() function.
  int index_size = g->IndexLookup.Size;
  if (dst < index_size && g->IndexLookup.Data[dst] == (unsigned short) - 1 && !overwrite_dst) { // 'dst' already exists
    return;
  }
  if (src >= index_size && dst >= index_size) { // both 'dst' and 'src' don't exist -> no-op
    return;
  }
  ImFont_GrowIndex(g, dst + 1);
  g->IndexLookup[dst] = (src < index_size) ? g->IndexLookup.Data[src] : (unsigned short) - 1;
  g->IndexXAdvance[dst] = (src < index_size) ? g->IndexXAdvance.Data[src] : 1.0f;
}
const ImFontGlyph* ImFont_FindGlyph(const ImFont* g, unsigned short c)
{
  if (c < g->IndexLookup.Size) {
    const unsigned short i = g->IndexLookup[c];
    if (i != (unsigned short) - 1) {
      return &g->Glyphs.Data[i];
    }
  }
  return g->FallbackGlyph;
}
const char* ImFont_CalcWordWrapPositionA(const ImFont* g, float scale, const char* text, const char* text_end, float wrap_width)
{
  // Simple word-wrapping for English, not full-featured. Please submit failing cases!
  // FIXME: Much possible improvements (don't cut things like "word !", "word!!!" but cut within "word,,,,", more sensible support for punctuations, support for Unicode punctuations, etc.)
  // For references, possible wrap point marked with ^
  // "aaa bbb, ccc,ddd. eee fff. ggg!"
  // ^ ^ ^ ^ ^__ ^ ^
  // List of hardcoded separators: .,;!?'"
  // Skip extra blanks after a line returns (that includes not counting them in width computation)
  // e.g. "Hello world" --> "Hello" "World"
  // Cut words that cannot possibly fit within one line.
  // e.g.: "The tropical fish" with ~5 characters worth of width --> "The tr" "opical" "fish"
  float line_width = 0.0f;
  float word_width = 0.0f;
  float blank_width = 0.0f;
  const char* word_end = text;
  const char* prev_word_end = NULL;
  bool inside_word = true;
  const char* s = text;
  while (s < text_end) {
    unsigned int c = (unsigned int) * s;
    const char* next_s;
    if (c < 0x80) {
      next_s = s + 1;
    }
    else {
      next_s = s + ImTextCharFromUtf8(&c, s, text_end);
    }
    if (c == 0) {
      break;
    }
    if (c < 32) {
      if (c == '\n') {
        line_width = word_width = blank_width = 0.0f;
        inside_word = true;
        s = next_s;
        continue;
      }
      if (c == '\r') {
        s = next_s;
        continue;
      }
    }
    const float char_width = ((int)c < g->IndexXAdvance.Size ? g->IndexXAdvance[(int)c] : g->FallbackXAdvance) * scale;
    if (ImCharIsSpace(c)) {
      if (inside_word) {
        line_width += blank_width;
        blank_width = 0.0f;
      }
      blank_width += char_width;
      inside_word = false;
    }
    else {
      word_width += char_width;
      if (inside_word) {
        word_end = next_s;
      }
      else {
        prev_word_end = word_end;
        line_width += word_width + blank_width;
        word_width = blank_width = 0.0f;
      }
      // Allow wrapping after punctuation.
      inside_word = !(c == '.' || c == ',' || c == ';' || c == '!' || c == '?' || c == '\"');
    }
    // We ignore blank width at the end of the line (they can be skipped)
    if (line_width + word_width >= wrap_width) {
      // Words that cannot possibly fit within an entire line will be cut anywhere.
      if (word_width < wrap_width) {
        s = prev_word_end ? prev_word_end : word_end;
      }
      break;
    }
    s = next_s;
  }
  return s;
}
ImVec2 ImFont_CalcTextSizeA(const ImFont* g, float size, float max_width, float wrap_width, const char* text_begin, const char* text_end, const char** remaining)
{
  if (!text_end) {
    text_end = text_begin + strlen(text_begin); // FIXME-OPT: Need to avoid this.
  }
  const float line_height = size;
  const float scale = size / g->FontSize;
  ImVec2 text_size = fVec2(0, 0);
  float line_width = 0.0f;
  const bool word_wrap_enabled = (wrap_width > 0.0f);
  const char* word_wrap_eol = NULL;
  const char* s = text_begin;
  while (s < text_end) {
    if (word_wrap_enabled) {
      // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
      if (!word_wrap_eol) {
        word_wrap_eol = ImFont_CalcWordWrapPositionA(g, scale, s, text_end, wrap_width - line_width);
        if (word_wrap_eol == s) { // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
          word_wrap_eol++; // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
        }
      }
      if (s >= word_wrap_eol) {
        if (text_size.x < line_width) {
          text_size.x = line_width;
        }
        text_size.y += line_height;
        line_width = 0.0f;
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
    const char* prev_s = s;
    unsigned int c = (unsigned int) * s;
    if (c < 0x80) {
      s += 1;
    }
    else {
      s += ImTextCharFromUtf8(&c, s, text_end);
      if (c == 0) {
        break;
      }
    }
    if (c < 32) {
      if (c == '\n') {
        text_size.x = ImMax(text_size.x, line_width);
        text_size.y += line_height;
        line_width = 0.0f;
        continue;
      }
      if (c == '\r') {
        continue;
      }
    }
    const float char_width = ((int)c < g->IndexXAdvance.Size ? g->IndexXAdvance[(int)c] : g->FallbackXAdvance) * scale;
    if (line_width + char_width >= max_width) {
      s = prev_s;
      break;
    }
    line_width += char_width;
  }
  if (text_size.x < line_width) {
    text_size.x = line_width;
  }
  if (line_width > 0 || text_size.y == 0.0f) {
    text_size.y += line_height;
  }
  if (remaining) {
    *remaining = s;
  }
  return text_size;
}
void ImFont_RenderChar(const ImFont* g, ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, unsigned short c)
{
  if (c == ' ' || c == '\t' || c == '\n' || c == '\r') { // Match behavior of RenderText(), those 4 codepoints are hard-coded.
    return;
  }
  if (const ImFontGlyph* glyph = ImFont_FindGlyph(g, c)) {
    float scale = (size >= 0.0f) ? (size / g->FontSize) : 1.0f;
    pos.x = (float)(int)pos.x + g->DisplayOffset.x;
    pos.y = (float)(int)pos.y + g->DisplayOffset.y;
    ImVec2 pos_tl = fVec2(pos.x + glyph->X0 * scale, pos.y + glyph->Y0 * scale);
    ImVec2 pos_br = fVec2(pos.x + glyph->X1 * scale, pos.y + glyph->Y1 * scale);
    ImDrawList_PrimReserve(draw_list, 6, 4);
    ImDrawList_PrimRectUV(draw_list, pos_tl, pos_br, fVec2(glyph->U0, glyph->V0), fVec2(glyph->U1, glyph->V1), col);
  }
}
void ImFont_RenderText(const ImFont* g, ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width, bool cpu_fine_clip)
{
  if (!text_end) {
    text_end = text_begin + strlen(text_begin);
  }
  // Align to be pixel perfect
  pos.x = (float)(int)pos.x + g->DisplayOffset.x;
  pos.y = (float)(int)pos.y + g->DisplayOffset.y;
  float x = pos.x;
  float y = pos.y;
  if (y > clip_rect.w) {
    return;
  }
  const float scale = size / g->FontSize;
  const float line_height = g->FontSize * scale;
  const bool word_wrap_enabled = (wrap_width > 0.0f);
  const char* word_wrap_eol = NULL;
  // Skip non-visible lines
  const char* s = text_begin;
  if (!word_wrap_enabled && y + line_height < clip_rect.y)
    while (s < text_end && *s != '\n') { // Fast-forward to next line
      s++;
    }
  // Reserve vertices for remaining worse case (over-reserving is useful and easily amortized)
  const int vtx_count_max = (int)(text_end - s) * 4;
  const int idx_count_max = (int)(text_end - s) * 6;
  const int idx_expected_size = draw_list->IdxBuffer.Size + idx_count_max;
  ImDrawList_PrimReserve(draw_list, idx_count_max, vtx_count_max);
  ImDrawVert* vtx_write = draw_list->_VtxWritePtr;
  ImDrawIdx* idx_write = draw_list->_IdxWritePtr;
  unsigned int vtx_current_idx = draw_list->_VtxCurrentIdx;
  while (s < text_end) {
    if (word_wrap_enabled) {
      // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
      if (!word_wrap_eol) {
        word_wrap_eol = ImFont_CalcWordWrapPositionA(g, scale, s, text_end, wrap_width - (x - pos.x));
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
    unsigned int c = (unsigned int) * s;
    if (c < 0x80) {
      s += 1;
    }
    else {
      s += ImTextCharFromUtf8(&c, s, text_end);
      if (c == 0) {
        break;
      }
    }
    if (c < 32) {
      if (c == '\n') {
        x = pos.x;
        y += line_height;
        if (y > clip_rect.w) {
          break;
        }
        if (!word_wrap_enabled && y + line_height < clip_rect.y)
          while (s < text_end && *s != '\n') { // Fast-forward to next line
            s++;
          }
        continue;
      }
      if (c == '\r') {
        continue;
      }
    }
    float char_width = 0.0f;
    if (const ImFontGlyph* glyph = ImFont_FindGlyph(g, (unsigned short)c)) {
      char_width = glyph->XAdvance * scale;
      // Arbitrarily assume that both space and tabs are empty glyphs as an optimization
      if (c != ' ' && c != '\t') {
        // We don't do a second finer clipping test on the Y axis as we've already skipped anything before clip_rect.y and exit once we pass clip_rect.w
        float x1 = x + glyph->X0 * scale;
        float x2 = x + glyph->X1 * scale;
        float y1 = y + glyph->Y0 * scale;
        float y2 = y + glyph->Y1 * scale;
        if (x1 <= clip_rect.z && x2 >= clip_rect.x) {
          // Render a character
          float u1 = glyph->U0;
          float v1 = glyph->V0;
          float u2 = glyph->U1;
          float v2 = glyph->V1;
          // CPU side clipping used to fit text in their frame when the frame is too small. Only does clipping for axis aligned quads.
          if (cpu_fine_clip) {
            if (x1 < clip_rect.x) {
              u1 = u1 + (1.0f - (x2 - clip_rect.x) / (x2 - x1)) * (u2 - u1);
              x1 = clip_rect.x;
            }
            if (y1 < clip_rect.y) {
              v1 = v1 + (1.0f - (y2 - clip_rect.y) / (y2 - y1)) * (v2 - v1);
              y1 = clip_rect.y;
            }
            if (x2 > clip_rect.z) {
              u2 = u1 + ((clip_rect.z - x1) / (x2 - x1)) * (u2 - u1);
              x2 = clip_rect.z;
            }
            if (y2 > clip_rect.w) {
              v2 = v1 + ((clip_rect.w - y1) / (y2 - y1)) * (v2 - v1);
              y2 = clip_rect.w;
            }
            if (y1 >= y2) {
              x += char_width;
              continue;
            }
          }
          // We are NOT calling PrimRectUV() here because non-inlined causes too much overhead in a debug build.
          // Inlined here:
          {
            idx_write[0] = (ImDrawIdx)(vtx_current_idx);
            idx_write[1] = (ImDrawIdx)(vtx_current_idx + 1);
            idx_write[2] = (ImDrawIdx)(vtx_current_idx + 2);
            idx_write[3] = (ImDrawIdx)(vtx_current_idx);
            idx_write[4] = (ImDrawIdx)(vtx_current_idx + 2);
            idx_write[5] = (ImDrawIdx)(vtx_current_idx + 3);
            vtx_write[0].pos.x = x1;
            vtx_write[0].pos.y = y1;
            vtx_write[0].col = col;
            vtx_write[0].uv.x = u1;
            vtx_write[0].uv.y = v1;
            vtx_write[1].pos.x = x2;
            vtx_write[1].pos.y = y1;
            vtx_write[1].col = col;
            vtx_write[1].uv.x = u2;
            vtx_write[1].uv.y = v1;
            vtx_write[2].pos.x = x2;
            vtx_write[2].pos.y = y2;
            vtx_write[2].col = col;
            vtx_write[2].uv.x = u2;
            vtx_write[2].uv.y = v2;
            vtx_write[3].pos.x = x1;
            vtx_write[3].pos.y = y2;
            vtx_write[3].col = col;
            vtx_write[3].uv.x = u1;
            vtx_write[3].uv.y = v2;
            vtx_write += 4;
            vtx_current_idx += 4;
            idx_write += 6;
          }
        }
      }
    }
    x += char_width;
  }
  // Give back unused vertices
  draw_list->VtxBuffer.resize((int)(vtx_write - draw_list->VtxBuffer.Data));
  draw_list->IdxBuffer.resize((int)(idx_write - draw_list->IdxBuffer.Data));
  draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 1].ElemCount -= (idx_expected_size - draw_list->IdxBuffer.Size);
  draw_list->_VtxWritePtr = vtx_write;
  draw_list->_IdxWritePtr = idx_write;
  draw_list->_VtxCurrentIdx = (unsigned int)draw_list->VtxBuffer.Size;
}

