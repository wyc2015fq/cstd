
#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include "findfile.h" 
/*
注意 :
   LHArc 使用一个 "percolating" 来更新它的 Lempel-Ziv 结构.
   如果你使用 percolating 方法, 压缩器将运行得稍快一些,
   使用更少的内存, 且比标准方法稍 less efficient 一些.
   你可以选择两种方法中的一种, 并应该注意到解压器不受该选择的影响并可以
   解压两种解压器生成的数据.
*/

typedef FILE* TStream;
typedef FILE* TStream;
typedef char* String;
typedef unsigned char Byte;
typedef unsigned short Word;
typedef short SmallInt;
typedef int Integer;
typedef int LongInt;

Integer HEX[ 'F' -'A' + 1 ] =
  { 10, 11, 12, 13, 14, 15 };
  
const char HexString[ 1 + 15 ] =
  "0123456789ABCDEF";
  
  
#ifdef WIN32
  
typedef SmallInt TwoByteInt;
#else

typedef Integer TwoByteInt;
#endif

//TwoByteInt PWord[32759+1];
//Byte PByte[65519+1];
typedef TwoByteInt* PWord;
typedef Byte* PByte;
typedef char* PChar;
typedef void* POINTER;

void LHACompress( TStream InStr, TStream OutStr, String filename );
/*  LHACompress 从 InStr 的当前位置开始压缩直到的尾部, 放置压缩后的输出 OutStr 的开始到
    OutStr 的当前位置. 如果你需要压缩整个 InStr
    则需要在调用前设置 InStr.Position == 0 .
*/
void LHAExpand( TStream InStr, TStream OutStr );
/*  LHAExpand starts expanding at InStr's current position and continues to
    the } of InStr, placing the expanded output in OutStr starting at
    OutStr's current position. If you need the entirety of InStr expanded
    you'll need to set InStr.Position == 0 before calling.
*/


/*
注意 :
   以下常数设置 LHArc 使用的常数值.
   你可以修改下面的三个 :
   DICBIT : Lempel-Ziv 字典大小.
   减少该常数会极大地影响压缩效果 !
   但增加它 (仅在 32 位平台中, 如 Delphi 2) 将不会 yield
   noticeably better results.
   如果你设置 DICBIT 到 15 或以上, 设置 PBIT 到 5; 而且如果你设置 DICBIT 到 19
   或以上, 也设置 NPT 为 NP.
 
   WINBIT : Sliding window 大小.
   压缩率很大程序上取决于该值.
   对于大的文件你可以增加它到 15 来取得更好的结果.
   I recomm} doing this if (you have enough memory, except if (you want that
   your compressed data remain compatible with LHArc.
   On a 32 bit platfor()m, you can increase it to 16. Using a larger value will
   only waste time and memory.
 
   BUFBIT : I/O Buffer size. You can lower it to save memory, or increase it
   to reduce disk access.
*/

#define BITBUFSIZ 16
#define UCHARMAX 255

#define DICBIT 13
#define DICSIZ (1 << DICBIT)

#define MATCHBIT 8
#define MAXMATCH (1 << MATCHBIT)
#define THRESHOLD 3
#define PERCFLAG 0x8000

#define NC (UCHARMAX+MAXMATCH+2-THRESHOLD)
#define CBIT 9
#define CODEBIT 16

#define NP DICBIT+1
#define NT CODEBIT+3
#define PBIT 4 ///*Log2(NP)*/
#define TBIT 5 ///*Log2(NT)*/
#define NPT NT ///*Greater from NP and NT*/
#define NUL 0
#define MAXHASHVAL (3*DICSIZ+(DICSIZ >> (9+1))*UCHARMAX)

#define WINBIT 14
#define WINDOWSIZE 1 << WINBIT

#define BUFBIT 13
#define BUFSIZE (1 << BUFBIT)

#define ord(x)   (Byte)(x)
#define PRED(x)  ((x)-1)
#define SUCC(x)  ((x)+1)
#define INC(x)   (++(x))
#define DEC(x)   (--(x))
#define INC2(_X, _N)   (_X)+=(_N)
#define DEC2(_X, _N)   (_X)-=(_N)
#define HALT(x)  exit(x)
#define Addr(x)  &(x)
#define word(x)  (Word)(x)

#define FREAD(InFile, ptr, size) fread(ptr, size, 1, InFile)
#define FWRITE(OutFile, ptr, size) fwrite(ptr, size, 1, OutFile)

#define REPEAT   do {
#define UNTIL(x) } while(!(x))

#define NOT(_x)  (~(_x))
#define EXIT  return
#define TRUE  (1)
#define FALSE (0)

#define GetMem(ptr, siz)        ptr = malloc(siz)
#define FreeMem(ptr, siz)       free(ptr)
#define FillChar(ptr, siz, _N)  memset(ptr, _N, siz)

#define Dispose(_X)  {}
#define New(_X)  {}
#define chr(_X)  (char)(_X)

#if 1
Byte Buffer[ 1 + PRED( BUFSIZE ) ];
Word Left[ 1 + 2*( NC - 1 ) ];
Word Right[ 1 + 2*( NC - 1 ) ];
Word CTable[ 1 + 4095 ];
Byte CLen[ 1 + PRED( NC ) ];
Word Heap[ 1 + NC ];
#else
PByte Buffer;
PWord Left;
PWord Right;
PWord CTable;
PByte CLen;
PWord Heap;
#endif

LongInt OrigSize, CompSize;
TStream InFile, OutFile;

Word BitBuf;

TwoByteInt n, HeapSize;
Word SubBitBuf, BitCount;
Word BufPtr;
Word PtTable[ 1 + 255 ];
Byte PtLen[ 1 + PRED( NPT ) ];
Word BlockSize;
Word LenCnt[ 1 + 16 ];

///* The following variables are used by the compression engine only */

PWord Freq;
PWord SortPtr;
PByte Len;
PByte Buf;
Word Depth;
Word CFreq[ 1 + 2*( NC - 1 ) ];
Word PFreq[ 1 + 2*( NP - 1 ) ];
Word TFreq[ 1 + 2*( NT - 1 ) ];
Word CCode[ 1 + PRED( NC ) ];
Word PtCode[ 1 + PRED( NPT ) ];
Word CPos, OutputPos, OutputMask;
PByte Text;
PByte ChildCount;
Word Pos, MatchPos, Avail;
PWord Position;
PWord Parent;
PWord Prev;
PWord Next;
TwoByteInt Remainder, MatchLen;
PByte Level;

/*********************************** File I/O ***********************************/
Integer GETCHECKSUM( String STR ) {
  Integer i;
  Integer b;
  b = 0;
  for ( i = 0; i < ( int ) strlen( STR ); ++i )
    b = b + ord( STR[ i ] );
  b = b % 256;
  return b;
};

// fread(InFile
Byte GetC() {
  Byte ret;
  if ( BufPtr == 0 )
    FREAD( InFile, Buffer, BUFSIZE );
  ret = Buffer[ BufPtr ];
  BufPtr = ( Word ) ( SUCC( BufPtr ) & PRED( BUFSIZE ) );
  return ret;
}

void PutC( Byte c ) {
  if ( BufPtr == BUFSIZE ) {
    FWRITE( OutFile, Buffer, BUFSIZE );
    BufPtr = 0;
  }
  Buffer[ BufPtr ] = c;
  INC( BufPtr );
}

//TwoByteInt BRead(POINTER p, TwoByteInt n) { return FREAD(InFile, p, n); }
//void BWrite(POINTER p, TwoByteInt n) { FWRITE(OutFile, p,n); }

//{**************************** Bit handling routines ***************************}

void FillBuf( TwoByteInt n ) {
  BitBuf = ( BitBuf << n );
  while ( n > BitCount ) {
    DEC2( n, BitCount );
    BitBuf = BitBuf | ( SubBitBuf << n );
    if ( ( CompSize != 0 ) ) {
      DEC( CompSize );
      SubBitBuf = GetC();
    } else
      SubBitBuf = 0;
    BitCount = 8;
  }
  DEC2( BitCount, n );
  BitBuf = BitBuf | ( SubBitBuf >> BitCount );
}

Word GetBits( TwoByteInt n ) {
  Word ret = BitBuf >> ( BITBUFSIZ - n );
  FillBuf( n );
  return ret;
}

void PutBits( TwoByteInt n, Word x ) {
  if ( n < BitCount ) {
    DEC2( BitCount, n );
    SubBitBuf = SubBitBuf | ( x << BitCount );
  } else {
    DEC2( n, BitCount );
    PutC( ( Byte ) ( SubBitBuf | ( x >> n ) ) );
    INC( CompSize );
    if ( n < 8 ) {
      BitCount = 8 - n;
      SubBitBuf = x << BitCount;
    } else {
      PutC( ( Byte ) ( x >> ( n - 8 ) ) );
      INC( CompSize );
      BitCount = 16 - n;
      SubBitBuf = x << BitCount;
    }
  }
}

void InitGetBits() {
  BitBuf = 0;
  SubBitBuf = 0;
  BitCount = 0;
  FillBuf( BITBUFSIZ );
}

void InitPutBits() {
  BitCount = 8;
  SubBitBuf = 0;
}

//{******************************** Decompression *******************************}

void MakeTable( TwoByteInt nchar, PByte BitLen, TwoByteInt TableBits, PWord Table ) {
  Word count[ 17 ], weight[ 17 ];
  Word start[ 17 ];
  PWord p;
  TwoByteInt k, i, Len, ch, jutbits, Avail, nextCode, mask;
  
  for ( i = 1; i <= 16; ++i )
    count[ i ] = 0;
  for ( i = 0; i < ( nchar ); ++i )
    INC( count[ BitLen[ i ] ] );
  start[ 1 ] = 0;
  for ( i = 1; i <= 16; ++i )
    start[ SUCC( i ) ] = start[ i ] + ( count[ i ] << ( 16 - i ) );
  if ( start[ 16 ] != 0 )
    HALT( 1 );
  jutbits = 16 - TableBits;
  for ( i = 0; i < TableBits; ++i ) {
    start[ i ] = start[ i ] >> jutbits;
    weight[ i ] = 1 << ( TableBits - i - 1 );
  }
  i = SUCC( TableBits );
  while ( i <= 16 ) {
    weight[ i ] = 1 << ( 16 - i );
    INC( i );
  }
  i = start[ SUCC( TableBits ) ] >> jutbits;
  if ( i != 0 ) {
    k = 1 << TableBits;
    while ( i != k ) {
      Table[ i ] = 0;
      INC( i );
    }
  }
  Avail = nchar;
  mask = 1 << ( 15 - TableBits );
  for ( ch = 0; ch <= PRED( nchar ); ++ch ) {
    Len = BitLen[ ch ];
    if ( Len == 0 )
      continue;
    k = start[ Len ];
    nextCode = k + weight[ Len ];
    //printf("%d\n", ch);
    if ( Len <= TableBits ) {
      //if (nextCode<=4096 && k>=0)
      for ( i = k; i < ( nextCode ); ++i )
        Table[ i ] = ch;
    } else {
      p = Addr( Table[ word( k ) >> jutbits ] );
      i = Len - TableBits;
      while ( i != 0 ) {
        if ( p[ 0 ] == 0 ) {
          Right[ Avail ] = 0;
          Left[ Avail ] = 0;
          p[ 0 ] = Avail;
          INC( Avail );
        }
        if ( ( k & mask ) != 0 )
          p = Addr( Right[ p[ 0 ] ] );
        else
          p = Addr( Left[ p[ 0 ] ] );
        k = k << 1;
        DEC( i );
      }
      p[ 0 ] = ch;
    }
    start[ Len ] = nextCode;
  }
}

void ReadPtLen( TwoByteInt nn, TwoByteInt nBit, TwoByteInt ispecial ) {
  TwoByteInt i, c, n;
  Word mask;
  n = GetBits( nBit );
  if ( n == 0 ) {
    c = GetBits( nBit );
    for ( i = 0; i <= PRED( nn ); ++i )
      PtLen[ i ] = 0;
    for ( i = 0; i <= 255; ++i )
      PtTable[ i ] = c;
  } else {
    i = 0;
    while ( ( i < n ) ) {
      c = BitBuf >> ( BITBUFSIZ - 3 );
      if ( c == 7 ) {
        mask = 1 << ( BITBUFSIZ - 4 );
        while ( ( mask & BitBuf ) != 0 ) {
          mask = mask >> 1;
          INC( c );
        }
      }
      if ( c < 7 )
        FillBuf( 3 );
      else
        FillBuf( ( TwoByteInt ) ( c - 3 ) );
      PtLen[ i ] = ( Byte ) c;
      INC( i );
      if ( i == ispecial ) {
        c = PRED( ( TwoByteInt ) ( GetBits( 2 ) ) );
        while ( c >= 0 ) {
          PtLen[ i ] = 0;
          INC( i );
          DEC( c );
        }
      }
    }
    while ( i < nn ) {
      PtLen[ i ] = 0;
      INC( i );
    }
    MakeTable( nn, PtLen, 8, PtTable );
  }
}

void ReadCLen() {
  TwoByteInt i, c, n;
  Word mask;
  n = GetBits( CBIT );
  if ( n == 0 ) {
    c = GetBits( CBIT );
    for ( i = 0; i <= PRED( NC ); ++i )
      CLen[ i ] = 0;
    for ( i = 0; i <= 4095; ++i )
      CTable[ i ] = c;
  } else {
    i = 0;
    while ( i < n ) {
      c = PtTable[ BitBuf >> ( BITBUFSIZ - 8 ) ];
      if ( c >= NT ) {
        mask = 1 << ( BITBUFSIZ - 9 );
        do {
          if ( ( BitBuf & mask ) != 0 )
            c = Right[ c ];
          else
            c = Left[ c ];
          mask = mask >> 1;
        } while ( c >= NT ); // UNTIL c<NT;
      }
      FillBuf( PtLen[ c ] );
      if ( c <= 2 ) {
        if ( c == 1 )
          c = 2 + GetBits( 4 );
        else
          if ( c == 2 )
            c = 19 + GetBits( CBIT );
        while ( c >= 0 ) {
          CLen[ i ] = 0;
          INC( i );
          DEC( c );
        }
      } else {
        CLen[ i ] = c - 2;
        INC( i );
      }
    }
    while ( i < NC ) {
      CLen[ i ] = 0;
      INC( i );
    }
    MakeTable( NC, ( CLen ), 12, ( CTable ) );
  }
}

Word DecodeC() {
  Word j, mask;
  if ( BlockSize == 0 ) {
    BlockSize = GetBits( 16 );
    ReadPtLen( NT, TBIT, 3 );
    ReadCLen();
    ReadPtLen( NP, PBIT, -1 );
  }
  DEC( BlockSize );
  j = CTable[ BitBuf >> ( BITBUFSIZ - 12 ) ];
  if ( j >= NC ) {
    mask = 1 << ( BITBUFSIZ - 13 );
    REPEAT
    if ( ( BitBuf & mask ) != 0 )
      j = Right[ j ];
    else
      j = Left[ j ];
    mask = mask >> 1;
    UNTIL( j < NC );
  }
  FillBuf( CLen[ j ] );
  return j;
}

Word DecodeP() {
  Word j, mask;
  j = PtTable[ BitBuf >> ( BITBUFSIZ - 8 ) ];
  if ( j >= NP ) {
    mask = 1 << ( BITBUFSIZ - 9 );
    REPEAT
    if ( ( BitBuf & mask ) != 0 )
      j = Right[ j ];
    else
      j = Left[ j ];
    mask = mask >> 1;
    UNTIL( j < NP );
  }
  FillBuf( PtLen[ j ] );
  if ( j != 0 ) {
    DEC( j );
    j = ( 1 << j ) + GetBits( j );
  }
  return j;
}

// {declared as static vars}
static Word Decode_i;
static TwoByteInt Decode_j;

void DecodeBuffer( Word count, PByte Buffer ) {
  Word c, r;
  r = 0;
  DEC( Decode_j );
  while ( ( Decode_j >= 0 ) ) {
    Buffer[ r ] = Buffer[ Decode_i ];
    Decode_i = ( Word ) ( SUCC( Decode_i ) & PRED( DICSIZ ) );
    INC( r );
    if ( r == count )
      EXIT;
    DEC( Decode_j );
  }
  while ( TRUE ) {
    c = DecodeC();
    if ( c <= UCHARMAX ) {
      Buffer[ r ] = ( Byte ) c;
      INC( r );
      if ( r == count )
        EXIT;
    } else {
      Decode_j = c - ( UCHARMAX + 1 - THRESHOLD );
      Decode_i = ( Word ) ( ( ( LongInt ) ( r ) - DecodeP() - 1 ) & PRED( DICSIZ ) );
      DEC( Decode_j );
      while ( Decode_j >= 0 ) {
        Buffer[ r ] = Buffer[ Decode_i ];
        Decode_i = ( Word ) ( SUCC( Decode_i ) & PRED( DICSIZ ) );
        INC( r );
        if ( r == count )
          EXIT;
        DEC( Decode_j );
      }
    }
  }
}

void Decode() {
  PByte p;
  LongInt l;
  Word a;
  //{Initialize Decoder variables}
  GetMem( p, DICSIZ );
  InitGetBits();
  BlockSize = 0;
  Decode_j = 0;
  //{skip file size}
  l = OrigSize;
  DEC2( CompSize, 4 );
  //{unpacks the file}
  while ( l > 0 ) {
    if ( l > DICSIZ )
      a = DICSIZ;
    else
      a = ( Word ) l;
    DecodeBuffer( a, p );
    FWRITE( OutFile, p, a );
    DEC2( l, a );
  }
  FreeMem( p, DICSIZ );
}

//{********************************* Compression ********************************}

//{-------------------------------- Huffman part --------------------------------}

void CountLen( TwoByteInt i ) {
  if ( i < n ) {
    if ( Depth < 16 )
      INC( LenCnt[ Depth ] );
    else
      INC( LenCnt[ 16 ] );
  } else {
    INC( Depth );
    CountLen( Left[ i ] );
    CountLen( Right[ i ] );
    DEC( Depth );
  }
}

void MakeLen( TwoByteInt root ) {
  TwoByteInt i, k;
  Word cum;
  for ( i = 0; i <= 16; ++i )
    LenCnt[ i ] = 0;
  CountLen( root );
  cum = 0;
  for ( i = 16; i >= 1; --i )
    INC2( cum, LenCnt[ i ] << ( 16 - i ) );
  while ( cum != 0 ) {
    DEC( LenCnt[ 16 ] );
    for ( i = 15; i >= 1; --i )
      if ( LenCnt[ i ] != 0 ) {
        DEC( LenCnt[ i ] );
        INC2( LenCnt[ SUCC( i ) ], 2 );
        break;
      }
    DEC( cum );
  }
  for ( i = 16; i >= 1; --i ) {
    k = PRED( ( LenCnt[ i ] ) );
    while ( k >= 0 ) {
      DEC( k );
      Len[ SortPtr[ 0 ] ] = ( Byte ) i;
      __asm {
        ADD WORD PTR SortPtr, 2; //{SortPtr=Addr(SortPtr[1]);}
      }
    }
  }
}

void DownHeap( TwoByteInt i ) {
  TwoByteInt j, k;
  k = Heap[ i ];
  j = i << 1;
  while ( ( j <= HeapSize ) ) {
    if ( ( j < HeapSize ) & ( Freq[ Heap[ j ] ] > Freq[ Heap[ SUCC( j ) ] ] ) )
      INC( j );
    if ( Freq[ k ] <= Freq[ Heap[ j ] ] )
      break;
    Heap[ i ] = Heap[ j ];
    i = j;
    j = i << 1;
  }
  Heap[ i ] = k;
}

void MakeCode( TwoByteInt n, PByte Len, PWord Code ) {
  TwoByteInt i, k;
  Word start[ 1 + 17 ];
  start[ 1 ] = 0;
  for ( i = 1; i <= 16; ++i )
    start[ SUCC( i ) ] = ( start[ i ] + LenCnt[ i ] ) << 1;
  for ( i = 0; i <= PRED( n ); ++i ) {
    k = Len[ i ];
    Code[ i ] = start[ k ];
    INC( start[ k ] );
  }
}

TwoByteInt MakeTree( TwoByteInt NParm, PWord Freqparm, PByte LenParm, PWord Codeparm ) {
  TwoByteInt i, j, k, Avail;
  n = NParm;
  Freq = Freqparm;
  Len = LenParm;
  Avail = n;
  HeapSize = 0;
  Heap[ 1 ] = 0;
  for ( i = 0; i <= PRED( n ); ++i ) {
    Len[ i ] = 0;
    if ( Freq[ i ] != 0 ) {
      INC( HeapSize );
      Heap[ HeapSize ] = i;
    }
  }
  if ( HeapSize < 2 ) {
    Codeparm[ Heap[ 1 ] ] = 0;
    return Heap[ 1 ];
  }
  for ( i = ( HeapSize / 2 ); i >= 1; --i )
    DownHeap( i );
  SortPtr = Codeparm;
  REPEAT
  i = Heap[ 1 ];
  if ( i < n ) {
    SortPtr[ 0 ] = i;
    __asm {
      ADD WORD PTR SortPtr, 2; //{SortPtr=Addr(SortPtr[1]);}
    }
  }
  Heap[ 1 ] = Heap[ HeapSize ];
  DEC( HeapSize );
  DownHeap( 1 );
  j = Heap[ 1 ];
  if ( j < n ) {
    SortPtr[ 0 ] = j;
    __asm {
      ADD WORD PTR SortPtr, 2;
      {
        SortPtr = Addr( SortPtr[ 1 ] );
      }
    }
  }
  k = Avail;
  INC( Avail );
  Freq[ k ] = Freq[ i ] + Freq[ j ];
  Heap[ 1 ] = k;
  DownHeap( 1 );
  Left[ k ] = i;
  Right[ k ] = j;
  UNTIL( HeapSize <= 1 );
  SortPtr = Codeparm;
  MakeLen( k );
  MakeCode( NParm, LenParm, Codeparm );
  return k;
}

void CountTFreq() {
  TwoByteInt i, k, n, Count;
  for ( i = 0; i <= PRED( NT ); ++i )
    TFreq[ i ] = 0;
  n = NC;
  while ( ( n > 0 ) & ( CLen[ PRED( n ) ] == 0 ) )
    DEC( n );
  i = 0;
  while ( i < n ) {
    k = CLen[ i ];
    INC( i );
    if ( k == 0 ) {
      Count = 1;
      while ( ( i < n ) & ( CLen[ i ] == 0 ) ) {
        INC( i );
        INC( Count );
      }
      if ( Count <= 2 )
        INC2( TFreq[ 0 ], Count );
      else
        if ( Count <= 18 )
          INC( TFreq[ 1 ] );
        else
          if ( Count == 19 ) {
            INC( TFreq[ 0 ] );
            INC( TFreq[ 1 ] );
          } else
            INC( TFreq[ 2 ] );
    } else
      INC( TFreq[ k + 2 ] );
  }
}

void WritePtLen( TwoByteInt n, TwoByteInt nBit, TwoByteInt ispecial ) {
  TwoByteInt i, k;
  while ( ( n > 0 ) & ( PtLen[ PRED( n ) ] == 0 ) )
    DEC( n );
  PutBits( nBit, n );
  i = 0;
  while ( ( i < n ) ) {
    k = PtLen[ i ];
    INC( i );
    if ( k <= 6 )
      PutBits( 3, k );
    else {
      DEC2( k, 3 );
      PutBits( k, ( Word ) ( ( 1 << k ) - 2 ) );
    }
    if ( i == ispecial ) {
      while ( ( i < 6 ) & ( PtLen[ i ] == 0 ) )
        INC( i );
      PutBits( 2, ( Word ) ( ( i - 3 ) & 3 ) );
    }
  }
}

void WriteCLen() {
  TwoByteInt i, k, n, Count;
  n = NC;
  while ( ( n > 0 ) & ( CLen[ PRED( n ) ] == 0 ) )
    DEC( n );
  PutBits( CBIT, n );
  i = 0;
  while ( ( i < n ) ) {
    k = CLen[ i ];
    INC( i );
    if ( k == 0 ) {
      Count = 1;
      while ( ( i < n ) & ( CLen[ i ] == 0 ) ) {
        INC( i );
        INC( Count );
      }
      if ( Count <= 2 )
        for ( k = 0; i <= PRED( Count ); ++k )
          PutBits( PtLen[ 0 ], PtCode[ 0 ] );
      else
        if ( Count <= 18 ) {
          PutBits( PtLen[ 1 ], ( Word ) ( PtCode[ 1 ] ) );
          PutBits( 4, ( Word ) ( Count - 3 ) );
        } else
          if ( Count == 19 ) {
            PutBits( PtLen[ 0 ], PtCode[ 0 ] );
            PutBits( PtLen[ 1 ], PtCode[ 1 ] );
            PutBits( 4, 15 );
          } else {
            PutBits( PtLen[ 2 ], PtCode[ 2 ] );
            PutBits( CBIT, ( Word ) ( Count - 20 ) );
          }
    } else
      PutBits( PtLen[ k + 2 ], PtCode[ k + 2 ] );
  }
}

void EncodeC( TwoByteInt c ) {
  PutBits( CLen[ c ], CCode[ c ] );
}

void EncodeP( Word p ) {
  Word c, q;
  c = 0;
  q = p;
  while ( q != 0 ) {
    q = q >> 1;
    INC( c );
  }
  PutBits( PtLen[ c ], PtCode[ c ] );
  if ( c > 1 )
    PutBits( ( TwoByteInt ) ( PRED( c ) ), ( Word ) ( p & ( 0xffff >> ( 17 - c ) ) ) );
}

void SendBlock() {
  Word i, k, flags, root, Pos, Size;
  root = MakeTree( NC, CFreq, ( CLen ), CCode );
  Size = CFreq[ root ];
  PutBits( 16, Size );
  if ( root >= NC ) {
    CountTFreq();
    root = MakeTree( NT, TFreq, PtLen, PtCode );
    if ( root >= NT )
      WritePtLen( NT, TBIT, 3 );
    else {
      PutBits( TBIT, 0 );
      PutBits( TBIT, root );
    }
    WriteCLen();
  } else {
    PutBits( TBIT, 0 );
    PutBits( TBIT, 0 );
    PutBits( CBIT, 0 );
    PutBits( CBIT, root );
  }
  root = MakeTree( NP, PFreq, PtLen, PtCode );
  if ( root >= NP )
    WritePtLen( NP, PBIT, -1 );
  else {
    PutBits( PBIT, 0 );
    PutBits( PBIT, root );
  }
  Pos = 0;
  for ( i = 0; i <= PRED( Size ); ++i ) {
    if ( ( i & 7 ) == 0 ) {
      flags = Buf[ Pos ];
      INC( Pos );
    } else
      flags = flags << 1;
    if ( ( flags & ( 1 << 7 ) ) != 0 ) {
      k = ( Word ) ( Buf[ Pos ] + ( 1 << 8 ) );
      INC( Pos );
      EncodeC( k );
      k = Buf[ Pos ] << 8;
      INC( Pos );
      INC2( k, Buf[ Pos ] );
      INC( Pos );
      EncodeP( k );
    } else {
      k = Buf[ Pos ];
      INC( Pos );
      EncodeC( k );
    }
  }
  for ( i = 0; i <= PRED( NC ); ++i )
    CFreq[ i ] = 0;
  for ( i = 0; i <= PRED( NP ); ++i )
    PFreq[ i ] = 0;
}

void Output( Word c, Word p ) {
  OutputMask = OutputMask >> 1;
  if ( OutputMask == 0 ) {
    OutputMask = 1 << 7;
    if ( ( OutputPos >= WINDOWSIZE - 24 ) ) {
      SendBlock();
      OutputPos = 0;
    }
    CPos = OutputPos;
    INC( OutputPos );
    Buf[ CPos ] = 0;
  }
  Buf[ OutputPos ] = ( Byte ) c;
  INC( OutputPos );
  INC( CFreq[ c ] );
  if ( c >= ( 1 << 8 ) ) {
    Buf[ CPos ] = Buf[ CPos ] | OutputMask;
    Buf[ OutputPos ] = ( p >> 8 );
    INC( OutputPos );
    Buf[ OutputPos ] = ( Byte ) p;
    INC( OutputPos );
    c = 0;
    while ( p != 0 ) {
      p = p >> 1;
      INC( c );
    }
    INC( PFreq[ c ] );
  }
}

//{------------------------------- Lempel-Ziv part ------------------------------}

void InitSlide() {
  Word i;
  for ( i = DICSIZ; i <= ( DICSIZ + UCHARMAX ); ++i ) {
    Level[ i ] = 1;
#ifdef PERCOLATE
    
    Position[ i ] = NUL;
#endif
    
  }
  for ( i = DICSIZ; i <= PRED( 2 * DICSIZ ); ++i )
    Parent[ i ] = NUL;
  Avail = 1;
  for ( i = 1; i <= DICSIZ - 2; ++i )
    Next[ i ] = SUCC( i );
  Next[ PRED( DICSIZ ) ] = NUL;
  for ( i = ( 2 * DICSIZ ); i <= MAXHASHVAL; ++i )
    Next[ i ] = NUL;
}

//{ Hash function }
TwoByteInt Hash( TwoByteInt p, Byte c ) {
  return ( short ) ( p + ( c << ( DICBIT - 9 ) ) + 2 * DICSIZ );
}

TwoByteInt Child( TwoByteInt q, Byte c ) {
  TwoByteInt r;
  r = Next[ Hash( q, c ) ];
  Parent[ NUL ] = q;
  while ( Parent[ r ] != q )
    r = Next[ r ];
  return r;
}

void MakeChild( TwoByteInt q, Byte c, TwoByteInt r ) {
  TwoByteInt h, t;
  h = Hash( q, c );
  t = Next[ h ];
  Next[ h ] = r;
  Next[ r ] = t;
  Prev[ t ] = r;
  Prev[ r ] = h;
  Parent[ r ] = q;
  INC( ChildCount[ q ] );
}

void Split( TwoByteInt old ) {
  TwoByteInt new, t;
  new = Avail;
  Avail = Next[ new ];
  ChildCount[ new ] = 0;
  t = Prev[ old ];
  Prev[ new ] = t;
  Next[ t ] = new;
  t = Next[ old ];
  Next[ new ] = t;
  Prev[ t ] = new;
  Parent[ new ] = Parent[ old ];
  Level[ new ] = ( Byte ) MatchLen;
  Position[ new ] = Pos;
  MakeChild( new, Text[ MatchPos + MatchLen ], old );
  MakeChild( new, Text[ Pos + MatchLen ], Pos );
}

void InsertNode() {
  TwoByteInt q, r, j, t;
  Byte c;
  PChar t1, t2;
  if ( MatchLen >= 4 ) {
    DEC( MatchLen );
    r = ( TwoByteInt ) ( SUCC( MatchPos ) | DICSIZ );
    q = Parent[ r ];
    while ( q == NUL ) {
      r = Next[ r ];
      q = Parent[ r ];
    }
    while ( Level[ q ] >= MatchLen ) {
      r = q;
      q = Parent[ q ];
    }
    t = q;
#ifdef PERCOLATE
    
    while ( Position[ t ] < 0 ) {
      Position[ t ] = Pos;
      t = Parent[ t ];
    }
    if ( t < DICSIZ )
      Position[ t ] = Pos | PERCFLAG;
#else
      
    while ( t < DICSIZ ) {
      Position[ t ] = Pos;
      t = Parent[ t ];
    }
#endif
      
  } else {
    q = ( TwoByteInt ) ( Text[ Pos ] + DICSIZ );
    c = Text[ SUCC( Pos ) ];
    r = Child( q, c );
    if ( r == NUL ) {
      MakeChild( q, c, Pos );
      MatchLen = 1;
      EXIT;
    }
    MatchLen = 2;
  }
  while ( TRUE ) {
    if ( r >= DICSIZ ) {
      j = MAXMATCH;
      MatchPos = r;
    } else {
      j = Level[ r ];
      MatchPos = Position[ r ] & NOT( PERCFLAG );
    }
    if ( MatchPos >= Pos )
      DEC2( MatchPos, DICSIZ );
    t1 = Addr( Text[ Pos + MatchLen ] );
    t2 = Addr( Text[ MatchPos + MatchLen ] );
    while ( MatchLen < j ) {
      if ( t1 != t2 ) {
        Split( r );
        EXIT;
      }
      INC( MatchLen );
      INC( t1 );
      INC( t2 );
    }
    if ( MatchLen >= MAXMATCH )
      break;
    Position[ r ] = Pos;
    q = r;
    r = Child( q, ( Byte ) ord( t1 ) );
    if ( r == NUL ) {
      MakeChild( q, ( Byte ) ord( t1 ), Pos );
      EXIT;
    }
    INC( MatchLen );
  }
  t = Prev[ r ];
  Prev[ Pos ] = t;
  Next[ t ] = Pos;
  t = Next[ r ];
  Next[ Pos ] = t;
  Prev[ t ] = Pos;
  Parent[ Pos ] = q;
  Parent[ r ] = NUL;
  Next[ r ] = Pos;
}

void DeleteNode() {
  TwoByteInt r, s, t, u;
#ifdef PERCOLATE
  
  TwoByteInt q;
#endif
  
  if ( Parent[ Pos ] == NUL )
    EXIT;
  r = Prev[ Pos ];
  s = Next[ Pos ];
  Next[ r ] = s;
  Prev[ s ] = r;
  r = Parent[ Pos ];
  Parent[ Pos ] = NUL;
  DEC( ChildCount[ r ] );
  if ( ( r >= DICSIZ ) || ( ChildCount[ r ] > 1 ) )
    EXIT;
#ifdef PERCOLATE
    
  t = Position[ r ] & NOT( PERCFLAG );
#else
  
  t = Position[ r ];
#endif
  
  if ( t >= Pos )
    DEC2( t, DICSIZ );
#ifdef PERCOLATE
    
  s = t;
  q = Parent[ r ];
  u = Position[ q ];
  while ( ( u & PERCFLAG ) != 0 ) {
    u = u & NOT PERCFLAG;
    if ( u >= Pos )
      DEC( u, DICSIZ );
    if ( u > s )
      s = u;
    Position[ q ] = s | DICSIZ;
    q = Parent[ q ];
    u = Position[ q ];
  }
  if ( q < DICSIZ ) {
    if ( u >= Pos )
      DEC( u, DICSIZ );
    if ( u > s )
      s = u;
    Position[ q ] = s | DICSIZ | PERCFLAG;
  }
#endif
  s = Child( r, Text[ t + Level[ r ] ] );
  t = Prev[ s ];
  u = Next[ s ];
  Next[ t ] = u;
  Prev[ u ] = t;
  t = Prev[ r ];
  Next[ t ] = s;
  Prev[ s ] = t;
  t = Next[ r ];
  Prev[ t ] = s;
  Next[ s ] = t;
  Parent[ s ] = Parent[ r ];
  Parent[ r ] = NUL;
  Next[ r ] = Avail;
  Avail = r;
}

void GetNextMatch() {
  TwoByteInt n;
  DEC( Remainder );
  INC( Pos );
  if ( Pos == 2 * DICSIZ ) {
    memmove( &Text[ DICSIZ ], &Text[ 0 ], DICSIZ + MAXMATCH );
    n = FREAD( InFile, &Text[ DICSIZ + MAXMATCH ], DICSIZ );
    INC2( Remainder, n );
    Pos = DICSIZ;
  }
  DeleteNode();
  InsertNode();
}

void Encode() {
  TwoByteInt LastMatchLen, LastMatchPos;
  //{ 初始化压缩器变量 }
  GetMem( Text, 2 * DICSIZ + MAXMATCH );
  GetMem( Level, DICSIZ + UCHARMAX + 1 );
  GetMem( ChildCount, DICSIZ + UCHARMAX + 1 );
#ifdef PERCOLATE
  
  GetMem( Position, ( DICSIZ + UCHARMAX + 1 ) * sizeof( Word ) );
#else
  
  GetMem( Position, ( DICSIZ ) * sizeof( Word ) );
#endif
  
  GetMem( Parent, ( DICSIZ * 2 ) * sizeof( Word ) );
  GetMem( Prev, ( DICSIZ * 2 ) * sizeof( Word ) );
  GetMem( Next, ( MAXHASHVAL + 1 ) * sizeof( Word ) );
  
  Depth = 0;
  InitSlide();
  GetMem( Buf, WINDOWSIZE );
  Buf[ 0 ] = 0;
  FillChar( CFreq, sizeof( CFreq ), 0 );
  FillChar( PFreq, sizeof( PFreq ), 0 );
  OutputPos = 0;
  OutputMask = 0;
  InitPutBits();
  Remainder = FREAD( InFile, &Text[ DICSIZ ], DICSIZ + MAXMATCH );
  MatchLen = 0;
  Pos = DICSIZ;
  InsertNode();
  if ( MatchLen > Remainder )
    MatchLen = Remainder;
  while ( Remainder > 0 ) {
    LastMatchLen = MatchLen;
    LastMatchPos = MatchPos;
    GetNextMatch();
    if ( MatchLen > Remainder )
      MatchLen = Remainder;
    if ( ( MatchLen > LastMatchLen ) || ( LastMatchLen < THRESHOLD ) )
      Output( Text[ PRED( Pos ) ], 0 );
    else {
      Output( ( Word ) ( LastMatchLen + ( UCHARMAX + 1 - THRESHOLD ) ), ( Word ) ( ( Pos - LastMatchPos - 2 ) & PRED( DICSIZ ) ) );
      DEC( LastMatchLen );
      while ( LastMatchLen > 0 ) {
        GetNextMatch();
        DEC( LastMatchLen );
      }
      if ( MatchLen > Remainder )
        MatchLen = Remainder;
    }
  }
  //{flush buffers}
  SendBlock();
  PutBits( 7, 0 );
  if ( BufPtr != 0 )
    FWRITE( OutFile, Buffer, BufPtr );
    
  FreeMem( Buf, WINDOWSIZE );
  FreeMem( Next, ( MAXHASHVAL + 1 ) * sizeof( Word ) );
  FreeMem( Prev, ( DICSIZ * 2 ) * sizeof( Word ) );
  FreeMem( Parent, ( DICSIZ * 2 ) * sizeof( Word ) );
#ifdef PERCOLATE
  
  FreeMem( Position, ( DICSIZ + UCHARMAX + 1 ) * sizeof( Word ) );
#else
  
  FreeMem( Position, ( DICSIZ ) * sizeof( Word ) );
#endif
  
  FreeMem( ChildCount, DICSIZ + UCHARMAX + 1 );
  FreeMem( Level, DICSIZ + UCHARMAX + 1 );
  FreeMem( Text, 2 * DICSIZ + MAXMATCH );
}

//{****************************** LH5 as Unit Procedures ************************}
void FreeMemory() {
  if ( CLen != NULL )
    Dispose( CLen );
  if ( CTable != NULL )
    Dispose( CTable );
  if ( Right != NULL )
    Dispose( Right );
  if ( Left != NULL )
    Dispose( Left );
  if ( Buffer != NULL )
    Dispose( Buffer );
  if ( Heap != NULL )
    Dispose( Heap );
}

void InitMemory() {
  /*{In should be harmless to call FreeMemory here, since it won't free
   unallocated memory (i.e., nil pointers).
   So let's call it in case an exception was thrown at some point and
   memory wasn't entirely freed.*/
  FreeMemory();
  
  New( Buffer );
  New( Left );
  New( Right );
  New( CTable );
  New( CLen );
  
  FillChar( Buffer, sizeof( Buffer ), 0 );
  FillChar( Left, sizeof( Left ), 0 );
  FillChar( Right, sizeof( Right ), 0 );
  FillChar( CTable, sizeof( CTable ), 0 );
  FillChar( CLen, sizeof( CLen ), 0 );
  
  Decode_i = 0;
  BitBuf = 0;
  n = 0;
  HeapSize = 0;
  SubBitBuf = 0;
  BitCount = 0;
  BufPtr = 0;
  FillChar( PtTable, sizeof( PtTable ), 0 );
  FillChar( PtLen, sizeof( PtLen ), 0 );
  BlockSize = 0;
  
  //{ The following variables are used by the compression engine only }
  New( Heap );
  FillChar( Heap, sizeof( Heap ), 0 );
  FillChar( LenCnt, sizeof( LenCnt ), 0 );
  Depth = 0;
  FillChar( CFreq, sizeof( CFreq ), 0 );
  FillChar( PFreq, sizeof( PFreq ), 0 );
  FillChar( TFreq, sizeof( TFreq ), 0 );
  FillChar( CCode, sizeof( CCode ), 0 );
  FillChar( PtCode, sizeof( PtCode ), 0 );
  CPos = 0;
  OutputPos = 0;
  OutputMask = 0;
  Pos = 0;
  MatchPos = 0;
  Avail = 0;
  Remainder = 0;
  MatchLen = 0;
}

static int GetLength( FILE* file ) {
  int old = ftell( file ), len = 0;
  fseek( file, 0, SEEK_END );
  len = ftell( file );
  fseek( file, old, SEEK_SET );
  return len;
}

//{******************************** Interface Procedures ************************}
void LHACompress( TStream InStr, TStream OutStr, String filename ) {
  char inivar[ 1024 ];
  Integer ix;
  char s[ 256 ];
  LongInt posi, posi1, posi2;
  InitMemory();
  {
    InFile = InStr;
    OutFile = OutStr;
    OrigSize = GetLength( InFile ) - ftell( InFile );
    //s=' -lh5-              ' + chr(strlen(filename))+filename + '  ';
    _snprintf( s, 256, " -lh5-              %c%s  ", chr( strlen( filename ) ), filename );
    //s=chr(strlen(s)-1)+s;
    _snprintf( s, 256, "%c%s", chr( strlen( filename ) ), s );
    CompSize = 0;
    posi = ftell( OutFile );
    for ( ix = 1; ix <= ( int ) strlen( s ); ++ix ) {
      if ( s[ ix ] != ' ' )
        inivar[ ix - 1 ] = s[ ix ];
      else
        inivar[ ix - 1 ] = chr( 0 );
    }
    FWRITE( OutFile, inivar, strlen( s ) );   //  写文件标记到压缩文件中
    Encode();
    posi1 = ftell( OutFile );
    //OutFile.Position=posi + 11;
    fseek( OutFile, ( long ) ( posi + 11 ), SEEK_SET );
    FWRITE( OutFile, &OrigSize, 4 );
    //OutFile.Position=posi + 7;
    fseek( OutFile, ( long ) ( posi + 7 ), SEEK_SET );
    posi2 = posi1 - posi - strlen( s );
    FWRITE( OutFile, &posi2, 4 );
    //OutFile.Position=posi1;
    fseek( OutFile, ( long ) ( posi1 ), SEEK_SET );
    //FreeMemory;
  }
}

void LHAExpand( TStream InStr, TStream OutStr ) {
  InitMemory();
  InFile = InStr;
  OutFile = OutStr;
  CompSize = GetLength( InFile ) - ftell( InFile );
  FREAD( InFile, &OrigSize, 4 );
  Decode();
  
  //FreeMemory();
}

#if 0

INITIALIZATION
CLen = nil;
CTable = nil;
Right = nil;
Left = nil;
Buffer = nil;
Heap = nil;
}.

#endif


Integer GETCHECKSUM2( String STR ) {
  Integer i;
  Integer b;
  b = 0;
  for ( i = 1 ; i <= ( int ) strlen( STR ); ++i )
    b = b + ord( STR[ i ] );
  b = b % 256;
  return b;
};

Integer HEX2Integer( String str ) {
  Integer i, Int = 0;
  for ( i = 1 ; i <= ( int ) strlen( str ) ; ++i )
    if ( str[ i ] < 'A' )
      Int = Int * 16 + ord( str[ i ] ) - 48;
    else
      Int = Int * 16 + HEX[ str[ i ] ];
  return Int;
};

String Dec2Hex( Word Num ) {
  static char S[ 10 ];
  _snprintf( S, 10, "%x", Num );
  return S;
};

#define xor ^
String ConvertTxt( String s ) {
  Byte x, xx;
  static char ss[ 256 ] = "";
  Integer ii;
  for ( x = 1 ; x <= strlen( s ); ++x ) {
    ii = ord( s[ x ] ) xor ( x + 126 );
    //ss = ss + Dec2Hex(ii);
    xx = ii;
    _snprintf( ss, 10, "%s%x", xx );
  };
  return ss;
};

typedef int Boolean;

Integer totallength;
Integer currentlength;

void Button2Click( const char * FileName, const char * path ) {
  FILE * iFileHandle;
  Integer iFileLength;
  Byte Buffer[ 1 + 65536 ];
  Integer aj, ii, i, j, k;
  char s[ 256 ];
  char s1[ 256 ];
  Integer filenameBlockBegin;
  Integer filenum;
  Integer filenamelen;
  Integer dataposi;
  Integer PackSize;
  Integer PrePackSize;
  Integer titlesize;
  //TListItem NewItem : ;
  Boolean IsComp;
  char BeforPack[ 256 ];
  char path1[ 256 ];
  if ( !path ) {
    strcpy( path1, FileName );
    *strrchr( path1, '.' ) = '\0';
    path = path1;
  }
  //if not OpenDialog1.Execute then Exit;
  {
    iFileHandle = fopen( FileName, "rb" );
    iFileLength = GetLength( iFileHandle ); // SEEK_END
    fseek( iFileHandle, iFileLength - 4, 0 );
    FREAD( iFileHandle, Buffer, 4 );
    
    // 简单的文件判断,可以去掉不要
    if ( ( ord( Buffer[ 0 ] ) != 239 ) ||
         ( ord( Buffer[ 1 ] ) != 81 ) ||
         ( ord( Buffer[ 2 ] ) != 42 ) ||
         ( ord( Buffer[ 3 ] ) != 1 ) ) {
      printf( "该文件不是 WebCompiler 生成的 EXE 文件" );
      fclose( iFileHandle );
      EXIT;
    };
    
    aj = 1;
    totallength = 0;
    fseek( iFileHandle, iFileLength - 8, 0 ); // 找到文件数据块的偏移地址
    FREAD( iFileHandle, Buffer, 4 ); // 读入四字节文件名数据块起始位置
    filenameBlockBegin = *( int* ) Buffer;
    
    // 移动文件指针到 文件名数据块偏移处 并求得总的文件数(总的文件数是最初四字节)
    fseek( iFileHandle, filenameBlockBegin, 0 );
    FREAD( iFileHandle, Buffer, 4 );
    filenum = *( int* ) Buffer; // 文件数量
    //edit1.Text = inttostr(filenum);
    
    // 移动文件指针到第一个文件头信息位置
    filenameBlockBegin = filenameBlockBegin + 4;
    //ListView1.Clear;
    
    //  逐个文件处理
    for ( i = 0 ; i < filenum ; ++i ) {
      fseek( iFileHandle, filenameBlockBegin, 0 ); // 移动文件指针到文件名大小偏移处
      FREAD( iFileHandle, Buffer, 4 ); // 读入文件名大小
      filenamelen = *( int* ) Buffer; // 计算文件名大小
      // ***************************************
      fseek( iFileHandle, filenameBlockBegin + 4, 0 ); //移动文件指针到实际的文件名开始处(文件大小后四字节)
      // ***************************************
      for ( ii = 1 ; ii <= 1024; ++ii )
        Buffer[ ii ] = chr( 0 );
      FREAD( iFileHandle, Buffer, filenamelen ); // 读入文件名
      s[ 0 ] = '\0';
      //         application.messagebox(pchar(inttostr(filenamelen)),'',0);
      for ( j = 0 ; j < filenamelen; ++j )   // 解码读出的文件名
      {
        while ( aj > 129 )
          aj = aj - 129;
        ii = ord( Buffer[ j ] ) xor ( aj + 126 );
        //s = s + chr(ii);
        _snprintf( s, 256, "%s%c", s, chr( ii ) );
        INC( aj );
      };
      //NewItem = ListView1.Items.Add;
      //NewItem.Caption=s;
      
      filenameBlockBegin = filenameBlockBegin + filenamelen + 4;
      fseek( iFileHandle, filenameBlockBegin, 0 ); // 移动文件指针到文件名结束处
      FREAD( iFileHandle, Buffer, 4 ); // 读入四字节压缩数据的起始位置
      dataposi = *( int* ) Buffer; // 计算压缩数据的起始位置
      //         s = s + ',' + inttostr(dataposi);
      //NewItem.SubItems.add(inttostr(dataposi));
      
      //+++++++++++++++++++++++++++++++++++++++++++
      // 文件压缩前的大小在压缩文件头中
      // 从压缩文件头中获得该信息
      
      fseek( iFileHandle, dataposi, 0 );
      FREAD( iFileHandle, Buffer, 8 ); // 读入文件数据块开始 8 字节
      
      //if (Buffer[1]+Buffer[2]+Buffer[3]+Buffer[4]+Buffer[5]+Buffer[6]+Buffer[7]+Buffer[8] != "SPIS"+chr(26)+"LH5" )
      if ( Buffer[ 0 ] != 'S' || Buffer[ 1 ] != 'P' || Buffer[ 2 ] != 'I' || Buffer[ 3 ] != 'S' || Buffer[ 4 ] != 26 || Buffer[ 5 ] != 'L' || Buffer[ 6 ] != 'H' || Buffer[ 7 ] != '5' ) {
        IsComp = FALSE;
        BeforPack[ 0 ] = '\0';
        //            NewItem.SubItems.add('未知');
      } else {
        IsComp = TRUE;
        fseek( iFileHandle, dataposi + 8, 0 );
        FREAD( iFileHandle, Buffer, 2 ); // 读入两字节压缩前文件大小
        PrePackSize = *( Word* ) ( Buffer ); // 压缩前数据大小
        //            NewItem.SubItems.add(inttostr(dataposi));
        // BeforPack=inttostr(dataposi);
        //_snprintf(BeforPack, 256, "%d", dataposi);
      };
      
      filenameBlockBegin = filenameBlockBegin + 4; // 移动文件指针到压缩后数据的大小偏移处
      fseek( iFileHandle, filenameBlockBegin, 0 );
      FREAD( iFileHandle, Buffer, 4 ); // 读入压缩后数据的大小
      PackSize = *( int* ) ( Buffer ); // 计算压缩后数据的大小
      totallength = totallength + PackSize;
      //if (BeforPack[0]=='\0' ) NewItem.SubItems.add(inttostr(PackSize)) else  NewItem.SubItems.add(BeforPack);
      //NewItem.SubItems.add(inttostr(PackSize));
      
      //
      // 以下是为读入下一标题作准备:
      //
      filenameBlockBegin = filenameBlockBegin + 8; // 移动文件指针到文件标题大小处(有四字节不明信息)
      // 4 字节数据大小加上四字节 2CEDFE7F 标记
      fseek( iFileHandle, filenameBlockBegin, 0 );
      
      // 读入四字节标题长
      FREAD( iFileHandle, Buffer, 4 );
      titlesize = *( int* ) ( Buffer ); // 标题一长度
      
      filenameBlockBegin = filenameBlockBegin + 4; // 移动文件指针到文件标题一开始处(有四字节不明信息)
      fseek( iFileHandle, filenameBlockBegin, 0 );
      //FREAD(iFileHandle, Buffer, titlesize); // 读入标题一
      
      if ( titlesize != 0 )      // 标题一有内容
      {
        aj = aj + titlesize;  // 递增解密因子
        
        filenameBlockBegin = filenameBlockBegin + titlesize; // 跳到标题二大小处
        fseek( iFileHandle, filenameBlockBegin, 0 );
        
        FREAD( iFileHandle, Buffer, 4 );  // 计算标题二长度
        titlesize = *( int* ) ( Buffer );
        
        if ( titlesize != 0 )       // 标题一有内容
        {
          filenameBlockBegin = filenameBlockBegin + 4; // 移动文件指针到文件标题二开始处(有四字节不明信息)
          fseek( iFileHandle, filenameBlockBegin, 0 );
          FREAD( iFileHandle, Buffer, titlesize ); // 读入标题二
          filenameBlockBegin = filenameBlockBegin + titlesize; // 跳过实际标题二大小
          aj = aj + titlesize;    // 递增解密因子
        } else
          filenameBlockBegin = filenameBlockBegin + 4; // 标题二为空,跳过
      } else
        filenameBlockBegin = filenameBlockBegin + 6; // 标题一为空,跳过
        
      filenameBlockBegin = filenameBlockBegin + 1;
      fseek( iFileHandle, filenameBlockBegin, 0 );
      FREAD( iFileHandle, Buffer, 1 );
      
      k = 0;
      
      while ( Buffer[ 0 ] == ( 0 ) )    // 移动文件指针到下一个非 nil 处
      {
        filenameBlockBegin = filenameBlockBegin + 1;
        FREAD( iFileHandle, Buffer, 1 );
        INC( k );
        if ( k == 255 )
          break; // 用于防止死循环
      }
      
      //If IsComp==True then NewItem.SubItems.add('压缩') else NewItem.SubItems.add('储存');
      
      FREAD( iFileHandle, Buffer, 1 );
      if ( ( k < 255 ) && ( Buffer[ 1 ] != ( 0 ) ) )
        filenameBlockBegin = filenameBlockBegin + 1;
        
      printf( "%16s %8d %8d %8d %5s\n", s, dataposi, PrePackSize, PackSize, IsComp ? "压缩" : "储存" );
      {
        PByte Buf;
        PByte Buf1;
        FILE* iFileHandle1;
        int dataOffset = dataposi;
        int dataSize = PackSize;
        
        // 调整文件读写缓存大小
        GetMem( Buf, dataSize );
        GetMem( Buf1, dataSize );
        // 移动文件指针到数据偏移处
        fseek( iFileHandle, dataOffset, 0 );
        // 读出压缩后的数据
        FREAD( iFileHandle, Buf, dataSize );
        
        _snprintf( s1, 256, "%s\\%s", path, s + 3 );
        MkDir( s1 );
        iFileHandle1 = fopen( s1, "wb" );
        if ( !IsComp ) {
          FWRITE( iFileHandle1, Buf, dataSize );
          fclose( iFileHandle1 );
          FreeMem( Buf, dataSize );
          FreeMem( Buf1, dataSize );
        } else {
          // 执行实际的解压
          FILE* InStr, *OutStr = iFileHandle1;
          // 修改读出的数据以符合解压函数的要求
          for ( ii = 0 ; ii < 4; ++ii )
            Buf1[ ii ] = Buf[ ii + 8 ];
          for ( ii = 4 ; ii < dataSize - 17; ++ii )
            Buf1[ ii ] = Buf[ ii + 17 ];
            
          // 创建一个临时输出文件(解压函数需要从一个文件解压到源文件)
          strcpy( strrchr( s1, '\\' ), "\\temp.tmp" );
          iFileHandle1 = fopen( s1, "wb" );
          InStr = fopen( s1, "rb" );
          
          // 把修改后的压缩数据写入到临时文件中
          FWRITE( iFileHandle1, Buf1, dataSize - 17 );
          fclose( iFileHandle1 );
          FreeMem( Buf, dataSize );
          FreeMem( Buf1, dataSize );
          LHAExpand( InStr, OutStr );
          fclose( OutStr );
        }
      }
    }
  }
}


