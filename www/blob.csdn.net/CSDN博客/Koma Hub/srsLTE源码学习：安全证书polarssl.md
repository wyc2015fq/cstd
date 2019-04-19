# srsLTE源码学习：安全证书polarssl - Koma Hub - CSDN博客
2019年04月08日 21:36:44[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：30
**Table of Contents**
[liblte_ssl.h](#liblte_ssl.h%C2%A0%C2%A0)
# liblte_ssl.h  
 lib\include\srslte\common    2101    4/8/2019    19
```cpp
#ifndef SRSLTE_LIBLTE_SSL_H
#define SRSLTE_LIBLTE_SSL_H
#ifdef HAVE_POLARSSL
/*
PolarSSL源码，也许是最小巧的ssl代码库。高效、便于移植和集成。尤其适合嵌入式应用。 [1] 
PolarSSL又称mbed TLS，支持常见的安全算法，如：AES、DES、RSA、ECC、SHA256、MD5、BASE64等等。除此之外还支持公钥证书体系。
https://tls.mbed.org/
https://www.oschina.net/p/polarssl
*/
#include "polarssl/sha256.h" //PolarSSL源码，也许是最小巧的ssl代码库。
#include "polarssl/aes.h"  //ssl安全证书
void sha256(const unsigned char *key, size_t keylen,
            const unsigned char *input, size_t ilen,
            unsigned char output[32], int is224 )
{
  sha256_hmac(key, keylen, input, ilen, output, is224);
}
#endif // HAVE_POLARSSL
#ifdef HAVE_MBEDTLS
#include "mbedtls/md.h"
#include "mbedtls/aes.h"
typedef mbedtls_aes_context aes_context;
#define AES_ENCRYPT     1
#define AES_DECRYPT     0
int aes_setkey_enc( aes_context *ctx, const unsigned char *key, unsigned int keysize )
{
  return mbedtls_aes_setkey_enc(ctx, key, keysize);
}
int aes_crypt_ecb( aes_context *ctx,
                    int mode,
                    const unsigned char input[16],
                    unsigned char output[16] )
{
  return mbedtls_aes_crypt_ecb(ctx, mode, input, output);
}
int aes_crypt_ctr(aes_context *ctx,
                  size_t length,
                  size_t *nc_off,
                  unsigned char nonce_counter[16],
                  unsigned char stream_block[16],
                  const unsigned char *input,
                  unsigned char *output )
{
  return mbedtls_aes_crypt_ctr(ctx, length, nc_off, nonce_counter,
      stream_block, input, output);
}
void sha256(const unsigned char *key, size_t keylen,
            const unsigned char *input, size_t ilen,
            unsigned char output[32], int is224 )
{
  mbedtls_md_hmac(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
                  key, keylen,
                  input, ilen,
                  output );
}
#endif // HAVE_MBEDTLS
#endif // SRSLTE_LIBLTE_SSL_H
```
