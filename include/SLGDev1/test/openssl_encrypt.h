
#ifndef _OPENSSL_ENCRYPT_INL_
#define _OPENSSL_ENCRYPT_INL_

#include "openssl_lib.inl"

// RSA加密部分代码demo
static int EncodeRSAKeyFile(const char* strPemFileName, const char* data, int data_len, char* strRet, int strRet_maxlen)
{
    FILE* hPubKeyFile = NULL;
    RSA* pRSAPublicKey;
    int nLen;

    if (strPemFileName == NULL || data == NULL)
    {
        assert(false);
        return -1;
    }

    hPubKeyFile = fopen(strPemFileName, "rb");

    if (hPubKeyFile == NULL)
    {
        assert(false);
        return -1;
    }

    pRSAPublicKey = RSA_new();

    if (PEM_read_RSA_PUBKEY(hPubKeyFile, &pRSAPublicKey, 0, 0) == NULL)
    {
        assert(false);
        return -1;
    }

    nLen = RSA_size(pRSAPublicKey);

    if (strRet_maxlen >= nLen)
    {
        nLen = RSA_public_encrypt(data_len, (const unsigned char*)data, (unsigned char*)strRet, pRSAPublicKey, RSA_PKCS1_PADDING);
    }

    RSA_free(pRSAPublicKey);
    fclose(hPubKeyFile);
    CRYPTO_cleanup_all_ex_data();
    return nLen;
}

// RSA解密部分代码demo：
static int DecodeRSAKeyFile(const char* strPemFileName, const char* data, int data_len, char* strRet, int strRet_maxlen)
{
    FILE* hPriKeyFile = NULL;
    RSA* pRSAPriKey;
    int nLen;

    if (strPemFileName == NULL || data == NULL)
    {
        assert(false);
        return -1;
    }

    hPriKeyFile = fopen(strPemFileName, "rb");

    if (hPriKeyFile == NULL)
    {
        assert(false);
        return -1;
    }

    pRSAPriKey = RSA_new();

    if (PEM_read_RSAPrivateKey(hPriKeyFile, &pRSAPriKey, 0, 0) == NULL)
    {
        assert(false);
        return -1;
    }

    nLen = RSA_size(pRSAPriKey);

    if (strRet_maxlen >= nLen)
    {
        nLen = RSA_private_decrypt(data_len, (const unsigned char*)data, (unsigned char*)strRet, pRSAPriKey, RSA_PKCS1_PADDING);
    }

    RSA_free(pRSAPriKey);
    fclose(hPriKeyFile);
    CRYPTO_cleanup_all_ex_data();
    return nLen;
}

// RSA的API中当使用参数RSA_PKCS1_PADDING时，明文长度不能大于密文长度-11；当使用参数RSA_NO_PADDING时，明文长度需要正好是128。
// AES加密部分代码：
static int EncodeAES(const char* key, int key_len, const char* data, int data_len, char* strRet, int strRet_maxlen)
{
    AES_KEY aes_key = {0};
    char* data_bak = NULL;
    int i, data_bak_len;

    if (AES_set_encrypt_key((const unsigned char*)key, key_len * 8, &aes_key) < 0)
    {
        assert(false);
        return -1;
    }

    data_bak_len = data_len + (AES_BLOCK_SIZE - data_len % AES_BLOCK_SIZE);

    if (data_bak_len <= strRet_maxlen)
    {
        data_bak = (char*)malloc(data_bak_len);
        memcpy(data_bak, data, data_len);
        memset(data_bak + data_len, 0, data_bak_len - data_len);

        for (i = 0; i < data_bak_len / AES_BLOCK_SIZE; i++)
        {
            const unsigned char* str16 = (const unsigned char*)data_bak + i * AES_BLOCK_SIZE; //AES_BLOCK_SIZE
            unsigned char* out = (unsigned char*)strRet + i * AES_BLOCK_SIZE; //[AES_BLOCK_SIZE];
            memset(out, 0, AES_BLOCK_SIZE);
            AES_encrypt(str16, out, &aes_key);
        }

        free(data_bak);
    }

    return data_bak_len;
}

// AES解密部分代码：
static int DecodeAES(const char* key, int key_len, const char* data, int data_len, char* strRet, int strRet_maxlen)
{
    AES_KEY aes_key;
    int i;

    if (AES_set_decrypt_key((const unsigned char*)key, key_len * 8, &aes_key) < 0)
    {
        assert(false);
        return -1;
    }

    for (i = 0; i < data_len / AES_BLOCK_SIZE; i++)
    {
        const unsigned char* str16 = (const unsigned char*)data + i * AES_BLOCK_SIZE; //AES_BLOCK_SIZE;
        unsigned char* out = (unsigned char*)strRet + i * AES_BLOCK_SIZE; //[AES_BLOCK_SIZE];
        memset(out, 0, AES_BLOCK_SIZE);
        AES_decrypt((const unsigned char*)str16, out, &aes_key);
    }

    return data_len;
}

#define AES_BITS 128
#define MSG_LEN 128
static int check_key_copy(const void* key, int key_len, void* key1, int max_key_len)
{
    if (key_len > max_key_len)
    {
        key_len = max_key_len;
    }

    else
    {
        while (max_key_len >= 32 && (max_key_len / 2) > key_len)
        {
            max_key_len /= 2;
        }

    }

    memcpy(key1, key, key_len);
    return max_key_len;
}

#define MAX_KEY_LEN 128
static int aes_encrypt(const void* key, int key_len, void* in, int in_len, void* out, int out_maxlen)//, int olen)可能会设置buf长度
{
    //加密的初始化向量,iv一般设置为全0,可以设置其他，但是加密解密要一样就行
    unsigned char iv[AES_BLOCK_SIZE] = {0};
    int len1 = in_len & 15;
    AES_KEY aes = {0};
    char key1[MAX_KEY_LEN] = {0};

    if (!in || !key || !out)
    {
        return 0;
    }

    key_len = check_key_copy(key, key_len, key1, MAX_KEY_LEN);
    key = key1;

    if (AES_set_encrypt_key((const unsigned char*)key, key_len * 8, &aes) < 0)
    {
        return 0;
    }

    in_len -= len1;
    //这里的长度是char*in的长度，但是如果in中间包含'\0'字符的话
    //那么就只会加密前面'\0'前面的一段，所以，这个len可以作为参数传进来，记录in的长度
    //至于解密也是一个道理，光以'\0'来判断字符串长度，确有不妥，后面都是一个道理。
    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, in_len, &aes, iv, AES_ENCRYPT);
    memcpy((char*)out + in_len, (char*)in + in_len, len1);
    return in_len;
}

static int aes_decrypt(const void* key, int key_len, void* in, int in_len, void* out, int out_maxlen)
{
    unsigned char iv[AES_BLOCK_SIZE] = {0};//加密的初始化向量
    int len1 = in_len & 15;
    AES_KEY aes = {0};
    char key1[MAX_KEY_LEN] = {0};

    if (!in || !key || !out)
    {
        return 0;
    }

    key_len = check_key_copy(key, key_len, key1, MAX_KEY_LEN);
    key = key1;

    if (AES_set_decrypt_key((const unsigned char*)key, key_len * 8, &aes) < 0)
    {
        return 0;
    }

    in_len -= len1;
    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, in_len, &aes, iv, AES_DECRYPT);
    memcpy((char*)out + in_len, (char*)in + in_len, len1);
    return in_len;
}

#ifdef TEST
#include "cstd.h"
int test_openssl()
{
    char* aa = NULL, *bb, *cc;
    int sz = 1001;
    char key[AES_BLOCK_SIZE + 1] = {0};
    aa = (char*)malloc(sz * 5);
    memset(aa, '1', sz);
    memset(key, '1', AES_BLOCK_SIZE);
    bb = aa + sz;
    cc = bb + sz * 2;
    aes_encrypt(key, AES_BLOCK_SIZE, aa, sz, bb, sz);
    bb[1] = 0;
    aes_decrypt(key, AES_BLOCK_SIZE, bb, sz, cc, sz);
    ASSERT(0 == memcmp(aa, cc, sz));
    free(aa);
    return 0;
}

#endif // TEST

#endif // _OPENSSL_ENCRYPT_INL_

