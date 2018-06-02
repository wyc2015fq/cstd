
#include <stdio.h>

//#define EOF -1
typedef struct tag_RLE {
  int rl_pixel;
  int rl_basecode;
  int rl_count;
  int rl_table_pixel;
  int rl_table_max;
  int just_cleared;
  int out_bits;
  int out_bits_init;
  int out_count;
  int out_bump;
  int out_bump_init;
  int out_clear;
  int out_clear_init;
  int max_ocodes;
  int code_clear;
  int code_eof;
  unsigned int obuf;
  int obits;
  unsigned char oblock[256];
  int oblen;
} struct_RLE;

typedef struct gif_save_t {
  stream_t* m_strm;
  uint16 biPlanes;
  uint32 biClrUsed;
  uint32 biClrImportant;
  uint16 biBitCount;
  int32 nBkgndIndex;
  uint8 dispmeth;
  uint32 dwFrameDelay;
  const PaletteEntry* pal;
  int m_loops;
  int curx, cury;
  int32 CountDown;
  uint32 cur_accum;
  int cur_bits;
  int a_count;
  char accum[256];
  int n_bits; /* number of bits/code */
  //LZW GIF Image compression routines
  int32 htab[HSIZE];
  unsigned short codetab[HSIZE];
  code_int maxcode; /* maximum code, given n_bits */
  code_int free_ent; /* first unused entry */
  int clear_flg;
  int g_init_bits;
  int ClearCode;
  int EOFCode;
  int32 xOffset;
  int32 yOffset;
  uint32 dwCodecOpt; ///< for GIF, TIF : 0=def.1=unc,2=fax3,3=fax4,4=pack,5=jpg
  char m_comment[256];
} gif_save_t;

static void gif_encode_header(const gif_save_t* fmt, const img_t* im)
{
  BYTE Flags = 0;
  Write("GIF89a", 1, 6); //GIF Header

  Putword(im->w); //Logical screen descriptor
  Putword(im->h);

  if (fmt->biClrUsed == 0) {
    Flags = 0x11;
  }
  else {
    Flags = 0x80;
    Flags |= (fmt->biBitCount - 1) << 5;
    Flags |= (fmt->biBitCount - 1);
  }

  PutC(Flags); //GIF "packed fields"
  PutC(0); //GIF "BackGround"
  PutC(0); //GIF "pixel aspect ratio"

  if (fmt->biClrUsed != 0) {
    uint32 i;

    for (i = 0; i < fmt->biClrUsed; ++i) {
      PutC(fmt->pal[i].r);
      PutC(fmt->pal[i].g);
      PutC(fmt->pal[i].b);
    }
  }
}
static void gif_encode_extension(gif_save_t* fmt)
{
  struct_gifgce gifgce;
  // TRK BEGIN : transparency
  PutC('!');
  PutC(TRANSPARENCY_CODE);

  gifgce.flags = 0;
  gifgce.flags |= ((fmt->nBkgndIndex != -1) ? 1 : 0);
  gifgce.flags |= ((fmt->dispmeth & 0x7) << 2);
  gifgce.delaytime = (WORD)fmt->dwFrameDelay;
  gifgce.transpcolindex = (BYTE)fmt->nBkgndIndex;

  //Invert byte order in case we use a byte order arch, then set it back <AMSN>
  gifgce.delaytime = bit_ntohs(gifgce.delaytime);
  PutC(sizeof(gifgce));
  Write(&gifgce, sizeof(gifgce), 1);
  gifgce.delaytime = bit_ntohs(gifgce.delaytime);

  PutC(0);
  // TRK END
}

static void gif_encode_loop_extension(gif_save_t* fmt)
{
  PutC('!'); //byte 1 : 33 (hex 0x21) GIF Extension code
  PutC(255); //byte 2 : 255 (hex 0xFF) Application Extension Label
  PutC(11); //byte 3 : 11 (hex (0x0B) Length of Application Block (eleven bytes of data to follow)
  Write("NETSCAPE2.0", 11, 1);
  PutC(3); //byte 15 : 3 (hex 0x03) Length of Data Sub-Block (three bytes of data to follow)
  PutC(1); //byte 16 : 1 (hex 0x01)
  Putword(fmt->m_loops); //bytes 17 to 18 : 0 to 65535, an unsigned integer in lo-hi byte format.
  //This indicate the number of iterations the loop should be executed.
  PutC(0); //bytes 19 : 0 (hex 0x00) a Data Sub-block Terminator.
}

// Return the next pixel from the image
// <DP> fix for 1 & 4 bpp images
static int gif_next_pixel(gif_save_t* fmt, const img_t* im)
{
  int r;

  if (fmt->cury >= im->h) {
    return EOF;
  }

  r = im->tt.data[fmt->cury * im->s + fmt->curx];
  // Bump the current X position
  ++fmt->curx;

  if (fmt->curx == im->w) {
    fmt->curx = 0;
    fmt->cury++; //bottom to top
  }

  return r;
}

//GIF specific
static void gif_flush_char(gif_save_t* fmt)
{
  if (fmt->a_count > 0) {
    PutC((BYTE)fmt->a_count);
    Write(fmt->accum, 1, fmt->a_count);
    fmt->a_count = 0;
  }
}

static void gif_char_out(gif_save_t* fmt, int c)
{
  fmt->accum[fmt->a_count++] = (char)c;

  if (fmt->a_count >= 254) {
    gif_flush_char(fmt);
  }
}

static void gif_output(gif_save_t* fmt, code_int code)
{
  fmt->cur_accum &= code_mask[ fmt->cur_bits ];

  if (fmt->cur_bits > 0) {
    fmt->cur_accum |= ((long)code << fmt->cur_bits);
  }
  else {
    fmt->cur_accum = code;
  }

  fmt->cur_bits += fmt->n_bits;

  while (fmt->cur_bits >= 8) {
    gif_char_out(fmt, (unsigned int)(fmt->cur_accum & 0xff));
    fmt->cur_accum >>= 8;
    fmt->cur_bits -= 8;
  }

  // If the next entry is going to be too big for the code size, then increase it, if possible.
  if (fmt->free_ent > fmt->maxcode || fmt->clear_flg) {
    if (fmt->clear_flg) {
      fmt->maxcode = (short)MAXCODE(fmt->n_bits = fmt->g_init_bits);
      fmt->clear_flg = 0;
    }
    else {
      ++fmt->n_bits;

      if (fmt->n_bits == MAXBITSCODES) {
        fmt->maxcode = (code_int)1 << MAXBITSCODES; // should NEVER generate this code
      }
      else {
        fmt->maxcode = (short)MAXCODE(fmt->n_bits);
      }
    }
  }

  if (code == fmt->EOFCode) {
    // At EOF, write the rest of the buffer.
    while (fmt->cur_bits > 0) {
      gif_char_out(fmt, (unsigned int)(fmt->cur_accum & 0xff));
      fmt->cur_accum >>= 8;
      fmt->cur_bits -= 8;
    }

    gif_flush_char(fmt);
    Flush();
    //if(Error(fmt->m_strm)) strcpy(fmt->szLastError,"Write Error in GIF file");
  }
}

// GIFCOMPR.C - LZW GIF Image compression routines
static void cl_hash(gif_save_t* fmt, long hsize)
{
  register int32* htab_p = fmt->htab + hsize;

  register long i;
  register long m1 = -1L;

  i = hsize - 16;

  do {
    *(htab_p - 16) = m1;
    *(htab_p - 15) = m1;
    *(htab_p - 14) = m1;
    *(htab_p - 13) = m1;
    *(htab_p - 12) = m1;
    *(htab_p - 11) = m1;
    *(htab_p - 10) = m1;
    *(htab_p - 9) = m1;
    *(htab_p - 8) = m1;
    *(htab_p - 7) = m1;
    *(htab_p - 6) = m1;
    *(htab_p - 5) = m1;
    *(htab_p - 4) = m1;
    *(htab_p - 3) = m1;
    *(htab_p - 2) = m1;
    *(htab_p - 1) = m1;

    htab_p -= 16;
  }
  while ((i -= 16) >= 0);

  for (i += 16; i > 0; --i) {
    *--htab_p = m1;
  }
}

static void LZW_encode(gif_save_t* fmt, const img_t* im, int init_bits)
{
  register long fcode;
  register long c;
  register long ent;
  register long hshift;
  register long disp;
  register long i;
  code_int maxmaxcode;

  // fmt->g_init_bits - initial number of bits
  // fmt->m_strm - pointer to gif_output file
  fmt->g_init_bits = init_bits;

  // Set up the necessary values
  fmt->cur_accum = fmt->cur_bits = fmt->clear_flg = 0;
  fmt->maxcode = (short)MAXCODE(fmt->n_bits = fmt->g_init_bits);
  maxmaxcode = (code_int)1 << MAXBITSCODES;

  fmt->ClearCode = (1 << (init_bits - 1));
  fmt->EOFCode = fmt->ClearCode + 1;
  fmt->free_ent = (short)(fmt->ClearCode + 2);

  fmt->a_count = 0;
  ent = gif_next_pixel(fmt, im);

  hshift = 0;

  for (fcode = (long) HSIZE; fcode < 65536L; fcode *= 2L) {
    ++hshift;
  }

  hshift = 8 - hshift; /* set hash code range bound */
  cl_hash(fmt, (long)HSIZE); /* fmt->clear hash table */
  gif_output(fmt, (code_int)fmt->ClearCode);

  while ((c = gif_next_pixel(fmt, im)) != EOF) {
    fcode = (long)(((long) c << MAXBITSCODES) + ent);
    i = (((code_int)c << hshift) ^ ent); /* xor hashing */

    if (HashTabOf(i) == fcode) {
      ent = CodeTabOf(i);
      continue;
    }
    else if ((long)HashTabOf(i) < 0) { /* empty fmt->slot */
      goto nomatch;
    }

    disp = HSIZE - i; /* secondary hash (after G. Knott) */

    if (i == 0) {
      disp = 1;
    }

probe:

    if ((i -= disp) < 0) {
      i += HSIZE;
    }

    if (HashTabOf(i) == fcode) {
      ent = CodeTabOf(i);
      continue;
    }

    if ((long)HashTabOf(i) > 0) {
      goto probe;
    }

nomatch:
    gif_output(fmt, (code_int) ent);
    ent = c;

    if (fmt->free_ent < maxmaxcode) {
      CodeTabOf(i) = fmt->free_ent++; /* code -> hashtable */
      HashTabOf(i) = fcode;
    }
    else {
      cl_hash(fmt, (long)HSIZE);
      fmt->free_ent = (short)(fmt->ClearCode + 2);
      fmt->clear_flg = 1;
      gif_output(fmt, (code_int)fmt->ClearCode);
    }
  }

  // Put out the final code.
  gif_output(fmt, (code_int)ent);
  gif_output(fmt, (code_int) fmt->EOFCode);
}


static void gif_compress_none(gif_save_t* fmt, const img_t* im, int init_bits)
{
  register long c;
  register long ent;
  code_int maxmaxcode;

  // fmt->g_init_bits - initial number of bits
  // fmt->m_strm - pointer to gif_output file
  fmt->g_init_bits = init_bits;

  // Set up the necessary values
  fmt->cur_accum = fmt->cur_bits = fmt->clear_flg = 0;
  fmt->maxcode = (short)MAXCODE(fmt->n_bits = fmt->g_init_bits);
  maxmaxcode = (code_int)1 << MAXBITSCODES;

  fmt->ClearCode = (1 << (init_bits - 1));
  fmt->EOFCode = fmt->ClearCode + 1;
  fmt->free_ent = (short)(fmt->ClearCode + 2);

  fmt->a_count = 0;
  ent = gif_next_pixel(fmt, im);

  gif_output(fmt, (code_int)fmt->ClearCode);

  while (ent != EOF) {
    c = gif_next_pixel(fmt, im);

    gif_output(fmt, (code_int) ent);
    ent = c;

    if (fmt->free_ent < maxmaxcode) {
      fmt->free_ent++;
    }
    else {
      fmt->free_ent = (short)(fmt->ClearCode + 2);
      fmt->clear_flg = 1;
      gif_output(fmt, (code_int)fmt->ClearCode);
    }
  }

  // Put out the final code.
  gif_output(fmt, (code_int) fmt->EOFCode);
}

/*-----------------------------------------------------------------------
*
* miGIF Compression - mouse and ivo's GIF-compatible compression
*
* -run length encoding compression routines-
*
* Copyright (C) 1998 Hutchison Avenue Software Corporation
* http://www.hasc.com
* fmt@hasc.com
*
* Permission to use, copy, modify, and distribute this software and its
* documentation for any purpose and without fee is hereby granted, provided
* that the above copyright notice appear in all copies and that both that
* copyright notice and this permission notice appear in supporting
* documentation. This software is provided "AS IS." The Hutchison Avenue
* Software Corporation disclaims all warranties, either express or implied,
* including but not limited to implied warranties of merchantability and
* fitness for a particular purpose, with respect to this code and accompanying
* documentation.
*
* The miGIF compression routines do not, strictly speaking, generate files
* conforming to the GIF spec, since the image data is not LZW-compressed
* (this is the point: in order to avoid transgression of the Unisys patent
* on the LZW algorithm.) However, miGIF generates data streams that any
* reasonably sane LZW decompresser will decompress to what we want.
*
* miGIF compression uses run length encoding. It compresses horizontal runs
* of pixels of the same color. This type of compression gives good results
* on images with many runs, for example images with lines, text and solid
* shapes on a solid-colored background. It gives little or no compression
* on images with few runs, for example digital or scanned photos.
*
* der Mouse
* mouse@rodents.montreal.qc.ca
* 7D C8 61 52 5D E7 2D 39 4E F1 31 3E E8 B3 27 4B
*
* ivo@hasc.com
*
* The Graphics Interchange Format(c) is the Copyright property of
* CompuServe Incorporated. GIF(sm) is a Service Mark property of
* CompuServe Incorporated.
*
*/

static void rle_clear(struct_RLE* rle)
{
  rle->out_bits = rle->out_bits_init;
  rle->out_bump = rle->out_bump_init;
  rle->out_clear = rle->out_clear_init;
  rle->out_count = 0;
  rle->rl_table_max = 0;
  rle->just_cleared = 1;
}

static void rle_write_block(gif_save_t* fmt, struct_RLE* rle)
{
  PutC((BYTE)rle->oblen);
  Write(rle->oblock, 1, rle->oblen);
  rle->oblen = 0;
}

static void rle_block_out(gif_save_t* fmt, unsigned char c, struct_RLE* rle)
{
  rle->oblock[rle->oblen++] = c;

  if (rle->oblen >= 255) {
    rle_write_block(fmt, rle);
  }
}

static void rle_output(gif_save_t* fmt, int val, struct_RLE* rle)
{
  rle->obuf |= val << rle->obits;
  rle->obits += rle->out_bits;

  while (rle->obits >= 8) {
    rle_block_out(fmt, (unsigned char)(rle->obuf & 0xff), rle);
    rle->obuf >>= 8;
    rle->obits -= 8;
  }
}

static void rle_output_plain(gif_save_t* fmt, int c, struct_RLE* rle)
{
  rle->just_cleared = 0;
  rle_output(fmt, c, rle);
  rle->out_count++;

  if (rle->out_count >= rle->out_bump) {
    rle->out_bits ++;
    rle->out_bump += 1 << (rle->out_bits - 1);
  }

  if (rle->out_count >= rle->out_clear) {
    rle_output(fmt, rle->code_clear, rle);
    rle_clear(rle);
  }
}

static void rle_reset_out_clear(gif_save_t* fmt, struct_RLE* rle)
{
  rle->out_clear = rle->out_clear_init;

  if (rle->out_count >= rle->out_clear) {
    rle_output(fmt, rle->code_clear, rle);
    rle_clear(rle);
  }
}

static void rle_flush_fromclear(gif_save_t* fmt, int count, struct_RLE* rle)
{
  int n;

  rle->out_clear = rle->max_ocodes;
  rle->rl_table_pixel = rle->rl_pixel;
  n = 1;

  while (count > 0) {
    if (n == 1) {
      rle->rl_table_max = 1;
      rle_output_plain(fmt, rle->rl_pixel, rle);
      count --;
    }
    else if (count >= n) {
      rle->rl_table_max = n;
      rle_output_plain(fmt, rle->rl_basecode + n - 2, rle);
      count -= n;
    }
    else if (count == 1) {
      rle->rl_table_max ++;
      rle_output_plain(fmt, rle->rl_pixel, rle);
      count = 0;
    }
    else {
      rle->rl_table_max ++;
      rle_output_plain(fmt, rle->rl_basecode + count - 2, rle);
      count = 0;
    }

    if (rle->out_count == 0) {
      n = 1;
    }
    else {
      n ++;
    }
  }

  rle_reset_out_clear(fmt, rle);
}

static unsigned int rle_isqrt(unsigned int x)
{
  unsigned int r;
  unsigned int v;

  if (x < 2) {
    return(x);
  }

  for (v = x, r = 1; v; v >>= 2, r <<= 1);

  for (;;) {
    v = ((x / r) + r) / 2;

    if ((v == r) || (v == r + 1)) {
      return(r);
    }

    r = v;
  }
}

static unsigned int rle_compute_triangle_count(unsigned int count, unsigned int nrepcodes)
{
  unsigned int perrep;
  unsigned int cost;

  cost = 0;
  perrep = (nrepcodes * (nrepcodes + 1)) / 2;

  while (count >= perrep) {
    cost += nrepcodes;
    count -= perrep;
  }

  if (count > 0) {
    unsigned int n;
    n = rle_isqrt(count);

    while ((n * (n + 1)) >= 2 * count) {
      n --;
    }

    while ((n * (n + 1)) < 2 * count) {
      n ++;
    }

    cost += n;
  }

  return(cost);
}

static void rle_flush_clearorrep(gif_save_t* fmt, int count, struct_RLE* rle)
{
  int withclr;
  withclr = 1 + rle_compute_triangle_count(count, rle->max_ocodes);

  if (withclr < count) {
    rle_output(fmt, rle->code_clear, rle);
    rle_clear(rle);
    rle_flush_fromclear(fmt, count, rle);
  }
  else {
    for (; count > 0; count--) {
      rle_output_plain(fmt, rle->rl_pixel, rle);
    }
  }
}

static void rle_flush_withtable(gif_save_t* fmt, int count, struct_RLE* rle)
{
  int repmax;
  int repleft;
  int leftover;

  repmax = count / rle->rl_table_max;
  leftover = count % rle->rl_table_max;
  repleft = (leftover ? 1 : 0);

  if (rle->out_count + repmax + repleft > rle->max_ocodes) {
    repmax = rle->max_ocodes - rle->out_count;
    leftover = count - (repmax * rle->rl_table_max);
    repleft = 1 + rle_compute_triangle_count(leftover, rle->max_ocodes);
  }

  if (1 + rle_compute_triangle_count(count, rle->max_ocodes) < (unsigned int)(repmax + repleft)) {
    rle_output(fmt, rle->code_clear, rle);
    rle_clear(rle);
    rle_flush_fromclear(fmt, count, rle);
    return;
  }

  rle->out_clear = rle->max_ocodes;

  for (; repmax > 0; repmax--) {
    rle_output_plain(fmt, rle->rl_basecode + rle->rl_table_max - 2, rle);
  }

  if (leftover) {
    if (rle->just_cleared) {
      rle_flush_fromclear(fmt, leftover, rle);
    }
    else if (leftover == 1) {
      rle_output_plain(fmt, rle->rl_pixel, rle);
    }
    else {
      rle_output_plain(fmt, rle->rl_basecode + leftover - 2, rle);
    }
  }

  rle_reset_out_clear(fmt, rle);
}

static void rle_flush(gif_save_t* fmt, struct_RLE* rle)
{
  if (rle->rl_count == 1) {
    rle_output_plain(fmt, rle->rl_pixel, rle);
    rle->rl_count = 0;
    return;
  }

  if (rle->just_cleared) {
    rle_flush_fromclear(fmt, rle->rl_count, rle);
  }
  else if ((rle->rl_table_max < 2) || (rle->rl_table_pixel != rle->rl_pixel)) {
    rle_flush_clearorrep(fmt, rle->rl_count, rle);
  }
  else {
    rle_flush_withtable(fmt, rle->rl_count, rle);
  }

  rle->rl_count = 0;
}

static void rle_block_flush(gif_save_t* fmt, struct_RLE* rle)
{
  if (rle->oblen > 0) {
    rle_write_block(fmt, rle);
  }
}

static void rle_output_flush(gif_save_t* fmt, struct_RLE* rle)
{
  if (rle->obits > 0) {
    rle_block_out(fmt, (unsigned char)(rle->obuf), rle);
  }

  rle_block_flush(fmt, rle);
}

static void RLE_encode(gif_save_t* fmt, const img_t* im, int init_bits)
{
  int c;
  struct_RLE rle;
  fmt->g_init_bits = init_bits;

  rle.code_clear = 1 << (init_bits - 1);
  rle.code_eof = rle.code_clear + 1;
  rle.rl_basecode = rle.code_eof + 1;
  rle.out_bump_init = (1 << (init_bits - 1)) - 1;
  rle.out_clear_init = (init_bits <= 3) ? 9 : (rle.out_bump_init - 1);
  rle.out_bits_init = init_bits;
  rle.max_ocodes = (1 << MAXBITSCODES) - ((1 << (rle.out_bits_init - 1)) + 3);
  rle.rl_count = 0;
  rle_clear(&rle);
  rle.obuf = 0;
  rle.obits = 0;
  rle.oblen = 0;

  rle_output(fmt, rle.code_clear, &rle);

  for (;;) {
    c = gif_next_pixel(fmt, im);

    if ((rle.rl_count > 0) && (c != rle.rl_pixel)) {
      rle_flush(fmt, &rle);
    }

    if (c == EOF) {
      break;
    }

    if (rle.rl_pixel == c) {
      rle.rl_count++;
    }
    else {
      rle.rl_pixel = c;
      rle.rl_count = 1;
    }
  }

  rle_output(fmt, rle.code_eof, &rle);
  rle_output_flush(fmt, &rle);
}

static void gif_encode_body(gif_save_t* fmt, const img_t* im, BOOL bLocalColorMap)
{
  int InitCodeSize;
  BYTE Flags = 0x00; //non-fmt->interlaced (0x40 = fmt->interlaced) (0x80 = LocalColorMap)
  fmt->curx = 0;
  fmt->cury = 0; //because we read the image bottom to top
  fmt->CountDown = im->w * im->h;

  PutC(',');

  Putword(fmt->xOffset);
  Putword(fmt->yOffset);
  Putword(im->w);
  Putword(im->h);

  if (bLocalColorMap) {
    Flags |= 0x80;
    Flags |= fmt->biBitCount - 1;
  }

  PutC(Flags);

  if (bLocalColorMap) {
    uint32 i;
    Flags |= 0x87;

    for (i = 0; i < fmt->biClrUsed; ++i) {
      PutC(fmt->pal[i].r);
      PutC(fmt->pal[i].g);
      PutC(fmt->pal[i].b);
    }
  }

  InitCodeSize = fmt->biBitCount <= 1 ? 2 : fmt->biBitCount;
  // Write out the initial code size
  PutC((BYTE)InitCodeSize);

  // Go and actually compress the data
  switch (fmt->dwCodecOpt) {
  case 1: //uncompressed
    gif_compress_none(fmt, im, InitCodeSize + 1);
    break;

  case 2: //RLE
    RLE_encode(fmt, im, InitCodeSize + 1);
    break;

  default: //LZW
    LZW_encode(fmt, im, InitCodeSize + 1);
  }

  // Write out a Zero-length packet (to end the series)
  PutC(0);
}

static void gif_encode_comment(gif_save_t* fmt)
{
  unsigned long n = (unsigned long) strlen(fmt->m_comment);

  if (n > 255) {
    n = 255;
  }

  if (n) {
    PutC('!'); //extension code:
    PutC(254); //comment extension
    PutC((BYTE)n); //size of comment
    Write(fmt->m_comment, n, 1);
    PutC(0); //block terminator
  }
}

static BOOL gif_encode_rgb(gif_save_t* fmt, const img_t* im)
{
  //unsigned long h=0;
  const long cellw = 17;
  const long cellh = 15;
  long x, y;
  const unsigned char* pix = im->tt.data;
  int step = im->s;
  fmt->m_strm = fmt->m_strm;

  gif_encode_header(fmt, im);
  // gif_encode_loop_extension(fp);
  gif_encode_comment(fmt);

  //w = h = 0;

  for (y = 0; y < im->h; y += cellh) {
    for (x = 0; x < im->w; x += cellw) {
      BYTE i = 0;
      unsigned long j = 0, k = 0;
      //w = MIN((im->w - x), cellw);
      //h = MIN((im->h - y), cellh);

      if (fmt->nBkgndIndex >= 0) {
        fmt->nBkgndIndex = 0;
        //SetPaletteColor(0,GetTransColor());
      }

      //SetOffset(x,y);
      gif_encode_extension(fmt);
      gif_encode_body(fmt, im, TRUE);
    }
  }

  PutC(';'); // Write the GIF file terminator
  return TRUE; // done!
}

static BOOL gif_encode(gif_save_t* fmt, const img_t* im, BOOL bLocalColorMap, BOOL bLocalDispMeth)
{
  int i;
  int pagecount = im->f;

  if (fmt->m_strm == NULL) {
    printf("invalid file pointer");
    return 0;
  }

  if (im == NULL || pagecount <= 0) {
    printf("multipage GIF, no images!");
    return 0;
  }

  //write the first image
  gif_encode_header(fmt, im);

  if (fmt->m_loops != 1) {
    fmt->m_loops = MAX(0, fmt->m_loops - 1);
    gif_encode_loop_extension(fmt);
  }

  if (bLocalDispMeth) {
    gif_encode_extension(fmt);
  }
  else {
    gif_encode_extension(fmt);
  }

  gif_encode_comment(fmt);

  gif_encode_body(fmt, im, FALSE);

  for (i = 1; i < pagecount; i++) {
    //Ghost(pImages[i]);

    if (bLocalDispMeth) {
      gif_encode_extension(fmt);
    }
    else {
      gif_encode_extension(fmt);
    }

    gif_encode_body(fmt, im, bLocalColorMap);
  }

  PutC(';'); // Write the GIF file terminator

  return TRUE;
}

// SaveFile - writes GIF87a gif file
// Randy Spann 6/15/97
// R.Spann@ConnRiver.net
static int gif_save(stream_t* m_strm, const img_t* im, const PaletteEntry* palette)
{
  gif_save_t fmt[1] = {0};
  const unsigned char* data = im->tt.data;
  PaletteEntry palette0[256];
  fmt->biBitCount = im->c << 3;
  fmt->biClrUsed = 256;
  fmt->nBkgndIndex = 255;
  fmt->m_strm = m_strm;
  fmt->pal = palette ? palette : palette0;
  palette_fillgray(1 << 8, (uchar*)palette0, FALSE);

  if (fmt->biBitCount > 8) {
    //strcpy(fmt->szLastError,"GIF Images must be 8 bit or less");
    //return FALSE;
    return gif_encode_rgb(fmt, im);
  }

  if (im->f > 1) {
    return gif_encode(fmt, im, FALSE, FALSE);
  }

  gif_encode_header(fmt, im);
  gif_encode_extension(fmt);
  gif_encode_comment(fmt);
  gif_encode_body(fmt, im, FALSE);
  PutC(';'); // Write the GIF file terminator
  return 0;
}
