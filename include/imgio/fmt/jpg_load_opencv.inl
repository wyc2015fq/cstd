
//#include "bitstrm.h"
#include <setjmp.h>

//#define RBS_THROW_EOS -123 /* <end of stream> exception code */
#define RBS_THROW_FORB -124 /* <forrbidden huffman code> exception code */
#define RBS_HUFF_FORB 2047 /* forrbidden huffman code "value" */


#define WRITE_PIX( ptr, clr ) \
  (((uchar*)(ptr))[0] = (clr).b, \
      ((uchar*)(ptr))[1] = (clr).g, \
      ((uchar*)(ptr))[2] = (clr).r)

#undef descale
#define descale(x,n) (((x) + (1 << ((n)-1))) >> (n))
#define saturate(x) (uchar)(((x) & ~255) == 0 ? (x) : ~((x)>>31))

#define BSWAP(v) (((v)<<24)|(((v)&0xff00)<<8)| \
    (((v)>>8)&0xff00)|((unsigned)(v)>>24))


//typedef unsigned char uchar;
typedef unsigned long ulong;

const ulong bs_bit_mask[] = {
  0,
  0x00000001, 0x00000003, 0x00000007, 0x0000000F,
  0x0000001F, 0x0000003F, 0x0000007F, 0x000000FF,
  0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF,
  0x00001FFF, 0x00003FFF, 0x00007FFF, 0x0000FFFF,
  0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000FFFFF,
  0x001FFFFF, 0x003FFFFF, 0x007FFFFF, 0x00FFFFFF,
  0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0FFFFFFF,
  0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF
};

CC_INLINE void bsBSwapBlock(uchar* start, uchar* end)
{
  ulong* data = (ulong*)start;
  int i, size = (int)(end - start + 3) / 4;

  for (i = 0; i < size; i++) {
    ulong temp = data[i];
    temp = BSWAP(temp);
    data[i] = temp;
  }
}

CC_INLINE BOOL bsIsBigEndian(void)
{
  return (((const int*)"\0\x1\x2\x3\x4\x5\x6\x7")[0] & 255) != 0;
}


enum { huff_val_shift = 20, huff_code_mask = (1 << huff_val_shift) - 1 };

CC_INLINE BOOL bsCreateDecodeHuffmanTable(const int* src, short* table, int max_size)
{
  const int forbidden_entry = (RBS_HUFF_FORB << 4) | 1;
  int first_bits = src[0];
  struct {
    int bits;
    int offset;
  }
  sub_tables[1 << 11];
  int size = (1 << first_bits) + 1;
  int i, k;

  /* calc bit depths of sub tables */
  memset(sub_tables, 0, ((size_t)1 << first_bits)*sizeof(sub_tables[0]));

  for (i = 1, k = 1; src[k] >= 0; i++) {
    int code_count = src[k++];
    int sb = i - first_bits;

    if (sb <= 0) {
      k += code_count;
    }
    else
      for (code_count += k; k < code_count; k++) {
        int code = src[k] & huff_code_mask;
        sub_tables[code >> sb].bits = sb;
      }
  }

  /* calc offsets of sub tables and whole size of table */
  for (i = 0; i < (1 << first_bits); i++) {
    int b = sub_tables[i].bits;

    if (b > 0) {
      b = 1 << b;
      sub_tables[i].offset = size;
      size += b + 1;
    }
  }

  if (size > max_size) {
    assert(0);
    return FALSE;
  }

  /* fill first table and subtables with forbidden values */
  for (i = 0; i < size; i++) {
    table[i] = (short)forbidden_entry;
  }

  /* write header of first table */
  table[0] = (short)first_bits;

  /* fill first table and sub tables */
  for (i = 1, k = 1; src[k] >= 0; i++) {
    int code_count = src[k++];

    for (code_count += k; k < code_count; k++) {
      int table_bits = first_bits;
      int code_bits = i;
      int code = src[k] & huff_code_mask;
      int val = src[k] >> huff_val_shift;
      int j, offset = 0;

      if (code_bits > table_bits) {
        int idx = code >> (code_bits -= table_bits);
        code &= (1 << code_bits) - 1;
        offset = sub_tables[idx].offset;
        table_bits = sub_tables[idx].bits;
        /* write header of subtable */
        table[offset] = (short)table_bits;
        /* write jump to subtable */
        table[idx + 1] = (short)(offset << 4);
      }

      table_bits -= code_bits;
      assert(table_bits >= 0);
      val = (val << 4) | code_bits;
      offset += (code << table_bits) + 1;

      for (j = 0; j < (1 << table_bits); j++) {
        assert(table[offset + j] == forbidden_entry);
        table[ offset + j ] = (short)val;
      }
    }
  }

  return TRUE;
}


static int* bsCreateSourceHuffmanTable(const uchar* src, int* dst, int max_bits, int first_bits)
{
  int i, k, val_idx, code = 0;
  int* table = dst;
  *dst++ = first_bits;

  for (i = 1, val_idx = max_bits; i <= max_bits; i++) {
    int code_count = src[i - 1];
    dst[0] = code_count;
    code <<= 1;

    for (k = 0; k < code_count; k++) {
      dst[k + 1] = (src[val_idx + k] << huff_val_shift) | (code + k);
    }

    code += code_count;
    dst += code_count + 1;
    val_idx += code_count;
  }

  dst[0] = -1;
  return table;
}


static BOOL bsCreateEncodeHuffmanTable(const int* src, ulong* table, int max_size)
{
  int i, k;
  int min_val = INT_MAX, max_val = INT_MIN;
  int size;

  /* calc min and max values in the table */
  for (i = 1, k = 1; src[k] >= 0; i++) {
    int code_count = src[k++];

    for (code_count += k; k < code_count; k++) {
      int val = src[k] >> huff_val_shift;

      if (val < min_val) {
        min_val = val;
      }

      if (val > max_val) {
        max_val = val;
      }
    }
  }

  size = max_val - min_val + 3;

  if (size > max_size) {
    assert(0);
    return FALSE;
  }

  memset(table, 0, size * sizeof(table[0]));

  table[0] = min_val;
  table[1] = size - 2;

  for (i = 1, k = 1; src[k] >= 0; i++) {
    int code_count = src[k++];

    for (code_count += k; k < code_count; k++) {
      int val = src[k] >> huff_val_shift;
      int code = src[k] & huff_code_mask;

      table[val - min_val + 2] = (code << 8) | i;
    }
  }

  return TRUE;
}


/* hand-crafted implementation */

// information about each component
typedef struct cmp_info {
  char h; // horizontal sampling factor
  char v; // vertical sampling factor
  char tq; // quantization table index
  char td, ta; // DC & AC huffman tables
  int dc_pred; // DC predictor
} cmp_info;

//////////////////// JPEG reader /////////////////////

enum { BS_DEF_BLOCK_SIZE = (1 << 15), max_dec_htable_size = 1 << 12, first_table_bits = 9 };


typedef struct jpg_reader_t {
  BOOL m_iscolor;
  int m_width; // width of the image ( filled by ReadHeader )
  int m_height; // height of the image ( filled by ReadHeader )
  int m_bit_depth;// bit depth per channel (normally 8)
  BOOL m_native_depth;// use the native bit depth of the image
  BOOL m_isfloat; // is image saved as float or double?

  int m_offset; // offset of first scan
  int m_version; // JFIF version
  int m_planes; // 3 (YCrCb) or 1 (Gray)
  int m_precision; // 8 or 12-bit per sample
  int m_type; // SOF type
  int m_MCUs; // MCUs in restart interval
  int m_ss, m_se, m_ah, m_al; // progressive JPEG parameters

  cmp_info m_ci[3];

  int m_tq[4][64];
  BOOL m_is_tq[4];

  short m_td[4][max_dec_htable_size];
  BOOL m_is_td[4];

  short m_ta[4][max_dec_htable_size];
  BOOL m_is_ta[4];

  int m_bit_idx;

  jmp_buf m_jmp_buf;
  uchar* m_start;
  uchar* m_end;
  uchar* m_current;

  stream_t* m_strm;

  int m_unGetsize;
  int m_block_size;
  int m_block_pos;
  BOOL m_jmp_set;
  BOOL m_is_opened;

  uchar m_buf[BS_DEF_BLOCK_SIZE + 4];
} jpg_reader_t;


static void jpg_load_SetPos(jpg_reader_t* s, int pos)
{
  int offset = pos & (s->m_block_size - 1);
  int block_pos = pos - offset;

  //assert(IsOpened() && pos >= 0);

  if (s->m_current < s->m_end && block_pos == s->m_block_pos - s->m_block_size) {
    s->m_current = s->m_start + offset;
  }
  else {
    s->m_block_pos = block_pos;
    s->m_current = s->m_start + s->m_block_size + offset;
  }

  offset = (int)(s->m_current - s->m_end);
  s->m_current = s->m_end + ((offset - 1) & -4);
  s->m_bit_idx = (32 - (offset & 3) * 8) & 31;
}


////////////////////// JPEG-oriented two-level bitstream ////////////////////////

static BOOL jpg_load_Open(jpg_reader_t* s, stream_t* f)
{
  s->m_start = s->m_end = s->m_current = 0;
  s->m_block_size = BS_DEF_BLOCK_SIZE;
  s->m_unGetsize = 4; // 32 bits
  s->m_is_opened = FALSE;
  s->m_jmp_set = FALSE;

  s->m_start = s->m_buf;
  s->m_start += s->m_unGetsize;

  s->m_end = s->m_start + s->m_block_size;
  s->m_current = s->m_end;

  s->m_strm = f;
  s->m_is_opened = f != NULL;

  if (s->m_is_opened) {
    jpg_load_SetPos(s, 0);
  }

  s->m_planes = -1;
  s->m_offset = -1;

  return s->m_is_opened;
}


static void jpg_load_Close(jpg_reader_t* s)
{
  s->m_is_opened = FALSE;
}

static void jpg_load_ReadBlock(jpg_reader_t* s)
{
  uchar* end = s->m_start + s->m_block_size;
  uchar* current = s->m_start;

  {
    int sz = s->m_unGetsize;
    memmove(current - sz, s->m_end - sz, sz);

    while (current < end) {
      int val = stream_get8(s->m_strm);

      if (val != 0xff) {
        *current++ = (uchar)val;
      }
      else {
        val = stream_get8(s->m_strm);

        if (val == 0) {
          *current++ = 0xFF;
        }
        else if (!(0xD0 <= val && val <= 0xD7)) {
          int pos1 = stream_tell(s->m_strm) - 2;
          stream_seek(s->m_strm, pos1, SEEK_SET);
          break;
        }
      }
    }
  }

  s->m_current = s->m_start;
  s->m_end = s->m_start + (((current - s->m_start) + 3) & -4);

  if (!bsIsBigEndian()) {
    bsBSwapBlock(s->m_start, s->m_end);
  }
}


static void jpg_load_Flush(jpg_reader_t* s)
{
  s->m_end = s->m_start + s->m_block_size;
  s->m_current = s->m_end - 4;
  s->m_bit_idx = 0;
}

static void jpg_load_AlignOnByte(jpg_reader_t* s)
{
  s->m_bit_idx &= -8;
}

static int jpg_load_FindMarker(jpg_reader_t* s)
{
  int code = stream_get16be(s->m_strm);

  while ((code & 0xFF00) != 0xFF00 || (code == 0xFFFF || code == 0xFF00)) {
    code = ((code & 255) << 8) | stream_get8(s->m_strm);
  }

  return code;
}

static int jpg_load_Get(jpg_reader_t* s, int bits)
{
  int bit_idx = s->m_bit_idx - bits;
  int mask = bit_idx >> 31;
  ulong* current = ((ulong*)s->m_current) - mask;

  printf("bits = %d\n", bits);
  assert((unsigned)bits < 32);

  if ((s->m_current = (uchar*)current) >= s->m_end) {
    jpg_load_ReadBlock(s);
    current = (ulong*)s->m_current;
  }

  s->m_bit_idx = bit_idx &= 31;
  return (((current[-1] << -bit_idx) & mask) |
      (current[0] >> bit_idx)) & bs_bit_mask[bits];
}


static int jpg_load_Show(jpg_reader_t* s, int bits)
{
  int bit_idx = s->m_bit_idx - bits;
  int mask = bit_idx >> 31;
  ulong* current = ((ulong*)s->m_current) - mask;

  assert((unsigned)bits < 32);

  if (((uchar*)current) >= s->m_end) {
    s->m_current = (uchar*)current;
    jpg_load_ReadBlock(s);
    current = (ulong*)s->m_current;
    s->m_current -= 4;
  }

  return (((current[-1] << -bit_idx) & mask) |
      (current[0] >> bit_idx)) & bs_bit_mask[bits];
}


static void jpg_load_Move(jpg_reader_t* s, int shift)
{
  int new_bit_idx = s->m_bit_idx - shift;
  s->m_current -= (new_bit_idx >> 5) << 2;
  s->m_bit_idx = new_bit_idx & 31;
}


static void jpg_load_Skip(jpg_reader_t* s, int bytes)
{
  jpg_load_Move(s, bytes * 8);
}

static int jpg_load_GetHuff(jpg_reader_t* s, const short* table)
{
  int val;
  int code_bits;

  for (;;) {
    int table_bits = table[0];
    val = table[jpg_load_Show(s, table_bits) + 1];
    code_bits = val & 15;
    val >>= 4;

    if (code_bits != 0) {
      break;
    }

    table += val;
    jpg_load_Move(s, table_bits);
  }

  jpg_load_Move(s, code_bits);

  if (val == RBS_HUFF_FORB) {
    if (s->m_jmp_set) {
      longjmp(s->m_jmp_buf, RBS_THROW_FORB);
    }
  }

  return val;
}

/****************************** JPEG (JFIF) reader ***************************/

// zigzag & IDCT prescaling (AAN algorithm) tables
static const uchar zigzag[] = {
  0, 8, 1, 2, 9, 16, 24, 17, 10, 3, 4, 11, 18, 25, 32, 40,
  33, 26, 19, 12, 5, 6, 13, 20, 27, 34, 41, 48, 56, 49, 42, 35,
  28, 21, 14, 7, 15, 22, 29, 36, 43, 50, 57, 58, 51, 44, 37, 30,
  23, 31, 38, 45, 52, 59, 60, 53, 46, 39, 47, 54, 61, 62, 55, 63,
  63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63
};


static const int idct_prescale[] = {
  16384, 22725, 21407, 19266, 16384, 12873, 8867, 4520,
  22725, 31521, 29692, 26722, 22725, 17855, 12299, 6270,
  21407, 29692, 27969, 25172, 21407, 16819, 11585, 5906,
  19266, 26722, 25172, 22654, 19266, 15137, 10426, 5315,
  16384, 22725, 21407, 19266, 16384, 12873, 8867, 4520,
  12873, 17855, 16819, 15137, 12873, 10114, 6967, 3552,
  8867, 12299, 11585, 10426, 8867, 6967, 4799, 2446,
  4520, 6270, 5906, 5315, 4520, 3552, 2446, 1247
};


#define fixb  14
#undef fix
#define fix(x, n) (int)((x)*(1 << (n)) + .5)
#define fix1(x, n) (x)
#define fixmul(x) (x)

#define C0_707 fix( 0.707106781f, fixb )
#define C0_924 fix( 0.923879533f, fixb )
#define C0_541 fix( 0.541196100f, fixb )
#define C0_382 fix( 0.382683432f, fixb )
#define C1_306 fix( 1.306562965f, fixb )

#define C1_082 fix( 1.082392200f, fixb )
#define C1_414 fix( 1.414213562f, fixb )
#define C1_847 fix( 1.847759065f, fixb )
#define C2_613 fix( 2.613125930f, fixb )

#define fixc 12
#define b_cb fix( 1.772, fixc )
#define g_cb -fix( 0.34414, fixc )
#define g_cr -fix( 0.71414, fixc )
#define r_cr fix( 1.402, fixc )

#define y_r fix( 0.299, fixc )
#define y_g fix( 0.587, fixc )
#define y_b fix( 0.114, fixc )

#define cb_r -fix( 0.1687, fixc )
#define cb_g -fix( 0.3313, fixc )
#define cb_b fix( 0.5, fixc )

#define cr_r fix( 0.5, fixc )
#define cr_g -fix( 0.4187, fixc )
#define cr_b -fix( 0.0813, fixc )


// IDCT without prescaling
static void aan_idct8x8(int* src, int* dst, int step)
{
  int workspace[64], *work = workspace;
  int i;

  /* Pass 1: process rows */
  for (i = 8; i > 0; i--, src += 8, work += 8) {
    /* Odd part */
    int x0 = src[5], x1 = src[3];
    int x2 = src[1], x3 = src[7];

    int x4 = x0 + x1;
    x0 -= x1;

    x1 = x2 + x3;
    x2 -= x3;
    x3 = x1 + x4;
    x1 -= x4;

    x4 = (x0 + x2) * C1_847;
    x0 = descale(x4 - x0 * C2_613, fixb);
    x2 = descale(x2 * C1_082 - x4, fixb);
    x1 = descale(x1 * C1_414, fixb);

    x0 -= x3;
    x1 -= x0;
    x2 += x1;

    work[7] = x3;
    work[6] = x0;
    work[5] = x1;
    work[4] = x2;

    /* Even part */
    x2 = src[2];
    x3 = src[6];
    x0 = src[0];
    x1 = src[4];

    x4 = x2 + x3;
    x2 = descale((x2 - x3) * C1_414, fixb) - x4;

    x3 = x0 + x1;
    x0 -= x1;
    x1 = x3 + x4;
    x3 -= x4;
    x4 = x0 + x2;
    x0 -= x2;

    x2 = work[7];
    x1 -= x2;
    x2 = 2 * x2 + x1;
    work[7] = x1;
    work[0] = x2;

    x2 = work[6];
    x1 = x4 + x2;
    x4 -= x2;
    work[1] = x1;
    work[6] = x4;

    x1 = work[5];
    x2 = work[4];
    x4 = x0 + x1;
    x0 -= x1;
    x1 = x3 + x2;
    x3 -= x2;

    work[2] = x4;
    work[5] = x0;
    work[3] = x3;
    work[4] = x1;
  }

  /* Pass 2: process columns */
  work = workspace;

  for (i = 8; i > 0; i--, dst += step, work++) {
    /* Odd part */
    int x0 = work[8 * 5], x1 = work[8 * 3];
    int x2 = work[8 * 1], x3 = work[8 * 7];

    int x4 = x0 + x1;
    x0 -= x1;
    x1 = x2 + x3;
    x2 -= x3;
    x3 = x1 + x4;
    x1 -= x4;

    x4 = (x0 + x2) * C1_847;
    x0 = descale(x4 - x0 * C2_613, fixb);
    x2 = descale(x2 * C1_082 - x4, fixb);
    x1 = descale(x1 * C1_414, fixb);

    x0 -= x3;
    x1 -= x0;
    x2 += x1;

    dst[7] = x3;
    dst[6] = x0;
    dst[5] = x1;
    dst[4] = x2;

    /* Even part */
    x2 = work[8 * 2];
    x3 = work[8 * 6];
    x0 = work[8 * 0];
    x1 = work[8 * 4];

    x4 = x2 + x3;
    x2 = descale((x2 - x3) * C1_414, fixb) - x4;

    x3 = x0 + x1;
    x0 -= x1;
    x1 = x3 + x4;
    x3 -= x4;
    x4 = x0 + x2;
    x0 -= x2;

    x2 = dst[7];
    x1 -= x2;
    x2 = 2 * x2 + x1;
    x1 = descale(x1, 3);
    x2 = descale(x2, 3);

    dst[7] = x1;
    dst[0] = x2;

    x2 = dst[6];
    x1 = descale(x4 + x2, 3);
    x4 = descale(x4 - x2, 3);
    dst[1] = x1;
    dst[6] = x4;

    x1 = dst[5];
    x2 = dst[4];

    x4 = descale(x0 + x1, 3);
    x0 = descale(x0 - x1, 3);
    x1 = descale(x3 + x2, 3);
    x3 = descale(x3 - x2, 3);

    dst[2] = x4;
    dst[5] = x0;
    dst[3] = x3;
    dst[4] = x1;
  }
}

static BOOL jpg_load_LoadQuantTables(jpg_reader_t* s, int length)
{
  uchar buffer[128];
  int i, tq_size;

  stream_t* lstrm = s->m_strm;
  length -= 2;

  while (length > 0) {
    int tq = stream_get8(lstrm);
    int size = tq >> 4;
    tq &= 15;

    tq_size = (64 << size) + 1;

    if (tq > 3 || size > 1 || length < tq_size) {
      return FALSE;
    }

    length -= tq_size;

    stream_read(lstrm, buffer, tq_size - 1);

    if (size == 0) { // 8 bit quant factors
      for (i = 0; i < 64; i++) {
        int idx = zigzag[i];
        s->m_tq[tq][idx] = buffer[i] * 16 * idct_prescale[idx];
      }
    }
    else { // 16 bit quant factors
      for (i = 0; i < 64; i++) {
        int idx = zigzag[i];
        s->m_tq[tq][idx] = ((unsigned short*)buffer)[i] * idct_prescale[idx];
      }
    }

    s->m_is_tq[tq] = TRUE;
  }

  return TRUE;
}


static BOOL jpg_load_LoadHuffmanTables(jpg_reader_t* s, int length)
{
  const int max_bits = 16;
  uchar buffer[1024];
  int buffer2[1024];

  int i, ht_size;
  stream_t* lstrm = s->m_strm;
  length -= 2;

  while (length > 0) {
    int t = stream_get8(lstrm);
    int hclass = t >> 4;
    t &= 15;

    if (t > 3 || hclass > 1 || length < 17) {
      return FALSE;
    }

    length -= 17;

    stream_read(lstrm, buffer, max_bits);

    for (i = 0, ht_size = 0; i < max_bits; i++) {
      ht_size += buffer[i];
    }

    if (length < ht_size) {
      return FALSE;
    }

    length -= ht_size;

    stream_read(lstrm, buffer + max_bits, ht_size);

    if (!bsCreateDecodeHuffmanTable(
        bsCreateSourceHuffmanTable(buffer, buffer2, max_bits, first_table_bits),
        hclass == 0 ? s->m_td[t] : s->m_ta[t],
        max_dec_htable_size)) {
      return FALSE;
    }

    if (hclass == 0) {
      s->m_is_td[t] = TRUE;
    }
    else {
      s->m_is_ta[t] = TRUE;
    }
  }

  return TRUE;
}

static void jpg_load_ResetDecoder(jpg_reader_t* s)
{
  s->m_ci[0].dc_pred = s->m_ci[1].dc_pred = s->m_ci[2].dc_pred = 0;
}

static void jpg_load_GetBlock(jpg_reader_t* s, int* block, int c)
{
  memset(block, 0, 64 * sizeof(block[0]));

  assert(0 <= c && c < 3);
  {
    const short* td = s->m_td[(uchar)s->m_ci[c].td];
    const short* ta = s->m_ta[(uchar)s->m_ci[c].ta];
    const int* tq = s->m_tq[(uchar)s->m_ci[c].tq];

    // Get DC coefficient
    int i = 0, cat = jpg_load_GetHuff(s, td);
    int mask = bs_bit_mask[cat];
    int val = jpg_load_Get(s, cat);

    val -= (val * 2 <= mask ? mask : 0);
    s->m_ci[c].dc_pred = val += s->m_ci[c].dc_pred;

    block[0] = descale(val * tq[0], 16);

    // Get AC coeffs
    for (;;) {
      cat = jpg_load_GetHuff(s, ta);

      if (cat == 0) {
        break; // end of block
      }

      i += (cat >> 4) + 1;
      cat &= 15;
      mask = bs_bit_mask[cat];
      val = jpg_load_Get(s, cat);
      cat = zigzag[i];
      val -= (val * 2 <= mask ? mask : 0);
      block[cat] = descale(val * tq[cat], 16);
      assert(i <= 63);

      if (i >= 63) {
        break;
      }
    }
  }
}

static void jpg_load_ProcessScan(jpg_reader_t* s, int* idx, int ns, uchar* data, int step, int req_cn)
{
  int i, t = 0, mcu, x1 = 0, y1 = 0;
  int temp[64];
  int blocks[10][64];
  int pos[3], h[3], v[3];
  int x_shift = 0, y_shift = 0;
  int nch = req_cn;
  int color = s->m_iscolor;

  assert(ns == s->m_planes && s->m_ss == 0 && s->m_se == 63 &&
      s->m_al == 0 && s->m_ah == 0); // sequental & single scan

  assert(idx[0] == 0 && (ns == 1 || (idx[1] == 1 && idx[2] == 2)));

  for (i = 0; i < ns; i++) {
    int c = idx[i];
    h[c] = s->m_ci[c].h * 8;
    v[c] = s->m_ci[c].v * 8;
    pos[c] = t >> 6;
    t += h[c] * v[c];
  }

  if (ns == 3) {
    x_shift = h[0] / (h[1] * 2);
    y_shift = v[0] / (v[1] * 2);
  }

  jpg_load_Flush(s);
  jpg_load_ResetDecoder(s);

  for (mcu = 0;; mcu++) {
    int x2, y2, x, y, xc;
    int* cmp;
    uchar* data1;
    uchar* data2;

    if (mcu == s->m_MCUs && s->m_MCUs != 0) {
      jpg_load_ResetDecoder(s);
      jpg_load_AlignOnByte(s);
      mcu = 0;
    }

    // Get mcu
    for (i = 0; i < ns; i++) {
      int c = idx[i];
      cmp = blocks[pos[c]];

      for (y = 0; y < v[c]; y += 8, cmp += h[c] * 8) {
        for (x = 0; x < h[c]; x += 8) {
          jpg_load_GetBlock(s, temp, c);

          if (i < (color ? 3 : 1)) {
            aan_idct8x8(temp, cmp + x, h[c]);
          }
        }
      }
    }

    y2 = v[0];
    x2 = h[0];

    if (y1 + y2 > s->m_height) {
      y2 = s->m_height - y1;
    }

    if (x1 + x2 > s->m_width) {
      x2 = s->m_width - x1;
    }

    cmp = blocks[0];
    data1 = data + x1 * nch;

    if (ns == 1) {
      for (y = 0; y < y2; y++, data1 += step, cmp += h[0]) {
        data2 = data1;
        if (req_cn>0) {
          for (x = 0; x < x2; x++, data2+=req_cn) {
            int val = descale(cmp[x] + 128 * 4, 2);
            data2[0] = data2[1] = data2[2] = saturate(val);
          }
        }
        else {
          for (x = 0; x < x2; x++, data2+=req_cn) {
            int val = descale(cmp[x] + 128 * 4, 2);
            data2[x] = saturate(val);
          }
        }
      }
    } else {
      for (y = 0; y < y2; y++, data1 += step, cmp += h[0]) {
        data2 = data1;
        if (req_cn>0) {
          int r, g, b;
          int shift = h[1] * (y >> y_shift);
          int* cmpCb = blocks[pos[1]] + shift;
          int* cmpCr = blocks[pos[2]] + shift;
          x = 0;

          if (x_shift == 0) {
            if (1==req_cn) {
              for (; x < x2; x++) {
                int Y = (cmp[x] + 128 * 4) << fixc;
                int Cb = cmpCb[x];
                int Cr = cmpCr[x];
                b = (Y + Cb * b_cb) >> (fixc + 2);
                g = (Y + Cb * g_cb + Cr * g_cr) >> (fixc + 2);
                r = (Y + Cr * r_cr) >> (fixc + 2);
                *data2++ = (saturate(b)+(saturate(g)<<1)+saturate(r))>>2;
              }
            } else if (req_cn>2) {
              for (; x < x2; x++, data2+=req_cn) {
                int Y = (cmp[x] + 128 * 4) << fixc;
                int Cb = cmpCb[x];
                int Cr = cmpCr[x];
                int b = (Y + Cb * b_cb) >> (fixc + 2);
                int g = (Y + Cb * g_cb + Cr * g_cr) >> (fixc + 2);
                int r = (Y + Cr * r_cr) >> (fixc + 2);
                data2[0] = saturate(b);
                data2[1] = saturate(g);
                data2[2] = saturate(r);
                if (req_cn>3) {
                  data2[3] = 255;
                }
              }
            }
          }
          else if (x_shift == 1) {
            if (1==req_cn) {
              for (xc = 0; x <= x2 - 2; x += 2, xc++) {
                uchar* data3 = data2+1;
                int Y = (cmp[x] + 128 * 4) << fixc;
                int Cb = cmpCb[xc];
                int Cr = cmpCr[xc];
                b = (Y + Cb * b_cb) >> (fixc + 2);
                g = (Y + Cb * g_cb + Cr * g_cr) >> (fixc + 2);
                r = (Y + Cr * r_cr) >> (fixc + 2);
                *data2++ = (saturate(b)+(saturate(g)<<1)+saturate(r))>>2;
                Y = (cmp[x + 1] + 128 * 4) << fixc;
                b = (Y + Cb * b_cb) >> (fixc + 2);
                g = (Y + Cb * g_cb + Cr * g_cr) >> (fixc + 2);
                r = (Y + Cr * r_cr) >> (fixc + 2);
                *data2++ = (saturate(b)+(saturate(g)<<1)+saturate(r))>>2;
              }
            } else if (req_cn>2) {
              for (xc = 0; x <= x2 - 2; x += 2, xc++, data2+=2*req_cn) {
                uchar* data3 = data2 + req_cn;
                int Y = (cmp[x] + 128 * 4) << fixc;
                int Cb = cmpCb[xc];
                int Cr = cmpCr[xc];
                b = (Y + Cb * b_cb) >> (fixc + 2);
                g = (Y + Cb * g_cb + Cr * g_cr) >> (fixc + 2);
                r = (Y + Cr * r_cr) >> (fixc + 2);
                data2[0] = saturate(b);
                data2[1] = saturate(g);
                data2[2] = saturate(r);
                Y = (cmp[x + 1] + 128 * 4) << fixc;
                b = (Y + Cb * b_cb) >> (fixc + 2);
                g = (Y + Cb * g_cb + Cr * g_cr) >> (fixc + 2);
                r = (Y + Cr * r_cr) >> (fixc + 2);
                data3[0] = saturate(b);
                data3[1] = saturate(g);
                data3[2] = saturate(r);
                if (req_cn>3) {
                  data3[3] = data2[3] = 255;
                }
              }
            }
          }

          for (; x < x2; x++, data2+=req_cn) {
            int Y = (cmp[x] + 128 * 4) << fixc;
            int Cb = cmpCb[x >> x_shift];
            int Cr = cmpCr[x >> x_shift];
            int t = (Y + Cb * b_cb) >> (fixc + 2);
            data2[0] = saturate(t);
            t = (Y + Cb * g_cb + Cr * g_cr) >> (fixc + 2);
            data2[1] = saturate(t);
            t = (Y + Cr * r_cr) >> (fixc + 2);
            data2[2] = saturate(t);
            if (req_cn>3) {
              data2[3] = 255;
            }
          }
        }
        else {
          for (x = 0; x < x2; x++) {
            int val = descale(cmp[x] + 128 * 4, 2);
            data1[x] = saturate(val);
          }
        }
      }
    }

    x1 += h[0];

    if (x1 >= s->m_width) {
      x1 = 0;
      y1 += v[0];
      data += v[0] * step;

      if (y1 >= s->m_height) {
        break;
      }
    }
  }
}

static BOOL jpg_load_ReadData(jpg_reader_t* s, uchar* data, int step, int req_cn)
{
  if (s->m_offset < 0 || !s->m_strm) {
    return FALSE;
  }

  if (setjmp(s->m_jmp_buf) == 0) {
    stream_t* lstrm = s->m_strm;
    stream_seek(lstrm, s->m_offset, SEEK_SET);

    for (;;) {
      int marker = jpg_load_FindMarker(s) & 255;

      if (marker == 0xD8 /* SOI */ || marker == 0xD9 /* EOI */) {
        goto decoding_end;
      }

      // check for standalone markers
      if (marker != 0x01 /* TEM */ && !(0xD0 <= marker && marker <= 0xD7)) {
        int pos = stream_tell(lstrm);
        int length = stream_get16be(lstrm);

        if (0xC4==marker) { // DHT
          if (!jpg_load_LoadHuffmanTables(s, length)) {
            goto decoding_end;
          }
        } else if (0xDA==marker) {// SOS
          // read scan header
          int idx[3] = { -1, -1, -1 };
          int i, ns = stream_get8(lstrm);
          int sum = 0, a; // spectral selection & approximation
          
          if (ns != s->m_planes) {
            goto decoding_end;
          }
          
          for (i = 0; i < ns; i++) {
            int td, ta, c = stream_get8(lstrm) - 1;
            
            if (c < 0 || s->m_planes <= c) {
              c = i; // hack
            }
            
            if (idx[c] != -1) {
              goto decoding_end;
            }
            
            idx[i] = c;
            td = stream_get8(lstrm);
            ta = td & 15;
            td >>= 4;
            
            if (!(ta <= 3 && s->m_is_ta[ta] &&
              td <= 3 && s->m_is_td[td] &&
              s->m_is_tq[(uchar)s->m_ci[c].tq])) {
              goto decoding_end;
            }
            
            s->m_ci[c].td = (char)td;
            s->m_ci[c].ta = (char)ta;
            
            sum += s->m_ci[c].h * s->m_ci[c].v;
          }
          
          if (sum > 10) {
            goto decoding_end;
          }
          
          s->m_ss = stream_get8(lstrm);
          s->m_se = stream_get8(lstrm);
          
          a = stream_get8(lstrm);
          s->m_al = a & 15;
          s->m_ah = a >> 4;
          
          jpg_load_ProcessScan(s, idx, ns, data, step, req_cn);
          goto decoding_end; // only single scan case is supported now
        } else if (0xDD==marker) {
          // DRI
          //s->m_offset = pos - 2;
          //break;
          s->m_MCUs = stream_get16be(lstrm);
        }
        
        if (marker != 0xDA) {
          stream_seek(lstrm, pos + length, SEEK_SET);
        }
      }
    }

decoding_end:
    ;
  }

  return TRUE;
}



static BOOL jpg_load_ReadHeader(jpg_reader_t* s)
{
  char buffer[16];
  int i;
  BOOL result = FALSE, is_sof = FALSE, is_qt = FALSE, is_ht = FALSE, is_sos = FALSE;
  stream_t* lstrm = s->m_strm;

  memset(s->m_is_tq, 0, sizeof(s->m_is_tq));
  memset(s->m_is_td, 0, sizeof(s->m_is_td));
  memset(s->m_is_ta, 0, sizeof(s->m_is_ta));
  s->m_MCUs = 0;
  stream_seek(lstrm, 0, SEEK_SET);
  stream_read(lstrm, buffer, 4);

  if (memcmp(buffer, "\xFF\xD8\xFF", 3)) {
    return 0;
  }

  if (setjmp(s->m_jmp_buf) == 0) {
    //"\xFF\xD8\xFF"
    stream_seek(lstrm, 0, SEEK_SET);
    stream_seek(lstrm, 2, SEEK_CUR); // skip SOI marker

    for (;;) {
      int marker = jpg_load_FindMarker(s) & 255;

      // check for standalone markers
      if (marker != 0xD8 /* SOI */ && marker != 0xD9 /* EOI */ &&
          marker != 0x01 /* TEM */ && !(0xD0 <= marker && marker <= 0xD7)) {
        int pos = stream_tell(lstrm);
        int length = stream_get16be(lstrm);

        switch (marker) {
        case 0xE0: // APP0
          stream_read(lstrm, buffer, 5);

          if (strcmp(buffer, "JFIF") == 0) { // JFIF identification
            s->m_version = stream_get16be(lstrm);
            //is_jfif = TRUE;
          }

          break;

        case 0xC0: // SOF0
          s->m_precision = stream_get8(lstrm);
          s->m_height = stream_get16be(lstrm);
          s->m_width = stream_get16be(lstrm);
          s->m_planes = stream_get8(lstrm);

          if (s->m_width == 0 || s->m_height == 0 || // DNL not supported
              (s->m_planes != 1 && s->m_planes != 3)) {
            goto parsing_end;
          }

          s->m_iscolor = s->m_planes == 3;

          memset(s->m_ci, -1, sizeof(s->m_ci));

          for (i = 0; i < s->m_planes; i++) {
            int idx = stream_get8(lstrm);
            cmp_info* ci = NULL;

            if (idx < 1 || idx > s->m_planes) { // wrong index
              idx = i + 1; // hack
            }

            ci = &(s->m_ci[idx - 1]);

            if (ci->tq > 0 /* duplicated description */) {
              goto parsing_end;
            }

            ci->h = (char) stream_get8(lstrm);
            ci->v = (char)(ci->h & 15);
            ci->h >>= 4;
            ci->tq = (char)stream_get8(lstrm);

            if (!((ci->h == 1 || ci->h == 2 || ci->h == 4) &&
                (ci->v == 1 || ci->v == 2 || ci->v == 4) &&
                ci->tq < 3) ||
                // chroma mcu-parts should have equal sizes and
                // be non greater then luma sizes
                !(i != 2 || (ci->h == s->m_ci[1].h && ci->v == s->m_ci[1].v &&
                    ci->h <= s->m_ci[0].h && ci->v <= s->m_ci[0].v))) {
              goto parsing_end;
            }
          }

          is_sof = TRUE;
          s->m_type = marker - 0xC0;
          break;

        case 0xDB: // DQT
          if (!jpg_load_LoadQuantTables(s, length)) {
            goto parsing_end;
          }

          is_qt = TRUE;
          break;

        case 0xC4: // DHT
          if (!jpg_load_LoadHuffmanTables(s, length)) {
            goto parsing_end;
          }

          is_ht = TRUE;
          break;

        case 0xDA: // SOS
          is_sos = TRUE;
          s->m_offset = pos - 2;
          goto parsing_end;

        case 0xDD: // DRI
          s->m_MCUs = stream_get16be(lstrm);
          break;
        }

        stream_seek(lstrm, pos + length, SEEK_SET);
      }
    }

parsing_end:
    ;
  }

  result = /*is_jfif &&*/ is_sof && is_qt && is_ht && is_sos;

  if (!result) {
    s->m_width = s->m_height = -1;
    s->m_offset = -1;
  }

  return result;
}



////////////////////// WJpegStream ///////////////////////

//////////////////// JPEG-specific output bitstream ///////////////////////



/////////////////////// GrFmtJpegWriter ///////////////////


// Standard JPEG quantization tables
static const uchar jpegTableK1_T[] = {
  16, 12, 14, 14, 18, 24, 49, 72,
  11, 12, 13, 17, 22, 35, 64, 92,
  10, 14, 16, 22, 37, 55, 78, 95,
  16, 19, 24, 29, 56, 64, 87, 98,
  24, 26, 40, 51, 68, 81, 103, 112,
  40, 58, 57, 87, 109, 104, 121, 100,
  51, 60, 69, 80, 103, 113, 120, 103,
  61, 55, 56, 62, 77, 92, 101, 99
};


static const uchar jpegTableK2_T[] = {
  17, 18, 24, 47, 99, 99, 99, 99,
  18, 21, 26, 66, 99, 99, 99, 99,
  24, 26, 56, 99, 99, 99, 99, 99,
  47, 66, 99, 99, 99, 99, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99
};


// Standard Huffman tables

// ... for luma DCs.
static const uchar jpegTableK3[] = {
  0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};


// ... for chroma DCs.
static const uchar jpegTableK4[] = {
  0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};


// ... for luma ACs.
static const uchar jpegTableK5[] = {
  0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 125,
  0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
  0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
  0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
  0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
  0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
  0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
  0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
  0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
  0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
  0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
  0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
  0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
  0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
  0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
  0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
  0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
  0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
  0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
  0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
  0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
  0xf9, 0xfa
};

// ... for chroma ACs
static const uchar jpegTableK6[] = {
  0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 119,
  0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
  0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
  0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
  0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
  0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
  0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
  0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
  0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
  0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
  0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
  0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
  0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
  0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
  0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
  0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
  0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
  0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
  0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
  0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
  0xf9, 0xfa
};


static const char jpegHeader[] =
    "\xFF\xD8" // SOI - start of image
    "\xFF\xE0" // APP0 - jfif extention
    "\x00\x10" // 2 bytes: length of APP0 segment
    "JFIF\x00" // JFIF signature
    "\x01\x02" // version of JFIF
    "\x00" // units = pixels ( 1 - inch, 2 - cm )
    "\x00\x01\x00\x01" // 2 2-bytes values: x density & y density
    "\x00\x00"; // width & height of thumbnail: ( 0x0 means no thumbnail)

#define postshift 14

// FDCT with postscaling
static void aan_fdct8x8(int* src, int* dst, int step, const int* postscale)
{
  int workspace[64], *work = workspace;
  int i;

  // Pass 1: process rows
  for (i = 8; i > 0; i--, src += step, work += 8) {
    int x0 = src[0], x1 = src[7];
    int x2 = src[3], x3 = src[4];

    int x4 = x0 + x1;
    x0 -= x1;
    x1 = x2 + x3;
    x2 -= x3;

    work[7] = x0;
    work[1] = x2;
    x2 = x4 + x1;
    x4 -= x1;

    x0 = src[1];
    x3 = src[6];
    x1 = x0 + x3;
    x0 -= x3;
    work[5] = x0;

    x0 = src[2];
    x3 = src[5];
    work[3] = x0 - x3;
    x0 += x3;

    x3 = x0 + x1;
    x0 -= x1;
    x1 = x2 + x3;
    x2 -= x3;

    work[0] = x1;
    work[4] = x2;

    x0 = descale((x0 - x4) * C0_707, fixb);
    x1 = x4 + x0;
    x4 -= x0;
    work[2] = x4;
    work[6] = x1;

    x0 = work[1];
    x1 = work[3];
    x2 = work[5];
    x3 = work[7];

    x0 += x1;
    x1 += x2;
    x2 += x3;
    x1 = descale(x1 * C0_707, fixb);

    x4 = x1 + x3;
    x3 -= x1;
    x1 = (x0 - x2) * C0_382;
    x0 = descale(x0 * C0_541 + x1, fixb);
    x2 = descale(x2 * C1_306 + x1, fixb);

    x1 = x0 + x3;
    x3 -= x0;
    x0 = x4 + x2;
    x4 -= x2;

    work[5] = x1;
    work[1] = x0;
    work[7] = x4;
    work[3] = x3;
  }

  work = workspace;

  // pass 2: process columns
  for (i = 8; i > 0; i--, work++, postscale += 8, dst += 8) {
    int x0 = work[8 * 0], x1 = work[8 * 7];
    int x2 = work[8 * 3], x3 = work[8 * 4];

    int x4 = x0 + x1;
    x0 -= x1;
    x1 = x2 + x3;
    x2 -= x3;

    work[8 * 7] = x0;
    work[8 * 0] = x2;
    x2 = x4 + x1;
    x4 -= x1;

    x0 = work[8 * 1];
    x3 = work[8 * 6];
    x1 = x0 + x3;
    x0 -= x3;
    work[8 * 4] = x0;

    x0 = work[8 * 2];
    x3 = work[8 * 5];
    work[8 * 3] = x0 - x3;
    x0 += x3;

    x3 = x0 + x1;
    x0 -= x1;
    x1 = x2 + x3;
    x2 -= x3;

    dst[0] = descale(x1 * postscale[0], postshift);
    dst[4] = descale(x2 * postscale[4], postshift);

    x0 = descale((x0 - x4) * C0_707, fixb);
    x1 = x4 + x0;
    x4 -= x0;

    dst[2] = descale(x4 * postscale[2], postshift);
    dst[6] = descale(x1 * postscale[6], postshift);

    x0 = work[8 * 0];
    x1 = work[8 * 3];
    x2 = work[8 * 4];
    x3 = work[8 * 7];

    x0 += x1;
    x1 += x2;
    x2 += x3;
    x1 = descale(x1 * C0_707, fixb);

    x4 = x1 + x3;
    x3 -= x1;
    x1 = (x0 - x2) * C0_382;
    x0 = descale(x0 * C0_541 + x1, fixb);
    x2 = descale(x2 * C1_306 + x1, fixb);

    x1 = x0 + x3;
    x3 -= x0;
    x0 = x4 + x2;
    x4 -= x2;

    dst[5] = descale(x1 * postscale[5], postshift);
    dst[1] = descale(x0 * postscale[1], postshift);
    dst[7] = descale(x4 * postscale[7], postshift);
    dst[3] = descale(x3 * postscale[3], postshift);
  }
}

static int jpg_load2(stream_t* f, int req_cn, img_t* im)
{
  int ret = 0;
  jpg_reader_t* s = (jpg_reader_t*)malloc(sizeof(jpg_reader_t));
  memset(s, 0, sizeof(jpg_reader_t));
  stream_seek(f, 0, SEEK_SET);
  // init
  jpg_load_Open(s, f);

  if ((ret = jpg_load_ReadHeader(s))!=0) {
    int cn = s->m_iscolor ? 3 : 1;
    req_cn = req_cn < 1 ? cn : req_cn;
    imsetsize(im, s->m_height, s->m_width, req_cn, 1);
    jpg_load_ReadData(s, im->tt.data, im->s, req_cn);
    free(s);
  }

  return ret;
}
#define jpg_load2_filename(fname, cn_req, im) jpg_load2_file_close(fopen(fname, "rb"), cn_req, im)
static int jpg_load2_file_close(FILE* pf, int cn_req, img_t* im)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = jpg_load2(s, cn_req, im);
    fclose(pf);
  }

  return ret;
}

