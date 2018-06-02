#ifdef USE_SSL
#if 1
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")
#else
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"libssl.lib")
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/aes.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/pkcs12.h>
#undef X509_NAME
SSL_CTX* m_Client = NULL;
SSL_CTX* m_Srvice = NULL;
#define OFFSET_SIZE 20
// 客户端
// 1. 建立普通链接
// 2. 用户调用函数上传子证书(函数内部：先用根证书验证，通过后才上传)
// 3. 收到dsp 的证书OK消息后切换到tls模式
// 服务器
// 1. 建立普通监听
// 2. 建立普通链接
// 3. 判断tls 功能是否打开。
//   3.1 如果关闭。不做处理。跳到6。
//   3.2 如果打开。开始10秒上传限制计时。
// 4. 超时关闭连接。
// 5. 验证证书。通过则切换到tls模式并发证书OK消息。不通过则关闭连接。
// 6. 开始通信。
//利用OpenSSL的PKCS12_parse函数进行证书格式转换的参考代码。
//目前这个函数还存在内存泄漏，这是OpenSSL 的Bug，做法上完全可以参考。
//EVP_CIPHER *g_pEncAlg = NULL;
X509* LoadCert2(const char* szFilePath)
{
    int len = 0;
    int size = 0; //
    char* pem_c_str = NULL;
    if (szFilePath == NULL)
    {
        return NULL;
    }
    FILE* fp = fopen(szFilePath, "rb");
    if (NULL == fp)
    {
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp) + 10;
    if (size > 1024 * 32)
    {
        size = 1024 * 32;
    }
    fseek(fp, 0, SEEK_SET);
    pem_c_str = (char*)malloc(size);
    len = fread(pem_c_str, 1, size - 1, fp);
    fclose(fp);
    pem_c_str[len] = 0;
    BIO* bio_mem = BIO_new(BIO_s_mem());
    BIO_puts(bio_mem, pem_c_str);
    X509* x509 = NULL;
    x509 = PEM_read_bio_X509(bio_mem, NULL, NULL, NULL);
    BIO_free(bio_mem);
    free(pem_c_str);
    return x509;
}
int VerifyCert2(X509* ca, X509* cert, char* pbCN, int size)
{
    int rv = -1;
    X509_STORE* caStore = NULL;
    X509_STORE_CTX* ctx = NULL;
    X509_NAME* subject = NULL;
    OpenSSL_add_all_algorithms();
    caStore = X509_STORE_new();
    ctx = X509_STORE_CTX_new();
    rv = X509_STORE_add_cert(caStore, ca);
    do
    {
        if (rv != 1)
        {
            rv = -3;
            break;
        }
        rv = X509_STORE_CTX_init(ctx, caStore, cert, NULL);
        if (rv != 1)
        {
            rv = -5;
            break;
        }
        rv = X509_verify_cert(ctx);
        if (rv != 1)
        {
            int err = X509_STORE_CTX_get_error(ctx);
            fprintf(stderr, "X509_verify_cert fail, rv = %d, error = %s\n", rv, X509_verify_cert_error_string(err));
            //rv = (rv == 0 ? 1 : rv);
            break;
        }
        subject = X509_get_subject_name(cert);
        if (subject)
        {
            X509_NAME_get_text_by_NID(subject, NID_commonName, pbCN, size);
        }
        //rv = (rv == 1 ? 0 : rv);
    }
    while (0);
    if (caStore)
    {
        X509_STORE_free(caStore);
    }
    if (ctx)
    {
        X509_STORE_CTX_cleanup(ctx);
        X509_STORE_CTX_free(ctx);
    }
    return rv;
}
typedef struct
{
    EVP_PKEY*   pPkey;
    X509*       pCert;
} pk12_t;
int pk12_unpack(pk12_t* pk12, const char* pk12_buf, int pk12_buflen, const char* pszP12Password)
{
    PKCS12*     p12 = NULL;
    BIO*        pBIO = NULL;
    STACK_OF(X509) *pCaCerts = NULL;
    OpenSSL_add_all_algorithms();
    //CRYPTO_malloc_debug_init();
    //MemCheck_start();
    //MemCheck_stop();
    do
    {
        if ((pBIO = BIO_new(BIO_s_mem())) == NULL)
        {
            //hr = CRED_E_BIO;
            break;
        }
        BIO_write(pBIO, pk12_buf, pk12_buflen);
        if ((p12 = d2i_PKCS12_bio(pBIO, &p12)) == NULL)
        {
            //hr = CRED_E_P12_OPEN;
            break;
        }
        if (PKCS12_parse(p12, pszP12Password ? pszP12Password : "", &pk12->pPkey, &pk12->pCert, &pCaCerts) <= 0)
        {
            //hr = CRED_E_P12_PARSE;
            break;
        }
        if (pszP12Password == NULL)
        {
            if (!PKCS12_verify_mac(p12, NULL, 0))
            {
                //hr = CRED_E_P12_BAD_PWD;
                break;
            }
        }
        else if (!PKCS12_verify_mac(p12, pszP12Password, -1))
        {
            //hr = CRED_E_P12_BAD_PWD;
            break;
        }
    }
    while (0);
    if (pCaCerts)
    {
        sk_X509_free(pCaCerts);
    }
    if (p12)
    {
        PKCS12_free(p12);
    }
    if (pBIO)
    {
        BIO_free(pBIO);
    }
    ERR_free_strings();
    ERR_remove_state(0);
    EVP_cleanup();
    return 0;
}
void pk12_free(pk12_t* pk12)
{
    if (pk12->pCert)
    {
        X509_free(pk12->pCert);
    }
    if (pk12->pPkey)
    {
        EVP_PKEY_free(pk12->pPkey);
    }
    pk12->pPkey = NULL;
    pk12->pCert = NULL;
}
BOOL OpenSsl_Init(int bInit)
{
    static BOOL m_ObjInit = 0;
    if (bInit)
    {
        if (!m_ObjInit)
        {
            m_ObjInit = 1;
            SSL_library_init();
            OpenSSL_add_all_algorithms();
            SSL_load_error_strings();
        }
    }
    return TRUE;
}
//#include <direct.h>
BOOL  OpenSsl_Free(SOCKET sock , void* pssl)
{
    if (pssl)
    {
        SSL_shutdown((SSL*)pssl);
        SSL_free((SSL*)pssl);
        DEBUG_INFO("%s" , "COpenSsl::Free OK");
    }
    return TRUE;
}
int VerifyCertFromfolder(const char* Path, X509* cert)
{
    char szFind[MAX_PATH];
    char cn[256] = {0};
    WIN32_FIND_DATA FindFileData;
    strcpy(szFind, Path);
    strcat(szFind, "\\*.*");
    char szFile[MAX_PATH] = {0};
    HANDLE hFind = FindFirstFile(szFind, &FindFileData);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        return 0;
    }
    do
    {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0)
            {
                //发现子目录，递归之
                strcpy(szFile, Path);
                strcat(szFile, "\\");
                strcat(szFile, FindFileData.cFileName);
                //printf(szFile);
            }
        }
        else
        {
            //找到文件，处理之
            //printf("%s\\%s\n", lpPath, FindFileData.cFileName);
            X509* ca = NULL;
            strcpy(szFile, Path);
            strcat(szFile, "\\");
            strcat(szFile, FindFileData.cFileName);
            ca = LoadCert2(szFile);
            if (ca)
            {
                int rv = VerifyCert2(ca, cert, cn, 255);
                X509_free(ca);
                if (1 == rv)
                {
                    FindClose(hFind);
                    return rv;
                }
            }
        }
    }
    while (FindNextFile(hFind, &FindFileData));
    FindClose(hFind);
    return 0;
}
BOOL ShowCerts(SSL* ssl, int type)
{
    X509* cert = NULL;
    int ret = 1;
    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL)
    {
        if (0)
        {
            char* line = NULL;
            DEBUG_INFO("%s" , "certificate info:\n");
            line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
            DEBUG_INFO("subject_name: %s\n", line);
            free(line);
            line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
            DEBUG_INFO("issuer_name: %s\n", line);
            free(line);
        }
        ret = 1;
        if (3 == type)
        {
            ret = VerifyCertFromfolder(SSL_ROOTCA_PATH, cert);
            DEBUG_INFO("VerifyCertFromfolder: %d\n", ret);
        }
        X509_free(cert);
        return ret;
    }
    else
    {
        DEBUG_INFO("%s" , "no certificate info\n");
    }
    return ret;
}
int pk12_VerifyCert(const char* pk12_buf, int pk12_buflen, const char* pass)
{
    int ret = 0;
    pk12_t pk12[1] = {0};
    pk12_unpack(pk12, pk12_buf, pk12_buflen, pass);
    ret = VerifyCertFromfolder(SSL_ROOTCA_PATH, pk12->pCert);
    pk12_free(pk12);
    return ret;
}
void* OpenSsl_BindSrvice(SOCKET sock)
{
    SSL* ssl = NULL;
    if (m_Srvice)
    {
        ssl = SSL_new(m_Srvice);
        SSL_set_fd(ssl, sock);
        if (SSL_accept(ssl) == -1)
        {
            OpenSsl_Free(sock, ssl);
            ERR_print_errors_fp(stderr);
            ssl = NULL;
        }
        else
        {
            DEBUG_INFO("certificate %s" , X509_V_OK == SSL_get_verify_result(ssl) ? "OK" : "No");
        }
    }
    return ssl;
}
BOOL OpenSsl_InitSrvice(const char* pk12_buf, int pk12_buflen, const char* pass)
{
    SSL_CTX* ctx = m_Srvice;
    int ret = 0;
    if (NULL == m_Srvice)
    {
        OpenSsl_Init(1);
        ctx = SSL_CTX_new(TLSv1_2_server_method());
        if (!ctx)
        {
            ERR_print_errors_fp(stdout);
            return 0;
        }
        m_Srvice = ctx;
    }
    if (pk12_buf)
    {
        pk12_t pk12[1] = {0};
        pk12_unpack(pk12, pk12_buf, pk12_buflen, pass);
        SSL_CTX_use_certificate(ctx, pk12->pCert);
        SSL_CTX_use_PrivateKey(ctx, pk12->pPkey);
        pk12_free(pk12);
        if (!SSL_CTX_check_private_key(ctx))
        {
            ERR_print_errors_fp(stdout);
            SSL_CTX_free(ctx);
            return 0;
        }
        SSL_CTX_set_options(ctx, SSL_OP_ALL);
        if (!SSL_CTX_set_cipher_list(ctx , TLS1_TXT_RSA_WITH_AES_256_GCM_SHA384))
        {
            DEBUG_INFO("%s" , "error setting cipher AES256-GCM-SHA384");
        }
    }
    if (0)
    {
        const char* s_cert_file = NULL;
        const char* s_key_file = NULL;
        const char* path = NULL;
        path = "E:/pub/bin/ssl/openssl_exe";
        //_chdir(path);
        s_cert_file = "usercrt1.pem";
        s_key_file = "userkey1.pem";
        if (SSL_CTX_use_certificate_file(ctx, s_cert_file, SSL_FILETYPE_PEM) <= 0)
        {
            ERR_print_errors_fp(stdout);
            ret = 0;
        }
        if (SSL_CTX_use_PrivateKey_file(ctx, s_key_file, SSL_FILETYPE_PEM) <= 0)
        {
            ERR_print_errors_fp(stdout);
            ret = 0;
        }
        if (!SSL_CTX_check_private_key(ctx))
        {
            ERR_print_errors_fp(stdout);
            ret = 0;
        }
    }
    return 1;
}
void* OpenSsl_BindClient(SOCKET sock)
{
    SSL* ssl = NULL;
    int ret = 0;
    if (NULL == m_Client)
    {
        SSL_CTX* ctx = NULL;
        OpenSsl_Init(1);
        ctx = SSL_CTX_new(TLSv1_2_client_method());
        if (ctx)
        {
            SSL_CTX_set_options(ctx, SSL_OP_ALL);
            if (!SSL_CTX_set_cipher_list(ctx , TLS1_TXT_RSA_WITH_AES_256_GCM_SHA384))
            {
                DEBUG_INFO("%s" , "error setting cipher AES256-GCM-SHA384");
                SSL_CTX_free(ctx);
                ctx = NULL;
            }
        }
        m_Client = ctx;
    }
    if (m_Client)
    {
        int ret = 0;
        ssl = SSL_new(m_Client);
        SSL_set_fd(ssl, sock);
        if ((ret = SSL_connect(ssl)) == -1)
        {
            OpenSsl_Free(sock, ssl);
            ssl = NULL;
            DEBUG_INFO("%s" , "SSL_connect failed!!!1");
            ERR_print_errors_fp(stderr);
        }
        else
        {
            DEBUG_INFO("certificate %s , encryption %s " , X509_V_OK == SSL_get_verify_result(ssl) ? "OK" : "No" , SSL_get_cipher(ssl));
            ret = ShowCerts(ssl, 3);
            if (!ret)
            {
                OpenSsl_Free(sock, ssl);
                ssl = NULL;
            }
        }
    }
    return ssl;
}
int32 OpenSsl_Recv(SOCKET sock, void* pssl , int8* data, int32 len)
{
    if (pssl)
    {
        DEBUG_INFO("SSL_read %d", len);
        return SSL_read((SSL*)pssl, data, len);
    }
    return sock_recv(sock, data, len, 0);
}
int32 OpenSsl_Send(SOCKET sock, void* pssl, int8* data, int32 len, const addr_in* pAddr)
{
    if (pssl)
    {
        DEBUG_INFO("SSL_write %d", len);
        return SSL_write((SSL*)pssl, data, len);
    }
    return sock_send(sock, data, len, pAddr);
}
#else
int32 OpenSsl_Recv(SOCKET sock, void* pssl , int8* data, int32 len)
{
    return sock_recv(sock, data, len, 0);
}
int32 OpenSsl_Send(SOCKET sock, void* pssl, int8* data, int32 len, const addr_in* pAddr)
{
    return sock_send(sock, data, len, pAddr);
}
#endif

