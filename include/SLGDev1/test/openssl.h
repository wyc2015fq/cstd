#ifndef COPENSSL_H
#define COPENSSL_H

#ifdef USE_SSL
#include "openssl_lib.inl"
class COpenSsl
{
protected:
    COpenSsl();
    SSL_CTX* ClientCtx(void);
    SSL_CTX* SrviceCtx(void);
    BOOL  Send_Imp(SOCKET sock, SSL* pssl, int8* data, int32 len, const addr_in* pAddr);
    int32 Send_Rel(SOCKET sock, SSL* pssl, int8* data, int32 len, const addr_in* pAddr);
public:
    static COpenSsl& Obj(void);
public:
    BOOL  Init(BOOL ssl);
    SSL*  Bind(SOCKET sock , int type , int& pRs);
    BOOL  Free(SOCKET sock , SSL* pssl);
    int32 Send(SOCKET sock, SSL* pssl , const int8* data, int32 len, uint32 nSeq, const addr_in* pAddr);
    int32 Recv(SOCKET sock, SSL* pssl , int8* data, int32 len);
public:
    BOOL     m_ObjInit;
    BOOL     m_bVerify;
    SSL_CTX* m_Client;
    SSL_CTX* m_Srvice;
};
#include "openssl.inl"
#else
#include "Socket.h"
typedef int SSL;
class COpenSsl
{
public:
    BOOL m_bVerify;
    static COpenSsl& Obj(void)
    {
        static COpenSsl l_obj;
        return l_obj;
    }
public:
    BOOL  Init(BOOL ssl)
    {
        return TRUE;
    }
    SSL*  Bind(SOCKET sock , int type , int& pRs)
    {
        pRs = 1;
        return NULL;
    }
    BOOL  Free(SOCKET sock , SSL* pssl)
    {
        return TRUE;
    }
    int32 Send(SOCKET sock, SSL* pssl , const int8* data, int32 len, uint32 nSeq, const addr_in* pAddr)
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
    BOOL Send_Imp(SOCKET sock, SSL* pssl, int8* data, int32 len, const addr_in* pAddr)
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
    int32 Send_Rel(SOCKET sock, SSL* pssl, int8* data, int32 len, const addr_in* pAddr)
    {
        return sock_send(sock, data, len, pAddr);
    }
    int32 Recv(SOCKET sock, SSL* pssl , int8* data, int32 len)
    {
        return sock_recv(sock, data, len, 0);
    }
};
#endif
#endif // COPENSSL_H

