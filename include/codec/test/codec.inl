
#include <stdlib.h>
#include <string.h>
//#include "codec.h"

#define USEWIN32 1

#ifndef TRUE
#define TRUE (1)
#endif // TRUE

#ifndef FALSE
#define FALSE (0)
#endif // FALSE

typedef int BOOL;
typedef unsigned char byte;

typedef unsigned short ushort;

#define STRREPLACE(_N, _A, _V, _R) do {                     \
    int _i;                                                   \
    for (_i=0; _i<_N; ++_i) {                                 \
      if (*(_A+_i) == (_V)) {                                 \
        *(_A+_i) = _R;                                        \
      }                                                       \
    }                                                         \
  } while(0)

#define CTISGB(_PCS)  (0==(*(_PCS)&0x80) || (FONT_ROW_BEGIN<=*(_PCS) && *(_PCS)<=FONT_ROW_END && FONT_COL_BEGIN<=*(_PCS+1) && *(_PCS+1)<=FONT_COL_END))

int isurl(const unsigned char* src, int len)
{
  int i;

  for (i = 0; i < len; i += 3) {
    if ('%' != src[ i ]) {
      return 0;
    }
  }

  return 1;
}

#if 1
// WIN32
#if (1==USEWIN32)
#include "codec_win32.inl"
#else
#include "codec_table.inl"
#endif
#else
#include "codec_table2.inl"
#endif

CODETYPE icodetype(const void* src, int len)
{
  CODETYPE t = ct_unknown;
  t = ct_gb;

  if (isurl(src, len)) {
    t = ct_url;
  }
  else if (isutf8(src, len)) {
    t = ct_utf;
  }
  else if (isgb(src, len)) {
    t = ct_gb;
  }
  else if (isuni(src, len)) {
    t = ct_uni;
  }

  if (t != ct_utf) {
    int asdf = isutf8(src, len);
  }

  return t;
}

// 64 characters for base64 coding
byte base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// 8 characters are safe just as base64 characters for MAIL gates
byte safeChars[] = "'(),-.:?";

// 4 characters all means space
byte spaceChars[] = " \t\n\r";

// mask value defined for indentify the type of a byte
#define BASE64  0x01
#define SAFE  0x02
#define SPACE  0x04
byte byteType[ 256 ];        // hash table used for find the type of a byte
BOOL firstTime = TRUE;     // the first time to use the lib, wait for init the table

// 注：为了解码base64编码部分的字符，需要一个哈希表，对一个base64字符都可以直接得到0-64之间的一个数：
byte base64Value[ 128 ];


void initUTF7Tables()
{
  byte* s;

  if (!firstTime) {
    return ;
  }

  // not necessary, but should do it to be robust
  memset(byteType, 0, 256);
  memset(base64Value, 0, 128);

  for (s = base64Chars; *s != '\0'; s++) {
    byteType[ *s ] |= BASE64;
    base64Value[ *s ] = s - base64Chars; // the offset, it is a 6bits value,0-64
  }

  for (s = safeChars; *s != '\0'; s++) {
    byteType[ *s ] |= SAFE;
  }

  for (s = spaceChars; *s != '\0'; s++) {
    byteType[ *s ] |= SPACE;
  }

  firstTime = FALSE;
}

// the state of current character
#define IN_ASCII 0
#define IN_BASE64 1
#define AFTER_PLUS 2

int state = IN_ASCII;          // state in which  we are working
int nbits = 0;                 // number of bits in the bit buffer
unsigned long bitBuffer = 0;   // used for base64 coding

int UnicodeToUTF7(ushort ucs2, byte* buffer, byte* s, int u2)
{
  byte* head = buffer;
  int index;

  // is an ASCII and is a byte in char set defined
  if (((ucs2 & 0xff80) == 0) && (byteType[(byte) u2 ] & (BASE64 | SAFE | SPACE))) {
    byte temp = (byte) ucs2;

    if (state == IN_BASE64) {   // should switch out from base64 coding here
      if (nbits > 0) {         // if some bits in buffer, then output them
        index = (bitBuffer << (6 - nbits)) & 0x3f;
        *s++ = base64Chars[ index ];
      }

      if ((byteType[ temp ] & BASE64) || (temp == '-')) {
        * s++ = '-';
      }

      state = IN_ASCII;
    }

    *s++ = temp;

    if (temp == '+') {
      * s++ = '-';
    }
  }
  else {
    if (state == IN_ASCII) {
      *s++ = '+';
      state = IN_BASE64;          // begins base64 coding here
      nbits = 0;
      bitBuffer = 0;
    }

    bitBuffer <<= 16;
    bitBuffer |= ucs2;
    nbits += 16;

    while (nbits >= 6) {
      nbits -= 6;
      index = (bitBuffer >> nbits) & 0x3f;     // output the high 6 bits
      *s++ = base64Chars[ index ];
    }
  }

  return (s - head);
}

#define RET0 0xfeff

ushort utf72uni(byte c)
{
  if (state == IN_ASCII) {
    if (c == '+') {
      state = AFTER_PLUS;
      return RET0;
    }
    else {
      return (ushort) c;
    }
  }

  if (state == AFTER_PLUS) {
    if (c == '-') {
      return (ushort) '+';
    }
    else {
      state = IN_BASE64;
      nbits = 0;
      bitBuffer = 0;  // it is not necessary
      // don't return yet, continue to the IN_BASE64 mode
    }
  }

  // state == Base64
  if (byteType[ c ] & BASE64) {
    bitBuffer <<= 6;
    bitBuffer |= base64Value[ c ];
    nbits += 6;

    if (nbits >= 16) {
      nbits -= 16;
      return (ushort)((bitBuffer >> nbits) & 0x0000ffff);
    }

    return RET0;
  }

  // encount a byte which is not in base64 character set, switch out of base64 coding
  state = IN_ASCII;

  if (c != '-') {
    return (ushort) c;
  }

  return RET0;
}

static const char* hexVals = "0123456789ABCDEF";
// UNSAFE String
static const char* csUnsafeString = "\"<>%\\^[]`+$,@:;/!#?=&";

// THIS IS A HELPER FUNCTION.
// PURPOSE OF THIS FUNCTION IS TO GENERATE A HEX REPRESENTATION OF GIVEN CHARACTER
#define DEC2HEX(dec, Hex) { *(Hex) = hexVals[(dec)&16]; *(Hex+1) = hexVals[((dec)>>4) & 16]; }
#define ISUNSAFE(_CH) (_CH & (1<<7))

int url2gb(const char* url, int urllen, char* gb)
{
  int i, j = 0;

  if (gb) {
    for (i = 0; i < urllen; ++i) {
      if (ISUNSAFE(url[ i ])) {
        // get Hex Value of the Character
        gb[ j++ ] = '%';
        DEC2HEX(url[ i ], gb + j);
        j += 2;
      }
      else {
        // Safe Character
        gb[ j ] = url[ i ];
        ++j;
      }
    }

    gb[ j ] = '\0';
  }
  else {
    for (i = 0; i < urllen; ++i) {
      if (ISUNSAFE(url[ i ])) {
        j += 3;
      }
      else {
        ++j;
      }
    }
  }

  return j;
}

static BYTE SZHEX[] = {
  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
  0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 0 , 0 , 0 , 0 , 0 , 0 ,
  0 , 10, 11, 12, 13, 14, 15, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
  0 , 10, 11, 12, 13, 14, 15, 0
}; //sizeof(SZHEX)=0x68

#define HEXChar(c) (c>0x68 ? 0 : SZHEX[c])

/* UrlDecode URL解码函数 */
int url2any(const void* src, int srclen, char* dst)
{
  char* srcc = (char*) src;
  char* dstc = (char*) dst;
  char* srce = srcc + srclen;

  if (!srcc || !dstc) {
    return 0;
  }

  for (; srcc < srce;) {
    if (*srcc == '+') {
      *dstc = ' ';
      srcc++;
    }
    else if (*srcc == '%') {
      *dstc = ((HEXChar(srcc[ 1 ]) << 4) | HEXChar(srcc[ 2 ]));
      srcc += 2;
    }
    else {
      *dstc = *srcc;
      srcc++;
    }

    dstc++;
  }

  *dstc = 0;
  return dstc - (char*) dst;
}

int any2url(const void* src, int srclen, void* dst)
{
  int i;
  unsigned char* srcc = (unsigned char*) src;
  unsigned char* dstc = (unsigned char*) dst;

  for (i = 0; i < srclen; ++i) {
    *dstc++ = '%';
    *dstc++ = hexVals[(srcc[ i ] >> 4) ];
    *dstc++ = hexVals[ srcc[ i ] & 0x0f ];
  }

  *dstc++ = '\0';
  return srclen * 3;
}

typedef int (*PCONV)(const void* src, int srclen, void* dst);
static PCONV conv_table[ ct_maxcodetype ][ ct_maxcodetype ] = {0};

#define CONV_TAB(_SRC, _DST)  conv_table[ct_##_SRC][ct_##_DST] = _SRC##2##_DST
int init_conv_table()
{
  CONV_TAB(gb, uni);
  CONV_TAB(uni, gb);
  CONV_TAB(uni, utf);
  CONV_TAB(utf, uni);
  CONV_TAB(utf, gb);
  CONV_TAB(gb, utf);
  return 1;
}

int iconv(CODETYPE dsttype, const void* src, int srclen, void* dst)
{
  CODETYPE srctype;
  PCONV conv = 0;
  static int inited_conv_table = 0;
  char* buf = 0;

  if (!inited_conv_table) {
    inited_conv_table = 1;
    init_conv_table();
  }

  srctype = icodetype(src, srclen);

  if (srctype == dsttype) {
    if (dst != src && dst) {
      memcpy(dst, src, srclen);
    }

    return srclen;
  }

  if (srctype == ct_url) {
    buf = (char*) malloc(srclen / 3 + 10);
    srclen = url2any(src, srclen, buf);
    srctype = icodetype(buf, srclen);
  }

  conv = conv_table[ srctype ][ dsttype ];

  if (conv) {
    int ret = 0;
    const void* src0 = src;

    if (dst == src) {
      src = malloc(srclen);
      memcpy((void*) src, src0, srclen);
    }

    ret = conv(src, srclen, dst);

    if (src0 != src) {
      free((void*) src);
    }

    return ret;
  }

  if (buf) {
    free(buf);
    buf = 0;
  }

  return 0;
}
