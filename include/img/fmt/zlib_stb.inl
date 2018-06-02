
#ifndef _ZLIB_STB_INL_
#define _ZLIB_STB_INL_

//////////////////////////////////////////////////////////////////////////////
//
//  generic converter from built-in img_n to req_comp
//    individual types do this automatically as much as possible (e.g. jpeg
//    does all cases internally since it needs to colorspace convert anyway,
//    and it never has alpha, so very few cases ). png can automatically
//    interleave an alpha=255 channel, but falls back to this for other cases
//
//  assume data buffer is malloced, so pmalloc a new one and pfree that one
//  only failure mode is pmalloc failing

static uint8 stbi__compute_y(int r, int g, int b)
{
  return (uint8)(((r * 77) + (g * 150) + (29 * b)) >> 8);
}

static unsigned char* stbi__convert_format(unsigned char* data, int img_n, int req_comp, unsigned int x, unsigned int y, unsigned char* good, int step, int bswaprb)
{
  int i, j;
  if (req_comp == img_n) {
    memcpy2d(good, step, data, x * img_n, y, x*img_n);
    return data;
  }

  STBI_ASSERT(req_comp >= 0 && req_comp <= 4);

  if (good == NULL) {
    STBI_FREE(data);
    return stbi__errpuc("outofmem", "Out of memory");
  }

  for (j = 0; j < (int) y; ++j) {
    unsigned char* src  = data + j * x * img_n;
    unsigned char* dest = good + j * step;

#define COMBO(a,b)  ((a)*8+(b))
#define CASE(a,b)   case COMBO(a,b): for(i=x-1; i >= 0; --i, src += a, dest += b)

    // convert source image with img_n components to one with req_comp components;
    // avoid switch per pixel, so use switch per scanline and massive macros
    switch (COMBO(img_n, req_comp)) {
      CASE(1, 2) dest[0] = src[0], dest[1] = 255;
      break;
      CASE(1, 3) dest[0] = dest[1] = dest[2] = src[0];
      break;
      CASE(1, 4) dest[0] = dest[1] = dest[2] = src[0], dest[3] = 255;
      break;
      CASE(2, 1) dest[0] = src[0];
      break;
      CASE(2, 3) dest[0] = dest[1] = dest[2] = src[0];
      break;
      CASE(2, 4) dest[0] = dest[1] = dest[2] = src[0], dest[3] = src[1];
      break;
      CASE(3, 4) dest[0] = src[0], dest[1] = src[1], dest[2] = src[2], dest[3] = 255;
      break;
      CASE(3, 1) dest[0] = stbi__compute_y(src[0], src[1], src[2]);
      break;
      CASE(3, 2) dest[0] = stbi__compute_y(src[0], src[1], src[2]), dest[1] = 255;
      break;
      CASE(4, 1) dest[0] = stbi__compute_y(src[0], src[1], src[2]);
      break;
      CASE(4, 2) dest[0] = stbi__compute_y(src[0], src[1], src[2]), dest[1] = src[3];
      break;
      CASE(4, 3) dest[0] = src[0], dest[1] = src[1], dest[2] = src[2];
      break;

    default:
      STBI_ASSERT(0);
    }

#undef CASE
#undef COMBO
  }

  STBI_FREE(data);
  return good;
}


// fast-way is faster to check than jpeg huffman, but slow way is slower
#define STBI__ZFAST_BITS  9 // accelerate all cases in default tables
#define STBI__ZFAST_MASK  ((1 << STBI__ZFAST_BITS) - 1)

// zlib-style huffman encoding
// (jpegs packs from left, zlib from right, so can't share code)
typedef struct {
  uint16 fast[1 << STBI__ZFAST_BITS];
  uint16 firstcode[16];
  int maxcode[17];
  uint16 firstsymbol[16];
  uint8  size[288];
  uint16 value[288];
} stbi__zhuffman;

stbi_inline static int stbi__bitreverse16(int n)
{
  n = ((n & 0xAAAA) >>  1) | ((n & 0x5555) << 1);
  n = ((n & 0xCCCC) >>  2) | ((n & 0x3333) << 2);
  n = ((n & 0xF0F0) >>  4) | ((n & 0x0F0F) << 4);
  n = ((n & 0xFF00) >>  8) | ((n & 0x00FF) << 8);
  return n;
}

stbi_inline static int stbi__bit_reverse(int v, int bits)
{
  STBI_ASSERT(bits <= 16);
  // to bit reverse n bits, reverse 16 and shift
  // e.g. 11 bits, bit reverse and shift away 5
  return stbi__bitreverse16(v) >> (16 - bits);
}

static int stbi__zbuild_huffman(stbi__zhuffman* z, uint8* sizelist, int num)
{
  int i, k = 0;
  int code, next_code[16], sizes[17];

  // DEFLATE spec for generating codes
  memset(sizes, 0, sizeof(sizes));
  memset(z->fast, 0, sizeof(z->fast));

  for (i = 0; i < num; ++i) {
    ++sizes[sizelist[i]];
  }

  sizes[0] = 0;

  for (i = 1; i < 16; ++i)
    if (sizes[i] > (1 << i)) {
      return stbi__err("bad sizes", "Corrupt PNG");
    }

  code = 0;

  for (i = 1; i < 16; ++i) {
    next_code[i] = code;
    z->firstcode[i] = (uint16) code;
    z->firstsymbol[i] = (uint16) k;
    code = (code + sizes[i]);

    if (sizes[i])
      if (code - 1 >= (1 << i)) {
        return stbi__err("bad codelengths", "Corrupt PNG");
      }

    z->maxcode[i] = code << (16 - i); // preshift for inner loop
    code <<= 1;
    k += sizes[i];
  }

  z->maxcode[16] = 0x10000; // sentinel

  for (i = 0; i < num; ++i) {
    int s = sizelist[i];

    if (s) {
      int c = next_code[s] - z->firstcode[s] + z->firstsymbol[s];
      uint16 fastv = (uint16)((s << 9) | i);
      z->size [c] = (uint8) s;
      z->value[c] = (uint16) i;

      if (s <= STBI__ZFAST_BITS) {
        int k = stbi__bit_reverse(next_code[s], s);

        while (k < (1 << STBI__ZFAST_BITS)) {
          z->fast[k] = fastv;
          k += (1 << s);
        }
      }

      ++next_code[s];
    }
  }

  return 1;
}

// zlib-from-memory implementation for PNG reading
//    because PNG allows splitting the zlib stream arbitrarily,
//    and it's annoying structurally to have PNG call ZLIB call PNG,
//    we require PNG read all the IDATs and combine them into a single
//    memory buffer

typedef struct {
  uint8* zbuffer, *zbuffer_end;
  int num_bits;
  uint32 code_buffer;

  char* zout;
  char* zout_start;
  char* zout_end;
  int   z_expandable;

  stbi__zhuffman z_length, z_distance;
} stbi__zbuf;

stbi_inline static uint8 stbi__zget8(stbi__zbuf* z)
{
  if (z->zbuffer >= z->zbuffer_end) {
    return 0;
  }

  return *z->zbuffer++;
}

static void stbi__fill_bits(stbi__zbuf* z)
{
  do {
    STBI_ASSERT(z->code_buffer < (1U << z->num_bits));
    z->code_buffer |= stbi__zget8(z) << z->num_bits;
    z->num_bits += 8;
  }
  while (z->num_bits <= 24);
}

stbi_inline static unsigned int stbi__zreceive(stbi__zbuf* z, int n)
{
  unsigned int k;

  if (z->num_bits < n) {
    stbi__fill_bits(z);
  }

  k = z->code_buffer & ((1 << n) - 1);
  z->code_buffer >>= n;
  z->num_bits -= n;
  return k;
}

static int stbi__zhuffman_decode_slowpath(stbi__zbuf* a, stbi__zhuffman* z)
{
  int b, s, k;
  // not resolved by fast table, so compute it the slow way
  // use jpeg approach, which requires MSbits at top
  k = stbi__bit_reverse(a->code_buffer, 16);

  for (s = STBI__ZFAST_BITS + 1; ; ++s)
    if (k < z->maxcode[s]) {
      break;
    }

  if (s == 16) {
    return -1;  // invalid code!
  }

  // code size is s, so:
  b = (k >> (16 - s)) - z->firstcode[s] + z->firstsymbol[s];
  STBI_ASSERT(z->size[b] == s);
  a->code_buffer >>= s;
  a->num_bits -= s;
  return z->value[b];
}

stbi_inline static int stbi__zhuffman_decode(stbi__zbuf* a, stbi__zhuffman* z)
{
  int b, s;

  if (a->num_bits < 16) {
    stbi__fill_bits(a);
  }

  b = z->fast[a->code_buffer & STBI__ZFAST_MASK];

  if (b) {
    s = b >> 9;
    a->code_buffer >>= s;
    a->num_bits -= s;
    return b & 511;
  }

  return stbi__zhuffman_decode_slowpath(a, z);
}

static int stbi__zexpand(stbi__zbuf* z, char* zout, int n)  // need to make room for n bytes
{
  char* q;
  int cur, limit;
  z->zout = zout;

  if (!z->z_expandable) {
    return stbi__err("output buffer limit", "Corrupt PNG");
  }

  cur   = (int)(z->zout     - z->zout_start);
  limit = (int)(z->zout_end - z->zout_start);

  while (cur + n > limit) {
    limit *= 2;
  }

  q = (char*) STBI_REALLOC(z->zout_start, limit);

  if (q == NULL) {
    return stbi__err("outofmem", "Out of memory");
  }

  z->zout_start = q;
  z->zout       = q + cur;
  z->zout_end   = q + limit;
  return 1;
}

static int stbi__zlength_base[31] = {
  3, 4, 5, 6, 7, 8, 9, 10, 11, 13,
  15, 17, 19, 23, 27, 31, 35, 43, 51, 59,
  67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0
};

static int stbi__zlength_extra[31] =
{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 0, 0 };

static int stbi__zdist_base[32] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
    257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577, 0, 0
                                  };

static int stbi__zdist_extra[32] =
{ 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};

static int stbi__parse_huffman_block(stbi__zbuf* a)
{
  char* zout = a->zout;

  for (;;) {
    int z = stbi__zhuffman_decode(a, &a->z_length);

    if (z < 256) {
      if (z < 0) {
        return stbi__err("bad huffman code", "Corrupt PNG");  // error in huffman codes
      }

      if (zout >= a->zout_end) {
        if (!stbi__zexpand(a, zout, 1)) {
          return 0;
        }

        zout = a->zout;
      }

      *zout++ = (char) z;
    }
    else {
      uint8* p;
      int len, dist;

      if (z == 256) {
        a->zout = zout;
        return 1;
      }

      z -= 257;
      len = stbi__zlength_base[z];

      if (stbi__zlength_extra[z]) {
        len += stbi__zreceive(a, stbi__zlength_extra[z]);
      }

      z = stbi__zhuffman_decode(a, &a->z_distance);

      if (z < 0) {
        return stbi__err("bad huffman code", "Corrupt PNG");
      }

      dist = stbi__zdist_base[z];

      if (stbi__zdist_extra[z]) {
        dist += stbi__zreceive(a, stbi__zdist_extra[z]);
      }

      if (zout - a->zout_start < dist) {
        return stbi__err("bad dist", "Corrupt PNG");
      }

      if (zout + len > a->zout_end) {
        if (!stbi__zexpand(a, zout, len)) {
          return 0;
        }

        zout = a->zout;
      }

      p = (uint8*)(zout - dist);

      if (dist == 1) { // run of one byte; common in images.
        uint8 v = *p;

        if (len) {
          do {
            *zout++ = v;
          }
          while (--len);
        }
      }
      else {
        if (len) {
          do {
            *zout++ = *p++;
          }
          while (--len);
        }
      }
    }
  }
}

static int stbi__compute_huffman_codes(stbi__zbuf* a)
{
  static uint8 length_dezigzag[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
  stbi__zhuffman z_codelength;
  uint8 lencodes[286 + 32 + 137]; //padding for maximum single op
  uint8 codelength_sizes[19];
  int i, n;

  int hlit  = stbi__zreceive(a, 5) + 257;
  int hdist = stbi__zreceive(a, 5) + 1;
  int hclen = stbi__zreceive(a, 4) + 4;

  memset(codelength_sizes, 0, sizeof(codelength_sizes));

  for (i = 0; i < hclen; ++i) {
    int s = stbi__zreceive(a, 3);
    codelength_sizes[length_dezigzag[i]] = (uint8) s;
  }

  if (!stbi__zbuild_huffman(&z_codelength, codelength_sizes, 19)) {
    return 0;
  }

  n = 0;

  while (n < hlit + hdist) {
    int c = stbi__zhuffman_decode(a, &z_codelength);

    if (c < 0 || c >= 19) {
      return stbi__err("bad codelengths", "Corrupt PNG");
    }

    if (c < 16) {
      lencodes[n++] = (uint8) c;
    }
    else if (c == 16) {
      c = stbi__zreceive(a, 2) + 3;
      memset(lencodes + n, lencodes[n - 1], c);
      n += c;
    }
    else if (c == 17) {
      c = stbi__zreceive(a, 3) + 3;
      memset(lencodes + n, 0, c);
      n += c;
    }
    else {
      STBI_ASSERT(c == 18);
      c = stbi__zreceive(a, 7) + 11;
      memset(lencodes + n, 0, c);
      n += c;
    }
  }

  if (n != hlit + hdist) {
    return stbi__err("bad codelengths", "Corrupt PNG");
  }

  if (!stbi__zbuild_huffman(&a->z_length, lencodes, hlit)) {
    return 0;
  }

  if (!stbi__zbuild_huffman(&a->z_distance, lencodes + hlit, hdist)) {
    return 0;
  }

  return 1;
}

static int stbi__parse_uncomperssed_block(stbi__zbuf* a)
{
  uint8 header[4];
  int len, nlen, k;

  if (a->num_bits & 7) {
    stbi__zreceive(a, a->num_bits & 7);  // discard
  }

  // drain the bit-packed data into header
  k = 0;

  while (a->num_bits > 0) {
    header[k++] = (uint8)(a->code_buffer & 255);  // suppress MSVC run-time check
    a->code_buffer >>= 8;
    a->num_bits -= 8;
  }

  STBI_ASSERT(a->num_bits == 0);

  // now fill header the normal way
  while (k < 4) {
    header[k++] = stbi__zget8(a);
  }

  len  = header[1] * 256 + header[0];
  nlen = header[3] * 256 + header[2];

  if (nlen != (len ^ 0xffff)) {
    return stbi__err("zlib corrupt", "Corrupt PNG");
  }

  if (a->zbuffer + len > a->zbuffer_end) {
    return stbi__err("read past buffer", "Corrupt PNG");
  }

  if (a->zout + len > a->zout_end)
    if (!stbi__zexpand(a, a->zout, len)) {
      return 0;
    }

  memcpy(a->zout, a->zbuffer, len);
  a->zbuffer += len;
  a->zout += len;
  return 1;
}

static int stbi__parse_zlib_header(stbi__zbuf* a)
{
  int cmf   = stbi__zget8(a);
  int cm    = cmf & 15;
  /* int cinfo = cmf >> 4; */
  int flg   = stbi__zget8(a);

  if ((cmf * 256 + flg) % 31 != 0) {
    return stbi__err("bad zlib header", "Corrupt PNG");  // zlib spec
  }

  if (flg & 32) {
    return stbi__err("no preset dict", "Corrupt PNG");  // preset dictionary not allowed in png
  }

  if (cm != 8) {
    return stbi__err("bad compression", "Corrupt PNG");  // DEFLATE required for png
  }

  // window = 1 << (8 + cinfo)... but who cares, we fully buffer output
  return 1;
}

// @TODO: should statically initialize these for optimal thread safety
static uint8 stbi__zdefault_length[288], stbi__zdefault_distance[32];
static void stbi__init_zdefaults(void)
{
  int i;   // use <= to match clearly with spec

  for (i = 0; i <= 143; ++i) {
    stbi__zdefault_length[i]   = 8;
  }

  for (; i <= 255; ++i) {
    stbi__zdefault_length[i]   = 9;
  }

  for (; i <= 279; ++i) {
    stbi__zdefault_length[i]   = 7;
  }

  for (; i <= 287; ++i) {
    stbi__zdefault_length[i]   = 8;
  }

  for (i = 0; i <=  31; ++i) {
    stbi__zdefault_distance[i] = 5;
  }
}

static int stbi__parse_zlib(stbi__zbuf* a, int parse_header)
{
  int final, type;

  if (parse_header)
    if (!stbi__parse_zlib_header(a)) {
      return 0;
    }

  a->num_bits = 0;
  a->code_buffer = 0;

  do {
    final = stbi__zreceive(a, 1);
    type = stbi__zreceive(a, 2);

    if (type == 0) {
      if (!stbi__parse_uncomperssed_block(a)) {
        return 0;
      }
    }
    else if (type == 3) {
      return 0;
    }
    else {
      if (type == 1) {
        // use fixed code lengths
        if (!stbi__zdefault_distance[31]) {
          stbi__init_zdefaults();
        }

        if (!stbi__zbuild_huffman(&a->z_length  , stbi__zdefault_length  , 288)) {
          return 0;
        }

        if (!stbi__zbuild_huffman(&a->z_distance, stbi__zdefault_distance,  32)) {
          return 0;
        }
      }
      else {
        if (!stbi__compute_huffman_codes(a)) {
          return 0;
        }
      }

      if (!stbi__parse_huffman_block(a)) {
        return 0;
      }
    }
  }
  while (!final);

  return 1;
}

static int stbi__do_zlib(stbi__zbuf* a, char* obuf, int olen, int exp, int parse_header)
{
  a->zout_start = obuf;
  a->zout       = obuf;
  a->zout_end   = obuf + olen;
  a->z_expandable = exp;

  return stbi__parse_zlib(a, parse_header);
}

STBIDEF char* stbi_zlib_decode_malloc_guesssize(const char* buffer, int len, int initial_size, int* outlen)
{
  stbi__zbuf a;
  char* p = (char*) stbi__malloc(initial_size);

  if (p == NULL) {
    return NULL;
  }

  a.zbuffer = (uint8*) buffer;
  a.zbuffer_end = (uint8*) buffer + len;

  if (stbi__do_zlib(&a, p, initial_size, 1, 1)) {
    if (outlen) {
      *outlen = (int)(a.zout - a.zout_start);
    }

    return a.zout_start;
  }
  else {
    STBI_FREE(a.zout_start);
    return NULL;
  }
}

STBIDEF char* stbi_zlib_decode_malloc(char const* buffer, int len, int* outlen)
{
  return stbi_zlib_decode_malloc_guesssize(buffer, len, 16384, outlen);
}

STBIDEF char* stbi_zlib_decode_malloc_guesssize_headerflag(const char* buffer, int len, int initial_size, int* outlen, int parse_header)
{
  stbi__zbuf a;
  char* p = (char*) stbi__malloc(initial_size);

  if (p == NULL) {
    return NULL;
  }

  a.zbuffer = (uint8*) buffer;
  a.zbuffer_end = (uint8*) buffer + len;

  if (stbi__do_zlib(&a, p, initial_size, 1, parse_header)) {
    if (outlen) {
      *outlen = (int)(a.zout - a.zout_start);
    }

    return a.zout_start;
  }
  else {
    STBI_FREE(a.zout_start);
    return NULL;
  }
}

STBIDEF int stbi_zlib_decode_buffer(char* obuffer, int olen, char const* ibuffer, int ilen)
{
  stbi__zbuf a;
  a.zbuffer = (uint8*) ibuffer;
  a.zbuffer_end = (uint8*) ibuffer + ilen;

  if (stbi__do_zlib(&a, obuffer, olen, 0, 1)) {
    return (int)(a.zout - a.zout_start);
  }
  else {
    return -1;
  }
}

STBIDEF char* stbi_zlib_decode_noheader_malloc(char const* buffer, int len, int* outlen)
{
  stbi__zbuf a;
  char* p = (char*) stbi__malloc(16384);

  if (p == NULL) {
    return NULL;
  }

  a.zbuffer = (uint8*) buffer;
  a.zbuffer_end = (uint8*) buffer + len;

  if (stbi__do_zlib(&a, p, 16384, 1, 0)) {
    if (outlen) {
      *outlen = (int)(a.zout - a.zout_start);
    }

    return a.zout_start;
  }
  else {
    STBI_FREE(a.zout_start);
    return NULL;
  }
}

STBIDEF int stbi_zlib_decode_noheader_buffer(char* obuffer, int olen, const char* ibuffer, int ilen)
{
  stbi__zbuf a;
  a.zbuffer = (uint8*) ibuffer;
  a.zbuffer_end = (uint8*) ibuffer + ilen;

  if (stbi__do_zlib(&a, obuffer, olen, 0, 0)) {
    return (int)(a.zout - a.zout_start);
  }
  else {
    return -1;
  }
}
#endif // _ZLIB_STB_INL_
