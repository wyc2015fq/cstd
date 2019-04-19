# RC4加密 - 三少GG - CSDN博客
2014年01月16日 11:01:32[三少GG](https://me.csdn.net/scut1135)阅读数：1033
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)

1.
### [RC4加密算法在C++中的实现](http://blog.csdn.net/chenxiaohua/article/details/2095443)
在一些场合，常需要用到一些简单的加密算法，这里的RC4就可以说是最简单的一种。只要设置一个足够强的密码，就可以适用于一些非常简单的场合了。我是用来加密HTTP传送的数据的。
RC4函数（加密/解密）    其实，RC4只有加密，将密文再加密一次，就是解密了。
GetKey函数   随机字符串产生器，呵呵，为了方便，大多数加密算法都有一个随机密码产生器，我也就附带一个了。
ByteToHex函数   把字节码转为十六进制码，一个字节两个十六进制。研究发现，十六进制字符串非常适合在HTTP中传输，Base64中的某些字符会造成转义，挺麻烦的。
HexToByte函数   把十六进制字符串，转为字节码。服务器也按照十六进制字符串的形式把数据传回来，这里就可以解码啦。同时，使用十六进制字符串传输，避开了传输过程中多国语言的问题。
Encrypt函数    把字符串经RC4加密后，再把密文转为十六进制字符串返回，可直接用于传输。
Decrypt函数    直接密码十六进制字符串密文，再解密，返回字符串明文。
源码如下：
Encrypt.h文件：
 #ifndef _ENCRYPT_RC4_
#define _ENCRYPT_RC4_
#include <string.h>
#define BOX_LEN 256
int GetKey(const unsigned char* pass, int pass_len, unsigned char *out);
int RC4(const unsigned char* data, int data_len, const unsigned char* key, int key_len, unsigned char* out, int* out_len); 
static void swap_byte(unsigned char* a, unsigned char* b);
char* Encrypt(const char* szSource, const char* szPassWord); // 加密，返回加密结果
char* Decrypt(const char* szSource, const char* szPassWord); // 解密，返回解密结果
char* ByteToHex(const unsigned char* vByte, const int vLen); // 把字节码pbBuffer转为十六进制字符串，方便传输
unsigned char* HexToByte(const char* szHex); // 把十六进制字符串转为字节码pbBuffer，解码
#endif // #ifndef _ENCRYPT_RC4_
Encrypt.cpp文件：
#include "Encrypt.h"
char* Encrypt(const char* szSource, const char* szPassWord) // 加密，返回加密结果
{
 if(szSource == NULL || szPassWord == NULL) return NULL;
 unsigned char* ret = new unsigned char[strlen(szSource)];
 int ret_len = 0;
 if(RC4((unsigned char*)szSource, 
   strlen(szSource), 
   (unsigned char*)szPassWord, 
   strlen(szPassWord), 
   ret, 
   &ret_len) == NULL) 
  return NULL;
 char* ret2 = ByteToHex(ret, ret_len);
 delete[] ret;
 return ret2;
}
char* Decrypt(const char* szSource, const char* szPassWord) // 解密，返回解密结果
{
 if(szSource == NULL || (strlen(szSource)%2 != 0) || szPassWord == NULL) 
  return NULL;
 unsigned char* src = HexToByte(szSource);
 unsigned char* ret = new unsigned char[strlen(szSource) / 2 + 1];
 int ret_len = 0;
 memset(ret, strlen(szSource) / 2 + 1,0);
 if(RC4(src, strlen(szSource) / 2, (unsigned char*)szPassWord, strlen(szPassWord), ret, &ret_len) == NULL) 
  return NULL;
 ret[ret_len] = '/0';
 return (char*)ret;
}
int RC4(const unsigned char* data, int data_len, const unsigned char* key, int key_len, unsigned char* out, int* out_len)
{
 if (data == NULL || key == NULL || out == NULL) 
  return NULL;
 unsigned char* mBox = new unsigned char[BOX_LEN];
 if(GetKey(key, key_len, mBox) == NULL) 
  return NULL;
 int i=0;
 int x=0;
 int y=0;
 for(int k = 0; k < data_len; k++)
 {
  x = (x + 1) % BOX_LEN;
  y = (mBox[x] + y) % BOX_LEN;
  swap_byte(&mBox[x], &mBox[y]);
  out[k] = data[k] ^ mBox[(mBox[x] + mBox[y]) % BOX_LEN];
 }
 *out_len = data_len;
 delete[] mBox;
 return -1;
}
int GetKey(const unsigned char* pass, int pass_len, unsigned char* out)
{
 if(pass == NULL || out == NULL) 
  return NULL;
 int i;
 for(i = 0; i < BOX_LEN; i++)
  out[i] = i;
 int j = 0;
 for(i = 0; i < BOX_LEN; i++)
 {
  j = (pass[i % pass_len] + out[i] + j) % BOX_LEN;
  swap_byte(&out[i], &out[j]); 
 }
 return -1;
}
static void swap_byte(unsigned char* a, unsigned char* b)
{
 unsigned char swapByte; 
 swapByte = *a;
 *a = *b;
 *b = swapByte;
}
// 把字节码转为十六进制码，一个字节两个十六进制，内部为字符串分配空间
char* ByteToHex(const unsigned char* vByte, const int vLen)
{
 if(!vByte) 
  return NULL;
 char* tmp = new char[vLen * 2 + 1]; // 一个字节两个十六进制码，最后要多一个'/0'
 int tmp2;
 for (int i=0;i<vLen;i++)
 {
  tmp2 = (int)(vByte[i])/16;
  tmp[i*2] = (char)(tmp2+((tmp2>9)?'A'-10:'0'));
  tmp2 = (int)(vByte[i])%16;
  tmp[i*2+1] = (char)(tmp2+((tmp2>9)?'A'-10:'0'));
 }
 tmp[vLen * 2] = '/0';
 return tmp;
}
// 把十六进制字符串，转为字节码，每两个十六进制字符作为一个字节
unsigned char* HexToByte(const char* szHex)
{
 if(!szHex) 
  return NULL;
 int iLen = strlen(szHex);
 if (iLen<=0 || 0!=iLen%2) 
  return NULL;
 unsigned char* pbBuf = new unsigned char[iLen/2];  // 数据缓冲区
 int tmp1, tmp2;
 for (int i=0;i<iLen/2;i++)
 {
  tmp1 = (int)szHex[i*2] - (((int)szHex[i*2]>='A')?'A'-10:'0');
  if(tmp1>=16) 
   return NULL;
  tmp2 = (int)szHex[i*2+1] - (((int)szHex[i*2+1]>='A')?'A'-10:'0');
  if(tmp2>=16)
   return NULL;
  pbBuf[i] = (tmp1*16+tmp2);
 }
 return pbBuf;
}
main.cpp文件
#include <iostream>
#include <string>
#include <stdio.h>
#include "Encrypt.h"
int main(int argc,char *argv[]) 
{
 int size = 0;
 char source[] = "chenli";
 char pass[] = "123456";
 char *result1 = NULL;
 char *result2 = NULL;
 result1 = Encrypt(source, pass);
 printf("result1=%s/n", result1);
 result2 = Decrypt(result1, pass);
 printf("result2=%s/n", result2);
 delete []result1;
 delete []result2;
 return 0; 
}
2. ulib
https://code.google.com/p/ulib/
- **Core Items**- bfilter.{h,c}: the Bloom filter
- bitmap.{h,c}: generic bitmap
- crypt_aes.{h,c}: the AES crypt
- crypt_md5.{h,c}: the MD5 algorithm
- crypt_rc4.{h,c}: the RC4 crypt
- crypt_sha1.{h,c}: the SHA1 algorithm
- crypt_sha256.{h,c}: the SHA256 algorithm
- hash_open.h: C++ containers for the open addressing hashmap and hashset
- hash_open_prot.h: prototypes for the open addressing hashmap and hashset
- hash_chain.h: C++ container for the chain hashmap
- hash_chain_prot.h: prototype for the chain hashmap
- hash_func.{h,c}: hash functions
- heap_prot.h: generic heap prototype
- list.h: doubly linked list, can be used to implement queue and stack
- math_bit.h: bit operations
- math_bn.{h,c}: big number arithmetics
- math_comb.{h,c}: combinatorics enumerator
- math_factorial.{h,c}: factorial approximations
- math_gcd.{h,c}: Euclidean and the Extended Euclidean GCD algorithms
- math_lcm.{h,c}: the least common multiple
- math_rand_prot.h: pseudo-random number generators, mix functions, and etc
- math_rng_gamma.{h,c}: gamma distribution RNG
- math_rng_normal.{h,c}: normal distribution RNG
- math_rng_zipf.{h,c}: Zipf distribution RNG
- search_line.{h,c}: binary search for the text lines
- sort_heap_prot.h: prototype for the heapsort
- sort_list.{h,c}: list sort
- sort_median_prot.h: prototype for the median algorithm
- str_util.{h,c}: parallel/supplementary string utilities
- tree.{h,c}: various binary search trees
- tree_util.{h,c}: tree utilities
- ulib_ver.{h,c}: ulib version
- util_algo.h: basic algorithms
- util_console.{h,c}: command-line parser
- util_hexdump: the hexdump utilities
- util_log.h: logging utilities
- util_timer.h: high-precision timer
- **Parallel Items**- hash_chain_r.h: concurrent chain hashmap
- hash_multi_r.h: concurrent multiple hashmap
- mr_dataset.{h,cpp}: the MapReduce data abstraction
- mr_engine.h: the MapReduce engine
- mr_interm.h: the MapReduce intermediate storage abstraction
- os_atomic_intel64.h: atomic operations for the x86_64
- os_rdtsc.h: the Intel rdtsc instruction
- os_regionlock.h: region locks
- os_spinlock.h: various spinlocks for the x86_64
- os_thread.{h,cpp}: thread wrapper class
- os_typelock.h: typed locks for C++
