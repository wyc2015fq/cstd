#ifndef _LZARI_INL_
#define _LZARI_INL_
/**************************************************************
LZARI.C -- A Data Compression Program
(tab = 4 spaces)
***************************************************************
4/7/1989 Haruhiko Okumura
Use, distribute, and modify this program freely.
Please send me your improved versions.
PC-VAN  SCIENCE
NIFTY-Serve PAF01022
CompuServe 74050,1022
**************************************************************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//#define _OUTPUT_STATUS

#define N		 4096	/* size of ring buffer */
#define F		   60	/* upper limit for match_length */
#define THRESHOLD	2   // encode string into position and length if match_length is greater than this 
#define NIL			N	// index for root of binary search trees */ 
/********** Arithmetic Compression **********/

/*  If you are not familiar with arithmetic compression, you should read
		I. E. Witten, R. M. Neal, and J. G. Cleary,
			Communications of the ACM, Vol. 30, pp. 520-540 (1987),
	from which much have been borrowed.  */

#define M   15

/*	Q1 (= 2 to the M) must be sufficiently large, but not so
	large as the unsigned long 4 * Q1 * (Q1 - 1) overflows.  */

#define Q1  (1UL << M)
#define Q2  (2 * Q1)
#define Q3  (3 * Q1)
#define Q4  (4 * Q1)
#define MAX_CUM (Q1 - 1)

#define N_CHAR  (256 - THRESHOLD + F)

typedef struct LZARI {
  unsigned long textsize;
  unsigned long codesize;
  unsigned long printcount;
  unsigned char text_buf[ N + F - 1 ];	/* ring buffer of size N,with extra F-1 bytes to facilitate string comparison */
  int	match_position;
  int	match_length;  /* of longest match.  These areset by the InsertNode() procedure. */
  int	lson[ N + 1 ];
  int rson[ N + 257 ];
  int dad[ N + 1 ];  /* left & right children &parents -- These constitute binary search trees. */
  
  /* character code = 0, 1, ..., N_CHAR - 1 */
  
  unsigned long low;
  unsigned long high;
  unsigned long value;
  int shifts;  /* counts for magnifying low and high around Q2 */
  int char_to_sym[ N_CHAR ];
  int sym_to_char[ N_CHAR + 1 ];
  unsigned int sym_freq[ N_CHAR + 1 ];  /* frequency for symbols */
  unsigned int sym_cum[ N_CHAR + 1 ];   /* cumulative freq for symbols */
  unsigned int position_cum[ N + 1 ];   /* cumulative freq for positions */
  
#define PUSH_BACK(c) lz->m_OutBuffer[lz->m_nOutCur++] = c
#define SIZE(arr) lz->m_nOutCur
#define PUSH_BACK_ARR(c)  (memcpy(lz->m_OutBuffer + lz->m_nOutCur,&(c),sizeof(c)), lz->m_nOutCur+=sizeof(c))
  
  unsigned char *m_OutBuffer;
  int m_nOutCur;
  
  const unsigned char *m_pSrc;
  int m_nInCur;
  
  unsigned int buffer_putbit, mask_putbit;
  unsigned int buffer_getbit, mask_getbit;
} LZARI;

void Lzari_Init( LZARI* lz ) {
  lz->textsize = 0;
  lz->codesize = 0;
  lz->printcount = 0;
  
  lz->low = 0;
  lz->high = Q4;
  lz->value = 0;
  lz->shifts = 0;
  
  lz->m_pSrc = NULL;
  lz->m_nInCur = 0;
  lz->m_OutBuffer = NULL;
  lz->m_nOutCur = 0;
  
  lz->buffer_putbit = 0;
  lz->mask_putbit = 128;
  
  lz->buffer_getbit = 0;
  lz->mask_getbit = 0;
}

void Error( char *message ) {
#ifdef _OUTPUT_STATUS
  printf( "\n%s\n", message );
#endif
  exit( EXIT_FAILURE );
}

/* LZARI_Output one bit (bit = 0,1) */
#if 1
void LZARI_PutBit( LZARI* lz, int bit ) {
  if ( bit ) {
    lz->buffer_putbit |= lz->mask_putbit;
  }
  if ( ( lz->mask_putbit >>= 1 ) == 0 ) {
    //if (lz->m_nOutCur == m_nOutLength)
    //  Error("Write Error");
    PUSH_BACK( lz->buffer_putbit );
    //m_pDes[lz->m_nOutCur++] = lz->buffer_putbit;
    //lz->m_OutBuffer.push_back(lz->buffer_putbit);
    //lz->m_nOutCur++;
    
    lz->buffer_putbit = 0;
    lz->mask_putbit = 128;
    lz->codesize++;
  }
}
/* Get one bit (0 or 1) */
int LZARI_GetBit( LZARI* lz ) {
  if ( ( lz->mask_getbit >>= 1 ) == 0 ) {
    lz->buffer_getbit = lz->m_pSrc[ lz->m_nInCur++ ];
    lz->mask_getbit = 128;
  }
  return ( ( lz->buffer_getbit & lz->mask_getbit ) != 0 );
}
/* LZARI_Output 1 bit, follz->lowed by its complements */
void LZARI_Output( LZARI* lz, int bit ) {
  LZARI_PutBit( lz, bit );
  for ( ; lz->shifts > 0; lz->shifts-- )
    LZARI_PutBit( lz, ! bit );
}
#else
#define LZARI_PutBit(lz, bit) { \
  if (bit) { lz->buffer_putbit |= lz->mask_putbit; } \
  if ((lz->mask_putbit >>= 1) == 0) { \
    PUSH_BACK(lz->buffer_putbit); \
    lz->buffer_putbit = 0; \
    lz->mask_putbit = 128; \
    lz->codesize++; \
  } \
}
#define LZARI_Output(lz, bit) { \
  LZARI_PutBit(lz, bit); \
  for ( ; lz->shifts > 0; lz->shifts--) { \
    LZARI_PutBit(lz, ! bit); } \
}
#define LZARI_GetBit(lz)  ((((lz->mask_getbit >>= 1) == 0) ? (lz->buffer_getbit = lz->m_pSrc[lz->m_nInCur++], lz->mask_getbit = 128) : (0)), ((lz->buffer_getbit & lz->mask_getbit) != 0))

#endif

/* Send remaining bits */
void FlushBitBuffer( LZARI* lz ) {
  int i;
  for ( i = 0; i < 7; i++ )
    LZARI_PutBit( lz, 0 );
}


/********** LZSS with multiple binary trees **********/

void InitTree( LZARI* lz )    /* Initialize trees */
{
  int i;
  
  /* For i = 0 to N - 1, rson[i] and lz->lson[i] will be the right and
    left children of node i.  These nodes need not be initialized.
    Also, dad[i] is the parent of node i.  These are initialized to
    NIL (= N), which stands for 'not used.'
    For i = 0 to 255, rson[N + i + 1] is the root of the tree
    for strings that begin with character i.  These are initialized
    to NIL.  Note there are 256 trees. */
  
  for ( i = N + 1; i <= N + 256; i++ )
    lz->rson[ i ] = NIL; /* root */
  for ( i = 0; i < N; i++ )
    lz->dad[ i ] = NIL; /* node */
}

void InsertNode( LZARI* lz, int r )
/* Inserts string of length F, lz->text_buf[r..r+F-1], into one of the
  trees (lz->text_buf[r]'th tree) and returns the longest-match position
  and length via the global variables lz->match_position and lz->match_length.
  If lz->match_length = F, then removes the old node in favor of the new
  one, because the old one will be deleted sooner.
  Note r plays double role, as tree node and position in buffer. */
{
  int i, p, cmp, temp;
  unsigned char *key;
  
  cmp = 1;
  key = &lz->text_buf[ r ];
  p = N + 1 + key[ 0 ];
  lz->rson[ r ] = lz->lson[ r ] = NIL;
  lz->match_length = 0;
  for ( ; ; ) {
    if ( cmp >= 0 ) {
      if ( lz->rson[ p ] != NIL )
        p = lz->rson[ p ];
      else {
        lz->rson[ p ] = r;
        lz->dad[ r ] = p;
        return ;
      }
    } else {
      if ( lz->lson[ p ] != NIL )
        p = lz->lson[ p ];
      else {
        lz->lson[ p ] = r;
        lz->dad[ r ] = p;
        return ;
      }
    }
    for ( i = 1; i < F; i++ )
      if ( ( cmp = key[ i ] - lz->text_buf[ p + i ] ) != 0 )
        break;
    if ( i > THRESHOLD ) {
      if ( i > lz->match_length ) {
        lz->match_position = ( r - p ) & ( N - 1 );
        if ( ( lz->match_length = i ) >= F )
          break;
      } else if ( i == lz->match_length ) {
        if ( ( temp = ( r - p ) & ( N - 1 ) ) < lz->match_position )
          lz->match_position = temp;
      }
    }
  }
  lz->dad[ r ] = lz->dad[ p ];
  lz->lson[ r ] = lz->lson[ p ];
  lz->rson[ r ] = lz->rson[ p ];
  lz->dad[ lz->lson[ p ] ] = r;
  lz->dad[ lz->rson[ p ] ] = r;
  if ( lz->rson[ lz->dad[ p ] ] == p )
    lz->rson[ lz->dad[ p ] ] = r;
  else
    lz->lson[ lz->dad[ p ] ] = r;
  lz->dad[ p ] = NIL;  /* remove p */
}

void DeleteNode( LZARI* lz, int p )    /* Delete node p from tree */
{
  int q;
  
  if ( lz->dad[ p ] == NIL )
    return ;  /* not in tree */
  if ( lz->rson[ p ] == NIL )
    q = lz->lson[ p ];
  else if ( lz->lson[ p ] == NIL )
    q = lz->rson[ p ];
  else {
    q = lz->lson[ p ];
    if ( lz->rson[ q ] != NIL ) {
      do {
        q = lz->rson[ q ];
      } while ( lz->rson[ q ] != NIL );
      lz->rson[ lz->dad[ q ] ] = lz->lson[ q ];
      lz->dad[ lz->lson[ q ] ] = lz->dad[ q ];
      lz->lson[ q ] = lz->lson[ p ];
      lz->dad[ lz->lson[ p ] ] = q;
    }
    lz->rson[ q ] = lz->rson[ p ];
    lz->dad[ lz->rson[ p ] ] = q;
  }
  lz->dad[ q ] = lz->dad[ p ];
  if ( lz->rson[ lz->dad[ p ] ] == p )
    lz->rson[ lz->dad[ p ] ] = q;
  else
    lz->lson[ lz->dad[ p ] ] = q;
  lz->dad[ p ] = NIL;
}

/********** Arithmetic Compression **********/

/*  If you are not familiar with arithmetic compression, you should read
  I. E. Witten, R. M. Neal, and J. G. Cleary,
   Communications of the ACM, Vol. 30, pp. 520-540 (1987),
 from which much have been borrowed.  */

/* character code = 0, 1, ..., N_CHAR - 1 */


void Lzari_StartModel( LZARI* lz )    /* Initialize model */
{
  int ch, sym, i;
  
  lz->sym_cum[ N_CHAR ] = 0;
  for ( sym = N_CHAR; sym >= 1; sym-- ) {
    ch = sym - 1;
    lz->char_to_sym[ ch ] = sym;
    lz->sym_to_char[ sym ] = ch;
    lz->sym_freq[ sym ] = 1;
    lz->sym_cum[ sym - 1 ] = lz->sym_cum[ sym ] + lz->sym_freq[ sym ];
  }
  lz->sym_freq[ 0 ] = 0;  /* sentinel (!= lz->sym_freq[1]) */
  lz->position_cum[ N ] = 0;
  for ( i = N; i >= 1; i-- )
    lz->position_cum[ i - 1 ] = lz->position_cum[ i ] + 10000 / ( i + 200 );
  /* empirical distribution function (quite tentative) */
  /* Please devise a better mechanism! */
}

void Lzari_UpdateModel( LZARI* lz, int sym ) {
  int i, c, ch_i, ch_sym;
  
  if ( lz->sym_cum[ 0 ] >= MAX_CUM ) {
    c = 0;
    for ( i = N_CHAR; i > 0; i-- ) {
      lz->sym_cum[ i ] = c;
      c += ( lz->sym_freq[ i ] = ( lz->sym_freq[ i ] + 1 ) >> 1 );
    }
    lz->sym_cum[ 0 ] = c;
  }
  for ( i = sym; lz->sym_freq[ i ] == lz->sym_freq[ i - 1 ]; i-- )
    ;
  if ( i < sym ) {
    ch_i = lz->sym_to_char[ i ];
    ch_sym = lz->sym_to_char[ sym ];
    lz->sym_to_char[ i ] = ch_sym;
    lz->sym_to_char[ sym ] = ch_i;
    lz->char_to_sym[ ch_i ] = sym;
    lz->char_to_sym[ ch_sym ] = i;
  }
  lz->sym_freq[ i ] ++;
  while ( --i >= 0 )
    lz->sym_cum[ i ] ++;
}

void Lzari_EncodeChar( LZARI* lz, int ch ) {
  int sym;
  unsigned long int range;
  
  sym = lz->char_to_sym[ ch ];
  range = lz->high - lz->low;
  lz->high = lz->low + ( range * lz->sym_cum[ sym - 1 ] ) / lz->sym_cum[ 0 ];
  lz->low += ( range * lz->sym_cum[ sym ] ) / lz->sym_cum[ 0 ];
  for ( ; ; ) {
    if ( lz->high <= Q2 ) {
      LZARI_Output( lz, 0 );
    } else if ( lz->low >= Q2 ) {
      LZARI_Output( lz, 1 );
      lz->low -= Q2;
      lz->high -= Q2;
    } else if ( lz->low >= Q1 && lz->high <= Q3 ) {
      lz->shifts++;
      lz->low -= Q1;
      lz->high -= Q1;
    } else
      break;
    lz->low += lz->low;
    lz->high += lz->high;
  }
  Lzari_UpdateModel( lz, sym );
}

void EncodePosition( LZARI* lz, int position ) {
  unsigned long int range;
  
  range = lz->high - lz->low;
  lz->high = lz->low + ( range * lz->position_cum[ position ] ) / lz->position_cum[ 0 ];
  lz->low += ( range * lz->position_cum[ position + 1 ] ) / lz->position_cum[ 0 ];
  for ( ; ; ) {
    if ( lz->high <= Q2 ) {
      LZARI_Output( lz, 0 );
    } else if ( lz->low >= Q2 ) {
      LZARI_Output( lz, 1 );
      lz->low -= Q2;
      lz->high -= Q2;
    } else if ( lz->low >= Q1 && lz->high <= Q3 ) {
      lz->shifts++;
      lz->low -= Q1;
      lz->high -= Q1;
    } else
      break;
    lz->low += lz->low;
    lz->high += lz->high;
  }
}

void Lzari_EncodeEnd( LZARI* lz ) {
  lz->shifts++;
  if ( lz->low < Q1 ) {
    LZARI_Output( lz, 0 );
  } else {
    LZARI_Output( lz, 1 );
  }
  FlushBitBuffer( lz );  /* flush bits remaining in buffer */
}

int BinarySearchSym( LZARI* lz, unsigned int x )
/* 1      if x >= lz->sym_cum[1],
  N_CHAR if lz->sym_cum[N_CHAR] > x,
  i such that lz->sym_cum[i - 1] > x >= lz->sym_cum[i] otherwise */
{
  int i, j, k;
  
  i = 1;
  j = N_CHAR;
  while ( i < j ) {
    k = ( i + j ) / 2;
    if ( lz->sym_cum[ k ] > x ) {
      i = k + 1;
    } else {
      j = k;
    }
  }
  return i;
}

int BinarySearchPos( LZARI* lz, unsigned int x )
/* 0 if x >= lz->position_cum[1],
  N - 1 if lz->position_cum[N] > x,
  i such that lz->position_cum[i] > x >= lz->position_cum[i + 1] otherwise */
{
  int i, j, k;
  
  i = 1;
  j = N;
  while ( i < j ) {
    k = ( i + j ) / 2;
    if ( lz->position_cum[ k ] > x ) {
      i = k + 1;
    } else {
      j = k;
    }
  }
  return i - 1;
}

void Lzari_StartDecode( LZARI* lz ) {
  int i;
  
  for ( i = 0; i < M + 2; i++ )
    lz->value = 2 * lz->value + LZARI_GetBit( lz );
}

int Lzari_DecodeChar( LZARI* lz ) {
  int sym, ch;
  unsigned long int range;
  
  range = lz->high - lz->low;
  sym = BinarySearchSym( lz, ( unsigned int )
                         ( ( ( lz->value - lz->low + 1 ) * lz->sym_cum[ 0 ] - 1 ) / range ) );
  lz->high = lz->low + ( range * lz->sym_cum[ sym - 1 ] ) / lz->sym_cum[ 0 ];
  lz->low += ( range * lz->sym_cum[ sym ] ) / lz->sym_cum[ 0 ];
  for ( ; ; ) {
    if ( lz->low >= Q2 ) {
      lz->value -= Q2;
      lz->low -= Q2;
      lz->high -= Q2;
    } else if ( lz->low >= Q1 && lz->high <= Q3 ) {
      lz->value -= Q1;
      lz->low -= Q1;
      lz->high -= Q1;
    } else if ( lz->high > Q2 ) {
      break;
    }
    lz->low += lz->low;
    lz->high += lz->high;
    lz->value = 2 * lz->value + LZARI_GetBit( lz );
  }
  ch = lz->sym_to_char[ sym ];
  Lzari_UpdateModel( lz, sym );
  return ch;
}

int DecodePosition( LZARI* lz ) {
  int position;
  unsigned long int range;
  
  range = lz->high - lz->low;
  position = BinarySearchPos( lz, ( unsigned int )
                              ( ( ( lz->value - lz->low + 1 ) * lz->position_cum[ 0 ] - 1 ) / range ) );
  lz->high = lz->low + ( range * lz->position_cum[ position ] ) / lz->position_cum[ 0 ];
  lz->low += ( range * lz->position_cum[ position + 1 ] ) / lz->position_cum[ 0 ];
  for ( ; ; ) {
    if ( lz->low >= Q2 ) {
      lz->value -= Q2;
      lz->low -= Q2;
      lz->high -= Q2;
    } else if ( lz->low >= Q1 && lz->high <= Q3 ) {
      lz->value -= Q1;
      lz->low -= Q1;
      lz->high -= Q1;
    } else if ( lz->high > Q2 ) {
      break;
    }
    lz->low += lz->low;
    lz->high += lz->high;
    lz->value = 2 * lz->value + LZARI_GetBit( lz );
  }
  return position;
}

/********** Encode and Decode **********/

int LZARI_Encode( const void *pSrc, int nSrcLen, const void * pDes ) {
  int i, c, len, r, s, last_match_length;
  LZARI lzari;
  LZARI* lz = &lzari;
  Lzari_Init( lz );
  
  if ( NULL == pDes ) {
    return nSrcLen;
  }
  lz->m_pSrc = ( const unsigned char * ) pSrc;
  //nSrcLen = nInLength;
  lz->m_OutBuffer = ( unsigned char * ) pDes;
  lz->m_nInCur = 0;
  
  // lz->m_nOutCur = 0;
  {
    lz->textsize = nSrcLen;
    PUSH_BACK_ARR( lz->textsize );
    //lz->m_OutBuffer.resize(sizeof lz->textsize);
    //memcpy(&lz->m_OutBuffer[0],&lz->textsize,sizeof lz->textsize);
    //lz->m_nOutCur += sizeof lz->textsize;
    lz->codesize += sizeof lz->textsize;
    if ( lz->textsize == 0 )
      return SIZE( lz->m_OutBuffer );
    lz->m_nInCur = 0;
    lz->textsize = 0;
  }
  
  Lzari_StartModel( lz );
  InitTree( lz );
  s = 0;
  r = N - F;
  for ( i = s; i < r; i++ ) {
    lz->text_buf[ i ] = ' ';
  }
  for ( len = 0; len < F && lz->m_nInCur < nSrcLen ; len++ ) {
    c = lz->m_pSrc[ lz->m_nInCur++ ];
    lz->text_buf[ r + len ] = c;
  }
  
  lz->textsize = len;
  for ( i = 1; i <= F; i++ ) {
    InsertNode( lz, r - i );
  }
  
  InsertNode( lz, r );
  
  do {
    if ( lz->match_length > len )
      lz->match_length = len;
    if ( lz->match_length <= THRESHOLD ) {
      lz->match_length = 1;
      Lzari_EncodeChar( lz, lz->text_buf[ r ] );
    } else {
      Lzari_EncodeChar( lz, 255 - THRESHOLD + lz->match_length );
      EncodePosition( lz, lz->match_position - 1 );
    }
    last_match_length = lz->match_length;
    {
      for ( i = 0; i < last_match_length && lz->m_nInCur < nSrcLen ; i++ ) {
        c = lz->m_pSrc[ lz->m_nInCur++ ];
        DeleteNode( lz, s );
        lz->text_buf[ s ] = c;
        if ( s < F - 1 ) {
          lz->text_buf[ s + N ] = c;
        }
        s = ( s + 1 ) & ( N - 1 );
        r = ( r + 1 ) & ( N - 1 );
        InsertNode( lz, r );
      }
    }
    if ( ( lz->textsize += i ) > lz->printcount ) {
#ifdef _OUTPUT_STATUS
      printf( "%12ld\r", lz->textsize );
#endif
      
      lz->printcount += 1024;
    }
    while ( i++ < last_match_length ) {
      DeleteNode( lz, s );
      s = ( s + 1 ) & ( N - 1 );
      r = ( r + 1 ) & ( N - 1 );
      if ( --len )
        InsertNode( lz, r );
    }
  } while ( len > 0 );
  
  Lzari_EncodeEnd( lz );
  
#ifdef _OUTPUT_STATUS
  
  printf( "In : %lu bytes\n", lz->textsize );
  printf( "Out: %lu bytes\n", lz->codesize );
  printf( "Out/In: %.3f\n", ( double ) lz->codesize / lz->textsize );
#endif
  
  return SIZE( lz->m_OutBuffer );
}

LZARI lzari;
int LZARI_Decode( const void *pSrc, int nSrcLen, const void* pDes ) {
  int i, j, k, r, c;
  unsigned long int count;
  LZARI* lz = &lzari;
  Lzari_Init( lz );
  
  lz->m_pSrc = ( const unsigned char * ) pSrc;
  //nSrcLen = nInLength;
  lz->m_OutBuffer = ( unsigned char * ) pDes;
  lz->m_nInCur = 0;
  
  {
    if ( nSrcLen < sizeof(lz->textsize) )
      Error( "Read Error" );
    memcpy( &lz->textsize, lz->m_pSrc + lz->m_nInCur, sizeof lz->textsize );
    
    //lz->m_OutBuffer.reserve(lz->textsize);
    //m_nOutLength = lz->textsize;
    //lz->m_nOutCur = 0;
    if ( NULL == lz->m_OutBuffer ) {
      return lz->textsize;
    }
    
    lz->m_nOutCur = 0;
    lz->m_nInCur += sizeof lz->textsize;
  }
  
  if ( lz->textsize == 0 )
    return SIZE( lz->m_OutBuffer );
    
  Lzari_StartDecode( lz );
  Lzari_StartModel( lz );
  
  for ( i = 0; i < N - F; i++ )
    lz->text_buf[ i ] = ' ';
  r = N - F;
  for ( count = 0; count < lz->textsize; ) {
    c = Lzari_DecodeChar( lz );
    if ( c < 256 ) {
      {
        PUSH_BACK( c );
        //lz->m_OutBuffer.push_back(c);
        //m_pDes[lz->m_nOutCur++] = c;
        //lz->m_nOutCur++;
      }
      lz->text_buf[ r++ ] = c;
      r &= ( N - 1 );
      count++;
    } else {
      i = ( r - DecodePosition( lz ) - 1 ) & ( N - 1 );
      j = c - 255 + THRESHOLD;
      for ( k = 0; k < j; k++ ) {
        c = lz->text_buf[ ( i + k ) & ( N - 1 ) ];
        {
          PUSH_BACK( c );
          //m_pDes[lz->m_nOutCur++] = c;
          //lz->m_OutBuffer.push_back(c);
          //lz->m_nOutCur ++;
        }
        lz->text_buf[ r++ ] = c;
        r &= ( N - 1 );
        count++;
      }
    }
    if ( count > lz->printcount ) {
#ifdef _OUTPUT_STATUS
      printf( "%12lu\r", count );
#endif
      
      lz->printcount += 1024;
    }
  }
  
#ifdef _OUTPUT_STATUS
  printf( "%12lu\n", count );
#endif
  
  return SIZE( lz->m_OutBuffer );
}

#undef N
#undef F
#undef THRESHOLD
#undef NIL
#undef M
#undef Q1
#undef Q2
#undef Q3
#undef Q4
#undef MAX_CUM
#undef N_CHAR

#undef PUSH_BACK
#undef SIZE
#undef PUSH_BACK_ARR
#undef LZARI_PutBit
#undef LZARI_Output

#endif // _LZARI_INL_
