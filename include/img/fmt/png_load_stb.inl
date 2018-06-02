
#include "zlib_stb.inl"

// public domain "baseline" PNG decoder   v0.10  Sean Barrett 2006-11-18
//    simple implementation
//      - only 8-bit samples
//      - no CRC checking
//      - allocates lots of intermediate memory
//        - avoids problem of streaming data between subsystems
//        - avoids explicit window management
//    performance
//      - uses stb_zlib, a PD zlib implementation with fast huffman decoding

typedef struct {
  uint32 length;
  uint32 type;
} stbi__pngchunk;

static stbi__pngchunk stbi__get_chunk_header(stbi__context* s)
{
  stbi__pngchunk c;
  c.length = stbi__get32be(s);
  c.type   = stbi__get32be(s);
  return c;
}

static int stbi__check_png_header(stbi__context* s)
{
  static uint8 png_sig[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
  int i;

  for (i = 0; i < 8; ++i)
    if (stbi__get8(s) != png_sig[i]) {
      return stbi__err("bad png sig", "Not a PNG");
    }

  return 1;
}

typedef struct {
  stbi__context* s;
  uint8* idata, *expanded, *out;
} stbi__png;


enum {
  STBI__F_none = 0,
  STBI__F_sub = 1,
  STBI__F_up = 2,
  STBI__F_avg = 3,
  STBI__F_paeth = 4,
  // synthetic filters used for first scanline to avoid needing a dummy row of 0s
  STBI__F_avg_first,
  STBI__F_paeth_first
};

static uint8 first_row_filter[5] = {
  STBI__F_none,
  STBI__F_sub,
  STBI__F_none,
  STBI__F_avg_first,
  STBI__F_paeth_first
};

static int stbi__paeth(int a, int b, int c)
{
  int p = a + b - c;
  int pa = abs(p - a);
  int pb = abs(p - b);
  int pc = abs(p - c);

  if (pa <= pb && pa <= pc) {
    return a;
  }

  if (pb <= pc) {
    return b;
  }

  return c;
}

static uint8 stbi__depth_scale_table[9] = { 0, 0xff, 0x55, 0, 0x11, 0, 0, 0, 0x01 };

// create the png data from post-deflated data
static int stbi__create_png_image_raw(stbi__png* a, uint8* raw, uint32 raw_len, int out_n, uint32 x, uint32 y, int depth, int color)
{
  stbi__context* s = a->s;
  uint32 i, j, stride = x * out_n;
  uint32 img_len, img_width_bytes;
  int k;
  int img_n = s->img_n; // copy it into a local for later

  STBI_ASSERT(out_n == s->img_n || out_n == s->img_n + 1);
  a->out = (uint8*) stbi__malloc(x * y * out_n);  // extra bytes to write off the end into

  if (!a->out) {
    return stbi__err("outofmem", "Out of memory");
  }

  img_width_bytes = (((img_n * x * depth) + 7) >> 3);
  img_len = (img_width_bytes + 1) * y;

  if (s->img_x == x && s->img_y == y) {
    if (raw_len != img_len) {
      return stbi__err("not enough pixels", "Corrupt PNG");
    }
  }
  else {   // interlaced:
    if (raw_len < img_len) {
      return stbi__err("not enough pixels", "Corrupt PNG");
    }
  }

  for (j = 0; j < y; ++j) {
    uint8* cur = a->out + stride * j;
    uint8* prior = cur - stride;
    int filter = *raw++;
    int filter_bytes = img_n;
    int width = x;

    if (filter > 4) {
      return stbi__err("invalid filter", "Corrupt PNG");
    }

    if (depth < 8) {
      STBI_ASSERT(img_width_bytes <= x);
      cur += x * out_n - img_width_bytes; // store output to the rightmost img_len bytes, so we can decode in place
      filter_bytes = 1;
      width = img_width_bytes;
    }

    // if first row, use special filter that doesn't sample previous row
    if (j == 0) {
      filter = first_row_filter[filter];
    }

    // handle first byte explicitly
    for (k = 0; k < filter_bytes; ++k) {
      switch (filter) {
      case STBI__F_none       :
        cur[k] = raw[k];
        break;

      case STBI__F_sub        :
        cur[k] = raw[k];
        break;

      case STBI__F_up         :
        cur[k] = STBI__BYTECAST(raw[k] + prior[k]);
        break;

      case STBI__F_avg        :
        cur[k] = STBI__BYTECAST(raw[k] + (prior[k] >> 1));
        break;

      case STBI__F_paeth      :
        cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(0, prior[k], 0));
        break;

      case STBI__F_avg_first  :
        cur[k] = raw[k];
        break;

      case STBI__F_paeth_first:
        cur[k] = raw[k];
        break;
      }
    }

    if (depth == 8) {
      if (img_n != out_n) {
        cur[img_n] = 255;  // first pixel
      }

      raw += img_n;
      cur += out_n;
      prior += out_n;
    }
    else {
      raw += 1;
      cur += 1;
      prior += 1;
    }

    // this is a little gross, so that we don't switch per-pixel or per-component
    if (depth < 8 || img_n == out_n) {
      int nk = (width - 1) * img_n;
#define CASE(f) \
case f:     \
  for (k=0; k < nk; ++k)

      switch (filter) {
        // "none" filter turns into a memcpy here; make that explicit.
      case STBI__F_none:
        memcpy(cur, raw, nk);
        break;
        CASE(STBI__F_sub)          cur[k] = STBI__BYTECAST(raw[k] + cur[k - filter_bytes]);
        break;
        CASE(STBI__F_up)           cur[k] = STBI__BYTECAST(raw[k] + prior[k]);
        break;
        CASE(STBI__F_avg)          cur[k] = STBI__BYTECAST(raw[k] + ((prior[k] + cur[k - filter_bytes]) >> 1));
        break;
        CASE(STBI__F_paeth)        cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k - filter_bytes], prior[k], prior[k - filter_bytes]));
        break;
        CASE(STBI__F_avg_first)    cur[k] = STBI__BYTECAST(raw[k] + (cur[k - filter_bytes] >> 1));
        break;
        CASE(STBI__F_paeth_first)  cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k - filter_bytes], 0, 0));
        break;
      }

#undef CASE
      raw += nk;
    }
    else {
      STBI_ASSERT(img_n + 1 == out_n);
#define CASE(f) \
case f:     \
  for (i=x-1; i >= 1; --i, cur[img_n]=255,raw+=img_n,cur+=out_n,prior+=out_n) \
    for (k=0; k < img_n; ++k)

      switch (filter) {
        CASE(STBI__F_none)         cur[k] = raw[k];
        break;
        CASE(STBI__F_sub)          cur[k] = STBI__BYTECAST(raw[k] + cur[k - out_n]);
        break;
        CASE(STBI__F_up)           cur[k] = STBI__BYTECAST(raw[k] + prior[k]);
        break;
        CASE(STBI__F_avg)          cur[k] = STBI__BYTECAST(raw[k] + ((prior[k] + cur[k - out_n]) >> 1));
        break;
        CASE(STBI__F_paeth)        cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k - out_n], prior[k], prior[k - out_n]));
        break;
        CASE(STBI__F_avg_first)    cur[k] = STBI__BYTECAST(raw[k] + (cur[k - out_n] >> 1));
        break;
        CASE(STBI__F_paeth_first)  cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k - out_n], 0, 0));
        break;
      }

#undef CASE
    }
  }

  // we make a separate pass to expand bits to pixels; for performance,
  // this could run two scanlines behind the above code, so it won't
  // intefere with filtering but will still be in the cache.
  if (depth < 8) {
    for (j = 0; j < y; ++j) {
      uint8* cur = a->out + stride * j;
      uint8* in  = a->out + stride * j + x * out_n - img_width_bytes;
      // unpack 1/2/4-bit into a 8-bit buffer. allows us to keep the common 8-bit path optimal at minimal cost for 1/2/4-bit
      // png guarante byte alignment, if width is not multiple of 8/4/2 we'll decode dummy trailing data that will be skipped in the later loop
      uint8 scale = (color == 0) ? stbi__depth_scale_table[depth] : 1; // scale grayscale values to 0..255 range

      // note that the final byte might overshoot and write more data than desired.
      // we can allocate enough data that this never writes out of memory, but it
      // could also overwrite the next scanline. can it overwrite non-empty data
      // on the next scanline? yes, consider 1-pixel-wide scanlines with 1-bit-per-pixel.
      // so we need to explicitly clamp the final ones

      if (depth == 4) {
        for (k = x * img_n; k >= 2; k -= 2, ++in) {
          *cur++ = scale * ((*in >> 4));
          *cur++ = scale * ((*in) & 0x0f);
        }

        if (k > 0) {
          *cur++ = scale * ((*in >> 4));
        }
      }
      else if (depth == 2) {
        for (k = x * img_n; k >= 4; k -= 4, ++in) {
          *cur++ = scale * ((*in >> 6));
          *cur++ = scale * ((*in >> 4) & 0x03);
          *cur++ = scale * ((*in >> 2) & 0x03);
          *cur++ = scale * ((*in) & 0x03);
        }

        if (k > 0) {
          *cur++ = scale * ((*in >> 6));
        }

        if (k > 1) {
          *cur++ = scale * ((*in >> 4) & 0x03);
        }

        if (k > 2) {
          *cur++ = scale * ((*in >> 2) & 0x03);
        }
      }
      else if (depth == 1) {
        for (k = x * img_n; k >= 8; k -= 8, ++in) {
          *cur++ = scale * ((*in >> 7));
          *cur++ = scale * ((*in >> 6) & 0x01);
          *cur++ = scale * ((*in >> 5) & 0x01);
          *cur++ = scale * ((*in >> 4) & 0x01);
          *cur++ = scale * ((*in >> 3) & 0x01);
          *cur++ = scale * ((*in >> 2) & 0x01);
          *cur++ = scale * ((*in >> 1) & 0x01);
          *cur++ = scale * ((*in) & 0x01);
        }

        if (k > 0) {
          *cur++ = scale * ((*in >> 7));
        }

        if (k > 1) {
          *cur++ = scale * ((*in >> 6) & 0x01);
        }

        if (k > 2) {
          *cur++ = scale * ((*in >> 5) & 0x01);
        }

        if (k > 3) {
          *cur++ = scale * ((*in >> 4) & 0x01);
        }

        if (k > 4) {
          *cur++ = scale * ((*in >> 3) & 0x01);
        }

        if (k > 5) {
          *cur++ = scale * ((*in >> 2) & 0x01);
        }

        if (k > 6) {
          *cur++ = scale * ((*in >> 1) & 0x01);
        }
      }

      if (img_n != out_n) {
        // insert alpha = 255
        uint8* cur = a->out + stride * j;
        int i;

        if (img_n == 1) {
          for (i = x - 1; i >= 0; --i) {
            cur[i * 2 + 1] = 255;
            cur[i * 2 + 0] = cur[i];
          }
        }
        else {
          STBI_ASSERT(img_n == 3);

          for (i = x - 1; i >= 0; --i) {
            cur[i * 4 + 3] = 255;
            cur[i * 4 + 2] = cur[i * 3 + 2];
            cur[i * 4 + 1] = cur[i * 3 + 1];
            cur[i * 4 + 0] = cur[i * 3 + 0];
          }
        }
      }
    }
  }

  return 1;
}

static int stbi__create_png_image(stbi__png* a, uint8* image_data, uint32 image_data_len, int out_n, int depth, int color, int interlaced)
{
  uint8* final;
  int p;

  if (!interlaced) {
    return stbi__create_png_image_raw(a, image_data, image_data_len, out_n, a->s->img_x, a->s->img_y, depth, color);
  }

  // de-interlacing
  final = (uint8*) stbi__malloc(a->s->img_x * a->s->img_y * out_n);

  for (p = 0; p < 7; ++p) {
    int xorig[] = { 0, 4, 0, 2, 0, 1, 0 };
    int yorig[] = { 0, 0, 4, 0, 2, 0, 1 };
    int xspc[]  = { 8, 8, 4, 4, 2, 2, 1 };
    int yspc[]  = { 8, 8, 8, 4, 4, 2, 2 };
    int i, j, x, y;
    // pass1_x[4] = 0, pass1_x[5] = 1, pass1_x[12] = 1
    x = (a->s->img_x - xorig[p] + xspc[p] - 1) / xspc[p];
    y = (a->s->img_y - yorig[p] + yspc[p] - 1) / yspc[p];

    if (x && y) {
      uint32 img_len = ((((a->s->img_n * x * depth) + 7) >> 3) + 1) * y;

      if (!stbi__create_png_image_raw(a, image_data, image_data_len, out_n, x, y, depth, color)) {
        STBI_FREE(final);
        return 0;
      }

      for (j = 0; j < y; ++j) {
        for (i = 0; i < x; ++i) {
          int out_y = j * yspc[p] + yorig[p];
          int out_x = i * xspc[p] + xorig[p];
          memcpy(final + out_y * a->s->img_x * out_n + out_x * out_n,
              a->out + (j * x + i)*out_n, out_n);
        }
      }

      STBI_FREE(a->out);
      image_data += img_len;
      image_data_len -= img_len;
    }
  }

  a->out = final;

  return 1;
}

static int stbi__compute_transparency(stbi__png* z, uint8 tc[3], int out_n)
{
  stbi__context* s = z->s;
  uint32 i, pixel_count = s->img_x * s->img_y;
  uint8* p = z->out;

  // compute color-based transparency, assuming we've
  // already got 255 as the alpha value in the output
  STBI_ASSERT(out_n == 2 || out_n == 4);

  if (out_n == 2) {
    for (i = 0; i < pixel_count; ++i) {
      p[1] = (p[0] == tc[0] ? 0 : 255);
      p += 2;
    }
  }
  else {
    for (i = 0; i < pixel_count; ++i) {
      if (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2]) {
        p[3] = 0;
      }

      p += 4;
    }
  }

  return 1;
}

static int stbi__expand_png_palette(stbi__png* a, uint8* palette, int len, int pal_img_n)
{
  uint32 i, pixel_count = a->s->img_x * a->s->img_y;
  uint8* p, *temp_out, *orig = a->out;

  p = (uint8*) stbi__malloc(pixel_count * pal_img_n);

  if (p == NULL) {
    return stbi__err("outofmem", "Out of memory");
  }

  // between here and pfree(out) below, exitting would leak
  temp_out = p;

  if (pal_img_n == 3) {
    for (i = 0; i < pixel_count; ++i) {
      int n = orig[i] * 4;
      p[0] = palette[n  ];
      p[1] = palette[n + 1];
      p[2] = palette[n + 2];
      p += 3;
    }
  }
  else {
    for (i = 0; i < pixel_count; ++i) {
      int n = orig[i] * 4;
      p[0] = palette[n  ];
      p[1] = palette[n + 1];
      p[2] = palette[n + 2];
      p[3] = palette[n + 3];
      p += 4;
    }
  }

  STBI_FREE(a->out);
  a->out = temp_out;

  STBI_NOTUSED(len);

  return 1;
}

static int stbi__unpremultiply_on_load = 0;
static int stbi__de_iphone_flag = 0;

STBIDEF void stbi_set_unpremultiply_on_load(int flag_true_if_should_unpremultiply)
{
  stbi__unpremultiply_on_load = flag_true_if_should_unpremultiply;
}

STBIDEF void stbi_convert_iphone_png_to_rgb(int flag_true_if_should_convert)
{
  stbi__de_iphone_flag = flag_true_if_should_convert;
}

static void stbi__de_iphone(stbi__png* z)
{
  stbi__context* s = z->s;
  uint32 i, pixel_count = s->img_x * s->img_y;
  uint8* p = z->out;

  if (s->img_out_n == 3) {  // convert bgr to rgb
    for (i = 0; i < pixel_count; ++i) {
      uint8 t = p[0];
      p[0] = p[2];
      p[2] = t;
      p += 3;
    }
  }
  else {
    STBI_ASSERT(s->img_out_n == 4);

    if (stbi__unpremultiply_on_load) {
      // convert bgr to rgb and unpremultiply
      for (i = 0; i < pixel_count; ++i) {
        uint8 a = p[3];
        uint8 t = p[0];

        if (a) {
          p[0] = p[2] * 255 / a;
          p[1] = p[1] * 255 / a;
          p[2] =  t   * 255 / a;
        }
        else {
          p[0] = p[2];
          p[2] = t;
        }

        p += 4;
      }
    }
    else {
      // convert bgr to rgb
      for (i = 0; i < pixel_count; ++i) {
        uint8 t = p[0];
        p[0] = p[2];
        p[2] = t;
        p += 4;
      }
    }
  }
}

#define STBI__PNG_TYPE(a,b,c,d)  (((a) << 24) + ((b) << 16) + ((c) << 8) + (d))

static int stbi__parse_png_file(stbi__png* z, int scan, int req_comp)
{
  uint8 palette[1024], pal_img_n = 0;
  uint8 has_trans = 0, tc[3];
  uint32 ioff = 0, idata_limit = 0, i, pal_len = 0;
  int first = 1, k, interlace = 0, color = 0, depth = 0, is_iphone = 0;
  stbi__context* s = z->s;

  z->expanded = NULL;
  z->idata = NULL;
  z->out = NULL;

  if (!stbi__check_png_header(s)) {
    return 0;
  }

  if (scan == STBI__SCAN_type) {
    return 1;
  }

  for (;;) {
    stbi__pngchunk c = stbi__get_chunk_header(s);

    switch (c.type) {
    case STBI__PNG_TYPE('C', 'g', 'B', 'I'):
      is_iphone = 1;
      stbi__skip(s, c.length);
      break;

    case STBI__PNG_TYPE('I', 'H', 'D', 'R'): {
      int comp, filter;

      if (!first) {
        return stbi__err("multiple IHDR", "Corrupt PNG");
      }

      first = 0;

      if (c.length != 13) {
        return stbi__err("bad IHDR len", "Corrupt PNG");
      }

      s->img_x = stbi__get32be(s);

      if (s->img_x > (1 << 24)) {
        return stbi__err("too large", "Very large image (corrupt?)");
      }

      s->img_y = stbi__get32be(s);

      if (s->img_y > (1 << 24)) {
        return stbi__err("too large", "Very large image (corrupt?)");
      }

      depth = stbi__get8(s);

      if (depth != 1 && depth != 2 && depth != 4 && depth != 8) {
        return stbi__err("1/2/4/8-bit only", "PNG not supported: 1/2/4/8-bit only");
      }

      color = stbi__get8(s);

      if (color > 6) {
        return stbi__err("bad ctype", "Corrupt PNG");
      }

      if (color == 3) {
        pal_img_n = 3;
      }
      else if (color & 1) {
        return stbi__err("bad ctype", "Corrupt PNG");
      }

      comp  = stbi__get8(s);

      if (comp) {
        return stbi__err("bad comp method", "Corrupt PNG");
      }

      filter = stbi__get8(s);

      if (filter) {
        return stbi__err("bad filter method", "Corrupt PNG");
      }

      interlace = stbi__get8(s);

      if (interlace > 1) {
        return stbi__err("bad interlace method", "Corrupt PNG");
      }

      if (!s->img_x || !s->img_y) {
        return stbi__err("0-pixel image", "Corrupt PNG");
      }

      if (!pal_img_n) {
        s->img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);

        if ((1 << 30) / s->img_x / s->img_n < s->img_y) {
          return stbi__err("too large", "Image too large to decode");
        }

        if (scan == STBI__SCAN_header) {
          return 1;
        }
      }
      else {
        // if paletted, then pal_n is our final components, and
        // img_n is # components to decompress/filter.
        s->img_n = 1;

        if ((1 << 30) / s->img_x / 4 < s->img_y) {
          return stbi__err("too large", "Corrupt PNG");
        }

        // if SCAN_header, have to scan to see if we have a tRNS
      }

      break;
    }

    case STBI__PNG_TYPE('P', 'L', 'T', 'E'):  {
      if (first) {
        return stbi__err("first not IHDR", "Corrupt PNG");
      }

      if (c.length > 256 * 3) {
        return stbi__err("invalid PLTE", "Corrupt PNG");
      }

      pal_len = c.length / 3;

      if (pal_len * 3 != c.length) {
        return stbi__err("invalid PLTE", "Corrupt PNG");
      }

      for (i = 0; i < pal_len; ++i) {
        palette[i * 4 + 0] = stbi__get8(s);
        palette[i * 4 + 1] = stbi__get8(s);
        palette[i * 4 + 2] = stbi__get8(s);
        palette[i * 4 + 3] = 255;
      }

      break;
    }

    case STBI__PNG_TYPE('t', 'R', 'N', 'S'): {
      if (first) {
        return stbi__err("first not IHDR", "Corrupt PNG");
      }

      if (z->idata) {
        return stbi__err("tRNS after IDAT", "Corrupt PNG");
      }

      if (pal_img_n) {
        if (scan == STBI__SCAN_header) {
          s->img_n = 4;
          return 1;
        }

        if (pal_len == 0) {
          return stbi__err("tRNS before PLTE", "Corrupt PNG");
        }

        if (c.length > pal_len) {
          return stbi__err("bad tRNS len", "Corrupt PNG");
        }

        pal_img_n = 4;

        for (i = 0; i < c.length; ++i) {
          palette[i * 4 + 3] = stbi__get8(s);
        }
      }
      else {
        if (!(s->img_n & 1)) {
          return stbi__err("tRNS with alpha", "Corrupt PNG");
        }

        if (c.length != (uint32) s->img_n * 2) {
          return stbi__err("bad tRNS len", "Corrupt PNG");
        }

        has_trans = 1;

        for (k = 0; k < s->img_n; ++k) {
          tc[k] = (uint8)(stbi__get16be(s) & 255) * stbi__depth_scale_table[depth];  // non 8-bit images will be larger
        }
      }

      break;
    }

    case STBI__PNG_TYPE('I', 'D', 'A', 'T'): {
      if (first) {
        return stbi__err("first not IHDR", "Corrupt PNG");
      }

      if (pal_img_n && !pal_len) {
        return stbi__err("no PLTE", "Corrupt PNG");
      }

      if (scan == STBI__SCAN_header) {
        s->img_n = pal_img_n;
        return 1;
      }

      if ((int)(ioff + c.length) < (int)ioff) {
        return 0;
      }

      if (ioff + c.length > idata_limit) {
        uint8* p;

        if (idata_limit == 0) {
          idata_limit = c.length > 4096 ? c.length : 4096;
        }

        while (ioff + c.length > idata_limit) {
          idata_limit *= 2;
        }

        p = (uint8*) STBI_REALLOC(z->idata, idata_limit);

        if (p == NULL) {
          return stbi__err("outofmem", "Out of memory");
        }

        z->idata = p;
      }

      if (!stbi__getn(s, z->idata + ioff, c.length)) {
        return stbi__err("outofdata", "Corrupt PNG");
      }

      ioff += c.length;
      break;
    }

    case STBI__PNG_TYPE('I', 'E', 'N', 'D'): {
      uint32 raw_len, bpl;

      if (first) {
        return stbi__err("first not IHDR", "Corrupt PNG");
      }

      if (scan != STBI__SCAN_load) {
        return 1;
      }

      if (z->idata == NULL) {
        return stbi__err("no IDAT", "Corrupt PNG");
      }

      // initial guess for decoded data size to avoid unnecessary reallocs
      bpl = (s->img_x * depth + 7) / 8; // bytes per line, per component
      raw_len = bpl * s->img_y * s->img_n /* pixels */ + s->img_y /* filter mode per row */;
      z->expanded = (uint8*) stbi_zlib_decode_malloc_guesssize_headerflag((char*) z->idata, ioff, raw_len, (int*) &raw_len, !is_iphone);

      if (z->expanded == NULL) {
        return 0;  // zlib should set error
      }

      STBI_FREE(z->idata);
      z->idata = NULL;

      if ((req_comp == s->img_n + 1 && req_comp != 3 && !pal_img_n) || has_trans) {
        s->img_out_n = s->img_n + 1;
      }
      else {
        s->img_out_n = s->img_n;
      }

      if (!stbi__create_png_image(z, z->expanded, raw_len, s->img_out_n, depth, color, interlace)) {
        return 0;
      }

      if (has_trans)
        if (!stbi__compute_transparency(z, tc, s->img_out_n)) {
          return 0;
        }

      if (is_iphone && stbi__de_iphone_flag && s->img_out_n > 2) {
        stbi__de_iphone(z);
      }

      if (pal_img_n) {
        // pal_img_n == 3 or 4
        s->img_n = pal_img_n; // record the actual colors we had
        s->img_out_n = pal_img_n;

        if (req_comp >= 3) {
          s->img_out_n = req_comp;
        }

        if (!stbi__expand_png_palette(z, palette, pal_len, s->img_out_n)) {
          return 0;
        }
      }

      STBI_FREE(z->expanded);
      z->expanded = NULL;
      return 1;
    }

    default:

      // if critical, fail
      if (first) {
        return stbi__err("first not IHDR", "Corrupt PNG");
      }

      if ((c.type & (1 << 29)) == 0) {
#ifndef STBI_NO_FAILURE_STRINGS
        // not threadsafe
        static char invalid_chunk[] = "XXXX PNG chunk not known";
        invalid_chunk[0] = STBI__BYTECAST(c.type >> 24);
        invalid_chunk[1] = STBI__BYTECAST(c.type >> 16);
        invalid_chunk[2] = STBI__BYTECAST(c.type >>  8);
        invalid_chunk[3] = STBI__BYTECAST(c.type >>  0);
#endif
        return stbi__err(invalid_chunk, "PNG not supported: unknown PNG chunk type");
      }

      stbi__skip(s, c.length);
      break;
    }

    // end of PNG chunk, read and skip CRC
    stbi__get32be(s);
  }

  return 0;
}

static int stbi__png_test(stbi__context* s)
{
  int r;
  r = stbi__check_png_header(s);
  stbi__rewind(s);
  return r;
}

static int stbi__png_info(stbi__png* p, int* x, int* y, int* comp)
{
  if (!stbi__parse_png_file(p, STBI__SCAN_header, 0)) {
    stbi__rewind(p->s);
    return 0;
  }

  if (x) {
    *x = p->s->img_x;
  }

  if (y) {
    *y = p->s->img_y;
  }

  if (comp) {
    *comp = p->s->img_n;
  }

  return 1;
}


static int load_png_image(stbi__png* p, img_t* im, int req_comp)
{
  unsigned char* result = NULL;

  if (req_comp < 0 || req_comp > 4) {
    return stbi__errpuc("bad req_comp", "Internal error");
  }

  if (stbi__parse_png_file(p, STBI__SCAN_load, req_comp)) {
    result = p->out;
    p->out = NULL;
    req_comp = req_comp ? req_comp : p->s->img_out_n;
    imsetsize(im, p->s->img_y, p->s->img_x, req_comp, 1);

    if (p->s->img_out_n>=3) {
      swap_rb(result, p->s->img_out_n * p->s->img_x, p->s->img_out_n, p->s->img_x, p->s->img_y);
    }
    stbi__convert_format(result, p->s->img_out_n, req_comp, p->s->img_x, p->s->img_y, im->tt.data, im->s, 1);
  }

  STBI_FREE(p->out);
  p->out      = NULL;
  STBI_FREE(p->expanded);
  p->expanded = NULL;
  STBI_FREE(p->idata);
  p->idata    = NULL;
  return result!=NULL;
}

static int png_load(stream_t* f, int req_comp, img_t* im)
{
  stbi__context s = {0};
  stbi__png p[1] = {0};
  int ret;
  stream_seek(f, 0, SEEK_SET);
  stbi__start_callbacks(&s, &stbi__stdio_callbacks, (void*) f);
  p->s = &s;
  ret = load_png_image(p, im, req_comp);
  return ret;
}

#define png_load_filename(fname, cn_req, im) png_load_file_close(fopen(fname, "rb"), cn_req, im)
static int png_load_file_close(FILE* pf, int cn_req, img_t* im)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = png_load(s, cn_req, im);
    fclose(pf);
  }

  return ret;
}
