
typedef struct bufio_t {
  uchar* buf;
  int i;
  int buflen;
}bufio_t;

#define PutBytes(h, p, n)  (memcpy(h->buf + h->i, p, n), h->i+=n)
#define PutByte(h, x)      { uchar _uc=x; PutBytes(h, &_uc, 1); }
#define PutWord(h, x)      { ushort _us=x; PutBytes(h, &_us, 2); }

#define  BS_DEF_BLOCK_SIZE   (1<<15)

#define BSWAP(v)    (((v)<<24)|(((v)&0xff00)<<8)| \
(((v)>>8)&0xff00)|((unsigned)(v)>>24))

const uint32 bs_bit_mask[] = {
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

void bsBSwapBlock(uchar* start, uchar* end)
{
  uint32* data = (uint32*)start;
  int i, size = (int)(end - start + 3) / 4;
  
  for (i = 0; i < size; i++) {
    uint32 temp = data[i];
    temp = BSWAP(temp);
    data[i] = temp;
  }
}

int m_bit_idx=32;
uint32 m_val=0;
uchar* m_current=0;
int m_pad_val=0;

void Put( bufio_t* xx, int val, int bits )
{
  int  bit_idx = m_bit_idx - bits;
  uint32  curval = m_val;
  
  assert( 0 <= bits && bits < 32 );
  
  val &= bs_bit_mask[bits];
  
  if( bit_idx >= 0 )
  {
    curval |= val << bit_idx;
  }
  else
  {
    *((uint32*)m_current)++ = curval | ((unsigned)val >> -bit_idx);
    bit_idx += 32;
    curval = val << bit_idx;
  }
  
  m_val = curval;
  m_bit_idx = bit_idx;
}


void Flush(bufio_t* xx)
{
  if( m_bit_idx < 32 )
  {
    Put( xx, m_pad_val, m_bit_idx & 7 );
    *((uint32*)m_current)++ = m_val;
  }
}

void  PutHuff( bufio_t* xx, int val, const uint32* table )
{
  uint32 code;
  int min_val = (int)table[0];
  val -= min_val;
  
  assert( (unsigned)val < table[1] );
  
  code = table[val + 2];
  assert( code != 0 );
  
  Put( xx, code >> 8, code & 255 );
}

#define huff_val_shift  20
#define huff_code_mask  ((1 << huff_val_shift) - 1)

#define RBS_THROW_EOS    -123  /* <end of stream> exception code */
#define RBS_THROW_FORB   -124  /* <forrbidden huffman code> exception code */
#define RBS_HUFF_FORB    2047  /* forrbidden huffman code "value" */

bool bsCreateDecodeHuffmanTable( const int* src, short* table, int max_size )
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
        return false;
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
    return true;
}


int*  bsCreateSourceHuffmanTable( const uchar* src, int* dst,
                                  int max_bits, int first_bits )
{
    int i, k, val_idx, code = 0;
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


bool bsCreateEncodeHuffmanTable( const int* src, uint32* table, int max_size )
{   
  int  i, k;
  int  min_val = INT_MAX, max_val = INT_MIN;
  int  size;
  
  /* calc min and max values in the table */
  for( i = 1, k = 1; src[k] >= 0; i++ )
  {
    int code_count = src[k++];
    
    for( code_count += k; k < code_count; k++ )
    {
      int  val = src[k] >> huff_val_shift;
      if( val < min_val )
        min_val = val;
      if( val > max_val )
        max_val = val;
    }
  }
  
  size = max_val - min_val + 3;
  
  if( size > max_size )
  {
    assert(0);
    return false;
  }
  
  memset( table, 0, size*sizeof(table[0]));
  
  table[0] = min_val;
  table[1] = size - 2;
  
  for( i = 1, k = 1; src[k] >= 0; i++ )
  {
    int code_count = src[k++];
    
    for( code_count += k; k < code_count; k++ )
    {
      int  val = src[k] >> huff_val_shift;
      int  code = src[k] & huff_code_mask;
      
      table[val - min_val + 2] = (code << 8) | i;
    }
  }
  return true;
}


#include "jpgr.inl"
#include "jpgw.inl"
