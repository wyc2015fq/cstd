
typedef char* (*nextchar_f)(unsigned int* pc, const char* text, const char* text_end);

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
const char* CalcWordWrapPosition(const FontInfo* ff, const char* text, const char* text_end, nextchar_f nextchar, float wrap_width, float scale)
{
  float line_width = 0.0f;
  float word_width = 0.0f;
  float blank_width = 0.0f;
  const char* word_end = text;
  const char* prev_word_end = NULL;
  bool inside_word = true;
  const char* s = text;
  float HalfXAdvance = ff->FallbackXAdvance/2;
  while (s < text_end) {
    unsigned int c = 0;
    const char* next_s;
    float char_width;
    next_s = nextchar(&c, s, text_end);
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
    if (ff->IndexXAdvance) {
      char_width = (int)c < ff->IndexXAdvance_Size ? ff->IndexXAdvance[c] : ff->FallbackXAdvance;
    } else {
      char_width = c < 128 ? HalfXAdvance : ff->FallbackXAdvance;
    }
    char_width *= scale;
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
    if (line_width + word_width > wrap_width) {
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

ImVec2 CalcTextSize(const FontInfo* ff, const char* text, const char* text_end, nextchar_f nextchar, const char** remaining, float max_width, float wrap_width, float line_height, float scale)
{
  ImVec2 text_size = fVec2(0, 0);
  float char_width, line_width = 0.0f;
  const bool word_wrap_enabled = (wrap_width > 0.0f);
  const char* word_wrap_eol = NULL;
  const char* s;
  const char* prev_s;
  unsigned int c;
  float HalfXAdvance = ff->FallbackXAdvance/2;
  if (NULL==text) {
    text = "test";
  }
  s = text;
  while (s < text_end) {
    if (word_wrap_enabled) {
      // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
      if (!word_wrap_eol) {
        word_wrap_eol = CalcWordWrapPosition(ff, s, text_end, nextchar, wrap_width - line_width, scale);
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
          c = 0;
          s = nextchar(&c, s, text_end);
          if (ImCharIsSpace(c)) {
          }
          else if (c == '\n') {
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
    prev_s = s;
    c = 0;
    s = nextchar(&c, s, text_end);
    if (c == 0) {
      break;
    }
    if (c < 32) {
      if (c == '\n') {
        text_size.x = MAX(text_size.x, line_width);
        text_size.y += line_height;
        line_width = 0.0f;
        continue;
      }
      if (c == '\r') {
        continue;
      }
    }
    if (ff->IndexXAdvance) {
      char_width = (int)c < ff->IndexXAdvance_Size ? ff->IndexXAdvance[c] : ff->FallbackXAdvance;
    } else {
      char_width = c < 128 ? HalfXAdvance : ff->FallbackXAdvance;
    }
    char_width *= scale;
    if (line_width + char_width >= (max_width)) {
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

int RanderStringFormat(const FontInfo* ff, const char* text, const char* text_end,
                        nextchar_f nextchar, get_glyph_f get_glyph, void* userdata, ImVec2 pos, const FRECT* pclip_rect,
                        float wrap_width, float scale, float angle)
{
  float x, y;
  float line_height = ff->size * scale;
  float rotate[6] = {0};
  bool word_wrap_enabled = (wrap_width > 0.0f);
  const char* word_wrap_eol = NULL;
  const char* s;
  FRECT clip_rect = pclip_rect ? *pclip_rect : fRECT2(pos.x, pos.y, 9999, 9999);
  float HalfXAdvance = ff->FallbackXAdvance/2;
  //StringAlignment halign = format ? format->align : StringAlignmentNear;
  if (!text_end) {
    text_end = text + strlen(text);
  }
  // Align to be pixel perfect
  pos.x = (float)(int)pos.x;
  pos.y = (float)(int)pos.y;
  x = pos.x;
  y = pos.y;
  if (y > clip_rect.b) {
    return 0;
  }
  M23Set(rotate);
  M23Rotate(rotate, angle, MatrixOrderPrepend);
  M23Scale(rotate, scale, scale, MatrixOrderPrepend);
  
  word_wrap_enabled = (wrap_width > 0.0f);
  // Skip non-visible lines
  s = text;
  if (!word_wrap_enabled && y + line_height < clip_rect.t) {
    while (s < text_end && *s != '\n') { // Fast-forward to next line
      s++;
    }
  }
  // Reserve vertices for remaining worse case (over-reserving is useful and easily amortized)
  while (s < text_end) {
    unsigned int c = 0;
    float char_width;
    if (word_wrap_enabled) {
      // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
      if (!word_wrap_eol) {
        word_wrap_eol = CalcWordWrapPosition(ff, s, text_end, nextchar, wrap_width - (x - pos.x), scale);
        if (word_wrap_eol == s) { // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
          word_wrap_eol++; // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
        }
      }
      if (s >= word_wrap_eol) {
        pos.x += line_height * rotate[2]/scale;
        pos.y += line_height * rotate[3]/scale;
        x = pos.x;
        y = pos.y;
        word_wrap_eol = NULL;
        // Wrapping skips upcoming blanks
        while (s < text_end) {
          c = 0;
          s = nextchar(&c, s, text_end);
          if (ImCharIsSpace(c)) {
          }
          else if (c == '\n') {
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
    c = 0;
    s = nextchar(&c, s, text_end);
    if (c == 0) {
      break;
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
    if (c != ' ' && c != '\t' && get_glyph) {
      rotate[4] = x;
      rotate[5] = y;
      get_glyph(c, rotate, userdata);
    }
    char_width = 0.0f;
    if (ff->IndexXAdvance) {
      char_width = (int)c < ff->IndexXAdvance_Size ? ff->IndexXAdvance[c] : ff->FallbackXAdvance;
    } else {
      char_width = c < 128 ? HalfXAdvance : ff->FallbackXAdvance;
    }
    x += char_width * rotate[0];
    y += char_width * rotate[1];
  }
  // Give back unused vertices
  return 0;
}
