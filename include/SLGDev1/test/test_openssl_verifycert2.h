#if 0
static int add_crls_store(X509_STORE* st, STACK_OF(X509_CRL) *crls)
{
    X509_CRL* crl;
    int i;
    for (i = 0; i < sk_X509_CRL_num(crls); i++)
    {
        crl = sk_X509_CRL_value(crls, i);
        X509_STORE_add_crl(st, crl);
    }
    return 1;
}
#include "cstd.h"
int ssl_load_stores1(SSL_CTX* ctx,
                     const char* vfyCApath, const char* vfyCAfile,
                     const char* chCApath, const char* chCAfile,
                     STACK_OF(X509_CRL) *crls, int crl_download)
{
    X509_STORE* vfy = NULL, *ch = NULL;
    int rv = 0;
    if (vfyCApath != NULL || vfyCAfile != NULL)
    {
        vfy = X509_STORE_new();
        if (vfy == NULL)
        {
            goto err;
        }
        if (!X509_STORE_load_locations(vfy, vfyCAfile, vfyCApath))
        {
            goto err;
        }
        add_crls_store(vfy, crls);
        SSL_CTX_set1_verify_cert_store(ctx, vfy);
    }
    if (chCApath != NULL || chCAfile != NULL)
    {
        ch = X509_STORE_new();
        if (ch == NULL)
        {
            goto err;
        }
        if (!X509_STORE_load_locations(ch, chCAfile, chCApath))
        {
            goto err;
        }
        SSL_CTX_set1_chain_cert_store(ctx, ch);
    }
    rv = 1;
err:
    X509_STORE_free(vfy);
    X509_STORE_free(ch);
    return rv;
}
SSL_CTX* SrviceCtx1(void)
{
    SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_server_method());
    if (!ctx)
    {
        ERR_print_errors_fp(stdout);
        return ctx;
    }
    if (SSL_CTX_use_certificate_file(ctx, "CAcert.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stdout);
        SSL_CTX_free(ctx);
        return NULL;
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "privkey.pem", SSL_FILETYPE_PEM) <= 0)
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
        DEBUG_INFO("%s" , "error setting cipher AES256-GCM-SHA384");
    }
    return ctx;
}
SSL_CTX* ClientCtx1(void)
{
    SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_client_method());
    SSL_CTX_set_options(ctx, SSL_OP_ALL);
    if (!SSL_CTX_set_cipher_list(ctx , TLS1_TXT_RSA_WITH_AES_256_GCM_SHA384))
    {
        DEBUG_INFO("%s" , "error setting cipher AES256-GCM-SHA384");
    }
    return ctx;
}
int test_openssl_VerifyCert2()
{
    int rv = 0;
    int i = 0;
    int caLen = 0;
    int certLen = 0;
    const char* path = NULL;
    const char* path1 = NULL;
    path = "E:/pub/bin/ssl/openssl_exe/rootca";
    path1 = "E:/pub/bin/ssl/openssl_exe/aaa";
    //_chdir(path);
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX* ctx = ClientCtx1();
    rv = ssl_load_stores1(ctx, path, NULL, path1, NULL, NULL, 0);
    return 0;
}
#endif

