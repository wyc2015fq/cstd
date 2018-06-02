
#define stream_setpos(stream, _pos)  stream_seek(stream, _pos, SEEK_SET)
#define stream_skip(stream, _N)      stream_seek(stream, _N, SEEK_CUR)
#define AlignOnByte(stream) (m_bit_idx &= -8)
#undef descale
#define descale(x,n)  (((x) + (1 << ((n)-1))) >> (n))
#define saturate(x)   (uchar)(((x) & ~255) == 0 ? (x) : ~((x)>>31))

typedef unsigned long ulong;

// huff

#define huff_val_shift  20
#define huff_code_mask  ((1 << huff_val_shift) - 1)

#define  RBS_THROW_EOS    -123  /* <end of stream> exception code */
#define  RBS_THROW_FORB   -124  /* <forrbidden huffman code> exception code */
#define  RBS_HUFF_FORB    2047  /* forrbidden huffman code "value" */

int* bsCreateSourceHuffmanTable( const uchar* src, int* dst,
                                 int max_bits, int first_bits )
{
  int   i, k, val_idx, code = 0;
  int*  table = dst;
  *dst++ = first_bits;
  for( i = 1, val_idx = max_bits; i <= max_bits; i++ )
  {
    int code_count = src[i - 1];
    dst[0] = code_count;
    code <<= 1;
    for( k = 0; k < code_count; k++ )
    {
      dst[k + 1] = (src[val_idx + k] << huff_val_shift)|(code + k);
    }
    code += code_count;
    dst += code_count + 1;
    val_idx += code_count;
  }
  dst[0] = -1;
  return  table;
}


BOOL bsCreateDecodeHuffmanTable( const int* src, short* table, int max_size )
{   
  const int forbidden_entry = (RBS_HUFF_FORB << 4)|1;
  int       first_bits = src[0];
  struct
  {
    int bits;
    int offset;
  }
  sub_tables[1 << 11];
  int  size = (1 << first_bits) + 1;
  int  i, k;
  
  /* calc bit depths of sub tables */
  memset( sub_tables, 0, ((size_t)1 << first_bits)*sizeof(sub_tables[0]) );
  for( i = 1, k = 1; src[k] >= 0; i++ )
  {
    int code_count = src[k++];
    int sb = i - first_bits;
    
    if( sb <= 0 )
      k += code_count;
    else
      for( code_count += k; k < code_count; k++ )
      {
        int  code = src[k] & huff_code_mask;
        sub_tables[code >> sb].bits = sb;
      }
  }
  
  /* calc offsets of sub tables and whole size of table */
  for( i = 0; i < (1 << first_bits); i++ )
  {
    int b = sub_tables[i].bits;
    if( b > 0 )
    {
      b = 1 << b;
      sub_tables[i].offset = size;
      size += b + 1;
    }
  }
  
  if( size > max_size )
  {
    assert(0);
    return FALSE;
  }
  
  /* fill first table and subtables with forbidden values */
  for( i = 0; i < size; i++ )
  {
    table[i] = (short)forbidden_entry;
  }
  
  /* write header of first table */
  table[0] = (short)first_bits;
  
  /* fill first table and sub tables */ 
  for( i = 1, k = 1; src[k] >= 0; i++ )
  {
    int code_count = src[k++];
    for( code_count += k; k < code_count; k++ )
    {
      int  table_bits= first_bits;
      int  code_bits = i;
      int  code = src[k] & huff_code_mask;
      int  val  = src[k] >>huff_val_shift;
      int  j, offset = 0;
      
      if( code_bits > table_bits )
      {
        int idx = code >> (code_bits -= table_bits);
        code &= (1 << code_bits) - 1;
        offset   = sub_tables[idx].offset;
        table_bits= sub_tables[idx].bits;
        /* write header of subtable */
        table[offset]  = (short)table_bits;
        /* write jump to subtable */
        table[idx + 1]= (short)(offset << 4);
      }
      
      table_bits -= code_bits;
      assert( table_bits >= 0 );
      val = (val << 4) | code_bits;
      offset += (code << table_bits) + 1;
      
      for( j = 0; j < (1 << table_bits); j++ )
      {
        assert( table[offset + j] == forbidden_entry );
        table[ offset + j ] = (short)val;
      }
    }
  }
  return TRUE;
}




// JPEG filter factory
int m_sign_len = 3;
const char* m_signature = "\xFF\xD8\xFF";
const char* m_description = "JPEG files (*.jpeg;*.jpg;*.jpe)";
int m_bit_idx;

//////////////////////  JPEG-oriented two-level bitstream ////////////////////////

#define FindMarker(stream, code) { code = stream_getu2(stream); \
  while( (code & 0xFF00) != 0xFF00 || (code == 0xFFFF || code == 0xFF00 )) {\
    code = ((code&255) << 8) | stream_getu1(stream); } }

/****************************** JPEG (JFIF) reader ***************************/

// zigzag & IDCT prescaling (AAN algorithm) tables
static const uchar zigzag[] =
{
  0,  8,  1,  2,  9, 16, 24, 17, 10,  3,  4, 11, 18, 25, 32, 40,
 33, 26, 19, 12,  5,  6, 13, 20, 27, 34, 41, 48, 56, 49, 42, 35,
 28, 21, 14,  7, 15, 22, 29, 36, 43, 50, 57, 58, 51, 44, 37, 30,
 23, 31, 38, 45, 52, 59, 60, 53, 46, 39, 47, 54, 61, 62, 55, 63,
 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63
};


static const int idct_prescale[] =
{
    16384, 22725, 21407, 19266, 16384, 12873,  8867,  4520,
    22725, 31521, 29692, 26722, 22725, 17855, 12299,  6270,
    21407, 29692, 27969, 25172, 21407, 16819, 11585,  5906,
    19266, 26722, 25172, 22654, 19266, 15137, 10426,  5315,
    16384, 22725, 21407, 19266, 16384, 12873,  8867,  4520,
    12873, 17855, 16819, 15137, 12873, 10114,  6967,  3552,
     8867, 12299, 11585, 10426,  8867,  6967,  4799,  2446,
     4520,  6270,  5906,  5315,  4520,  3552,  2446,  1247
};

#undef fix
#define fixb         14
#define fix(x, n)    (int)((x)*(1 << (n)) + .5)
#define fix1(x, n)   (x)
#define fixmul(x)    (x)

#define C0_707     fix( 0.707106781f, fixb )
#define C0_924     fix( 0.923879533f, fixb )
#define C0_541     fix( 0.541196100f, fixb )
#define C0_382     fix( 0.382683432f, fixb )
#define C1_306     fix( 1.306562965f, fixb )

#define C1_082     fix( 1.082392200f, fixb )
#define C1_414     fix( 1.414213562f, fixb )
#define C1_847     fix( 1.847759065f, fixb )
#define C2_613     fix( 2.613125930f, fixb )

#define fixc       12
#define b_cb       fix( 1.772, fixc )
#define g_cb      -fix( 0.34414, fixc )
#define g_cr      -fix( 0.71414, fixc )
#define r_cr       fix( 1.402, fixc )

#define y_r        fix( 0.299, fixc )
#define y_g        fix( 0.587, fixc )
#define y_b        fix( 0.114, fixc )

#define cb_r      -fix( 0.1687, fixc )
#define cb_g      -fix( 0.3313, fixc )
#define cb_b       fix( 0.5,    fixc )

#define cr_r       fix( 0.5,    fixc )
#define cr_g      -fix( 0.4187, fixc )
#define cr_b      -fix( 0.0813, fixc )


// IDCT without prescaling
static void aan_idct8x8( int *src, int *dst, int step )
{
    int   workspace[64], *work = workspace;
    int   i;

    /* Pass 1: process rows */
    for( i = 8; i > 0; i--, src += 8, work += 8 )
    {
        /* Odd part */
        int  x0 = src[5], x1 = src[3];
        int  x2 = src[1], x3 = src[7];

        int  x4 = x0 + x1; x0 -= x1;

        x1 = x2 + x3; x2 -= x3;
        x3 = x1 + x4; x1 -= x4;

        x4 = (x0 + x2)*C1_847;
        x0 = descale( x4 - x0*C2_613, fixb);
        x2 = descale( x2*C1_082 - x4, fixb);
        x1 = descale( x1*C1_414, fixb);

        x0 -= x3;
        x1 -= x0;
        x2 += x1;

        work[7] = x3; work[6] = x0;
        work[5] = x1; work[4] = x2;

        /* Even part */
        x2 = src[2]; x3 = src[6];
        x0 = src[0]; x1 = src[4];

        x4 = x2 + x3;
        x2 = descale((x2-x3)*C1_414, fixb) - x4;

        x3 = x0 + x1; x0 -= x1;
        x1 = x3 + x4; x3 -= x4;
        x4 = x0 + x2; x0 -= x2;

        x2 = work[7];
        x1 -= x2; x2 = 2*x2 + x1;
        work[7] = x1; work[0] = x2;

        x2 = work[6];
        x1 = x4 + x2; x4 -= x2;
        work[1] = x1; work[6] = x4;

        x1 = work[5]; x2 = work[4];
        x4 = x0 + x1; x0 -= x1;
        x1 = x3 + x2; x3 -= x2;

        work[2] = x4; work[5] = x0;
        work[3] = x3; work[4] = x1;
    }

    /* Pass 2: process columns */
    work = workspace;
    for( i = 8; i > 0; i--, dst += step, work++ )
    {
        /* Odd part */
        int  x0 = work[8*5], x1 = work[8*3];
        int  x2 = work[8*1], x3 = work[8*7];

        int  x4 = x0 + x1; x0 -= x1;
        x1 = x2 + x3; x2 -= x3;
        x3 = x1 + x4; x1 -= x4;

        x4 = (x0 + x2)*C1_847;
        x0 = descale( x4 - x0*C2_613, fixb);
        x2 = descale( x2*C1_082 - x4, fixb);
        x1 = descale( x1*C1_414, fixb);

        x0 -= x3;
        x1 -= x0;
        x2 += x1;

        dst[7] = x3; dst[6] = x0;
        dst[5] = x1; dst[4] = x2;

        /* Even part */
        x2 = work[8*2]; x3 = work[8*6];
        x0 = work[8*0]; x1 = work[8*4];

        x4 = x2 + x3;
        x2 = descale((x2-x3)*C1_414, fixb) - x4;

        x3 = x0 + x1; x0 -= x1;
        x1 = x3 + x4; x3 -= x4;
        x4 = x0 + x2; x0 -= x2;

        x2 = dst[7];
        x1 -= x2; x2 = 2*x2 + x1;
        x1 = descale(x1,3);
        x2 = descale(x2,3);

        dst[7] = x1; dst[0] = x2;

        x2 = dst[6];
        x1 = descale(x4 + x2,3);
        x4 = descale(x4 - x2,3);
        dst[1] = x1; dst[6] = x4;

        x1 = dst[5]; x2 = dst[4];

        x4 = descale(x0 + x1,3);
        x0 = descale(x0 - x1,3);
        x1 = descale(x3 + x2,3);
        x3 = descale(x3 - x2,3);
       
        dst[2] = x4; dst[5] = x0;
        dst[3] = x3; dst[4] = x1;
    }
}


int   m_offset; // offset of first scan
int   m_version; // JFIF version
int   m_planes; // 3 (YCrCb) or 1 (Gray)
int   m_precision; // 8 or 12-bit per sample
int   m_type; // SOF type
int   m_MCUs; // MCUs in restart interval
int   m_ss, m_se, m_ah, m_al; // progressive JPEG parameters

// information about each component
typedef struct cmp_info
{
  char h;  // horizontal sampling factor
  char v;  // vertical   sampling factor
  char tq; // quantization table index
  char td, ta; // DC & AC huffman tables
  int  dc_pred; // DC predictor
} cmp_info;

cmp_info m_ci[3];

#define max_dec_htable_size   (1 << 12)
#define first_table_bits      (9)
short m_td[4][max_dec_htable_size];
short m_ta[4][max_dec_htable_size];
int m_planes;
int m_offset;
JpegReaderInit()
{
  int i;
    m_planes= -1;
    m_offset= -1;

    for( i = 0; i < 4; i++ )
    {
      FILL(max_dec_htable_size, m_td[i], 0);
      FILL(max_dec_htable_size, m_ta[i], 0);
    }
}

int m_iscolor;
int m_width;
int m_height;
int  m_version;
int  m_MCUs;
int  m_tq[4][64];
BOOL m_is_tq[4];
BOOL m_is_td[4];
BOOL m_is_ta[4];

BOOL LoadQuantTables( stream_t* m_strm, int length )
{
  uchar buffer[128];
  int  i, tq_size;
  
  length -= 2;
  
  while( length > 0 )
  {
    int tq = stream_getu1(m_strm);
    int size = tq >> 4;
    tq &= 15;
    
    tq_size = (64<<size) + 1; 
    if( tq > 3 || size > 1 || length < tq_size ) return FALSE;
    length -= tq_size;
    
    stream_read(m_strm, buffer, tq_size - 1 );
    
    if( size == 0 ) // 8 bit quant factors
    {
      for( i = 0; i < 64; i++ )
      {
        int idx = zigzag[i];
        m_tq[tq][idx] = buffer[i] * 16 * idct_prescale[idx];
      }
    }
    else // 16 bit quant factors
    {
      for( i = 0; i < 64; i++ )
      {
        int idx = zigzag[i];
        m_tq[tq][idx] = ((unsigned short*)buffer)[i] * idct_prescale[idx];
      }
    }
    m_is_tq[tq] = TRUE;
  }
  
  return TRUE;
}


BOOL LoadHuffmanTables( stream_t* m_strm, int length )
{
  const int max_bits = 16;
  uchar buffer[1024];
  int  buffer2[1024];
  
  int  i, ht_size;
  length -= 2;
  
  while( length > 0 )
  {
    int t = stream_getu1(m_strm);
    int hclass = t >> 4;
    t &= 15;
    
    if( t > 3 || hclass > 1 || length < 17 ) return FALSE;
    length -= 17;
    
    stream_read(m_strm, buffer, max_bits );
    for( i = 0, ht_size = 0; i < max_bits; i++ ) ht_size += buffer[i];
    
    if( length < ht_size ) return FALSE;
    length -= ht_size;
    
    stream_read(m_strm, buffer + max_bits, ht_size );
    
    if( !bsCreateDecodeHuffmanTable( 
      bsCreateSourceHuffmanTable( 
      buffer, buffer2, max_bits, first_table_bits ),
      hclass == 0 ? m_td[t] : m_ta[t], 
      max_dec_htable_size )) return FALSE;
    if( hclass == 0 )
      m_is_td[t] = TRUE;
    else
      m_is_ta[t] = TRUE;
  }
  return TRUE;
}

BOOL jpg_load(stream_t* m_strm)
{
    char buffer[16];
    int  i;
    BOOL result = FALSE, is_sof = FALSE, is_qt = FALSE, is_ht = FALSE, is_sos = FALSE;
    
    JpegReaderInit();
    memset( m_is_tq, 0, sizeof(m_is_tq));
    memset( m_is_td, 0, sizeof(m_is_td));
    memset( m_is_ta, 0, sizeof(m_is_ta));
    m_MCUs = 0;

    //if( setjmp( m_strm.JmpBuf()) == 0 )
    {
        stream_skip( m_strm, 2 ); // skip SOI marker
        
        for(;;)
        {
            int marker;
            FindMarker(m_strm, marker);
            marker &= 255;

            // check for standalone markers
            if( marker != 0xD8 /* SOI */ && marker != 0xD9 /* EOI */ &&
                marker != 0x01 /* TEM */ && !( 0xD0 <= marker && marker <= 0xD7 ))
            {
                int pos = stream_tell(m_strm);
                int length = stream_getu2(m_strm);
            
                switch( marker )
                {
                case 0xE0: // APP0
                    stream_read( m_strm, buffer, 5 );
                    if( strcmp(buffer, "JFIF") == 0 ) // JFIF identification
                    {
                        m_version = stream_getu2(m_strm);
                        //is_jfif = TRUE;
                    }
                    break;

                case 0xC0: // SOF0
                    m_precision = stream_getu1(m_strm);
                    m_height = stream_getu2(m_strm);
                    m_width = stream_getu2(m_strm);
                    m_planes = stream_getu1(m_strm);

                    if( m_width == 0 || m_height == 0 || // DNL not supported
                       (m_planes != 1 && m_planes != 3)) goto parsing_end;

                    m_iscolor = m_planes == 3;
                
                    memset( m_ci, -1, sizeof(m_ci));

                    for( i = 0; i < m_planes; i++ )
                    {
                        cmp_info* ci;
                        int idx = stream_getu1(m_strm);

                        if( idx < 1 || idx > m_planes ) // wrong index
                        {
                            idx = i+1; // hack
                        }
                        ci = &m_ci[idx-1];
                        
                        if( ci->tq > 0 /* duplicated description */) goto parsing_end;

                        ci->h = (char)stream_getu1(m_strm);
                        ci->v = (char)(ci->h & 15);
                        ci->h >>= 4;
                        ci->tq = (char)stream_getu1(m_strm);
                        if( !((ci->h == 1 || ci->h == 2 || ci->h == 4) &&
                              (ci->v == 1 || ci->v == 2 || ci->v == 4) &&
                              ci->tq < 3) ||
                            // chroma mcu-parts should have equal sizes and
                            // be non greater then luma sizes
                            !( i != 2 || (ci->h == m_ci[1].h && ci->v == m_ci[1].v &&
                                          ci->h <= m_ci[0].h && ci->v <= m_ci[0].v)))
                            goto parsing_end;
                    }
                    is_sof = TRUE;
                    m_type = marker - 0xC0;
                    break;

                case 0xDB: // DQT
                    if( !LoadQuantTables( m_strm, length )) goto parsing_end;
                    is_qt = TRUE;
                    break;

                case 0xC4: // DHT
                    if( !LoadHuffmanTables( m_strm, length )) goto parsing_end;
                    is_ht = TRUE;
                    break;

                case 0xDA: // SOS
                    is_sos = TRUE;
                    m_offset = pos - 2;
                    goto parsing_end;

                case 0xDD: // DRI
                    m_MCUs = stream_getu2(m_strm);
                    break;
                }
                stream_setpos(m_strm, pos + length );
            }
        }
parsing_end: ;
    }

    result = /*is_jfif &&*/ is_sof && is_qt && is_ht && is_sos;
    if( !result )
    {
        m_width = m_height = -1;
        m_offset = -1;
    }
    return result;
}

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

#define Move( m_current, shift ) { int new_bit_idx = m_bit_idx - shift; m_current -= (new_bit_idx >> 5)<<2; m_bit_idx  = new_bit_idx & 31; }

int Show( unsigned char* m_current, int bits )
{
  int    bit_idx = m_bit_idx - bits;
  int    mask    = bit_idx >> 31;
  ulong* current = ((ulong*)m_current) - mask;
  
  assert( (unsigned)bits < 32 );
  
  return (((current[-1]<<-bit_idx) & mask)| (current[0] >> bit_idx)) & bs_bit_mask[bits];
}

int  GetHuff( stream_t* m_strm, unsigned char* m_current, const short* table )
{
  int  val;
  int  code_bits;
  
  for(;;)
  {
    int table_bits = table[0];
    val = table[Show(m_current, table_bits) + 2];
    code_bits = val & 15;
    val >>= 4;
    
    if( code_bits != 0 ) break;
    table += val*2;
    Move( m_current, table_bits );
  }
  
  Move( m_current, code_bits );
  if( val == RBS_HUFF_FORB )
  {
  }
  
  return val;
}

int  Get( unsigned char* m_current, int bits )
{
  int    bit_idx = m_bit_idx - bits;
  int    mask    = bit_idx >> 31;
  ulong* current = ((ulong*)m_current) - mask;
  
  assert( (unsigned)bits < 32 );
  
  m_bit_idx = bit_idx &= 31;
  return (((current[-1] << -bit_idx) & mask)|
    (current[0] >> bit_idx)) & bs_bit_mask[bits];
}

void  ResetDecoder()
{
  m_ci[0].dc_pred = m_ci[1].dc_pred = m_ci[2].dc_pred = 0; 
}

void  GetBlock( stream_t* m_strm, uchar* block, int* block, int c )
{
  const short* td = m_td[m_ci[c].td];
  const short* ta = m_ta[m_ci[c].ta];
  const int* tq = m_tq[m_ci[c].tq];
  
  // Get DC coefficient
  int i = 0, cat  = GetHuff(m_strm, td );
  int mask = bs_bit_mask[cat];
  int val  = Get(m_strm, cat );

  memset( block, 0, 64*sizeof(block[0]) );
  assert( 0 <= c && c < 3 );

  val -= (val*2 <= mask ? mask : 0);
  m_ci[c].dc_pred = val += m_ci[c].dc_pred;
  
  block[0] = descale(val * tq[0],16);
  
  // Get AC coeffs
  for(;;)
  {
    cat = GetHuff(m_strm, ta );
    if( cat == 0 ) break; // end of block
    
    i += (cat >> 4) + 1;
    cat &= 15;
    mask = bs_bit_mask[cat];
    val  = Get(m_strm, cat );
    cat  = zigzag[i];
    val -= (val*2 <= mask ? mask : 0);
    block[cat] = descale(val * tq[cat], 16);
    assert( i <= 63 );
    if( i >= 63 ) break;
  }
}

void  ProcessScan( int* idx, int ns, uchar* data, int step, int color )
{
    int   i, s = 0, mcu, x1 = 0, y1 = 0;
    int   temp[64];
    int   blocks[10][64];
    int   pos[3], h[3], v[3];
    int   x_shift = 0, y_shift = 0;
    int   nch = color ? 3 : 1;

    assert( ns == m_planes && m_ss == 0 && m_se == 63 &&
            m_al == 0 && m_ah == 0 ); // sequental & single scan

    assert( idx[0] == 0 && (ns ==1 || (idx[1] == 1 && idx[2] == 2)));

    for( i = 0; i < ns; i++ )
    {
        int c = idx[i];
        h[c] = m_ci[c].h*8;
        v[c] = m_ci[c].v*8;
        pos[c] = s >> 6;
        s += h[c]*v[c];
    }

    if( ns == 3 )
    {
        x_shift = h[0]/(h[1]*2);
        y_shift = v[0]/(v[1]*2);
    }

    ResetDecoder();

    for( mcu = 0;; mcu++ )
    {
        int  x2, y2, x, y, xc;
        int* cmp;
        uchar* data1;
        
        if( mcu == m_MCUs && m_MCUs != 0 )
        {
            ResetDecoder();
            AlignOnByte(m_strm);
            mcu = 0;
        }

        // Get mcu
        for( i = 0; i < ns; i++ )
        {
            int  c = idx[i];
            cmp = blocks[pos[c]];
            for( y = 0; y < v[c]; y += 8, cmp += h[c]*8 )
                for( x = 0; x < h[c]; x += 8 )
                {
                    GetBlock( (uchar*)temp, c );
                    if( i < (color ? 3 : 1))
                    {
                        aan_idct8x8( temp, cmp + x, h[c] );
                    }
                }
        }

        y2 = v[0];
        x2 = h[0];

        if( y1 + y2 > m_height ) y2 = m_height - y1;
        if( x1 + x2 > m_width ) x2 = m_width - x1;

        cmp = blocks[0];
        data1 = data + x1*nch;

        if( ns == 1 )
            for( y = 0; y < y2; y++, data1 += step, cmp += h[0] )
            {
                if( color )
                {
                    for( x = 0; x < x2; x++ )
                    {
                        int val = descale( cmp[x] + 128*4, 2 );
                        data1[x*3] = data1[x*3 + 1] = data1[x*3 + 2] = saturate( val );
                    }
                }
                else
                {
                    for( x = 0; x < x2; x++ )
                    {
                        int val = descale( cmp[x] + 128*4, 2 );
                        data1[x] = saturate( val );
                    }
                }
            }
        else
        {
            for( y = 0; y < y2; y++, data1 += step, cmp += h[0] )
            {
                if( color )
                {
                    int  shift = h[1]*(y >> y_shift);
                    int* cmpCb = blocks[pos[1]] + shift; 
                    int* cmpCr = blocks[pos[2]] + shift;
                    x = 0;
                    if( x_shift == 0 )
                    {
                        for( ; x < x2; x++ )
                        {
                            int Y  = (cmp[x] + 128*4) << fixc;
                            int Cb = cmpCb[x];
                            int Cr = cmpCr[x];
                            int t = (Y + Cb*b_cb) >> (fixc + 2);
                            data1[x*3] = saturate(t);
                            t = (Y + Cb*g_cb + Cr*g_cr) >> (fixc + 2);
                            data1[x*3 + 1] = saturate(t);
                            t = (Y + Cr*r_cr) >> (fixc + 2);
                            data1[x*3 + 2] = saturate(t);
                        }
                    }
                    else if( x_shift == 1 )
                    {
                        for( xc = 0; x <= x2 - 2; x += 2, xc++ )
                        {
                            int Y  = (cmp[x] + 128*4) << fixc;
                            int Cb = cmpCb[xc];
                            int Cr = cmpCr[xc];
                            int t = (Y + Cb*b_cb) >> (fixc + 2);
                            data1[x*3] = saturate(t);
                            t = (Y + Cb*g_cb + Cr*g_cr) >> (fixc + 2);
                            data1[x*3 + 1] = saturate(t);
                            t = (Y + Cr*r_cr) >> (fixc + 2);
                            data1[x*3 + 2] = saturate(t);
                            Y = (cmp[x+1] + 128*4) << fixc;
                            t = (Y + Cb*b_cb) >> (fixc + 2);
                            data1[x*3 + 3] = saturate(t);
                            t = (Y + Cb*g_cb + Cr*g_cr) >> (fixc + 2);
                            data1[x*3 + 4] = saturate(t);
                            t = (Y + Cr*r_cr) >> (fixc + 2);
                            data1[x*3 + 5] = saturate(t);
                        }
                    }
                    for( ; x < x2; x++ )
                    {
                        int Y  = (cmp[x] + 128*4) << fixc;
                        int Cb = cmpCb[x >> x_shift];
                        int Cr = cmpCr[x >> x_shift];
                        int t = (Y + Cb*b_cb) >> (fixc + 2);
                        data1[x*3] = saturate(t);
                        t = (Y + Cb*g_cb + Cr*g_cr) >> (fixc + 2);
                        data1[x*3 + 1] = saturate(t);
                        t = (Y + Cr*r_cr) >> (fixc + 2);
                        data1[x*3 + 2] = saturate(t);
                    }
                }
                else
                {
                    for( x = 0; x < x2; x++ )
                    {
                        int val = descale( cmp[x] + 128*4, 2 );
                        data1[x] = saturate(val);
                    }
                }
            }
        }

        x1 += h[0];
        if( x1 >= m_width )
        {
            x1 = 0;
            y1 += v[0];
            data += v[0]*step;
            if( y1 >= m_height ) break;
        }
    }
}


BOOL ReadData( uchar* data, int step, int color )
{
    //if( setjmp( m_strm.JmpBuf()) == 0 )
    {
        unsigned char* m_strm = m_low_strm;
        stream_setpos(m_strm, m_offset );

        for(;;)
        {
            int marker;
            FindMarker(m_strm, marker);
            marker &= 255;

            if( marker == 0xD8 /* SOI */ || marker == 0xD9 /* EOI */ )
                goto decoding_end;

            // check for standalone markers
            if( marker != 0x01 /* TEM */ && !( 0xD0 <= marker && marker <= 0xD7 ))
            {
                int pos    = stream_tell(m_strm);
                int length = stream_getu2(m_strm);
            
                switch( marker )
                {
                case 0xC4: // DHT
                    if( !LoadHuffmanTables( length )) goto decoding_end;
                    break;

                case 0xDA: // SOS
                    // read scan header
                    {
                        int idx[3] = { -1, -1, -1 };
                        int i, ns = stream_getu1(m_strm);
                        int sum = 0, a; // spectral selection & approximation

                        if( ns != m_planes ) goto decoding_end;
                        for( i = 0; i < ns; i++ )
                        {
                            int td, ta, c = stream_getu1(m_strm) - 1;
                            if( c < 0 || m_planes <= c )
                            {
                                c = i; // hack
                            }
                            
                            if( idx[c] != -1 ) goto decoding_end;
                            idx[i] = c;
                            td = stream_getu1(m_strm);
                            ta = td & 15;
                            td >>= 4;
                            if( !(ta <= 3 && m_is_ta[ta] && 
                                  td <= 3 && m_is_td[td] &&
                                  m_is_tq[m_ci[c].tq]) )
                                goto decoding_end;

                            m_ci[c].td = (char)td;
                            m_ci[c].ta = (char)ta;

                            sum += m_ci[c].h*m_ci[c].v;
                        }

                        if( sum > 10 ) goto decoding_end;

                        m_ss = stream_getu1(m_strm);
                        m_se = stream_getu1(m_strm);

                        a = stream_getu1(m_strm);
                        m_al = a & 15;
                        m_ah = a >> 4;

                        ProcessScan( idx, ns, data, step, color );
                        goto decoding_end; // only single scan case is supported now
                    }

                    //m_offset = pos - 2;
                    //break;

                case 0xDD: // DRI
                    m_MCUs = stream_getu2(m_strm);
                    break;
                }
                
                if( marker != 0xDA ) stream_setpos(m_strm, pos + length );
            }
        }
decoding_end: ;
    }

    return TRUE;
}


/////////////////////// JpegWriter ///////////////////

//  Standard JPEG quantization tables
static const uchar jpegTableK1_T[] =
{
    16, 12, 14, 14,  18,  24,  49,  72,
    11, 12, 13, 17,  22,  35,  64,  92,
    10, 14, 16, 22,  37,  55,  78,  95,
    16, 19, 24, 29,  56,  64,  87,  98,
    24, 26, 40, 51,  68,  81, 103, 112,
    40, 58, 57, 87, 109, 104, 121, 100,
    51, 60, 69, 80, 103, 113, 120, 103,
    61, 55, 56, 62,  77,  92, 101,  99
};


static const uchar jpegTableK2_T[] =
{
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
static const uchar jpegTableK3[] =
{
    0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};


// ... for chroma DCs.
static const uchar jpegTableK4[] =
{
    0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};


// ... for luma ACs.
static const uchar jpegTableK5[] =
{
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
static const uchar jpegTableK6[] =
{
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
    "\xFF\xD8"  // SOI  - start of image
    "\xFF\xE0"  // APP0 - jfif extention
    "\x00\x10"  // 2 bytes: length of APP0 segment
    "JFIF\x00"  // JFIF signature
    "\x01\x02"  // version of JFIF
    "\x00"      // units = pixels ( 1 - inch, 2 - cm )
    "\x00\x01\x00\x01" // 2 2-bytes values: x density & y density
    "\x00\x00"; // width & height of thumbnail: ( 0x0 means no thumbnail)

#define postshift 14

// FDCT with postscaling
static void aan_fdct8x8( int *src, int *dst,
                         int step, const int *postscale )
{
    int  workspace[64], *work = workspace;
    int  i;

    // Pass 1: process rows
    for( i = 8; i > 0; i--, src += step, work += 8 )
    {
        int x0 = src[0], x1 = src[7];
        int x2 = src[3], x3 = src[4];

        int x4 = x0 + x1; x0 -= x1;
        x1 = x2 + x3; x2 -= x3;
    
        work[7] = x0; work[1] = x2;
        x2 = x4 + x1; x4 -= x1;

        x0 = src[1]; x3 = src[6];
        x1 = x0 + x3; x0 -= x3;
        work[5] = x0;

        x0 = src[2]; x3 = src[5];
        work[3] = x0 - x3; x0 += x3;

        x3 = x0 + x1; x0 -= x1;
        x1 = x2 + x3; x2 -= x3;

        work[0] = x1; work[4] = x2;

        x0 = descale((x0 - x4)*C0_707, fixb);
        x1 = x4 + x0; x4 -= x0;
        work[2] = x4; work[6] = x1;

        x0 = work[1]; x1 = work[3];
        x2 = work[5]; x3 = work[7];

        x0 += x1; x1 += x2; x2 += x3;
        x1 = descale(x1*C0_707, fixb);

        x4 = x1 + x3; x3 -= x1;
        x1 = (x0 - x2)*C0_382;
        x0 = descale(x0*C0_541 + x1, fixb);
        x2 = descale(x2*C1_306 + x1, fixb);

        x1 = x0 + x3; x3 -= x0;
        x0 = x4 + x2; x4 -= x2;

        work[5] = x1; work[1] = x0;
        work[7] = x4; work[3] = x3;
    }

    work = workspace;
    // pass 2: process columns
    for( i = 8; i > 0; i--, work++, postscale += 8, dst += 8 )
    {
        int  x0 = work[8*0], x1 = work[8*7];
        int  x2 = work[8*3], x3 = work[8*4];

        int  x4 = x0 + x1; x0 -= x1;
        x1 = x2 + x3; x2 -= x3;
    
        work[8*7] = x0; work[8*0] = x2;
        x2 = x4 + x1; x4 -= x1;

        x0 = work[8*1]; x3 = work[8*6];
        x1 = x0 + x3; x0 -= x3;
        work[8*4] = x0;

        x0 = work[8*2]; x3 = work[8*5];
        work[8*3] = x0 - x3; x0 += x3;

        x3 = x0 + x1; x0 -= x1;
        x1 = x2 + x3; x2 -= x3;

        dst[0] = descale(x1*postscale[0], postshift);
        dst[4] = descale(x2*postscale[4], postshift);

        x0 = descale((x0 - x4)*C0_707, fixb);
        x1 = x4 + x0; x4 -= x0;

        dst[2] = descale(x4*postscale[2], postshift);
        dst[6] = descale(x1*postscale[6], postshift);

        x0 = work[8*0]; x1 = work[8*3];
        x2 = work[8*4]; x3 = work[8*7];

        x0 += x1; x1 += x2; x2 += x3;
        x1 = descale(x1*C0_707, fixb);

        x4 = x1 + x3; x3 -= x1;
        x1 = (x0 - x2)*C0_382;
        x0 = descale(x0*C0_541 + x1, fixb);
        x2 = descale(x2*C1_306 + x1, fixb);

        x1 = x0 + x3; x3 -= x0;
        x0 = x4 + x2; x4 -= x2;

        dst[5] = descale(x1*postscale[5], postshift);
        dst[1] = descale(x0*postscale[1], postshift);
        dst[7] = descale(x4*postscale[7], postshift);
        dst[3] = descale(x3*postscale[3], postshift);
    }
}

#if 0

BOOL WriteImage( int height, int width, const uchar* data, int step, int _channels )
{

    // encode the header and tables
    // for each mcu:
    //   convert rgb to yuv with downsampling (if color).
    //   for every block:
    //     calc dct and quantize
    //     encode block.
    int x, y;
    int i, j;
    const int max_quality = 12;
    int   quality = max_quality;
    unsigned char* lowstrm = m_low_strm;
    int   fdct_qtab[2][64];
    ulong huff_dc_tab[2][16];
    ulong huff_ac_tab[2][256];
    int  channels = _channels > 1 ? 3 : 1;
    int  x_scale = channels > 1 ? 2 : 1, y_scale = x_scale;
    int  dc_pred[] = { 0, 0, 0 };
    int  x_step = x_scale * 8;
    int  y_step = y_scale * 8;
    int  block[6][64];
    int  buffer[1024];
    int  luma_count = x_scale*y_scale;
    int  block_count = luma_count + channels - 1;
    int  Y_step = x_scale*8;
    const int UV_step = 16;
    double inv_quality;
    
    assert( data && width > 0 && height > 0 );

    if( quality < 3 ) quality = 3;
    if( quality > max_quality ) quality = max_quality;

    inv_quality = 1./quality;

    // Encode header
    PutBytes(lowstrm, jpegHeader, sizeof(jpegHeader) - 1 );
    
    // Encode quantization tables
    for( i = 0; i < (channels > 1 ? 2 : 1); i++ )
    {
        const uchar* qtable = i == 0 ? jpegTableK1_T : jpegTableK2_T;
        int chroma_scale = i > 0 ? luma_count : 1;
        
        PutWord(lowstrm, 0xffdb );   // DQT marker
        PutWord(lowstrm, 2 + 65*1 ); // put single qtable
        PutByte(lowstrm, 0*16 + i ); // 8-bit table

        // put coefficients
        for( j = 0; j < 64; j++ )
        {
            int idx = zigzag[j];
            int qval = cvRound(qtable[idx]*inv_quality);
            if( qval < 1 )
                qval = 1;
            if( qval > 255 )
                qval = 255;
            fdct_qtab[i][idx] = cvRound((1 << (postshift + 9))/
                                      (qval*chroma_scale*idct_prescale[idx]));
            PutByte(lowstrm, qval );
        }
    }

    // Encode huffman tables
    for( i = 0; i < (channels > 1 ? 4 : 2); i++ )
    {
        const uchar* htable = i == 0 ? jpegTableK3 : i == 1 ? jpegTableK5 :
                              i == 2 ? jpegTableK4 : jpegTableK6;
        int is_ac_tab = i & 1;
        int idx = i >= 2;
        int tableSize = 16 + (is_ac_tab ? 162 : 12);

        PutWord(lowstrm, 0xFFC4   );      // DHT marker
        PutWord(lowstrm, 3 + tableSize ); // define one huffman table
        PutByte(lowstrm, is_ac_tab*16 + idx ); // put DC/AC flag and table index
        PutBytes(lowstrm, htable, tableSize ); // put table

        bsCreateEncodeHuffmanTable( bsCreateSourceHuffmanTable(
            htable, buffer, 16, 9 ), is_ac_tab ? huff_ac_tab[idx] :
            huff_dc_tab[idx], is_ac_tab ? 256 : 16 );
    }

    // put frame header
    PutWord(lowstrm, 0xFFC0 );          // SOF0 marker
    PutWord(lowstrm, 8 + 3*channels );  // length of frame header
    PutByte(lowstrm, 8 );               // sample precision
    PutWord(lowstrm, height );
    PutWord(lowstrm, width );
    PutByte(lowstrm, channels );        // number of components

    for( i = 0; i < channels; i++ )
    {
        PutByte(lowstrm, i + 1 );  // (i+1)-th component id (Y,U or V)
        if( i == 0 )
            PutByte(lowstrm,x_scale*16 + y_scale); // chroma scale factors
        else
            PutByte(lowstrm,1*16 + 1);
        PutByte(lowstrm, i > 0 ); // quantization table idx
    }

    // put scan header
    PutWord(lowstrm, 0xFFDA );          // SOS marker
    PutWord(lowstrm, 6 + 2*channels );  // length of scan header
    PutByte(lowstrm, channels );        // number of components in the scan

    for( i = 0; i < channels; i++ )
    {
        PutByte(lowstrm, i+1 );             // component id
        PutByte(lowstrm, (i>0)*16 + (i>0) );// selection of DC & AC tables
    }

    PutWord(lowstrm,0*256 + 63);// start and end of spectral selection - for
                                // sequental DCT start is 0 and end is 63

    PutByte(lowstrm, 0 );  // successive approximation bit position 
                           // high & low - (0,0) for sequental DCT  

    // encode data
    for( y = 0; y < height; y += y_step, data += y_step*step )
    {
        for( x = 0; x < width; x += x_step )
        {
            int x_limit = x_step;
            int y_limit = y_step;
            const uchar* rgb_data = data + x*_channels;
            int* Y_data = block[0];

            if( x + x_limit > width ) x_limit = width - x;
            if( y + y_limit > height ) y_limit = height - y;

            memset( block, 0, block_count*64*sizeof(block[0][0]));
            
            if( channels > 1 )
            {
                int* UV_data = block[luma_count];

                for( i = 0; i < y_limit; i++, rgb_data += step, Y_data += Y_step )
                {
                    for( j = 0; j < x_limit; j++, rgb_data += _channels )
                    {
                        int r = rgb_data[2];
                        int g = rgb_data[1];
                        int b = rgb_data[0];

                        int Y = descale( r*y_r + g*y_g + b*y_b, fixc - 2) - 128*4;
                        int U = descale( r*cb_r + g*cb_g + b*cb_b, fixc - 2 );
                        int V = descale( r*cr_r + g*cr_g + b*cr_b, fixc - 2 );
                        int j2 = j >> (x_scale - 1); 

                        Y_data[j] = Y;
                        UV_data[j2] += U;
                        UV_data[j2 + 8] += V;
                    }

                    rgb_data -= x_limit*_channels;
                    if( ((i+1) & (y_scale - 1)) == 0 )
                    {
                        UV_data += UV_step;
                    }
                }
            }
            else
            {
                for( i = 0; i < y_limit; i++, rgb_data += step, Y_data += Y_step )
                {
                    for( j = 0; j < x_limit; j++ )
                        Y_data[j] = rgb_data[j]*4 - 128*4;
                }
            }

            for( i = 0; i < block_count; i++ )
            {
                int is_chroma = i >= luma_count;
                int src_step = x_scale * 8;
                int run = 0, val;
                int* src_ptr = block[i & -2] + (i & 1)*8;
                const ulong* htable = huff_ac_tab[is_chroma];

                aan_fdct8x8( src_ptr, buffer, src_step, fdct_qtab[is_chroma] );

                j = is_chroma + (i > luma_count);
                val = buffer[0] - dc_pred[j];
                dc_pred[j] = buffer[0];

                {
                float a = (float)val;
                int cat = (((int)a >> 23) & 255) - (126 & (val ? -1 : 0));

                assert( cat <= 11 );
                PutHuff(m_strm, cat, huff_dc_tab[is_chroma] );
                Put(m_strm, val - (val < 0 ? 1 : 0), cat );
                }

                for( j = 1; j < 64; j++ )
                {
                    val = buffer[zigzag[j]];

                    if( val == 0 )
                    {
                        run++;
                    }
                    else
                    {
                        while( run >= 16 )
                        {
                            PutHuff(m_strm, 0xF0, htable ); // encode 16 zeros
                            run -= 16;
                        }

                        {
                          float a = (float)val;
                          //int cat = (((int&)a >> 23) & 255) - (126 & (val ? -1 : 0));
                        int cat = (((int)a >> 23) & 255) - (126 & (val ? -1 : 0));

                        assert( cat <= 10 );
                        PutHuff(m_strm, cat + run*16, htable );
                        Put(m_strm, val - (val < 0 ? 1 : 0), cat );
                        }

                        run = 0;
                    }
                }

                if( run )
                {
                    PutHuff(m_strm, 0x00, htable ); // encode EOB
                }
            }
        }
    }

    PutWord(lowstrm, 0xFFD9 ); // EOI marker
    return TRUE;
}

#endif


