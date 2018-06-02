
#include "../slgdev_private.h"
#include "openssl.h"

#ifdef USE_SSL
#undef X509_NAME
//#include "cfile.h"
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

    if (rv != 1)
    {
        rv = -3;
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
        int err = X509_STORE_CTX_get_error(ctx);
        fprintf(stderr, "X509_verify_cert fail, rv = %d, error = %s\n", rv, X509_verify_cert_error_string(err));
        //rv = (rv == 0 ? 1 : rv);
        goto EXIT_VERIFY;
    }

    subject = X509_get_subject_name(cert);

    if (subject)
    {
        X509_NAME_get_text_by_NID(subject, NID_commonName, pbCN, size);
    }

    //rv = (rv == 1 ? 0 : rv);
EXIT_VERIFY:

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

#ifdef _WIN32
//#include "verifyCertificate.inl"
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
//#include "openssl_encrypt.inl"

BOOL ShowCerts(SSL* ssl, int type)
{
    X509* cert = NULL;
    char* line = NULL;
    int ret = 1;
    cert = SSL_get_peer_certificate(ssl);

    if (cert != NULL)
    {
        DEBUG_INFO("%s" , "数字证书信息:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        DEBUG_INFO("证书: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        DEBUG_INFO("颁发者: %s\n", line);
        free(line);
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
        DEBUG_INFO("%s" , "无证书信息！\n");
    }

    return ret;
}
#endif

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
#ifdef _WIN32
int pk12_VerifyCert(const char* pk12_buf, int pk12_buflen, const char* pass)
{
    int ret = 0;
    pk12_t pk12[1] = {0};
    pk12_unpack(pk12, pk12_buf, pk12_buflen, pass);
    ret = VerifyCertFromfolder(SSL_ROOTCA_PATH, pk12->pCert);
    pk12_free(pk12);
    return ret;
}
#endif
COpenSsl& COpenSsl::Obj(void)
{
    static COpenSsl l_obj;
    return l_obj;
}

COpenSsl::COpenSsl() :
    m_ObjInit(false),
    m_Client(NULL),
    m_Srvice(NULL)
{
    DEBUG_INFO("%s" , "COpenSsl::COpenSsl!!!");
}

BOOL COpenSsl::Init(BOOL ssl)
{
    if (m_ObjInit)
    {
        return true;
    }

    m_ObjInit = true;

    if (!ssl)
    {
        return true;
    }

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    m_Client = ClientCtx();
    m_Srvice = SrviceCtx();
    DEBUG_INFO("m_Client = %X , m_Srvice = %X" , (long)m_Client , (long)m_Srvice);
    return m_Client && m_Srvice;
}

SSL_CTX* COpenSsl::ClientCtx(void)
{
    SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_client_method());

    if (ctx)
    {
        SSL_CTX_set_options(ctx, SSL_OP_ALL);

        if (!SSL_CTX_set_cipher_list(ctx , TLS1_TXT_RSA_WITH_AES_256_GCM_SHA384))
        {
            DEBUG_INFO("%s" , "error setting cipher AES256-GCM-SHA384");
        }

    }

    return ctx;
}

SSL_CTX* COpenSsl::SrviceCtx(void)
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

BOOL COpenSsl::InitSrvice(const char* pk12_buf, int pk12_buflen, const char* pass) {
  
  SSL_CTX* ctx = m_Srvice;
  int ret = 0;
  if (NULL == m_Srvice)
  {
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
SSL* COpenSsl::Bind(SOCKET sock , int type , int& pRs)
{
    DEBUG_INFO("m_Client = %X , m_Srvice = %X" , (long)m_Client , (long)m_Srvice);
    DEBUG_INFO("%s , %d , type = %d", __FILE__, __LINE__ , type);
    pRs = 0;
    SSL* ssl = NULL;

    switch (type)
    {
    case 1 :
        if (m_Srvice)
        {
            ssl = SSL_new(m_Srvice);
            SSL_set_fd(ssl, sock);

            if (SSL_accept(ssl) == -1)
            {
                ERR_print_errors_fp(stderr);
            }

            else
            {
                pRs = 1;
                DEBUG_INFO("zheng shu %s" , X509_V_OK == SSL_get_verify_result(ssl) ? "OK" : "No");
            }

        }

        break;

    case 2:
    case 3:
        if (m_Client)
        {
            int ret = 0;
            ssl = SSL_new(m_Client);
            SSL_set_fd(ssl, sock);

            if ((ret = SSL_connect(ssl)) == -1)
            {
                DEBUG_INFO("%s" , "SSL_connect failed!!!1");
                ERR_print_errors_fp(stderr);
            }

            else
            {
                pRs = 1;
                DEBUG_INFO("zheng shu %s , encryption %s " , X509_V_OK == SSL_get_verify_result(ssl) ? "OK" : "No" , SSL_get_cipher(ssl));
                //pRs = ShowCerts(ssl, type);

            }

        }

        else
        {
            DEBUG_INFO("m_Client = %X" , (long)m_Client);
        }

        break;

    default:
        pRs = 1;
        break;
    }

    if (!pRs)
    {
        Free(sock, ssl);
        ssl = NULL;
    }

    return ssl;
}

BOOL COpenSsl::Free(SOCKET sock , SSL* pssl)
{
    if (pssl)
    {
        SSL_shutdown(pssl);
        SSL_free(pssl);
        DEBUG_INFO("%s" , "COpenSsl::Free OK");
    }

    return true;
}

int32 COpenSsl::Send(SOCKET sock, SSL* pssl , const int8* data, int32 len, uint32 nSeq, const addr_in* pAddr)
{
#define OFFSET_SIZE 20

    if (NULL == data || len <= 0)
    {
        return 0;
    }

    uint32 pieces = len / PACK_SIZE;
    uint32 arith = len % PACK_SIZE;
    int32 nTotalSend = 0;
    BOOL bOK = true;
    char* msg = (char*)data;
    uint32 i = 0;

    for (; i < pieces; ++i)
    {
        SOCK_DATA_HEADER* hdr = (SOCK_DATA_HEADER*)(msg - OFFSET_SIZE);
        hdr->msg_identify = nSeq; //标识
        hdr->msg_offset = i * PACK_SIZE ; //偏移
        hdr->msg_len = PACK_SIZE; //数据长度
        hdr->msg_total = len; //数据总长度
        hdr->msg_checksum = 0; //检验和
        hdr->msg_checksum = sock_checksum((uint16*)hdr, sizeof(SOCK_DATA_HEADER));
        int32 nSend = sizeof(SOCK_DATA_HEADER) + hdr->msg_len;

        if (!Send_Imp(sock, pssl, (char*)hdr, nSend, pAddr))
        {
            bOK = false;
            break;
        }

        else
        {
            nTotalSend += PACK_SIZE;
            msg += PACK_SIZE;
        }

        SLEEP(pAddr ? MILLISEC * 8 : MILLISEC * 5);
    }

    if (!bOK)
    {
        return nTotalSend;
    }

    if (arith)
    {
        SOCK_DATA_HEADER* hdr = (SOCK_DATA_HEADER*)(msg - OFFSET_SIZE);
        hdr->msg_identify = nSeq; // 标识
        hdr->msg_offset = i * PACK_SIZE ; // 偏移
        hdr->msg_len = arith; // 数据长度
        hdr->msg_total = len; // 数据总长度
        hdr->msg_checksum = 0; // 检验和
        hdr->msg_checksum = sock_checksum((uint16*)hdr, sizeof(SOCK_DATA_HEADER));
        int32 nSend = sizeof(SOCK_DATA_HEADER) + ALIGN(hdr->msg_len, 4) ;

        if (!Send_Imp(sock, pssl, (char*)hdr, nSend, pAddr))
        {
            bOK = false;
        }

        else
        {
            nTotalSend += arith;
        }

    }

    return nTotalSend;
}

BOOL COpenSsl::Send_Imp(SOCKET sock, SSL* pssl, int8* data, int32 len, const addr_in* pAddr)
{
    int32 nTotalSends = len;
    BOOL bOK = false;
    int32 send_counts = 0;

    while (send_counts++ < MAX_SEND_COUNTS)
    {
        WRITE_TIME_OUT(sock , 300);

        if (bTimeOut)
        {
            continue;
        }

        int32 nHasSend = Send_Rel(sock, pssl, data, len , pAddr);

        if (nHasSend <= 0)
        {
            int32 nError = sock_error();

            if (WSAEINTR == nError || WSAEWOULDBLOCK == nError)
            {
                continue;
            }

            else
            {
                break;
            }

        }

        data += nHasSend;
        nTotalSends -= nHasSend;

        if (nTotalSends <= 0)
        {
            bOK = true;
            break;
        }

    }

    return bOK;
}

int32 COpenSsl::Send_Rel(SOCKET sock, SSL* pssl, int8* data, int32 len, const addr_in* pAddr)
{
    if (pAddr || NULL == pssl)
    {
        return sock_send(sock, data, len, pAddr);
    }

    else
    {
        return SSL_write(pssl, data, len);
    }

}

int32 COpenSsl::Recv(SOCKET sock, SSL* pssl , int8* data, int32 len)
{
    if (pssl)
    {
        return SSL_read(pssl, data, len);
    }

    else
    {
        return sock_recv(sock, data, len, 0);
    }

}


#endif
