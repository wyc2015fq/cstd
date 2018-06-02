#include "Socket.h"
void ShowCerts(SSL* ssl)
{
  X509* cert = NULL;
  char* line = NULL;
  cert = SSL_get_peer_certificate(ssl);

  if (cert != NULL) {
    DEBUG_INFO1("%s" , "数字证书信息:\n");
    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    DEBUG_INFO1("证书: %s\n", line);
    free(line);
    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    DEBUG_INFO1("颁发者: %s\n", line);
    free(line);
    X509_free(cert);
  }
  else {
    DEBUG_INFO1("%s" , "无证书信息！\n");
  }
}
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
  DEBUG_INFO1("%s" , "COpenSsl::COpenSsl!!!");
}
BOOL COpenSsl::Init(BOOL ssl)
{
  if (m_ObjInit) {
    return true;
  }

  m_ObjInit = true;

  if (!ssl) {
    return true;
  }

  SSL_library_init();
  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();
  m_Client = ClientCtx();
  m_Srvice = SrviceCtx();
  DEBUG_INFO2("m_Client = %X , m_Srvice = %X" , (long)m_Client , (long)m_Srvice);
  return m_Client && m_Srvice;
}
SSL_CTX* COpenSsl::ClientCtx(void)
{
  SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_client_method());
  SSL_CTX_set_options(ctx, SSL_OP_ALL);

  if (!SSL_CTX_set_cipher_list(ctx , TLS1_TXT_RSA_WITH_AES_256_GCM_SHA384)) {
    DEBUG_INFO1("%s" , "error setting cipher AES256-GCM-SHA384");
  }

  return ctx;
}
SSL_CTX* COpenSsl::SrviceCtx(void)
{
  SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_server_method());

  if (!ctx) {
    ERR_print_errors_fp(stdout);
    return ctx;
  }

  if (SSL_CTX_use_certificate_file(ctx, "CAcert.pem", SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stdout);
    SSL_CTX_free(ctx);
    return NULL;
  }

  if (SSL_CTX_use_PrivateKey_file(ctx, "privkey.pem", SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stdout);
    SSL_CTX_free(ctx);
    return NULL;
  }

  if (!SSL_CTX_check_private_key(ctx)) {
    ERR_print_errors_fp(stdout);
    SSL_CTX_free(ctx);
    return NULL;
  }

  SSL_CTX_set_options(ctx, SSL_OP_ALL);

  if (!SSL_CTX_set_cipher_list(ctx , TLS1_TXT_RSA_WITH_AES_256_GCM_SHA384)) {
    DEBUG_INFO1("%s" , "error setting cipher AES256-GCM-SHA384");
  }

  return ctx;
}
SSL* COpenSsl::Bind(SOCKET sock , int type , int& pRs)
{
  DEBUG_INFO2("m_Client = %X , m_Srvice = %X" , (long)m_Client , (long)m_Srvice);
  DEBUG_INFO3("%s , %d , type = %d", __FILE__, __LINE__ , type);
  pRs = 0;
  SSL* ssl = NULL;

  switch (type) {
  case 1 :
    if (m_Srvice) {
      ssl = SSL_new(m_Srvice);
      SSL_set_fd(ssl, sock);

      if (SSL_accept(ssl) == -1) {
        ERR_print_errors_fp(stderr);
      }
      else {
        pRs = 1;
        DEBUG_INFO1("zheng shu %s" , X509_V_OK == SSL_get_verify_result(ssl) ? "OK" : "No");
      }
    }

    break;

  case 2:
    if (m_Client) {
      ssl = SSL_new(m_Client);
      SSL_set_fd(ssl, sock);

      if (SSL_connect(ssl) == -1) {
        DEBUG_INFO1("%s" , "SSL_connect failed!!!1");
        ERR_print_errors_fp(stderr);
      }
      else {
        pRs = 1;
        DEBUG_INFO2("zheng shu %s , encryption %s " , X509_V_OK == SSL_get_verify_result(ssl) ? "OK" : "No" , SSL_get_cipher(ssl));
        ShowCerts(ssl);
      }
    }
    else {
      DEBUG_INFO1("m_Client = %X" , (long)m_Client);
    }

    break;

  default:
    pRs = 1;
    break;
  }

  return ssl;
}
BOOL COpenSsl::Free(SOCKET sock , SSL* pssl)
{
  if (pssl) {
    SSL_shutdown(pssl);
    SSL_free(pssl);
    DEBUG_INFO1("%s" , "COpenSsl::Free OK");
  }

  return true;
}
int32 COpenSsl::Send(SOCKET sock, SSL* pssl , const int8* data, int32 len, uint32 nSeq, const addr_in* pAddr)
{
#define OFFSET_SIZE 20

  if (NULL == data || len <= 0) {
    return 0;
  }

  uint32 pieces = len / PACK_SIZE;
  uint32 arith = len % PACK_SIZE;
  int32 nTotalSend = 0;
  BOOL bOK = true;
  char* msg = (char*)data;
  uint32 i = 0;

  for (; i < pieces; ++i) {
    SOCK_DATA_HEADER* hdr = (SOCK_DATA_HEADER*)(msg - OFFSET_SIZE);
    hdr->msg_identify = nSeq; //标识
    hdr->msg_offset = i * PACK_SIZE ; //偏移
    hdr->msg_len = PACK_SIZE; //数据长度
    hdr->msg_total = len; //数据总长度
    hdr->msg_checksum = 0; //检验和
    hdr->msg_checksum = sock_checksum((uint16*)hdr, sizeof(SOCK_DATA_HEADER));
    int32 nSend = sizeof(SOCK_DATA_HEADER) + hdr->msg_len;

    if (!Send_Imp(sock, pssl, (char*)hdr, nSend, pAddr)) {
      bOK = false;
      break;
    }
    else {
      nTotalSend += PACK_SIZE;
      msg += PACK_SIZE;
    }

    SLEEP(pAddr ? MILLISEC * 8 : MILLISEC * 5);
  }

  if (!bOK) {
    return nTotalSend;
  }

  if (arith) {
    SOCK_DATA_HEADER* hdr = (SOCK_DATA_HEADER*)(msg - OFFSET_SIZE);
    hdr->msg_identify = nSeq; // 标识
    hdr->msg_offset = i * PACK_SIZE ; // 偏移
    hdr->msg_len = arith; // 数据长度
    hdr->msg_total = len; // 数据总长度
    hdr->msg_checksum = 0; // 检验和
    hdr->msg_checksum = sock_checksum((uint16*)hdr, sizeof(SOCK_DATA_HEADER));
    int32 nSend = sizeof(SOCK_DATA_HEADER) + ALIGN(hdr->msg_len, 4) ;

    if (!Send_Imp(sock, pssl, (char*)hdr, nSend, pAddr)) {
      bOK = false;
    }
    else {
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

  while (send_counts++ < MAX_SEND_COUNTS) {
    WRITE_TIME_OUT(sock , 300);

    if (bTimeOut) {
      continue;
    }

    int32 nHasSend = Send_Rel(sock, pssl, data, len , pAddr);

    if (nHasSend <= 0) {
      int32 nError = sock_error();

      if (WSAEINTR == nError || WSAEWOULDBLOCK == nError) {
        continue;
      }
      else {
        break;
      }
    }

    data += nHasSend;
    nTotalSends -= nHasSend;

    if (nTotalSends <= 0) {
      bOK = true;
      break;
    }
  }

  return bOK;
}
int32 COpenSsl::Send_Rel(SOCKET sock, SSL* pssl, int8* data, int32 len, const addr_in* pAddr)
{
  if (pAddr || NULL == pssl) {
    return sock_send(sock, data, len, pAddr);
  }
  else {
    return SSL_write(pssl, data, len);
  }
}
int32 COpenSsl::Recv(SOCKET sock, SSL* pssl , int8* data, int32 len)
{
  if (pssl) {
    return SSL_read(pssl, data, len);
  }
  else {
    return sock_recv(sock, data, len, 0);
  }
}

