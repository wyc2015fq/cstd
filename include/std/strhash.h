
#include "inttypes_c.h"

// 字符串Hash函数对比
// http://www.byvoid.com/blog/string-hash-compare/
//我对这些hash的散列质量及效率作了一个简单测试，测试结果如下：
//测试1：对100000个由大小写字母与数字随机的ANSI字符串（无重复，每个字符串最大长度不超过64字符）进行散列：
//字符串函数    冲突数  除1000003取余后的冲突数
//
// BKDRHash     0       4826
// SDBMHash     2       4814
// RSHash       2       4886
// APHash       0       4846
// ELFHash      1515    6120
// JSHash       779     5587
// DEKHash      863     5643
// FNVHash      2       4872
// DJBHash      832     5645
// DJB2Hash     695     5309
// PJWHash      1515    6120
//
// 测试2：对100000个由任意UNICODE组成随机字符串（无重复，每个字符串最大长度不超过64字符）进行散列：
// 字符串函数   冲突数  除1000003取余后的冲突数
// BKDRHash     3       4710
// SDBMHash     3       4904
// RSHash       3       4822
// APHash       2       4891
// ELFHash      16      4869
// JSHash       3       4812
// DEKHash      1       4755
// FNVHash      1       4803
// DJBHash      1       4749
// DJB2Hash     2       4817
// PJWHash      16      4869
//
// 测试3：对1000000个随机ANSI字符串（无重复，每个字符串最大长度不超过64字符）进行散列：
// 字符串函数   耗时（毫秒）
// BKDRHash     109
// SDBMHash     109
// RSHash       124
// APHash       187
// ELFHash      249
// JSHash       172
// DEKHash      140
// FNVHash      125
// DJBHash      125
// DJB2Hash     125
// PJWHash      234
//
// 结论：也许是我的样本存在一些特殊性，在对ASCII码字符串进行散列时，PJW与ELF Hash（它们其实是同一种算法）无论是质量还是效率，都相当糟糕；例如："b5"与“aE"，这两个字符串按照PJW散列出来的hash值就是一样的。另外，其它几种依靠异或来散列的哈希函数，如：JS/DEK/DJB Hash，在对字母与数字组成的字符串的散列效果也不怎么好。相对而言，还是BKDR与SDBM这类简单的Hash效率与效果更好。
//作者：icefireelf
//出处：http://blog.csdn.net/icefireelf/article/details/5796529

//各种字符串Hash函数比较
//常用的字符串Hash函数还有ELFHash，APHash等等，都是十分简单有效的方法。这些函数使用位运算使得每一个字符都对最后的函数值产生 影响。另外还有以MD5和SHA1为代表的杂凑函数，这些函数几乎不可能找到碰撞。
//常用字符串哈希函数有 BKDRHash，APHash，DJBHash，JSHash，RSHash，SDBMHash，PJWHash，ELFHash等等。对于以上几种哈 希函数，我对其进行了一个小小的评测。
// Hash函数     数据1   数据2   数据3   数据4   数据1得分       数据2得分       数据3得分       数据4得分       平均分
// BKDRHash     2       0       4774    481     96.55   100     90.95   82.05   92.64
// APHash       2       3       4754    493     96.55   88.46   100     51.28   86.28
// DJBHash      2       2       4975    474     96.55   92.31   0       100     83.43
// JSHash       1       4       4761    506     100     84.62   96.83   17.95   81.94
// RSHash       1       0       4861    505     100     100     51.58   20.51   75.96
// SDBMHash     3       2       4849    504     93.1    92.31   57.01   23.08   72.41
// PJWHash      30      26      4878    513     0       0       43.89   0       21.95
// ELFHash      30      26      4878    513     0       0       43.89   0       21.95
//
// 其中数据1为100000个字母和数字组成的随机串哈希冲突个数。数据2为100000个有意义的英文句子哈希冲突个数。数据3为数据1的哈希值与 1000003(大素数)求模后存储到线性表中冲突的个数。数据4为数据1的哈希值与10000019(更大素数)求模后存储到线性表中冲突的个数。
//
// 经过比较，得出以上平均得分。平均数为平方平均数。可以发现，BKDRHash无论是在实际效果还是编码实现中，效果都是最突出的。APHash也 是较为优秀的算法。DJBHash,JSHash,RSHash与SDBMHash各有千秋。PJWHash与ELFHash效果最差，但得分相似，其算 法本质是相似的。

/// @brief BKDR Hash Function
/// @detail 本算法由于在Brian Kernighan与Dennis Ritchie的《The C Programming Language》一书被展示而得名，是一种简单快捷的hash算法，也是Java目前采用的字符串的Hash算法（累乘因子为31）。
// 也可以乘以31、131、1313、13131、131313..
// 有人说将乘法分解为位运算及加减法可以提高效率，如将上式表达为：hash = hash << 7 + hash << 1 + hash + ch;
// 但其实在Intel平台上，CPU内部对二者的处理效率都是差不多的，
// 我分别进行了100亿次的上述两种运算，发现二者时间差距基本为0（如果是Debug版，分解成位运算后的耗时还要高1/3）；
// 在ARM这类RISC系统上没有测试过，由于ARM内部使用Booth's Algorithm来模拟32位整数乘法运算，它的效率与乘数有关：
// 当乘数8-31位都为1或0时，需要1个时钟周期
// 当乘数16-31位都为1或0时，需要2个时钟周期
// 当乘数24-31位都为1或0时，需要3个时钟周期
// 否则，需要4个时钟周期
// 因此，虽然我没有实际测试，但是我依然认为二者效率上差别不大
static size_t BKDRHash(const char* str, size_t len)
{
  size_t hash = 0;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    /* 31 131 1313 13131 131313 etc.. */
    hash = (hash * 131) + (*str);
  }
  return hash;
}
/// @brief SDBM Hash Function
/// @detail 本算法是由于在开源项目SDBM（一种简单的数据库引擎）中被应用而得名，它与BKDRHash思想一致，只是种子不同而已。
static size_t SDBMHash(const char* str, size_t len)
{
  size_t hash = 0;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = 65599 * hash + (*str);
    //hash = (*str) + (hash << 6) + (hash << 16) - hash;
  }
  return hash;
}
/// @brief RS Hash Function
/// @detail 因Robert Sedgwicks在其《Algorithms in C》一书中展示而得名。
static size_t RSHash(const char* str, size_t len)
{
  size_t a = 63689;
  size_t hash = 0;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = hash * a + (*str);
    a = a * 378551;
  }
  return hash;
}
/// @brief AP Hash Function
/// @detail 由Arash Partow发明的一种hash算法。
/* End Of FNV Hash Function */
static size_t APHash(const char* str, size_t len)
{
  size_t hash = 0xAAAAAAAA;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) : ((~((hash << 11)) + ((*str) ^ (hash >> 5))));
  }
  return hash;
}
/// @brief JS Hash Function
/// 由Justin Sobel发明的一种hash算法。
static size_t JSHash(const char* str, size_t len)
{
  size_t hash = 1315423911;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash ^= ((hash << 5) + (*str) + (hash >> 2));
  }
  return hash;
}
/// @brief DEK Function
/// @detail 本算法是由于Donald E. Knuth在《Art Of Computer Programming Volume 3》中展示而得名。
static size_t DEKHash(const char* str, size_t len)
{
  size_t hash = 1315423911; // len
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
  }
  return hash;
}
/// @brief FNV Hash Function
/// @detail Unix system系统中使用的一种著名hash算法，后来微软也在其hash_map中实现。
static size_t FNVHash(const char* str, size_t len)
{
  size_t hash = 0x811c9dc5;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash *= 16777619;
    hash ^= (*str);
  }
  return hash;
}
/* End Of BP Hash Function */
static size_t FNVHash1(const char* str, size_t len)
{
  const size_t fnv_prime = 0x811C9DC5;
  size_t hash = 0;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash *= fnv_prime;
    hash ^= (*str);
  }
  return hash;
}
/// @brief DJB Hash Function
/// @detail 由Daniel J. Bernstein教授发明的一种hash算法。
/* End Of SDBM Hash Function */
static size_t DJBHash(const char* str, size_t len)
{
  size_t hash = 5381;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = ((hash << 5) + hash) + (*str);
  }
  return hash;
}
/// @brief DJB Hash Function 2
/// @detail 由Daniel J. Bernstein 发明的另一种hash算法。
static size_t DJB2Hash(const char* str, size_t len)
{
  register size_t hash = 5381;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = hash * 33 ^ (*str);
  }
  return hash;
}
/// @brief PJW Hash Function
/// @detail 本算法是基于AT&T贝尔实验室的Peter J. Weinberger的论文而发明的一种hash算法。
/* End Of JS Hash Function */
static size_t PJWHash(const char* str, size_t len)
{
  const size_t BitsInUnsignedInt = (size_t)(sizeof(size_t) * 8);
  const size_t ThreeQuarters = (size_t)((BitsInUnsignedInt * 3) / 4);
  const size_t OneEighth = (size_t)(BitsInUnsignedInt / 8);
  const size_t HighBits = (size_t)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
  size_t hash = 0;
  size_t test = 0;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = (hash << OneEighth) + (*str);
    if ((test = hash & HighBits) != 0) {
      hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }
  }
  return hash;
}
/// @brief ELF Hash Function
/// @detail 由于在Unix的Extended Library Function被附带而得名的一种hash算法，它其实就是PJW Hash的变形。
static size_t ELFHash(const char* str, size_t len)
{
  enum {
    TotalBits = sizeof(size_t) * 8,
    ThreeQuarters = (TotalBits * 3) / 4,
    OneEighth = TotalBits / 8,
    HighBits = ((size_t) - 1) << (TotalBits - OneEighth)
  };
  register size_t hash = 0;
  size_t magic = 0;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = (hash << OneEighth) + (*str);
    if ((magic = hash & HighBits) != 0) {
      hash ^= (magic >> ThreeQuarters);
      hash &= ~magic;
    }
  }
  return hash;
}
/* End Of P. J. Weinberger Hash Function */
static size_t ELFHash2(const char* str, size_t len)
{
  size_t hash = 0;
  size_t x = 0;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = (hash << 4) + (*str);
    if ((x = hash & 0xF0000000L) != 0) {
      hash ^= (x >> 24);
    }
    hash &= ~x;
  }
  return hash;
}
/* End Of DEK Hash Function */
static size_t BPHash(const char* str, size_t len)
{
  size_t hash = 0;
  size_t i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = hash << 7 ^ (*str);
  }
  return hash;
}
/* End Of AP Hash Function */
//几种经典的Hash算法的实现(源代码)
//●PHP中出现的字符串Hash函数
static size_t hashpjw(char* arKey, size_t nKeyLength)
{
  size_t h = 0, g;
  char* arEnd = arKey + nKeyLength;
  while (arKey < arEnd) {
    h = (h << 4) + *arKey++;
    if ((g = (h & 0xF0000000))) {
      h = h ^ (g >> 24);
      h = h ^ g;
    }
  }
  return h;
}
//●OpenSSL中出现的字符串Hash函数
static size_t lh_strhash11(const char* str)
{
  int i, l;
  size_t ret = 0;
  unsigned short* s;
  if (str == NULL) {
    return(0);
  }
  l = ((int)strlen(str) + 1) / 2;
  s = (unsigned short*)str;
  for (i = 0; i < l; ++i) {
    ret ^= (s[i] << (i & 0x0f));
  }
  return(ret);
}
/* The following hash seems to work very well on normal text strings
* no collisions on /usr/dict/words and it distributes on %2^n quite
* well, not as good as MD5, but still good.
*/
static size_t lh_strhash1(const char* c)
{
  size_t ret = 0;
  long n;
  size_t v;
  int r;
  if ((c == NULL) || (*c == '\0')) {
    return(ret);
  }
  /*
  unsigned char b[16];
  MD5(c,strlen(c),b);
  return(b[0]|(b[1]<<8)|(b[2]<<16)|(b[3]<<24));
  */
  n = 0x100;
  while (*c) {
    v = n | (*c);
    n += 0x100;
    r = (int)((v >> 2) ^ v) & 0x0f;
    ret = (ret * (32 - r));
    ret &= 0xFFFFFFFFL;
    ret ^= v * v;
    c++;
  }
  return((ret >> 16) ^ ret);
}
//●MySql中出现的字符串Hash函数
#ifndef NEW_HASH_FUNCTION
/* Calc hashvalue for a key */
static size_t calc_hashnr(const char* key, size_t length)
{
  register size_t nr = 1, nr2 = 4;
  while (length--) {
    nr ^= (((nr & 63) + nr2) * ((size_t)(uint8_t) * key++)) + (nr << 8);
    nr2 += 3;
  }
  return((size_t)nr);
}
/* Calc hashvalue for a key, case indepenently */
static size_t calc_hashnr_caseup(const char* key, size_t length)
{
  register size_t nr = 1, nr2 = 4;
  while (length--) {
    nr ^= (((nr & 63) + nr2) * ((size_t)(uint8_t)toupper(*key++))) + (nr << 8);
    nr2 += 3;
  }
  return((size_t)nr);
}
#else
/*
* Fowler/Noll/Vo hash
*
* The basis of the hash algorithm was taken from an idea sent by email to the
* IEEE Posix P1003.2 mailing list from Phong Vo (kpv@research.att.com) and
* Glenn Fowler (gsf@research.att.com). Landon Curt Noll (chongo@toad.com)
* later improved on their algorithm.
*
* The magic is in the interesting relationship between the special prime
* 16777619 (2^24 + 403) and 2^32 and 2^8.
*
* This hash produces the fewest collisions of any function that we've seen so
* far, and works well on both numbers and strings.
*/
static size_t calc_hashnr(const byte* key, size_t len)
{
  const byte* end = key + len;
  size_t hash;
  for (hash = 0; key < end; key++) {
    hash *= 16777619;
    hash ^= (size_t) * (uint8_t*)key;
  }
  return (hash);
}
static size_t calc_hashnr_caseup(const byte* key, size_t len)
{
  const byte* end = key + len;
  size_t hash;
  for (hash = 0; key < end; key++) {
    hash *= 16777619;
    hash ^= (size_t)(uint8_t)toupper(*key);
  }
  return (hash);
}
#endif
//Mysql中对字符串Hash函数还区分了大小写
//●另一个经典字符串Hash函数
static size_t MysqlHash(const char* str, size_t len)
{
  register size_t h = 0;
  register const unsigned char* p = (const unsigned char*)str;
  size_t i = 0;
  for (; i < len; ++i, ++p) {
    h = 31 * h + *p;
  }
  return h;
}
//可以考虑如下几个哈希函数，
//MurmurHash，CityHash，FarmHash，XXHash，SpookyHash，lookup3，SimHash，SuperFastHash，FNV，CRC32。
//上述哈希函数的哈希值一般有32bit、64bit和128bit。另外也可以用比如MD5和SHA1等加密哈希函数，
//当然加密哈希函数相对于非加密哈希函数速度要慢些，但是它们的哈希值要更长，发生冲突的概率会更小。
//一般认为SHA不会有冲突，因为它发生冲突的概率远远小于硬件发生错误的概率。
//另外，我做过一个实验，哈希函数用java实现，输入是一千三百多万条字符串，哈希值是int型，整数，范围是0~0x7fffffff。
//除了SuperFastHash，其他的哈希函数的冲突率是0.3%左右，如果我对某个函数稍加修改，
//比如把murmurhash中的乘法操作换为异或操作，或者把XXHash中某一行代码去掉，冲突率一般会变大不少，比较神奇。

// MurMurHash 算法，是非加密HASH算法，性能很高，
// 比传统的CRC32,MD5，SHA-1（这两个算法都是加密HASH算法，复杂度本身就很高，带来的性能上的损害也不可避免）
// 等HASH算法要快很多，而且据说这个算法的碰撞率很低.
// http://murmurhash.googlepages.com/
static uint64_t MurMurHash(const char* key, int len)
{
  uint8_t* buf = (uint8_t*)key;
  size_t seed = 0x1234ABCD;
  uint64_t m = 0xc6a4a7935bd1e995L;
  int r = 47, remaining = len;
  uint64_t h = seed ^ (len * m);
  uint64_t k;
  while (remaining >= 8) {
    k = *(uint64_t*)buf;
    k *= m;
    k ^= k >> r;
    k *= m;
    h ^= k;
    h *= m;
    remaining -= sizeof(k);
    buf += sizeof(k);
  }
  if (remaining > 0) {
    uint64_t finish = 0;
    memcpy(&finish, buf, remaining);
    // for big-endian version, do this first:
    // finish.position(8-buf.remaining());
    h ^= finish;
    h *= m;
  }
  h ^= h >> r;
  h *= m;
  h ^= h >> r;
  return h;
}
static size_t murMurHash(const void* key, int len)
{
  const size_t m = 0x5bd1e995;
  const int r = 24;
  const int seed = 97;
  size_t h = seed ^ len;
  // Mix 4 bytes at a time into the hash
  const unsigned char* data = (const unsigned char*)key;
  while (len >= 4) {
    size_t k = *(size_t*)data;
    k *= m;
    k ^= k >> r;
    k *= m;
    h *= m;
    h ^= k;
    data += 4;
    len -= 4;
  }
  // Handle the last few bytes of the input array
  switch (len) {
  case 3:
    h ^= data[2] << 16;
  case 2:
    h ^= data[1] << 8;
  case 1:
    h ^= data[0];
    h *= m;
  };
  // Do a few final mixes of the hash to ensure the last few
  // bytes are well-incorporated.
  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;
  return h;
}