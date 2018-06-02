//#define DEBUG_INFO printf
//#include "sock/mynet.h"
#include "slgdev/test_slgdev.inl"
#include "openssl/test_openssl_net.inl"
#pragma comment(lib, "Ws2_32.lib")
#pragma comment( lib, "mswsock.lib")
//#include "openssl/s_server.inl"
//#include "openssl/apps.inl"
//#include "openssl/s_cb.inl"
#define EXIT_IF_TRUE(x) if (x) {return NULL;}
int LoadCert(const char* szFilePath, unsigned char* pbCert, int size)
{
    int len = 0;
    if (szFilePath == NULL || pbCert == NULL || size < 128)
    {
        return -1;
    }
    FILE* fp = fopen(szFilePath, "rb");
    if (NULL == fp)
    {
        return -2;
    }
    len = fread(pbCert, 1, size, fp);
    fclose(fp);
    return len;
}
int VerifyCert(unsigned char* pbCaCert, int nCaLen, unsigned char* pbCert, int nCertLen, char* pbCN, int size)
{
    int rv = -1;
    if (pbCaCert == NULL || nCaLen < 128 || pbCert == NULL || nCertLen < 128)
    {
        return rv;
    }
    X509* ca = NULL;
    X509* cert = NULL;
    X509_STORE* caStore = NULL;
    X509_STORE_CTX* ctx = NULL;
    X509_NAME* subject = NULL;
    OpenSSL_add_all_algorithms();
    caStore = X509_STORE_new();
    ctx = X509_STORE_CTX_new();
    ca = d2i_X509(NULL, (const unsigned char**)&pbCaCert, nCaLen);
    if (ca == NULL)
    {
        return -2;
    }
    rv = X509_STORE_add_cert(caStore, ca);
    if (rv != 1)
    {
        rv = -3;
        goto EXIT_VERIFY;
    }
    cert = d2i_X509(NULL, (const unsigned char**)&pbCert, nCertLen);
    if (cert == NULL)
    {
        rv = -4;
        goto EXIT_VERIFY;
    }
    rv = X509_STORE_CTX_init(ctx, caStore, cert, NULL);
    if (rv != 1)
    {
        rv = -5;
        goto EXIT_VERIFY;
    }
    rv = X509_verify_cert(ctx);
    if (rv != 1)
    {
        //fprintf(stderr, "X509_verify_cert fail, rv = %d, error id = %d, %s\n",
        // rv, ctx->error, X509_verify_cert_error_string(ctx->error));
        rv = (rv == 0 ? 1 : rv);
        goto EXIT_VERIFY;
    }
    subject = X509_get_subject_name(cert);
    if (subject)
    {
        X509_NAME_get_text_by_NID(subject, NID_commonName, pbCN, size);
    }
    rv = (rv == 1 ? 0 : rv);
EXIT_VERIFY:
    if (cert)
    {
        X509_free(cert);
    }
    if (ca)
    {
        X509_free(ca);
    }
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
// note: check certificate with OPENSSL
// parameter:
// char *szRootFP, root CA certificate file path
// char *szCertFP, subject certificate file path
// char *szCRLFP, CRL file path
// return:
// 1, success
// 0, error
bool verify509Cert(unsigned char* bRoot, int iRootLen, unsigned char* bCert, int iCertLen, unsigned char* bCRL, int iCrlLen)
{
    int iRet = 0;
    X509_STORE_CTX* ctx = NULL; // 证书存储区句柄
    X509* usrCert =       NULL; //X509 证书结构体，保存用户证书
    X509* caCert =        NULL; //X509 证书结构体，保存CA 证书
    X509* rootCert =      NULL; //X509 证书结构体，保存根证书
    X509_CRL* Crl =       NULL; //X509_CRL 结构体，保存CRL
    STACK_OF(X509) *caCertStack = NULL;
    X509_STORE* rootCertStore = NULL; // 证书存储区
    // 把DER 编码的根证书转化为X509 结构体
    rootCert = d2i_X509(NULL, (const unsigned char**)&bRoot, iRootLen);
    if (rootCert == NULL)
    {
        return false ;
    }
    // 把DER 编码的用户证书转化为X509 结构体
    usrCert = d2i_X509(NULL, (const unsigned char**)&bCert, iCertLen);
    if (usrCert == NULL)
    {
        return false ;
    }
    // 新建X509 证书存储区
    rootCertStore = X509_STORE_new();
    // 添加根证书到证书存储区
    X509_STORE_add_cert(rootCertStore, rootCert);
    // 设置检查CRL 标志位，如果设置此标志位，则检查CRL ，否则不检查CRL 。
    if (NULL != bCRL)
    {
        if (*bCRL != '/0')
        {
            // 把DER 编码的CRL 转化为X509_CRL 结构体
            Crl = d2i_X509_CRL(NULL, (const unsigned char**)&bCRL, iCrlLen);
            if (Crl == NULL)
            {
                X509_free(rootCert);
                return false ;
            }
            X509_STORE_set_flags(rootCertStore, X509_V_FLAG_CRL_CHECK);
            // 添加CRL 到证书存储区
            X509_STORE_add_crl(rootCertStore, Crl);
        }
    }
    // 新建证书存储区句柄
    ctx = X509_STORE_CTX_new();
    // 初始化根证书存储区、用户证书1
    iRet = X509_STORE_CTX_init(ctx, rootCertStore, usrCert, caCertStack);
    if (iRet != 1)
    {
        printf("X509_STORE_CTX_init err/n");
        X509_free(usrCert);
        X509_free(rootCert);
        X509_STORE_CTX_cleanup(ctx);
        X509_STORE_CTX_free(ctx);
        X509_STORE_free(rootCertStore);
        return false ;
    }
    // 验证用户证书
    iRet = X509_verify_cert(ctx);
    if (iRet != 1)
    {
        //printf("verify cer err.error= %d,info:%s/n", ctx->error, X509_verify_cert_error_string(ctx->error));
    }
    // 释放内存
    X509_free(usrCert);
    X509_free(rootCert);
    X509_STORE_CTX_cleanup(ctx);
    X509_STORE_CTX_free(ctx);
    X509_STORE_free(rootCertStore);
    return 1 == iRet ? true : false ;
}
// CryptoAPI
// To use CryptoAPI, we need to include "Wincrypt.h" and link to crypt32.lib first.
// Steps to check certificate and CRL
// 1, Add rood certificate and CRL into one cert store,
// 2, Add subject certificate into a cert context.
// 3, Check cert by calling CertVerifySubjectCertificateContext().
// By setting the flag we can check signature, RCL, and time:
// the flag can be the combination of
// CERT_STORE_REVOCATION_FLAG 0x04
// CERT_STORE_SIGNATURE_FLAG 0x01
// CERT_STORE_TIME_VALIDITY_FLAG 0x02
// Here we need to note that the function return true no matter if the cert is valid. We need to check the validation of cert by checking returned flag value. If be checked OK the flag will be 0, otherwise the flag remain unchanged.
// For example, to enable both signature and time validity, the value CERT_STORE_SIGNATURE_FLAG | CERT_STORE_TIME_VALIDITY_FLAG is placed in the pdwFlags DWORD value as an input parameter. If CERT_STORE_SIGNATURE_FLAG verification succeeds, but CERT_STORE_TIME_VALIDITY_FLAG verification fails, pdwFlags is set to CERT_STORE_TIME_VALIDITY_FLAG when the function returns.
// If CERT_STORE_REVOCATION_FLAG was enabled and the issuer does not have a CRL in the store, then CERT_STORE_NO_CRL_FLAG is set in addition to CERT_STORE_REVOCATION_FLAG. //CERT_STORE_NO_CRL_FLAG 0x00010000
// So if there is no CRL in store, we check a valid cert and we got the returned flag value 0x00010004, and we check a invalid subject certificate, the return value will be 0x00010005.
// And here is the code of certificate checking function:
// notes: check certificate with CryptoAPI
// parameter:
// char *szRootFP, root CA certificate file path
// char *szCertFP, subject certificate file path
// char *szCRLFP, CRL file path
// return:
// 1, success
// 0, system error
// -1, invalid certificate
// 4, revoked certificate
#if 0
int checkCert(char* szRootFP, char* szCertFP, char* szCRLFP)
{
    int iRet = 0;
    PCCERT_CONTEXT pCertRootContext = NULL;
    PCCERT_CONTEXT pCertContext = NULL;
    HCERTSTORE hMemStore = NULL;
    unsigned char usrCertificate[4096] = {0}; //DER 证书缓冲区数组
    unsigned long usrCertificateLen =    0;   // 证书长度
    unsigned char derCrl[4096] =         {0}; //CRL 缓冲区数组
    unsigned long derCrlLen =            0;   //CRL 长度
    unsigned char derRootCert[4096] =    {0}; // 根证书缓冲区数组
    unsigned long derRootCertLen =       0;   // 根证书长度
    DWORD dwFlags =                      0;   // flag for CertVerifySubjectCertificateContext
    // error check
    if (NULL == szRootFP || NULL == szCertFP)
    {
        return false ;
    }
    FILE* fp;
    // 读取根证书
    fp = fopen(szRootFP, "rb");
    if (fp == NULL)
    {
        return false ;
    }
    derRootCertLen = fread(derRootCert, 1, sizeof(derRootCert), fp);
    fclose(fp);
    // 读取CRL 文件
    if (NULL != szCRLFP)
    {
        if (*szCRLFP != '/0')
        {
            fp = fopen(szCRL, "rb");
            if (fp == NULL)
            {
                printf("open file err/n");
                return false ;
            }
            derCrlLen = fread(derCrl, 1, sizeof(derCrl), fp);
            fclose(fp);
            dwFlags = CERT_STORE_REVOCATION_FLAG;
        }
    }
    // 读取待验证的用户证书
    fp = fopen(szCertFP, "rb");
    if (fp == NULL)
    {
        printf("open file err/n");
        return false ;
    }
    usrCertificateLen = fread(usrCertificate, 1, sizeof(usrCertificate), fp);
    fclose(fp);
    // add subject cert to context
    pCertContext = CertCreateCertificateContext(
                       MY_ENCODING_TYPE,   // Encoding type
                       usrCertificate,     // Encoded data from the certificate retrieved
                       usrCertificateLen); // Length of the encoded data
    if (NULL == pCertContext)
    {
        goto end;
    }
    // add to root context
    // here we use store
    //pCertRootContext = CertCreateCertificateContext(
    // MY_ENCODING_TYPE, // Encoding type
    // derRootCert, // Encoded data from the certificate retrieved
    // derRootCertLen); // Length of the encoded data
    //if(NULL == pCertRootContext)
    // goto end;
    //iRet = CertAddCertificateContextToStore(
    // hMemStore,
    // pCertRootContext,
    // CERT_STORE_ADD_ALWAYS,
    // &pStoreContext
    //);
    //if (iRet != true)
    // goto end;
    hMemStore = CertOpenStore(
                    CERT_STORE_PROV_MEMORY, // the memory provider type
                    0,                      // the encoding type is not needed
                    NULL,                   // use the default HCRYPTPROV
                    0,                      // accept the default dwFlags
                    NULL                    // pvPara is not used
                );
    if (NULL == hMemStore)
    {
        goto end;
    }
    // add root cert
    iRet = CertAddEncodedCertificateToStore(hMemStore, MY_ENCODING_TYPE, derRootCert, derRootCertLen, CERT_STORE_ADD_ALWAYS, &pCertRootContext);
    if (iRet != true)
    {
        goto end;
    }
    // if need check RCL, go this way
    if (CERT_STORE_REVOCATION_FLAG == dwFlags)
    {
        // add crl
        iRet = CertAddEncodedCRLToStore(hMemStore, MY_ENCODING_TYPE, derCrl, derCrlLen, CERT_STORE_ADD_ALWAYS, NULL);
        if (iRet != true)
        {
            goto end;
        }
    }
    // check the cert
    // the flag can be
    // CERT_STORE_REVOCATION_FLAG 0x04
    // CERT_STORE_SIGNATURE_FLAG 0x01
    // CERT_STORE_TIME_VALIDITY_FLAG 0x02
    dwFlags |= CERT_STORE_SIGNATURE_FLAG;// | CERT_STORE_REVOCATION_FLAG;
    iRet = CertVerifySubjectCertificateContext(pCertContext, pCertRootContext, &dwFlags);
end:
    // clean resource
    if (pCertContext)
    {
        CertFreeCertificateContext(pCertContext);
    }
    if (pCertRootContext)
    {
        CertFreeCertificateContext(pCertRootContext);
    }
    if (hMemStore)
    {
        CertCloseStore(hMemStore, 0);
    }
    // return result
    if (iRet != true)
    {
        return false ;
    }
    if (dwFlags != 0)
    {
        return dwFlags == CERT_STORE_SIGNATURE_FLAG ? -1 : dwFlags;
    }
    return true ;
}
#endif
static SSL_CTX* SrviceCtx1(void)
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX* ctx = SSL_CTX_new(SSLv23_method());
    if (!ctx)
    {
        ERR_print_errors_fp(stdout);
        return ctx;
    }
    //X509 *s_cert = NULL;
    //int ret;
    //s_cert = LoadCert2("usercrt1.pem");
    //ret = SSL_CTX_use_certificate(ctx, s_cert);
    //SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    //设置会话的握手方式并加载CA证书
    //SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    //SSL_CTX_load_verify_locations(ctx, "rootcrt.pem", NULL);
    //SSL_CTX_use_certificate_file(ctx, "usercrt1.pem", SSL_FILETYPE_PEM);
    if (SSL_CTX_use_certificate_file(ctx, "usercrt1.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stdout);
        SSL_CTX_free(ctx);
        return NULL;
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "userkey1.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stdout);
        SSL_CTX_free(ctx);
        return NULL;
    }
    if (!SSL_CTX_check_private_key(ctx))
    {
        ERR_print_errors_fp(stdout);
        SSL_CTX_free(ctx);
        return NULL;
    }
    SSL_CTX_set_options(ctx, SSL_OP_ALL);
    if (!SSL_CTX_set_cipher_list(ctx , TLS1_TXT_RSA_WITH_AES_256_GCM_SHA384))
    {
        printf("%s" , "error setting cipher AES256-GCM-SHA384");
    }
    return ctx;
}
#if 0
int s_server()
{
    const char* pass = "123456";
    ENGINE* engine = NULL;
    const char* s_key_file = NULL;
    const char* s_cert_file = NULL;
    int s_cert_format = FORMAT_PEM, s_key_format = FORMAT_PEM;
    int s_dcert_format = FORMAT_PEM, s_dkey_format = FORMAT_PEM;
    EVP_PKEY* s_key = NULL, *s_dkey = NULL;
    X509* s_cert = NULL, *s_dcert = NULL;
    SSL_CTX* ctx = NULL;
    const SSL_METHOD* meth = TLS_server_method();
    s_key_file = "userkey1.pem";
    s_cert_file = "usercrt1.pem";
    s_key = load_key(s_key_file, s_key_format, 0, pass, engine,
                     "server certificate private key file");
    if (!s_key)
    {
        ERR_print_errors(bio_err);
        return 0;
    }
    s_cert = load_cert(s_cert_file, s_cert_format,
                       "server certificate file");
    if (!s_cert)
    {
        ERR_print_errors(bio_err);
        return 0;
    }
    ctx = SSL_CTX_new(meth);
    SSL_CTX_set_dh_auto(ctx, 1);
    if (!set_cert_key_stuff(ctx, s_cert, s_key, NULL, 0))
    {
        return 0;
    }
    return 0;
}
#endif
#include "cstd.h"
int test_VerifyCert()
{
    X509* ca = NULL;
    X509* cert = NULL;
    int rv = 0;
    char cn[256] = {0};
    const char* path = NULL;
    const char* path2 = NULL;
    path = "E:/pub/bin/ssl/openssl_exe";
    path2 = "C:/rootca";
    path2 = path;
    _chdir(path);
    //s_server();
    SrviceCtx1();
    ca = LoadCert2("rootcrt.pem");
    cert = LoadCert2("usercrt1.pem");
    //cert = LoadCert2("user.crt");
    if (1)
    {
        //rv = VerifyCertFromfolder(path2, cert);
    }
    rv = VerifyCert2(ca, cert, cn, 255);
    printf("rv=%d, cn=%s\n", rv, cn);
    if (cert)
    {
        X509_free(cert);
    }
    if (ca)
    {
        X509_free(ca);
    }
    return 0;
}
int test_VerifyCert1()
{
    int rv = 0;
    int i = 0;
    int caLen = 0;
    int certLen = 0;
    char cn[255] = {0};
    unsigned char cert[4096] = {0};
    unsigned char ca[4096] = {0};
    const char* path = NULL;
    path = "E:/pub/bin/ssl/openssl_exe";
    _chdir(path);
    caLen = LoadCert("rootca.der", ca, 4096);
    certLen = LoadCert("user.der", cert, 4096);
    //rv = VerifyCert(ca, caLen, cert, certLen, cn, 255);
    rv = VerifyCert(ca, caLen, cert, certLen, NULL, 0);
    printf("rv=%d, cn=%s\n", rv, cn);
    return 0;
}

