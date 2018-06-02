#ifndef _LZF_INL_
#define _LZF_INL_
#define TRUE 1
#define FALSE 0
#ifndef ASSERT
#define ASSERT(x)  (0)
#endif 
//for cost time
#define CAL_TIME 1 
//for compress
#define USE_HUFF 0
typedef struct {
  char strMask[ 4 ];      //"DIP"
  char strExt[ 4 ];        //扩展名
  unsigned int dFileLen;        //文件长度
}
COMHEAD, *PCOMHEAD;
//for LZSS
#define N    4096 /* size of ring buffer */ 
//由于采用长连续编码, F不能超过17
#define F      17 /* upper limit for L->match_length */ 
/* encode string into position and length if L->match_length is greater than this */
#define THRESHOLD 2
#define NIL    N /* index for root of binary search trees */

typedef struct LZF {
  unsigned int textsize;   /* text size counter */
  unsigned int codesize;   /* code size counter */
  unsigned int printcount;  /* counter for reporting progress every 1K bytes */
  int match_position, match_length;   /* of longest match.  These are set by the LZF_InsertNode() procedure. */
  unsigned char code_buf[ 50 ], mask;
  unsigned int dSameLen;
  int bcon;
  unsigned char byValue;
  int iSameNum;
  //for huff
#if USE_HUFF
  unsigned int dHufftemp;
  unsigned char *pbyhuf;
  unsigned char *pbydata;
  unsigned char *pbytemp;
  unsigned char *pbybeg;
#endif
  /* ring buffer of size N, with extra F-1 bytes to facilitate string comparison */
  int text_buf[ N + F - 1 ];
  /* left & right children & parents -- These constitute binary search trees. */
  int lson[ N + 1 ], rson[ N + 257 ], dad[ N + 1 ]; 
} LZF;

static void LZF_InitTree( LZF* L )   /* initialize trees */
{
  int i;
  /* For i = 0 to N - 1, L->rson[i] and L->lson[i] will be the right and
     left children of node i.  These nodes need not be initialized.
     Also, L->dad[i] is the parent of node i.  These are initialized to
     NIL (= N), which stands for 'not used.'
     For i = 0 to 255, L->rson[N + i + 1] is the root of the tree
     for strings that begin with character i.  These are initialized
     to NIL.  Note there are 256 trees. */
  for ( i = N + 1; i <= N + 256; i++ )
    L->rson[ i ] = NIL;
  for ( i = 0; i < N; i++ )
    L->dad[ i ] = NIL;
  L->textsize = 0;   /* text size counter */
  L->codesize = 0;   /* code size counter */
  L->printcount = 0;  /* counter for reporting progress every 1K bytes */
  L->dSameLen = 0;
  L->bcon = FALSE;
  L->byValue = -1;
  L->iSameNum = 0;
  //for huff
#if USE_HUFF
  L->dHufftemp = 0;
  L->pbyhuf = NULL;
  L->pbydata = NULL;
  L->pbytemp = NULL;
  L->pbybeg = NULL;
#endif
}
static void LZF_InsertNode( LZF* L, int r )
/* Inserts string of length F, L->text_buf[r..r+F-1], into one of the
   trees (L->text_buf[r]'th tree) and returns the longest-match position
   and length via the global variables L->match_position and L->match_length.
   If L->match_length = F, then removes the old node in favor of the new
   one, because the old one will be deleted sooner.
   Note r plays double role, as tree node and position in buffer. */
{
  int i, p, cmp;
  int *key;
  cmp = 1;
  key = &L->text_buf[ r ];
  p = N + 1 + key[ 0 ];
  L->rson[ r ] = L->lson[ r ] = NIL;
  L->match_length = 0;
  for ( ; ; ) {
    if ( cmp >= 0 ) {
      if ( L->rson[ p ] != NIL )
        p = L->rson[ p ];
      else {
        L->rson[ p ] = r;
        L->dad[ r ] = p;
        return ;
      }
    } else {
      if ( L->lson[ p ] != NIL )
        p = L->lson[ p ];
      else {
        L->lson[ p ] = r;
        L->dad[ r ] = p;
        return ;
      }
    }
    for ( i = 1; i < F; i++ ) {
      if ( ( cmp = key[ i ] - L->text_buf[ p + i ] ) != 0 )
        break;
    }
    if ( i > L->match_length ) {
      L->match_position = p;
      if ( ( L->match_length = i ) >= F )
        break;
    }
  }
  L->dad[ r ] = L->dad[ p ];
  L->lson[ r ] = L->lson[ p ];
  L->rson[ r ] = L->rson[ p ];
  L->dad[ L->lson[ p ] ] = r;
  L->dad[ L->rson[ p ] ] = r;
  if ( L->rson[ L->dad[ p ] ] == p )
    L->rson[ L->dad[ p ] ] = r;
  else
    L->lson[ L->dad[ p ] ] = r;
  L->dad[ p ] = NIL;  /* remove p */
}
/* deletes node p from tree */
static void LZF_DeleteNode( LZF* L, int p ) {
  int q;
  if ( L->dad[ p ] == NIL )
    return ;  /* not in tree */
  if ( L->rson[ p ] == NIL )
    q = L->lson[ p ];
  else if ( L->lson[ p ] == NIL )
    q = L->rson[ p ];
  else {
    q = L->lson[ p ];
    if ( L->rson[ q ] != NIL ) {
      do {
        q = L->rson[ q ];
      } while ( L->rson[ q ] != NIL );
      L->rson[ L->dad[ q ] ] = L->lson[ q ];
      L->dad[ L->lson[ q ] ] = L->dad[ q ];
      L->lson[ q ] = L->lson[ p ];
      L->dad[ L->lson[ p ] ] = q;
    }
    L->rson[ q ] = L->rson[ p ];
    L->dad[ L->rson[ p ] ] = q;
  }
  L->dad[ q ] = L->dad[ p ];
  if ( L->rson[ L->dad[ p ] ] == p )
    L->rson[ L->dad[ p ] ] = q;
  else
    L->lson[ L->dad[ p ] ] = q;
  L->dad[ p ] = NIL;
}
//保存字节
#if USE_HUFF
static void Save_Bytes( LZF* L, unsigned char **pbydes, int *buf_ptr ) {
  int i = 0;
  ASSERT( pbydes != NULL );
  ASSERT( buf_ptr != NULL );
  for ( i = 1; i < *buf_ptr; i++ )   /* Send at most 8 units of */
    *( L->pbydata++ ) = L->code_buf[ i ]; //putc(L->code_buf[i], outfile);     /* code together */
  L->codesize += ( *buf_ptr ) - 1;
  *L->pbyhuf++ = L->code_buf[ 0 ];
  L->dHufftemp ++;
  L->code_buf[ 0 ] = 0;
  *buf_ptr = L->mask = 1;
}
#else
static void Save_Bytes( LZF* L, unsigned char **pbydes, int *buf_ptr ) {
  int i = 0;
  ASSERT( pbydes != NULL );
  ASSERT( buf_ptr != NULL );
  for ( i = 0; i < *buf_ptr; i++ )   /* Send at most 8 units of */
    *( ( *pbydes ) ++ ) = L->code_buf[ i ]; //putc(L->code_buf[i], outfile);     /* code together */
  L->codesize += *buf_ptr;
  L->code_buf[ 0 ] = 0;
  *buf_ptr = L->mask = 1;
}
#endif
//编码连续字节, L->byValue L->iSameNum|0xf L->dSameLen msb--->lsb
//由于F = 17, 所以在编码中匹配长度不超过0x0e, 如果为0xf, 则为连续长字节
static void Encode_Byte( LZF* L, int *buf_ptr ) {
  unsigned char i = 0;
  ASSERT( buf_ptr != NULL );
  //计算连续长度占用字节
  if ( L->dSameLen > 0xffffff ) {
    L->iSameNum = 4;
  } else if ( L->dSameLen > 0xffff ) {
    L->iSameNum = 3;
  } else if ( L->dSameLen > 0xff ) {
    L->iSameNum = 2;
  } else {
    L->iSameNum = 1;
  }
  //编码
  L->code_buf[ ( *buf_ptr ) ++ ] = L->byValue;
  L->code_buf[ ( *buf_ptr ) ++ ] = ( ( L->iSameNum & 0xf ) << 4 ) | 0xf;  //0xf 为连续字节标志
  for ( i = L->iSameNum; i > 0; i-- ) {
    L->code_buf[ ( *buf_ptr ) ++ ] = ( unsigned char ) ( L->dSameLen >> ( ( i - 1 ) * 8 ) ) & 0xff;
  }
  L->iSameNum = 0;
}
//编码单个字节
static void Encode_Char( LZF* L, unsigned char **pbydes, int *buf_ptr, int ipos ) {
  ASSERT( pbydes != NULL );
  ASSERT( buf_ptr != NULL );
  //如果先前为连续字节, 先处理连续字节
  if ( L->bcon ) {
    Encode_Byte( L, buf_ptr );  //编码连续字节
    L->mask <<= 1;        //处理L->mask
    L->bcon = FALSE;
  }
  L->dSameLen = 0;
  if ( L->mask == 0 )         //处于结束外, 保存编码字节
  {
    Save_Bytes( L, pbydes, buf_ptr );
  }
  //开始下一个L->mask编码
  //编码单字节
  L->match_length = 1;  /* Not long enough match.  Send one byte. */
  L->code_buf[ 0 ] |= L->mask;  /* 'send one byte' flag */
  L->code_buf[ ( *buf_ptr ) ++ ] = L->text_buf[ ipos ];  /* Send uncoded. */
  L->mask <<= 1;
}
//编码匹配字节
static void Encode_Match( LZF* L, int *buf_ptr ) {
  ASSERT( buf_ptr != NULL );
  L->code_buf[ ( *buf_ptr ) ++ ] = ( unsigned char ) L->match_position;
  L->code_buf[ ( *buf_ptr ) ++ ] = ( unsigned char )
                                ( ( ( L->match_position >> 4 ) & 0xf0 )
                                  | ( L->match_length - ( THRESHOLD + 1 ) ) );  /* Send position and
       length pair. Note L->match_length > THRESHOLD. */
  L->mask <<= 1;
}
//编码匹配字节
static void Encode_Position( LZF* L, unsigned char **pbydes, int *buf_ptr ) {
  int i = 0;
  int m_pos = L->match_position;
  unsigned int dLen = 0;
  unsigned char bytemp = L->text_buf[ m_pos ];
  ASSERT( pbydes != NULL );
  ASSERT( buf_ptr != NULL );
  //计算连续长度
  for ( i = 0; i < L->match_length; i++ ) {
    if ( L->text_buf[ m_pos + i ] - L->text_buf[ m_pos ] != 0 ) {
      break;
    }
  }
  if ( i == L->match_length )     //连续长度等于匹配长度
  {
    //   cout << "L->match_length = " << L->match_length << endl;
    dLen = L->match_length;
    if ( L->bcon == FALSE )     //先前为不连续字节
    {
      if ( i == F )       //连续字节开始必须有F长度,否则视为匹配
      {
        // L->mask <<= 1;
        L->bcon = TRUE;
        L->dSameLen = dLen;
      } else       //编码匹配字节
      {
        L->dSameLen = 0;
        Encode_Match( L, buf_ptr );
      }
    }
    else {
      if ( L->byValue != bytemp )     //一个连续结束, 另一个连续开始
      {
        Encode_Byte( L, buf_ptr );    //编码连续字节
        L->mask <<= 1;          //处理L->mask
        if ( L->mask == 0 )         //一段结束
        {
          Save_Bytes( L, pbydes, buf_ptr );
        }
        //L->dSameLen = dLen;
        if ( i == F )         //连续开始必须为F个字节
        {
          //开下一段
          L->dSameLen = dLen;
        } else         //编码匹配字节
        {
          L->bcon = FALSE;
          L->dSameLen = 0;
          Encode_Match( L, buf_ptr );
        }
      }
      else {
        L->dSameLen += dLen;
      }
    }
    L->byValue = bytemp;
  } else {
    if ( L->bcon ) {
      Encode_Byte( L, buf_ptr );    //编码连续字节
      L->mask <<= 1;          //处理L->mask
      L->bcon = FALSE;        //去连续标志
    }
    if ( L->mask == 0 )           //一段结束
    {
      Save_Bytes( L, pbydes, buf_ptr );
    }
    L->dSameLen = 0;
    //编码匹配字节
    Encode_Match( L, buf_ptr );
  }
}
//在编码结束时, 如果连续字节没结束, 则编码连续字节, 结束连续
static void Encode_End( LZF* L, unsigned char **pbydes, int *buf_ptr ) {
  ASSERT( pbydes != NULL );
  ASSERT( buf_ptr != NULL );
  if ( L->bcon ) {
    Encode_Byte( L, buf_ptr );      //编码连续字节
    Save_Bytes( L, pbydes, buf_ptr );  //保存
    L->bcon = FALSE;          //去掉连续标志
  }
}
int LZF_Encode0( LZF* L, const void* pbysrc0, unsigned int dInLen, void* pbydes0, int maxOutLen ) {
  int i, c, len, r, s, last_match_length, code_buf_ptr;
  unsigned int pdOutLen;
  const unsigned char* pbysrc = ( const unsigned char* ) pbysrc0;
  unsigned char* pbydes = ( unsigned char* ) pbydes0;
#if USE_HUFF
  unsigned char *pbytest = NULL;
  unsigned int dHuffLen = 0;
  L->pbyhuf = ( unsigned char* ) malloc( dInLen / 8 );
  pbytest = L->pbyhuf;
  L->pbydata = pbydes + dInLen / 8 + 1;
  L->pbytemp = L->pbydata;
  L->pbybeg = pbydes;
  L->dHufftemp = 0;
#endif
  ASSERT( pbysrc != NULL );
  ASSERT( pbydes != NULL );
  L->codesize = 0;
#if USE_HUFF
  StartHuff();
#endif
  LZF_InitTree(L);  /* initialize trees */
  L->code_buf[ 0 ] = 0;  /* L->code_buf[1..16] saves eight units of code, and
     L->code_buf[0] works as eight flags, "1" representing that the unit
     is an unencoded letter (1 byte), "0" a position-and-length pair
      (2 bytes).  Thus, eight units require at most 16 bytes of code. */
  code_buf_ptr = L->mask = 1;
  s = 0;
  r = N - F;
  for ( i = s; i < r; i++ )
    L->text_buf[ i ] = ' ';  /* Clear the buffer with
     any character that will appear often. */
  for ( len = 0; len < F && ( ( int ) dInLen-- > 0 ); len++ ) {
    c = ( int ) * ( pbysrc++ );
    L->text_buf[ r + len ] = c;  /* Read F bytes into the last F bytes of
         the buffer */
  }
  if ( ( L->textsize = len ) == 0 )
    return 0;  /* text of size zero */
  for ( i = 1; i <= F; i++ )
    LZF_InsertNode( L, r - i );  /* Insert the F strings,
     each of which begins with one or more 'space' characters.  Note
     the order in which these strings are inserted.  This way,
     degenerate trees will be less likely to occur. */
  LZF_InsertNode( L, r );  /* Finally, insert the whole string just read.  The
     global variables L->match_length and L->match_position are set. */
  do {
    if ( L->match_length > len )
      L->match_length = len;  /* L->match_length
         may be spuriously long near the end of text. */
    if ( L->match_length <= THRESHOLD ) {
      Encode_Char( L, &pbydes, &code_buf_ptr, r );
    } else {
      Encode_Position( L, &pbydes, &code_buf_ptr );
    }
    if ( L->mask == 0 ) {  /* Shift L->mask left one bit. */
      Save_Bytes( L, &pbydes, &code_buf_ptr );
    }
    last_match_length = L->match_length;
    for ( i = 0; i < last_match_length && ( ( int ) dInLen-- > 0 ); i++ ) {
      c = ( unsigned char ) * ( pbysrc++ );
      LZF_DeleteNode( L, s );    /* Delete old strings and */
      L->text_buf[ s ] = c;  /* read new bytes */
      if ( s < F - 1 )
        L->text_buf[ s + N ] = c;  /* If the position is
             near the end of buffer, extend the buffer to make
             string comparison easier. */
      s = ( s + 1 ) & ( N - 1 );
      r = ( r + 1 ) & ( N - 1 );
      /* Since this is a ring buffer, increment the position modulo N. */
      LZF_InsertNode( L, r );  /* Register the string in L->text_buf[r..r+F-1] */
    }
    if ( ( L->textsize += i ) > L->printcount ) {
      //printf( "%12ld\r", L->textsize );
      L->printcount += 1024;
      /* Reports progress each time the L->textsize exceeds
         multiples of 1024. */
    }
    while ( i++ < last_match_length ) {  /* After the end of text, */
      LZF_DeleteNode( L, s );          /* no need to read, but */
      s = ( s + 1 ) & ( N - 1 );
      r = ( r + 1 ) & ( N - 1 );
      if ( --len )
        LZF_InsertNode( L, r );    /* buffer may not be empty. */
    }
  } while ( len > 0 );  /* until length of string to be processed is zero */
  Encode_End( L, &pbydes, &code_buf_ptr );
#if USE_HUFF
  if ( code_buf_ptr > 1 ) {    /* Send remaining code. */
    for ( i = 1; i < code_buf_ptr; i++ )
      *( L->pbydata++ ) = L->code_buf[ i ];
    L->codesize += code_buf_ptr;
    *L->pbyhuf ++ = L->code_buf[ 0 ];
    L->dHufftemp ++;
  }
#else
  if ( code_buf_ptr > 1 ) {    /* Send remaining code. */
    for ( i = 0; i < code_buf_ptr; i++ )
      *( pbydes++ ) = L->code_buf[ i ];
    L->codesize += code_buf_ptr;
  }
#endif
#if USE_HUFF
  HUFF_Encode( pbytest, ( unsigned char * ) ( pbydes + 4 ), L->dHufftemp, &dHuffLen );
  memcpy( ( unsigned char* ) ( pbydes + 4 + dHuffLen ), L->pbytemp, L->codesize );
  *( ( unsigned int * ) pbydes ) = dHuffLen;
  free( pbytest );
  *pdOutLen = ( unsigned int ) ( L->codesize + dHuffLen + 4 );
#else
  pdOutLen = L->codesize;
#endif
  return pdOutLen;
}
int LZF_Decode0( LZF* L, const void* pbysrc0, unsigned int dInLen, void* pbydes0, int maxOutLen ) {
  int i, j, r, k;
  int flags;
  const unsigned char* pbysrc = ( const unsigned char* ) pbysrc0;
  unsigned char* pbydes = ( unsigned char* ) pbydes0;
  unsigned char *pbymatch = NULL;
  unsigned char *pbybeg = pbydes;
  unsigned int pdOutLen;
#if USE_HUFF
  unsigned int dHuffLen = *( ( unsigned int * ) pbysrc );
  pbysrc += 4;
  dInLen -= 4;
  L->pbyhuf = ( unsigned char* ) pbysrc;
  L->pbydata = ( unsigned char * ) pbysrc + dHuffLen;
#endif
  ASSERT( pbysrc != NULL );
  ASSERT( pbydes != NULL );
#if USE_HUFF
  StartHuff();
#endif
  // for (i = 0; i < N - F; i++) L->text_buf[i] = ' ';
  r = N - F;
  flags = 0;
  for ( ; pbydes - pbybeg<maxOutLen; ) {
    if ( ( ( flags >>= 1 ) & 256 ) == 0 ) {
#if USE_HUFF
      flags = DecodeChar( &L->pbyhuf, &dInLen ) | 0xff00;
#else
      if ( ( int ) ( dInLen -- ) < 0 )
        break;
      flags = *( pbysrc++ ) | 0xff00;    /* uses higher byte cleverly */
#endif
    }              /* to count eight */
    if ( flags & 1 ) {
      if ( ( int ) ( dInLen -- ) < 0 )
        break;
#if USE_HUFF
      *( pbydes++ ) = *( L->pbydata++ );
#else
      *( pbydes++ ) = *( pbysrc++ );
#endif
      r++;
      r &= ( N - 1 );
      //(*pdOutLen)++;
    } else {
#if USE_HUFF
      i = *( L->pbydata++ );
      j = *( L->pbydata++ );
#else
      i = *( pbysrc++ );
      j = *( pbysrc++ );
#endif
      if ( ( int ) ( dInLen -= 2 ) < 0 )
        break;
      L->dSameLen = 0;
      if ( ( j & 0xf ) == 0xf )         //发现长连续字段
      {
        L->byValue = i;          //连续字值
        L->iSameNum = ( j & 0xf0 ) >> 4;  //连续字占用位数
        for ( i = 0; i < L->iSameNum; i++ ) {
          L->dSameLen <<= 8;
#if USE_HUFF
          L->dSameLen |= *( L->pbydata++ );  //连续长度
#else
          L->dSameLen |= *( pbysrc++ );  //连续长度
#endif
        }
        if ( ( int ) ( dInLen -= L->iSameNum ) < 0 )
          break;
        for ( i = 0; i < ( int ) L->dSameLen; i++ )   //输出连续字节
        {
          *( pbydes++ ) = L->byValue;
          r++;
          r &= ( N - 1 );
          //(*pdOutLen)++;
        }
      }
      else {
        i |= ( ( j & 0xf0 ) << 4 );
        j = ( j & 0x0f ) + THRESHOLD;
        if ( r > i ) {
          pbymatch = pbydes - ( r - i );
        } else {
          pbymatch = pbydes - r - ( N - i );
        }
        for ( k = 0; k <= j; k++ ) {
          //c = L->text_buf[(i + k) & (N - 1)];
          //*(pbydes++) = c;
          //L->text_buf[r++] = c;
          if ( pbymatch < pbybeg ) {
            *pbydes++ = 0x20;
          } else {
            *pbydes++ = *pbymatch;
          }
          pbymatch ++;
          r++;
          r &= ( N - 1 );
          //(*pdOutLen)++;
        }
      }
    }
  }
  pdOutLen = pbydes - pbybeg;
  return pdOutLen;
}
int LZF_Decode( const void* pbysrc0, unsigned int dInLen, void* pbydes0, int maxOutLen ) {
  LZF Lzf;
  return LZF_Decode0( &Lzf, pbysrc0, dInLen, pbydes0, maxOutLen );
}
int LZF_Encode( const void* pbysrc0, unsigned int dInLen, void* pbydes0, int maxOutLen ) {
  LZF Lzf;
  return LZF_Encode0( &Lzf, pbysrc0, dInLen, pbydes0, maxOutLen );
}
#undef N
#undef F
#undef THRESHOLD
#undef NIL
#endif // _LZF_INL_
