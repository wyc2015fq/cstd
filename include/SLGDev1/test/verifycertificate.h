#include "sock/openssl_lib.inl"
void verifyCertificate() ;
X509* loadCert(char*) ;
void loadToStore(char*, X509_STORE*&) ;
void test_verifyCertificate()
{
    int i = 0 ;
    X509* cert = loadCert("rootca.crt");
    X509_STORE* store = X509_STORE_new();
    loadToStore("user.crt", store);
    X509_STORE_CTX* ctx = X509_STORE_CTX_new();
    X509_STORE_CTX_init(ctx, store, cert, NULL);
    i = X509_verify_cert(ctx);
    printf("i = %d\n", i) ; // Returns i = 0
    if (i != 1)
    {
        printf("%s", X509_verify_cert_error_string(ctx->error));
    }
    // Returns "Certificate Signature Failure"
    X509_STORE_CTX_cleanup(ctx);
    X509_STORE_CTX_free(ctx);
    X509_STORE_free(store);
    ctx = NULL;
    store = NULL;
}
void loadToStore(char* file, X509_STORE*& store)
{
    X509* cert = loadCert(file);
    if (cert != NULL)
    {
        X509_STORE_add_cert(store, cert);
    }
    else
    {
        printf("Can not load certificate");
    }
}
int verify_cert(char* pbCaCert, int nCaLen)
{
    BIO* bio_mem = BIO_new(BIO_s_mem());
    BIO_puts(bio_mem, pem_c_str);
    X509* x509 = PEM_read_bio_X509(bio_mem, NULL, NULL, NULL);
    EVP_PKEY* pkey = X509_get_pubkey(x509);
    int r = X509_verify(x509, pkey);
    EVP_PKEY_free(pkey);
    BIO_free(bio_mem);
    X509_free(x509);
    return r;
}

