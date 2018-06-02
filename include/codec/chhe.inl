#include "huff.inl"
#include "md5.inl"
#include "CPMDev/sock/openssl_encrypt.inl"
// 加密算法可以公开
static int encrypt_xor(void* data, int len, int key)
{
  unsigned char* udata = (unsigned char*)data;
  int i;
  for (i = 0; i < len; ++i) {
    udata[i] = udata[i] ^ key;
  }
  return len;
}
#ifndef CV_SWAP
#define CV_SWAP(_A,_B,_T) ((_T) = (_A), (_A) = (_B), (_B) = (_T))
#endif
// 线性同余伪随机数生成器
// 返回一个无符号32位整型的伪随机数
static unsigned int my_rand(unsigned int* pseed)
{
  unsigned int r;
#if 1
  enum {a = 1103515245, c = 12345};
#else
  enum {a = 1664525, c = 1013904223};
  enum {a = 22695477, c = 1};
  enum {a = 134775813, c = 1};
  enum {a = 214013, c = 2531011};
#endif
  r = *pseed = *pseed * a + c;
  return (r << 16) | ((r >> 16) & 0xFFFF);
}
static int str_map(char* str, int len, unsigned int key, int isinv)
{
  int i, j;
  enum {nn = 95, mn = 32};
  BYTE list[256] = {0};
  BYTE map[256] = {0};
  unsigned int seed = key;
  for (i = 0; i < nn; ++i) {
    list[i] = i;
  }
  for (i = 0; i < nn; ++i) {
    unsigned int r = my_rand(&seed) % nn, t;
    CV_SWAP(list[i], list[r], t);
  }
  for (i = 0; i < nn; ++i) {
    j = list[i];
    if (isinv) {
      map[mn + j] = mn + i;
    }
    else {
      map[mn + i] = mn + j;
    }
  }
  for (i = 0; i < len; ++i) {
    str[i] = map[(uchar)str[i]];
  }
  return 0;
}
#ifdef TEST
#include "cstd.h"
#include "cmath.h"
#include "rand.inl"
static int test_str_map()
{
  char str[100] = "Stone*Lock?65535*Year";
  char str2[100] = {0};
  int len = strlen(str), key = 123;
  char strm[100] = {0};
  int i, n = 50;
  uint seed=123;
  strcpy(str2, str);
  str_map(str, len, key, 0);
  str_map(str, len, key, 1);
  ASSERT(0 == strcmp(str2, str));
  for (i = 0; i < 95; ++i) {
    strm[i] = i + 32;
  }
  len = 50;
  for (i = 0; i < 100000; ++i) {
    randstr(&seed, str, n, strm);
    strcpy(str2, str);
    str_map(str, len, key, 0);
    str_map(str, len, key, 1);
    ASSERT(0 == strcmp(str2, str));
  }
  return 0;
}
#endif
#define HEXSTRMAP "0123456789ABCDEF"
static int data_to_hexstr(const void* data, int datalen, char* hexstr)
{
  int i;
  for (i = 0; i < datalen; ++i) {
    uchar uc = ((uchar*)data)[i];
    hexstr[i * 2 + 0] = HEXSTRMAP[uc & 0x0f];
    hexstr[i * 2 + 1] = HEXSTRMAP[uc >> 4];
  }
  hexstr[datalen * 2] = 0;
  return datalen * 2;
}
static BYTE g_SZHEX[256] = {
  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
  0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 0 , 0 , 0 , 0 , 0 , 0 ,
  0 , 10, 11, 12, 13, 14, 15, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
  0 , 10, 11, 12, 13, 14, 15, 0
}; //sizeof(SZHEX)=0x68
#undef HEXChar
#define HEXChar(c) (g_SZHEX[c])
static int hexstr_to_data(const char* hexstr, void* data, int datalen)
{
  int i;
  for (i = 0; i < datalen; ++i) {
    ((uchar*)data)[i] = HEXChar(hexstr[2 * i + 0]) | (HEXChar(hexstr[2 * i + 1]) << 4);
  }
  return datalen;
}
static int test_hexstr()
{
  uchar data[100] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  uchar data2[100] = {0};
  char str[100] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  data_to_hexstr(data, 10, str);
  hexstr_to_data(str, data2, 10);
  int ret = memcmp(data, data2, 10);
  return 0;
}
#define CHHE_HEAD_MAGIC_LEN 8
#define CHHE_HEAD_MD5_LEN 16
#define CHHE_HEAD_MAGIC *(int*)"CHHE"
#define CHHE_HEAD_LEN (CHHE_HEAD_MAGIC_LEN+CHHE_HEAD_MD5_LEN)
int my_md5(const void* data, int len, int key, void* outmd5)
{
  md5_state_t s;
  unsigned char* result = (unsigned char*)outmd5;
  md5_init(&s);
  md5_append(&s, (const unsigned char*)data, len);
  md5_finish(&s, (unsigned char*)result);
  encrypt_xor(result, CHHE_HEAD_MD5_LEN, key);
  return 0;
}
// CHHE加密流程
// 判断是否以 "CHHE" 开头，是表示已经加过密返回-1
// 哈弗曼压缩(自己实现的)[头四字节存原数据长度。然后是 4*256 字节的频度表 然后是数据][len][4*256 hist][data]
// 求md5 再逆序 存入 第4 到第20字节
// 对哈弗曼压缩之后的数据 做对称加密,调用openssl 中的 AES_cbc_encrypt 函数
// 返回加密后数据长度
static int chhe_encode(const void* key, int klen, const void* src, int len, void* dst, int maxlen)
{
  int dlen = 0;
  if (len > CHHE_HEAD_LEN && CHHE_HEAD_MAGIC == *(const int*)src) {
    return -1; // 已经加过密了
  }
  if (NULL == dst) {
    return len * 2;
  }
  if (maxlen < CHHE_HEAD_LEN) {
    return -2; // 空间不够
  }
  *(int*)dst = CHHE_HEAD_MAGIC;
  ((int*)dst)[1] = dlen = HUFF_Encode(src, len, (char*)dst + CHHE_HEAD_LEN, maxlen - CHHE_HEAD_LEN);
  if (1) {
    my_md5((const unsigned char*)((char*)dst + CHHE_HEAD_LEN), dlen, klen, (unsigned char*)dst + CHHE_HEAD_MAGIC_LEN);
  }
#ifdef _OPENSSL_ENCRYPT_INL_
  aes_encrypt(key, klen, (char*)dst + CHHE_HEAD_LEN, dlen, (char*)dst + CHHE_HEAD_LEN, dlen);
#endif // _OPENSSL_ENCRYPT_INL_
  return dlen + CHHE_HEAD_LEN;
}
// 返回解密后数据长度
static int chhe_decode(const void* key, int klen, const void* src, int len, void* dst, int maxlen)
{
  int dlen = 0, ret = 0;
  char* src1 = NULL;
  if (!(len > CHHE_HEAD_LEN && CHHE_HEAD_MAGIC == *(const int*)src)) {
    return -1; // 没加过密
  }
#ifdef _OPENSSL_ENCRYPT_INL_
  src1 = (char*)malloc(len);
  memcpy(src1, src, len);
  src = src1;
  aes_decrypt(key, klen, (char*)src1 + CHHE_HEAD_LEN, len - CHHE_HEAD_LEN, (char*)src1 + CHHE_HEAD_LEN, len - CHHE_HEAD_LEN);
#endif // _OPENSSL_ENCRYPT_INL_
  if (1) {
    unsigned char result[CHHE_HEAD_MD5_LEN + 4] = {0};
    int dlen2 = ((int*)src)[1];
    const unsigned char* result1 = (const unsigned char*)src + CHHE_HEAD_MAGIC_LEN;
    dlen2 = MIN(dlen2, len - CHHE_HEAD_MAGIC_LEN);
    my_md5((const unsigned char*)((char*)src + CHHE_HEAD_LEN), dlen2, klen, result);
    if (0 != memcmp(result, result1, CHHE_HEAD_MD5_LEN)) {
      ret = -3; // md5 校验错误
    }
  }
  if (ret >= 0) {
    ret = dlen = HUFF_Decode((const char*)src + CHHE_HEAD_LEN, len - CHHE_HEAD_LEN, dst, maxlen);
  }
  if (src1) {
    free(src1);
  }
  return ret;
}
#ifdef TEST
#include "cmath.h"
int test_rand() {
  int arr[100] = {0};
  float arrf[100] = {0};
  double arrd[100] = {0};
  double arr2d[100] = {0};
  double pa1[] = {0, 50, 0, 0};
  double pa2[] = {50, 100, 1, 1};
  uint64 seed = 123;
  int cn=2;
  int len = 100/cn;
  RNG_fill_unifrom<int, 0, 128>(len, arr, cn, pa1, pa2, cn, &seed, TRUE);
  RNG_fill_unifrom_float(len, arrf, cn, pa1, pa2, cn, &seed);
  RNG_fill_unifrom_double(len, arrd, cn, pa1, pa2, cn, &seed);
  RNG_fill_normal(len, arr2d, cn, pa1, pa2, cn, &seed, 0);
  return 0;
}
// 加密算法
int test_chhe(int n, char** args)
{
  test_rand();
  buf_t bf[3] = {0};
  buf_t* bf1 = bf + 1;
  buf_t* bf2 = bf + 2;
  int len1 = 0;
  int len2 = 0;
  const char* path = NULL;
  const char* fn = "";
  char buf[256];
  char* aaa_key = "Stone*Lock?65535*Year";
  int klen = 0;
  uint seed=123;
  //klen =strlen(aaa_key);
#ifdef _DEBUG
  path = "E:/pub/bin/codec";
  _chdir(path);
  aaa_key = "12345";
  fn = "338 new.tar.lzma";
  fn = "338.tar.lzma";
  fn = "W0104.rtf";
  fn = "W0104.rtf.chhe";
  fn = "T339telnetF.bin";
#endif
  if (1) {
    if (n > 2) {
      aaa_key = args[2];
    }
    if (n > 1) {
      fn = args[1];
    }
    else {
      if (!file_exist(fn)) {
        printf("%s file not exist!\n", fn);
        printf("usage: %s file [pass]\n", args[0]);
        return 0;
      }
    }
    buf_load(fn, bf);
  }
  else {
    bfsetsize(bf, 10000000);
    randstr(&seed, (char*)bf->data, bf->len, "abcdefghijklmnopqrstuvwxyz 0123456789");
  }
  klen = strlen(aaa_key);
  bfsetsize(bf1, bf->len * 2);
  bfsetsize(bf2, bf->len * 2);
  len1 = chhe_encode(aaa_key, klen, bf->data, bf->len, NULL, bf1->len);
  if (len1 > 0) {
    len1 = chhe_encode(aaa_key, klen, bf->data, bf->len, bf1->data, bf1->len);
    //int len1a = chhe_encode(bf1->data, bf1->len, bf2->data, bf2->len);
    //out[10001] = 0;
    _snprintf(buf, 256, "%s.chhe", fn);
    savefile(buf, bf1->data, len1);
    printf("encode OK!\n");
  }
  else {
    if (-1 == len1) {
      len1 = bf->len;
      memcpy(bf1->data, bf->data, bf->len);
      printf("encode fail! 已经加过密了\n");
    }
    else {
      printf("encode fail!\n");
    }
  }
  len2 = chhe_decode(aaa_key, klen, bf1->data, len1, NULL, 0);
  if (len2 > 0) {
    len2 = chhe_decode(aaa_key, klen, bf1->data, len1, bf2->data, bf2->len);
    //if (!(len2==bf->len && 0==memcmp(bf->data, bf2->data, bf->len)))
    {
      _snprintf(buf, 256, "%s.out", fn);
      savefile(buf, bf2->data, len2);
      printf("decode OK!\n");
    }
  }
  else {
    if (-1 == len2) {
      printf("decode fail! 没加过密\n");
    }
    else if (-3 == len2) {
      printf("decode fail! 校验失败\n");
    }
    else {
      printf("decode fail!\n");
    }
  }
  //ASSERT(len2==bf->len && 0==memcmp(bf->data, bf2->data, bf->len));
  bffrees(bf);
  return 0;
}
#endif // TEST

