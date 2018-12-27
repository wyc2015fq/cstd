#ifndef _BASE64_INL_
#define _BASE64_INL_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int base64_encode(const void* input, long size, char *base64Char)
{
  unsigned char *in = (unsigned char *)input;
  static const char *ALPHA_BASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  int i = 0, len = 0;
  char* a = base64Char;
  enum { int63 = 0x3F,  // 00111111
    int255 = 0xFF, // 11111111
  };
  //printf("in:%s\n",in);
  for (; i < size; a+=4) {
    unsigned char b0 = in[i++];
    unsigned char b1 = (i < size) ? in[i++] : 0;
    unsigned char b2 = (i < size) ? in[i++] : 0;
    
    a[0] = ALPHA_BASE[(b0 >> 2) & int63];
    a[1] = ALPHA_BASE[((b0 << 4) | ((b1 & int255) >> 4)) & int63];
    a[2] = ALPHA_BASE[((b1 << 2) | ((b2 & int255) >> 6)) & int63];
    a[3] = ALPHA_BASE[b2 & int63];
  }
  a[0] = 0;              //末尾置零
  len = a - base64Char;
  switch (size % 3) {
  case 1:
    a[-1] = '=';
  case 2:
    a[-2] = '=';
  }
  return len;
}
static int base64_decode_map[256] = {  
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0   - 15  
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 16  - 31  
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, // 32  - 47  
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, // 48  - 63  
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, // 64  - 79  
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 80  - 95  
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, // 96  - 111  
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, // 112 - 127  
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 128 - 143  
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 144 - 159   
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 160 - 175  
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 176 - 191  
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 192 - 207  
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 208 - 223  
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 224 - 239  
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 240 - 255  
};  
static int base64_decode(const char* input, int input_len, void* output)  
{  
  int len;
  //char _p[4] = {'=', '=', '=', '='};
  const char* p = input;
  const char *p_end = input + (input_len&~3);
  uchar *p_out = (uchar*)output;
  
  if (input_len == 0)
    return 0;
  
  // 0xFC -> 11111100  
  // 0x03 -> 00000011  
  // 0xF0 -> 11110000  
  // 0x0F -> 00001111  
  // 0xC0 -> 11000000  
  
  p_out[0] = '\0';
  for (; p < p_end; p += 4, p_out+=3) {
    p_out[0] = ((base64_decode_map[p[0]] << 2) & 0xFC) | ((base64_decode_map[p[1]] >> 4) & 0x03);  
    p_out[1] = ((base64_decode_map[p[1]] << 4) & 0xF0) | ((base64_decode_map[p[2]] >> 2) & 0x0F);  
    p_out[2] = ((base64_decode_map[p[2]] << 6) & 0xC0) | (base64_decode_map[p[3]]);   
  }
  if (input_len&3) {
    int i, n = input_len&3;
    for (i=0; i<n; ++i) {
    }
  }
  len = p_out - (uchar*)output;
  if (p[-2] == '=') {
    len-=2;
  } else if (p[-1] == '=') {  
    len-=1;
  }
  ((uchar*)output)[len] = 0;
  return len;  
}  

#if 0
// ndst >= (nsrc + 2) / 3 * 4 + 1
static int base64_encode(const void* src, int nsrc, void* dst, int ndst)
{
  int i, j;
  const unsigned char* usrc = (const unsigned char*)src;
  unsigned char* lpDestByte = (unsigned char*)dst;
  unsigned char byte[3];
  static const char key[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  if (ndst < (nsrc + 2) / 3 * 4) {
    // No enough room to fill the encoded data
    return 0;
  }
  //memset(dst, 0, ndst);
  for (i = 0, j = 0; i < nsrc; i += 3, j += 4) {
    byte[0] = usrc[i + 0];
    lpDestByte[j + 0] = key[(byte[0] & 0xfc) >> 2];
    if (i + 1 < nsrc) {
      byte[1] = usrc[i + 1];
      lpDestByte[j + 1] = key[((byte[0] & 0x03) << 4) | ((byte[1] & 0xf0) >> 4)];
    }
    else {
      byte[1] = 0;
      lpDestByte[j + 1] = key[((byte[0] & 0x03) << 4) | ((byte[1] & 0xf0) >> 4)];
      lpDestByte[j + 2] = '=';
      lpDestByte[j + 3] = '=';
      break;
    }
    if (i + 2 < nsrc) {
      byte[2] = usrc[i + 2];
      lpDestByte[j + 2] = key[((byte[1] & 0x0f) << 2) | ((byte[2] & 0xc0) >> 6)];
      lpDestByte[j + 3] = key[(byte[2] & 0x3f)];
    }
    else {
      byte[2] = 0;
      lpDestByte[j + 2] = key[((byte[1] & 0x0f) << 2) | ((byte[2] & 0xc0) >> 6)];
      lpDestByte[j + 3] = '=';
    }
  }
  return j;
}
static unsigned char DecodeByte(char ch)
{
  int i;
  static const char key[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  static uchar keymap[128] = {0};
  static int inited = 0;
  if (!inited) {
    inited = 1;
    for (i = 0; i < (int)strlen(key); ++i) {
      keymap[(uchar)key[i]] = i;
    }
  }
  return (ch == '=') ? ('\x80') : keymap[(uchar)ch];
#if 0
  for (i = 0; i < 64; i ++) {
    if (key[i] == ch) {
      return i;
    }
    if (ch == '=') {
      return '\x80';
    }
  }
  //ASSERT(FALSE);
  return '\x40';
#endif
}
// ndst >= nsrc * 3 / 4
static int base64_decode(const void* src, int nsrc, void* dst, int ndst)
{
  int i, j;
  const unsigned char* usrc = (const unsigned char*)src;
  unsigned char* lpDestByte = (unsigned char*)dst;
  unsigned char byte[4];
  nsrc = nsrc < 0 ? strlen((const char*)src) : nsrc;
  if (ndst < nsrc * 3 / 4) {
    // No enough room to fill the encoded data
    return -1;
  }
  //memset(dst, 0, ndst);
  for (i = 0, j = 0; i < nsrc;) {
    // 现在这4个字节都是 00ABCDEF 00GHIJKL 00MNOPQR 00STUVWX
    if (usrc[i + 0] <= ' ') {
      i ++;
      continue;
    }
    byte[0] = DecodeByte(usrc[i + 0]);
    if (byte[0] & 0xC0) {
      break;
    }
    byte[1] = DecodeByte(usrc[i + 1]);
    if (byte[1] & 0xC0) {
      break;
    }
    lpDestByte[j++] = (unsigned char)(byte[0] << 2) | (unsigned char)((byte[1] & 0x30) >> 4); // ABCDEF-GH
    byte[2] = DecodeByte(usrc[i + 2]);
    if (byte[2] & 0xC0) {
      break;
    }
    lpDestByte[j++] = (unsigned char)(byte[1] << 4) | (unsigned char)((byte[2] & 0x3C) >> 2); // IJKL-MNOP
    byte[3] = DecodeByte(usrc[i + 3]);
    if (byte[3] & 0xC0) {
      break;
    }
    lpDestByte[j++] = (unsigned char)(byte[2] << 6) | (unsigned char)byte[3]; // QRSTUVWX
    i += 4;
  }
  return j;
}
#endif
static int QuotedPrintableDecode(const char* src, char* dst)
{
  const char* p = src;
  char* q = dst;
  while (*p) {
    char ch = *p;
    if (ch == '=') {
      int nValue = 0;
      sscanf(p, "=%02X", &nValue);
      *q = (char)nValue;
      p += 3;
    }
    else {
      *q = ch;
      p ++;
    }
    q ++;
  }
  *q = '\0';
  return q - src;
}
#endif // _BASE64_INL_
