#ifndef _HUFF_INL_
#define _HUFF_INL_
#include <string.h>
#ifndef TRUE
#define TRUE 1
#endif // TRUE
#ifndef FALSE
#define FALSE 0
#endif // FALSE
#define MEM_LIMITLEN 1024000
#define END_OF_TABLE 256
#define MAX_TABLELEN 0x200
typedef unsigned int hist_type;
typedef struct Huff_Item {
  short child_0;
  short child_1;
  int parent;
  unsigned int count;
}
Huff_Item;
typedef struct Huff_Code {
  unsigned int code;
  int bitCount;
}
Huff_Code;
static int Build_HuffTree(Huff_Item table[])
{
  int i, min1, min2, cur;
  table[ MAX_TABLELEN - 1 ].count = 0xffffffff;
  for (cur = END_OF_TABLE;; cur++) {
    min1 = MAX_TABLELEN - 1;
    min2 = MAX_TABLELEN - 1;
    // 找最小和次小
    for (i = 0; i < cur; i++) {
      if (table[ i ].count != 0) {
        if (table[ i ].count < table[ min1 ].count) {
          min2 = min1;
          min1 = i;
        }
        else if (table[ i ].count < table[ min2 ].count) {
          min2 = i;
        }
      }
    }
    if (min2 == MAX_TABLELEN - 1) {
      break;
    }
    table[ cur ].child_0 = min1;
    table[ cur ].child_1 = min2;
    table[ cur ].parent = -1;
    table[ cur ].count = table[ min1 ].count + table[ min2 ].count;
    table[ min1 ].count = 0;
    table[ min2 ].count = 0;
    table[ min1 ].parent = cur;
    table[ min2 ].parent = cur;
  }
  return (cur - 1);
}
static void Covert_HuffTree_toCode(Huff_Item* huffTable, Huff_Code* codeTable)
{
  int i, cur, parent, bitCount;
  unsigned int code, mask;
  for (i = 0; i < END_OF_TABLE; i++) {
    bitCount = 0;
    code = 0;
    mask = 1;
    cur = i;
    parent = huffTable[ i ].parent;
    while (parent != -1) {
      if (cur == huffTable[ parent ].child_1) {
        code |= mask;
      }
      bitCount++;
      mask <<= 1;
      cur = parent;
      parent = huffTable[ parent ].parent;
    }
    codeTable[ i ].bitCount = bitCount;
    codeTable[ i ].code = code;
    //printf("%d -> \t%d:\t%d\n",i,bitCount,code);
  }
  return ;
}
static int Encode_Data(const unsigned char* pSrc, int nSrcLen, unsigned char* pDes, Huff_Code table[])
{
  int i, pDesLen;
  unsigned int code = 0;
  int bitCount = 0;
  const unsigned char* p1 = pSrc;
  unsigned char* p2 = pDes;
  if (pDes == NULL) {
    return 0;
  }
  for (i = 0; i < nSrcLen; i++, p1++) {
    code <<= table[ *p1 ].bitCount;
    code |= table[ *p1 ].code;
    bitCount += table[ *p1 ].bitCount;
    if (bitCount > 8) {
      while (bitCount > 8) {
        bitCount = bitCount - 8;
        *p2++ = (unsigned char)((code >> bitCount) & 0xff);
      }
      code &= (0xff >> (8 - bitCount));
    }
  }
  if (bitCount != 0) {
    * p2++ = (unsigned char)(code << (8 - bitCount));
  }
  pDesLen = p2 - pDes;
  return pDesLen;
}
static int Decode_Data(const unsigned char* pSrc, int nSrcLen, unsigned char* pDes, int nDesLen, Huff_Item table[], int root)
{
  unsigned int code = 0, mask = 0x80;
  int cur = 0, parent = 0, bitCount = 0;
  const unsigned char* p1 = pSrc;
  unsigned char* p2 = pDes;
  while (nDesLen > 0) {
    cur = root;
    while (1) {
      if (bitCount <= 0) {
        code |= *p1++;
        bitCount += 8;
      }
      parent = cur;
      if (code & mask) {
        cur = table[ parent ].child_1;
      }
      else {
        cur = table[ parent ].child_0;
      }
      if (cur == -1) {
        break;
      }
      code <<= 1;
      bitCount--;
    }
    *p2++ = parent;
    nDesLen--;
  }
  if (p1 - pSrc != nSrcLen) {
    return FALSE;
  }
  return TRUE;
}
static int Make_HuffTree(Huff_Item* huffTable, const hist_type* count)
{
  int i, root = 0;
  //初始化霍夫曼树
  memset(huffTable, 0, sizeof(huffTable[ 0 ]) * END_OF_TABLE);
  for (i = 0; i < END_OF_TABLE; i++) {
    huffTable[ i ].child_0 = -1;
    huffTable[ i ].child_1 = -1;
    huffTable[ i ].parent = -1;
    huffTable[ i ].count = count[ i ];
  }
  //创建霍夫曼树
  root = Build_HuffTree(huffTable);
  return root;
}
typedef struct HUFF {
  Huff_Item huffTable[ MAX_TABLELEN ];
  Huff_Code codeTable[ END_OF_TABLE ];
  //unsigned int count_u32[ END_OF_TABLE ];
  hist_type count[ END_OF_TABLE ];
} HUFF;
static int comphist(unsigned int* a, hist_type* b)
{
  int i;
  if (sizeof(unsigned int) != sizeof(unsigned int)) {
    //int bin = (1<<(sizeof(hist_type)*8))-1;
    int bin = -1;
    unsigned int ma = a[0];
    for (i = 1; i < 256; ++i) {
      if (a[i] > ma) {
        ma = a[i];
      }
    }
    for (i = 0; i < 256; ++i) {
      b[i] = (a[i] * bin + ma - 1) / ma;
    }
  }
  else {
    for (i = 0; i < 256; ++i) {
      b[i] = a[i];
    }
  }
  return 0;
}
static int HUFF_Encode0(HUFF* huff, const void* pSrc0, int nSrcLen, void* pDes0, int maxOutLen)
{
  int pDesLen, off;
  const unsigned char* pSrc = (const unsigned char*) pSrc0;
  unsigned char* pDes = (unsigned char*) pDes0;
  const unsigned char* pMax = pSrc + nSrcLen;
  const unsigned char* ptmp = pSrc;
  if (nSrcLen <= 3) {
    return 0;
  }
  //计算频度
  //memset(huff->count_u32, 0, sizeof(huff->count_u32[0])*END_OF_TABLE);
  memset(huff->count, 0, sizeof(huff->count[0])*END_OF_TABLE);
  while (ptmp < pMax) {
    huff->count[ *ptmp++ ]++;
  }
  //comphist(huff->count_u32, huff->count);
  //计算霍夫曼树
  Make_HuffTree(huff->huffTable, huff->count);
  //转换霍夫曼树为编码表
  Covert_HuffTree_toCode(huff->huffTable, huff->codeTable);
  //存储数据
  //off = sizeof(nSrcLen)+sizeof(root)+sizeof(huff->huffTable[0])*(root+1);
  off = 0;
  memcpy(pDes + off, &nSrcLen, sizeof(nSrcLen));
  off += sizeof(nSrcLen);
  memcpy(pDes + off, huff->count, sizeof(huff->count[0])*END_OF_TABLE);
  off += sizeof(huff->count[0]) * END_OF_TABLE;
  //编码数据
  pDesLen = Encode_Data(pSrc, nSrcLen, pDes + off, huff->codeTable);
  pDesLen += off;
  return pDesLen;
}
static int HUFF_Decode0(HUFF* huff, const void* pSrc0, int nSrcLen, void* pDes0, int maxOutLen)
{
  //准备压缩参数
  int nDesLen = 0, root = 0, off = 0;
  const unsigned char* pSrc = (const unsigned char*) pSrc0;
  unsigned char* pDes = (unsigned char*) pDes0;
  if (nSrcLen <= 3) {
    return 0;
  }
  //得到字节数
  memcpy(&nDesLen, pSrc + off, sizeof(nDesLen));
  if (NULL == pDes0) {
    return nDesLen;
  }
  if (nDesLen > maxOutLen) {
    return 0;
  }
  off += sizeof(nDesLen);
  if (nSrcLen < (int)(sizeof(nDesLen) + sizeof(huff->count[0])*END_OF_TABLE)) {
    return 0;
  }
  //得到根节点号
  memcpy(huff->count, pSrc + off, sizeof(huff->count[0])*END_OF_TABLE);
  off += sizeof(huff->count[0]) * END_OF_TABLE;
  //得到霍夫曼表
  //memcpy(huff->huffTable,pSrc+off,sizeof(huff->huffTable[0])*(root+1)); off += sizeof(huff->huffTable[0])*(root+1);
  root = Make_HuffTree(huff->huffTable, huff->count);
  //解压数据
  Decode_Data(pSrc + off, nSrcLen - off, pDes, nDesLen, huff->huffTable, root);
  return nDesLen;
}
static int HUFF_Encode(const void* pSrc0, int nSrcLen, void* pDes0, int maxOutLen)
{
  HUFF huff;
  memset(&huff, 0, sizeof(huff));
  return HUFF_Encode0(&huff, pSrc0, nSrcLen, pDes0, maxOutLen);
}
static int HUFF_Decode(const void* pSrc0, int nSrcLen, void* pDes0, int maxOutLen)
{
  HUFF huff;
  memset(&huff, 0, sizeof(huff));
  return HUFF_Decode0(&huff, pSrc0, nSrcLen, pDes0, maxOutLen);
}
#undef MAX_TABLELEN
#undef END_OF_TABLE
#ifdef TEST1
#include "str.h"
#include "cfile.h"
//#include "img/imgio.inl"
static int test_huff()
{
  vstr_t sv[1] = {0};
  buf_t bf[1] = {0};
  buf_t bf2[1] = {0};
  img_t im[1] = {0};
  int i, w = 720, h = 576;
  char buf[256];
#ifdef _DEBUG
  _chdir("E:/pub/bin/codec/huff/algo_20150917");
#endif
  vstr_load("list.txt", sv);
  for (i = 0; i < sv->n; ++i) {
    buf_load(sv->v[i].s, bf);
    bfsetsize(bf2, *(int*)bf->data);
    HUFF_Decode(bf->data, bf->len, bf2->data, bf2->len);
    IMINIT(im, h, w, bf2->data, w, 1, 1);
    //imtrans2(im, im, 1, 1);
    //bffree(bf2);
    //imshow(im);cvWaitKey(-1);
    _snprintf(buf, 256, "%s.bmp", sv->v[i].s);
    printf("%s\n", buf);
    //imwrite(buf, im);
  }
  bffree(bf);
  bffree(bf2);
  return 0;
}
static int test_huff1()
{
  enum {len = 10000};
  uchar buf[len] = {0};
  uchar out[2 * len] = {0};
  uchar out2[2 * len] = {0};
  int i, n, n2;
  if (0) {
    for (i = 0; i < len; ++i) {
      buf[i] = rand() % 10;
    }
  }
  else {
    for (i = 0; i < len / 2; ++i) {
      buf[i] = i % 10;
    }
  }
  n = HUFF_Encode(buf, len, out, len * 2);
  n2 = HUFF_Decode(out, n, out2, 2 * len);
  for (i = 0; i < len; ++i) {
    ASSERT(buf[i] == out2[i]);
  }
  return 0;
}
#endif
#endif // _HUFF_INL_

