typedef struct dev_t_ devsock_t;
struct dev_t_
{
    devsock_t* next;
    devsock_t* prev;
    addr_in addr;
    SOCKET m_sSock;
    SOCK_ADDRESS m_Addr;
    BOOL m_bCalTimeOut;
    VideoPro* m_Pro;
    CriticalSection m_syn;
    BOOL m_srv;
    unsigned int hash;
    SELECT_DATA data[1];
    devsock_t* vsock;
    int32 m_PackHdr[(sizeof(int32) + sizeof(SOCK_DATA_HEADER)) >> 2]; //接收包头
    int msg_identify;
    int8* pRetMem;
    void* m_pSsl;
};
typedef struct devmap_t_ devmap_t;
struct devmap_t_
{
    CriticalSection m_syn;
    CriticalSection m_SynSrvCb;
    CriticalSection m_synLink;
    BOOL m_bInit;
    BOOL m_Trans;
    QuerySrvCB m_SrvCb;
    void* m_SrvCtx;
    dev_t_* dev;
    SOCKET m_hSockSrv;
    DevConnectStatus m_pfnCN;
    void* m_pCNContext;
    DevOperResultNotify m_pfnOpNotify;
    void* m_pOpNotifyContext;
    CSelectRecv m_recv[1];
};
static devmap_t g_dev[1];
unsigned int myRSHash(const char* str, unsigned int len)
{
    unsigned int a = 63689;
    unsigned int hash = 0;
    unsigned int i = 0;
    for (i = 0; i < len; str++, i++)
    {
        hash = hash * a + (*str);
        a = a * 378551;
    }
    return hash;
}
void dev_free(devsock_t* s)
{
    CriticalSection_Lock(&g_dev->m_syn);
    LIST_DEL(g_dev->dev, s);
    CriticalSection_Unlock(&g_dev->m_syn);
    if (s->vsock)
    {
        dev_free(s->vsock);
    }
    if (s->pRetMem)
    {
        free(s->pRetMem);
    }
    if (s->m_pSsl)
    {
        OpenSsl_Free(s->m_sSock, s->m_pSsl);
    }
    free(s);
}
devsock_t* dev_new(SOCKET sSOCK, addr_in* paddr, const char* ip, USHORT port, BOOL bCalTimeOut, BOOL srv)
{
    devsock_t* s = NULL;
    REALLOC_PUSH1(s, 0, 1, 0);
    memset(s, 0, sizeof(*s));
    s->hash = myRSHash(ip, strlen(ip));
    s->m_sSock = (sSOCK), s->m_bCalTimeOut = (bCalTimeOut);
    memset(&s->m_Addr, 0x0, sizeof(s->m_Addr));
    strncpy(s->m_Addr.IP_Address, ip, sizeof(s->m_Addr.IP_Address) - 1);
    s->m_Addr.Port = port;
    s->addr = *paddr;
    s->m_Pro = NULL;
    s->m_srv = srv;
    s->msg_identify = -1;
    CriticalSection_Lock(&g_dev->m_syn);
    LIST_ADDFRONT(g_dev->dev, s);
    CriticalSection_Unlock(&g_dev->m_syn);
    return s;
}
#define MCMDDEFDEF(MCMDDEF, TT) \
  MCMDDEF( TT, CMD_M_USER ) \
  MCMDDEF( TT, CMD_M_VERIFY ) \
  MCMDDEF( TT, CMD_M_ALARM ) \
  MCMDDEF( TT, CMD_M_OPTLOG ) \
  MCMDDEF( TT, CMD_M_CIMAGE ) \
  MCMDDEF( TT, CMD_M_RANGE ) \
  MCMDDEF( TT, CMD_M_AUTH ) \
  MCMDDEF( TT, CMD_M_TIME ) \
  MCMDDEF( TT, CMD_M_HEART ) \
  MCMDDEF( TT, CMD_M_DEVINFO ) \
  MCMDDEF( TT, CMD_M_STATIS ) \
  MCMDDEF( TT, CMD_M_DEVSET ) \
  MCMDDEF( TT, CMD_M_FORMAT ) \
  MCMDDEF( TT, CMD_M_DOOR_STATE ) \
  MCMDDEF( TT, CMD_M_REMOTE_OPEN ) \
  MCMDDEF( TT, CMD_M_TGROUP ) \
  MCMDDEF( TT, CMD_M_DSPRIGHT ) \
  MCMDDEF( TT, CMD_M_UGROUP ) \
  MCMDDEF( TT, CMD_M_MCAST ) \
  MCMDDEF( TT, CMD_M_APPUPDATE ) \
  MCMDDEF( TT, CMD_M_IMGCHANGE ) \
  MCMDDEF( TT, CMD_M_VIDEO_OUT ) \
  MCMDDEF( TT, CMD_M_USER_EX ) \
  MCMDDEF( TT, CMD_M_REBOOT ) \
  MCMDDEF( TT, CMD_M_RECORD_EX ) \
  MCMDDEF( TT, CMD_M_SET_IP ) \
  MCMDDEF( TT, CMD_M_NOPEN ) \
  MCMDDEF( TT, CMD_M_IOCTRL ) \
  MCMDDEF( TT, CMD_M_NETPRO ) \
  MCMDDEF( TT, CMD_M_RESETSCREEN )
#define SCMDDEFDEF(MCMDDEF, TT) \
  MCMDDEF( TT, CMD_S_SET ) \
  MCMDDEF( TT, CMD_S_DEL ) \
  MCMDDEF( TT, CMD_S_GET ) \
  MCMDDEF( TT, CMD_S_REALTIME_SEND ) \
  MCMDDEF( TT, CMD_S_REALTIME_LOAD ) \
  MCMDDEF( TT, CMD_S_REALTIME_CAP ) \
  MCMDDEF( TT, CMD_S_REALTIME_CAP_INFO ) \
  MCMDDEF( TT, CMD_S_UPDATE_SEND ) \
  MCMDDEF( TT, CMD_S_CLEAR_INOUT )
static const char* MCmdtoString(int mcmd)
{
    static char buf[32] = {0};
#define MCMDDEF(TT, XX ) if (mcmd==XX) return #XX ;
    MCMDDEFDEF(MCMDDEF, 0);
    _snprintf(buf, 32, "%d", mcmd);
#undef MCMDDEF
    return buf;
}
static const char* SCmdtoString(int mcmd)
{
    static char buf[32] = {0};
#define MCMDDEF(TT, XX ) if (mcmd==XX) return #XX ;
    SCMDDEFDEF(MCMDDEF, 0);
    _snprintf(buf, 32, "%d", mcmd);
#undef MCMDDEF
    return buf;
}
typedef struct
{
    DEV_CID m_UserName; //用户名
    DEV_CID m_Password; //密码
} DEV_AUTH;
typedef enum
{
    DL_SINGLE = 0x01,
    DL_ALL,
    DL_SEGTIME,
    DL_RANGE
} DATA_DL_TYPE;
//获得指定时间段的ID集合
typedef struct
{
    DEV_DATE m_Start; //记录起始时间
    DEV_DATE m_End;   //记录结束时间
} DEV_TIMESEG;
typedef struct
{
    DATA_DL_TYPE m_DLType;
    int m_DLFlag;
    char m_DLContent[4];
} DEV_DATA_DL;
//REG_OBJECT(CMD_M_USER_EX, DevUserEx, true);
#define OFF_SET(T, member) (size_t( &(((T*)0)->member)))
void addr_to_addrx(const addr_in* addr, unsigned int* pout)
{
    if (AF_INET == addr->family)
    {
        const struct sockaddr_in* p = (const struct sockaddr_in*)addr;
        //unsigned int aa = 0;
        //DotIpToLong1("192.168.1.150", &aa);
        *pout = p->sin_addr.s_addr;
    }
    else
    {
        *pout = 0xffffffff;
    }
}
BOOL Send_Imp(SOCKET sock, void* pssl, int8* data, int32 len, const addr_in* pAddr)
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
        int32 nHasSend = OpenSsl_Send(sock, pssl, data, len , pAddr);
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
int32 dev_send(const devsock_t* s, const void* pbData, int32 len)
{
    SOCKET sock = s->m_sSock;
    void* pssl = s->m_pSsl;
    const int8* data = (const int8*)pbData;
    const addr_in* pAddr = &s->addr;
    static int nSeq = 0;
    nSeq++;
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
        hdr->msg_offset = i * PACK_SIZE; //偏移
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
        hdr->msg_offset = i * PACK_SIZE; // 偏移
        hdr->msg_len = arith; // 数据长度
        hdr->msg_total = len; // 数据总长度
        hdr->msg_checksum = 0; // 检验和
        hdr->msg_checksum = sock_checksum((uint16*)hdr, sizeof(SOCK_DATA_HEADER));
        int32 nSend = sizeof(SOCK_DATA_HEADER) + ALIGN(hdr->msg_len, 4);
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
BOOL sendNCMD(const devsock_t* s, int m_MCmd, int m_SCmd, int m_CFlag, int m_Param1, const void* pbBuf, int uLen)
{
    const addr_in* addr = &s->addr;
    const SOCK_ADDRESS* pAddr = NULL;
    int ret, len = uLen + OFFSET_SIZE + sizeof(NCMD) + (pAddr ? sizeof(SOCK_ADDRESS) : 0);
    char* buf1 = (char*)malloc(len);
    memset(buf1, 0, len);
    NCMD* pTMP = (NCMD*)(buf1 + OFFSET_SIZE);
    pTMP->m_MCmd = m_MCmd;
    pTMP->m_SCmd = m_SCmd;
    pTMP->m_CFlag = m_CFlag;
    pTMP->m_Param1 = m_Param1;
    addr_to_addrx(addr, &pTMP->m_Param2);
    if (pbBuf)
    {
        memcpy(buf1 + OFFSET_SIZE + sizeof(NCMD), pbBuf, uLen);
    }
    if (pAddr)
    {
        *(SOCK_ADDRESS*)(buf1 + uLen + OFFSET_SIZE + sizeof(NCMD)) = *pAddr;
    }
    uLen = len - OFFSET_SIZE;
    ret = dev_send(s, buf1 + OFFSET_SIZE, uLen);
    free(buf1);
    return ret;
}
int OnCNNotify(const char* sIP, DEV_CONNECT_STATUS eFlag)
{
    int type = 0;
    if (g_dev->m_pfnCN)
    {
        type = g_dev->m_pfnCN(sIP , eFlag, g_dev->m_pCNContext);
    }
    return type;
}
void OnDevCMDNotify(devsock_t* s, const char* cszClientAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if (g_dev->m_pfnOpNotify)
    {
        g_dev->m_pfnOpNotify(s->m_Addr.IP_Address, eType, eFlag, pvContent, nSeq, eResult, g_dev->m_pOpNotifyContext);
    }
}
void DspRec2UspRec(RECORD& dsp, DEV_RECORD& usp, BOOL realTime)
{
    usp.m_RecType = DEV_REC_TYPE(dsp.m_RecType);
    usp.m_bRealTime = realTime;
    usp.m_Score = dsp.m_Score;
    usp.m_PicLen = dsp.m_PicBytes;
    usp.m_VerifyMode = dsp.m_VerifyMode;
    usp.m_ConcretSource = dsp.m_Source;
    usp.m_Source = DEV_DOOR_SOURCE;
    usp.m_PicData = dsp.m_Block;
    memcpy(usp.m_SerialID.m_ID, dsp.m_SerialID.m_ID, sizeof(CID));
    memcpy(usp.m_UserID.m_ID, dsp.m_UserID.m_ID, sizeof(CID));
    memcpy(usp.m_ChangID.m_ID, dsp.m_ChangID.m_ID, sizeof(CID));
    memcpy(usp.m_AppendID.m_ID, dsp.m_AppendID.m_ID, sizeof(CID));
    memcpy(usp.m_UserName.m_ID, dsp.m_UserName.m_ID, sizeof(CID));
    memcpy(usp.m_Department.m_ID, dsp.m_Department.m_ID, sizeof(CID));
    memcpy(&(usp.m_RecTime), &(dsp.m_RecTime), sizeof(DATETIME));
}
int CmdRecv(devsock_t* s, SOCKET m_SOCK, const addr_in* Addr, void* user, int socktype,
            void (*pRecvNotify)(void* user, const char* pbData, int nLen, int socktype))
{
    addr_in addr1[1] = {0};
    int32 nLen = 0;
    int32 nError = 0;
    int32* m_PackHdr = s->m_PackHdr;
    int32 nRecvBytes = sizeof(SOCK_DATA_HEADER) - m_PackHdr[0];
    char* pRecvHdr = (char*)(s->m_PackHdr + 1);
    SOCK_DATA_HEADER& hdr = *(SOCK_DATA_HEADER*)pRecvHdr;
    void* m_pSsl = s->m_pSsl;
    if (nRecvBytes)
    {
        nLen = OpenSsl_Recv(m_SOCK, m_pSsl, pRecvHdr + m_PackHdr[0], nRecvBytes);
        if (nLen < 0)
        {
            nError = sock_error();
            DEBUG_INFO("1---nError = %d , nLen = %d nRecvBytes = %d\r\n" , nError , nLen , nRecvBytes);
            return WSAEINTR == nError || WSAEWOULDBLOCK == nError;
        }
        else if (nLen == 0)
        {
            DEBUG_INFO("2---nError = %d , nLen = %d nRecvBytes = %d\r\n" , nError , nLen , nRecvBytes);
            return false;
        }
        m_PackHdr[0] += nLen;
        if (m_PackHdr[0] < (int)sizeof(SOCK_DATA_HEADER))
        {
            return true;
        }
        if (0 != sock_checksum((uint16*)&hdr, sizeof(SOCK_DATA_HEADER)))
        {
            DEBUG_INFO("%s", "sock_checksum err \r\n");
            return false;
        }
        else
        {
#ifndef _WIN32
            m_Timer.Restart();
#endif
        }
    }
    if (NULL == s->pRetMem || (int)hdr.msg_identify != (int)s->msg_identify)
    {
        s->msg_identify = hdr.msg_identify;
        s->pRetMem = (int8*)realloc(s->pRetMem, hdr.msg_total + sizeof(SOCK_DATA_HEADER) + sizeof(uint32) + 4);
        if (NULL != s->pRetMem)
        {
            SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*)s->pRetMem;
            memset(pHdr , 0 , sizeof(SOCK_DATA_HEADER));
            pHdr->msg_identify = hdr.msg_identify;
        }
    }
    if (s->pRetMem == NULL)
    {
        return true;
    }
    //assert((pRetMem & 3) == 0);
    SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*)s->pRetMem;
    uint32* pnInitTime = (uint32*)(pHdr + 1);
#ifdef _WIN32
    *pnInitTime = ::GetTickCount();
#else
    *pnInitTime = m_Timer.Elapsed();
#endif
    int8* pMem = s->pRetMem + sizeof(SOCK_DATA_HEADER) + sizeof(uint32);
    nLen = OpenSsl_Recv(m_SOCK, m_pSsl, pMem + hdr.msg_offset + pHdr->msg_len, ALIGN(hdr.msg_len, 4) - pHdr->msg_len);
    if (nLen < 0)
    {
        nError = sock_error();
        DEBUG_INFO("3---nError = %d , nLen = %d nRecvBytes = %d\r\n" , nError , nLen , nRecvBytes);
        return WSAEINTR == nError || WSAEWOULDBLOCK == nError;
    }
    else if (nLen == 0)
    {
        DEBUG_INFO("4---nError = %d , nLen = %d nRecvBytes = %d\r\n" , nError , nLen , nRecvBytes);
        return false;
    }
    pHdr->msg_len += nLen;
    if (pHdr->msg_len < ALIGN(hdr.msg_len, 4))
    {
        return true;//小包未接收完成
    }
    pHdr->msg_len = 0;
    pHdr->msg_total += hdr.msg_len;
    if (pHdr->msg_total == hdr.msg_total)
    {
        pRecvNotify(s, (char*)s->pRetMem + sizeof(SOCK_DATA_HEADER) + sizeof(uint32), pHdr->msg_total, socktype);
    }
    m_PackHdr[0] = 0;
    return true;
}
#include "yuv2bmp.inl"
int VideoProCB_defult(const VideoPro* pro)
{
#ifdef WIN32
    const char* winname = (const char*)(pro->m_hWnd);
    BITMAPINFO m_pBitmapInfo[1] = {0};
    RECT m_Rect;
    HWND hWnd = (HWND)(pro->m_hWnd);
    ::GetWindowRect(hWnd, &(m_Rect));
    m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFO);
    m_pBitmapInfo->bmiHeader.biPlanes = 1;
    m_pBitmapInfo->bmiHeader.biBitCount = 24;
    m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
    m_pBitmapInfo->bmiHeader.biSizeImage = 0;
    m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 11811;
    m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 11811;
    m_pBitmapInfo->bmiHeader.biClrUsed = 0;
    m_pBitmapInfo->bmiHeader.biClrImportant = 0;
    m_pBitmapInfo->bmiHeader.biHeight = pro->biHeight;
    m_pBitmapInfo->bmiHeader.biWidth = pro->biWidth;
    int lStillWidth = m_Rect.right - m_Rect.left;
    int lStillHeight = m_Rect.bottom - m_Rect.top;
    HDC hdc = ::GetDC(hWnd);
    ::SetStretchBltMode(hdc, COLORONCOLOR);
    ::StretchDIBits(hdc, 0, 0, lStillWidth, lStillHeight, 0, 0, pro->biWidth, pro->biHeight, pro->m_pRGBTRANS, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    ::ReleaseDC(hWnd, hdc);
#endif
    //printf("w=%d h=%d\n", pro->biWidth, pro->biHeight);
    return 0;
}
void ShowVideo(devsock_t* s, const char* cpbImage, int w, int h, int step, int nImageType)
{
    CriticalSection_Lock(&g_dev->m_syn);
    VideoPro* pro = s->m_Pro;
    static int inited = 0;
    if (!inited)
    {
        InitialTable();
    }
    if (NULL == pro)
    {
        return;
    }
    if (NULL == pro->m_pRGBTMP || w != pro->biWidth || h != pro->biHeight)
    {
        free(pro->m_pRGBTMP);
        free(pro->m_pRGBTRANS);
        pro->m_pRGBTMP = (BYTE*)malloc(w * h * 3);
        pro->m_pRGBTRANS = (BYTE*)malloc(w * h * 3);
    }
    if (IMAGE_UYVY == nImageType)
    {
        pro->biWidth = h;
        pro->biHeight = w;
        YUV422_C_RGB((BYTE*)cpbImage, pro->m_pRGBTMP, h, w);
        RGB_EDDY(pro->m_pRGBTMP, h, w, pro->m_pRGBTRANS);
    }
    else if (IMAGE_YUYV == nImageType)
    {
        pro->biWidth = w;
        pro->biHeight = h;
        YUYV_C_RGB((BYTE*)cpbImage, pro->m_pRGBTRANS, h, w);
    }
#if _WIN32
    if (IsWindow((HWND)pro->m_hWnd))
    {
        VideoProCB_defult(pro);
    }
#endif
    if (pro->VideoCB)
    {
        pro->VideoCB(pro);
    }
    CriticalSection_Unlock(&g_dev->m_syn);
}
int SLGDEV_CALL DspRecvFromCB(SOCKET m_SOCK, const addr_in* addr, void* user, int socktype)
{
    devsock_t* s = (devsock_t*)user;
    addr_in Addr = {0};
    int32* tmp = (int32*)malloc((sizeof(SOCK_DATA_HEADER) + PACK_SIZE));
    int32 nLen = sock_recvfrom(m_SOCK, (char*)tmp, sizeof(SOCK_DATA_HEADER) + PACK_SIZE, &Addr, 0);
    SOCK_DATA_HEADER& hdr = *(SOCK_DATA_HEADER*)tmp;
    //int rcvbuf=0, len=sizeof(int);
    //getsockopt(m_SOCK, SOL_SOCKET, SO_SNDBUF, (int8*)&rcvbuf, &len);
    //printf("this=%08x id = %d hdr.msg_len=%d rcvbuf=%d len=%d\n", this, hdr.msg_identify, hdr.msg_len, rcvbuf, len);
    if (0 != sock_checksum((uint16*)&hdr, sizeof(SOCK_DATA_HEADER)))
    {
        free(tmp);
        return 0;
    }
    if ((int)hdr.msg_identify != (int)s->msg_identify)
    {
        s->msg_identify = hdr.msg_identify;
        s->pRetMem = (int8*)realloc(s->pRetMem, hdr.msg_total + sizeof(SOCK_DATA_HEADER) + sizeof(uint32) + 4);
        if (NULL != s->pRetMem)
        {
            SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*)s->pRetMem;
            memset(pHdr , 0 , sizeof(SOCK_DATA_HEADER));
            pHdr->msg_identify = hdr.msg_identify;
        }
    }
    if (s->pRetMem == NULL)
    {
        free(tmp);
        return 0;
    }
    //assert((pRetMem & 3) == 0);
    SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*)s->pRetMem;
    pHdr->msg_total += hdr.msg_len;
    int8* pMem = s->pRetMem + sizeof(SOCK_DATA_HEADER) + sizeof(uint32);
    memcpy(pMem + hdr.msg_offset, (char*)tmp + sizeof(SOCK_DATA_HEADER), hdr.msg_len);
    uint32* pnInitTime = (uint32*)(pHdr + 1);
#ifdef WIN32
    *pnInitTime =::GetTickCount();
#else
    *pnInitTime = m_Timer.Elapsed();
#endif
    if (pHdr->msg_total == hdr.msg_total)
    {
        //CJob* pJob = CPMNew(CMemPool::GetInstance()) CDataRecvJob(m_SOCK, pRetMem, pHdr->msg_total, &Addr, m_user);
        //m_pThreadPool->SetJob(pJob);
        //CDataRecvJob_Run(m_SOCK, pRetMem, pHdr->msg_total, &Addr, m_user);
        //DEBUG_INFO2( "RecvFrom:: %d %d" , pHdr->msg_identify, pHdr->msg_total );
        //pRecvNotify(s, (char*)s->pRetMem, pHdr->msg_total, socktype);
        char* pbData = (char*)s->pRetMem + sizeof(SOCK_DATA_HEADER) + sizeof(uint32);
        NCMD* pCmd = (NCMD*)pbData;
        switch (pCmd->m_MCmd)
        {
        case CMD_M_VIDEO_OUT:
        {
            DEVIMAGE* pImage = (DEVIMAGE*)(pCmd + 1);
            ShowVideo(s, pImage->m_Block, pImage->m_W, pImage->m_H, pImage->m_Step, pImage->m_Type);
        }
        break;
        default:
            break;
        }
    }
    free(tmp);
    return 0;
}
void RecvNotify(void* user, const char* pbData, int nLen, int socktype)
{
    devsock_t* s = (devsock_t*)user;
    NCMD* pCmd = (NCMD*)pbData;
    SOCK_ADDRESS l_Addr = s->m_Addr;
    //s->SetActive();//重置心跳包
    DEBUG_INFO("SOCKET=%d NCMD len=%d M=%s P2=%u", s->m_sSock, nLen, MCmdtoString(pCmd->m_MCmd), pCmd->m_Param2);
    switch (pCmd->m_MCmd)
    {
    case C_CNDEV:
        if (S_CN == pCmd->m_SCmd)
        {
            if (GET_ERR(pCmd->m_CFlag) == SUC_EXECUT)
            {
                //NetCN_Imp(l_Addr.IP_Address);
            }
            else
            {
                OnCNNotify(l_Addr.IP_Address, DEV_CONNECT_FAILUE);
            }
        }
        else if (S_DN == pCmd->m_SCmd)
        {
            //NetDCN(s, DEV_CONNECT_CUT, false);
        }
        return;
    case CMD_M_APPUPDATE:
        switch (pCmd->m_CFlag)
        {
        case F_PSD_CERTIFI:
            if (pCmd->m_Param1)
            {
                s->m_pSsl = OpenSsl_BindClient(s->m_sSock);
            }
            break;
        default:
            break;
        }
        break;
    case C_HEART:
        //回发心跳
        dev_send(s, pbData, nLen);
        return;
    case C_VERIFY:
        OnDevCMDNotify(s, NULL, CLI_AUTH_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, (DEV_OPER_RESULT)GET_ERR(pCmd->m_CFlag));
        return;
    default:
        break;
    }
    {
        if (pCmd->m_CFlag & F_DL_ID)
        {
            switch (pCmd->m_MCmd)
            {
            case CMD_M_USER:
            {
                DEV_DL_USER_RANGE Range = {0};
                if (0 == GET_ERR(pCmd->m_CFlag))
                {
                    CARRAY* pArray = (CARRAY*)(pCmd + 1);
                    Range.m_Count = pArray->m_Count;
                    Range.m_pArray = (DEV_VUSER*)(pArray->m_Block);
                }
                OnDevCMDNotify(s, NULL, DEV_USER_RANGE_OPER, DEV_OPER_DOWNLOAD, &Range, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            }
            break;
            case CMD_M_VERIFY:
            case CMD_M_ALARM:
            case CMD_M_OPTLOG:
            case CMD_M_CIMAGE:
            case CMD_M_RANGE:
            {
                DEV_OPER_TYPE type = DEV_RECORD_RANGE_OPER;
                switch (pCmd->m_MCmd)
                {
                case CMD_M_VERIFY:
                    type = DEV_RECORD_RANGE_OPER;
                    break;
                case CMD_M_ALARM:
                    type = DEV_ALARM_RANGE_OPER;
                    break;
                case CMD_M_OPTLOG:
                    type = DEV_LOG_RANGE_OPER;
                    break;
                case CMD_M_CIMAGE:
                    type = DEV_CIMAGE_RANGE_OPER;
                    break;
                default:
                    break;
                }
                DEV_DL_RECORD_ARRAY temp = {0};
                if (0 == GET_ERR(pCmd->m_CFlag))
                {
                    CARRAY* pArray = (CARRAY*)(pCmd + 1);
                    temp.m_Count = pArray->m_Count;
                    temp.m_pRange = (DEV_DL_RECORD_RANGE*)(pArray->m_Block);
                }
                OnDevCMDNotify(s, NULL, type, DEV_OPER_DOWNLOAD, &temp, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            }
            break;
            default:
                break;
            }
            return ;
        }
        switch (pCmd->m_MCmd)
        {
        case CMD_M_AUTH:
            assert(pCmd || CMD_S_SET == pCmd->m_SCmd);
            OnDevCMDNotify(s, NULL, (pCmd->m_CFlag & F_AUTH_VERIFY) ? DEV_AUTH_OPER : DEV_AUTH_SET_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            break;
        case CMD_M_DEVINFO:
            OnDevCMDNotify(s, NULL, DEV_BASEINFO_OPER, DEV_OPER_DOWNLOAD, (DEV_BASEINFO*)(pCmd + 1), pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            break;
        case CMD_M_REBOOT:
            OnDevCMDNotify(s, NULL, DEV_REBOOT_OPER, DEV_OPER_DOWNLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            break;
        case CMD_M_RESETSCREEN:
            OnDevCMDNotify(s, NULL, DEV_SCREEN_OPER, DEV_OPER_DOWNLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            break;
        case CMD_M_NETPRO:
            OnDevCMDNotify(s, NULL, DEV_NETPRO_OPER, DEV_OPER_DOWNLOAD, (DEV_NETPRO*)(pCmd + 1), pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            break;
        case CMD_M_FORMAT:
            OnDevCMDNotify(s, NULL, DEV_FORMAT_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            break;
        case M_WPA_SET:
            OnDevCMDNotify(s, NULL, DEV_WAPINFO_OPER, pCmd->m_SCmd == CMD_S_GET ? DEV_OPER_DOWNLOAD : DEV_OPER_UPLOAD, pCmd->m_SCmd == CMD_S_GET ? (WAPINFO*)(pCmd + 1) : NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            break;
        case CMD_M_HEART:
        {
            sendNCMD(s, CMD_M_HEART, CMD_S_SET, 0, 0, 0, 0);
        }
        break;
        case CMD_M_OPTLOG:
        {
            DEV_LOG Log ;
            assert(pCmd);
            memset(&Log, 0x0, sizeof(DEV_LOG));
            if (0 == GET_ERR(pCmd->m_CFlag))
            {
                DSPLOG* pRecord = (DSPLOG*)(pCmd + 1);
                Log.m_LogType = DEV_LOG_TYPE(pRecord->m_LogType);
                Log.m_bRTLog = (pCmd->m_SCmd == CMD_S_REALTIME_SEND) ? TRUE : FALSE;
                memcpy(Log.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
                memcpy(Log.m_BeOptUser.m_ID, pRecord->m_BeOptUser.m_ID, sizeof(CID));
                memcpy(Log.m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
                memcpy(&(Log.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
            }
            else if (OPER_ERR_BUSY != GET_ERR(pCmd->m_CFlag))
            {
                memcpy(Log.m_SerialID.m_ID, pCmd + 1, sizeof(CID));
            }
            OnDevCMDNotify(s, NULL, DEV_LOG_OPER, DEV_OPER_DOWNLOAD, &Log, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
        }
        break;
        case CMD_M_DOOR_STATE:
        {
            if (pCmd->m_SCmd == CMD_S_SET)
            {
                OnDevCMDNotify(s, NULL, DEV_DOOR_STATE_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            }
            else if (pCmd->m_SCmd == CMD_S_GET)
            {
                DEV_DOOR_STATE nStatus = DEV_DOOR_NOMAL;
                if (pCmd->m_CFlag & DEV_DOOR_OPEN)
                {
                    nStatus = DEV_DOOR_OPEN;
                }
                else if (pCmd->m_CFlag & DEV_DOOR_CLOSE)
                {
                    nStatus = DEV_DOOR_CLOSE;
                }
                else
                {
                    nStatus = DEV_DOOR_NOMAL;
                }
                OnDevCMDNotify(s, NULL, DEV_DOOR_STATE_OPER, DEV_OPER_DOWNLOAD, &nStatus, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            }
        }
        break;
        case CMD_M_REMOTE_OPEN:
            OnDevCMDNotify(s, NULL, DEV_REMOTE_OPEN_DOOR, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            break;
        case CMD_M_NOPEN:
            OnDevCMDNotify(s, NULL, DEV_NOPEN_OPER, DEV_OPER_DOWNLOAD, (DEV_NOPEN*)(pCmd + 1), pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            break;
        case CMD_M_IOCTRL:
            OnDevCMDNotify(s, NULL, DEV_IOCTRL_OPER, DEV_OPER_DOWNLOAD, (DEV_IOCTRL*)(pCmd + 1), pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            break;
        case CMD_M_APPUPDATE:
            OnDevCMDNotify(s, NULL, DEV_UPDATE_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            break;
        case CMD_M_STATIS:
            OnDevCMDNotify(s, NULL, DEV_STATISINFO_OPER, DEV_OPER_DOWNLOAD, (DEV_STATIS*)(pCmd + 1), pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            break;
        case CMD_M_CIMAGE:
        {
            DEV_CHANGEIMAGE Image = {0};
            if (SUC_EXECUT == GET_ERR(pCmd->m_CFlag))
            {
                CHANGEIMAGE* pRecord = (CHANGEIMAGE*)(pCmd + 1);
                Image.m_PicBytes = pRecord->m_PicBytes;
                Image.m_bRTChangeImage = (pCmd->m_SCmd == CMD_S_REALTIME_SEND) ? TRUE : FALSE;
                memcpy(Image.m_UserID.m_ID, pRecord->m_UserID.m_ID, sizeof(CID));
                memcpy(Image.m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
                memcpy(Image.m_AppendID.m_ID, pRecord->m_AppendID.m_ID, sizeof(CID));
                memcpy(Image.m_UserName.m_ID, pRecord->m_UserName.m_ID, sizeof(CID));
                memcpy(Image.m_Department.m_ID, pRecord->m_Department.m_ID, sizeof(CID));
                memcpy(Image.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
                memcpy(&(Image.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
                Image.m_PicData = pRecord->m_Block;
            }
            else if (OPER_ERR_BUSY != GET_ERR(pCmd->m_CFlag))
            {
                memcpy(&(Image.m_SerialID), pCmd + 1, sizeof(CID));
            }
            OnDevCMDNotify(s, NULL, DEV_CHANGEIMAGE_OPER, DEV_OPER_DOWNLOAD, &Image, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
        }
        break;
        case CMD_M_DEVSET:
        {
            switch (pCmd->m_SCmd)
            {
            case CMD_S_SET:
                OnDevCMDNotify(s, NULL, DEV_WORKATT_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                break;
            case CMD_S_GET:
            {
                DEV_WORKATT att = {0};
                if (0 == GET_ERR(pCmd->m_CFlag))
                {
                    DEVSET* pSet = (DEVSET*)(pCmd + 1);
                    memcpy(att.m_TimeGID, pSet->m_DevCtx, sizeof(pSet->m_DevCtx));
                    att.m_BaseSet = pSet->m_BaseSet;
                    att.m_DoorMangetTime = pSet->m_DoorMangetTime;
                    att.m_LockTime = pSet->m_LockTime;
                    att.m_VerifyMode = pSet->m_VerifyMode;
#if 0
                    att.m_bWGOutPut = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_YES);
                    att.m_nWGType = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_34);
                    att.m_nWGOutType = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_CTX);
                    att.m_nWGOutIDType = (BOOL)(pSet->m_OutputWG & OUTPUT_WG_UID);
#else
                    if (pSet->m_OutputWG & 0xf0)
                    {
                        att.m_bWGOutPut = 1;
                        att.m_nWGType = (pSet->m_OutputWG & 0xf0) >> 4;
                    }
                    else
                    {
                        att.m_bWGOutPut = 0;
                        att.m_nWGType = 0;
                    }
                    att.m_nWGOutType = (pSet->m_OutputWG & OUTPUT_WG_CTX) == OUTPUT_WG_CTX;
                    att.m_nWGOutIDType = (pSet->m_OutputWG & OUTPUT_WG_UID) == OUTPUT_WG_UID;
#endif
                    att.m_nWGOutContent = pSet->m_OutputCtx;
                    att.m_DefaultRight = (DEV_DEFAULT_RIGHT)(pSet->m_DefaultRight);
                    memcpy(att.m_szSuperPWD.m_ID, pSet->m_SuperPassword.m_ID, sizeof(DEV_CID));
                }
                OnDevCMDNotify(s, NULL, DEV_WORKATT_OPER, DEV_OPER_DOWNLOAD, &att, pCmd->m_Param1, (DEV_OPER_RESULT)pCmd->m_CFlag);
            }
            break;
            default:
                break;
            }
        }
        break;
        case CMD_M_TIME:
        {
            switch (pCmd->m_SCmd)
            {
            case CMD_S_SET:
                OnDevCMDNotify(s, NULL, DEV_SYS_TIME_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                break;
            case CMD_S_GET:
                OnDevCMDNotify(s, NULL, DEV_SYS_TIME_OPER, DEV_OPER_DOWNLOAD, (DEV_DATETIME*)(pCmd + 1), pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                break;
            default:
                break;
            }
        }
        break;
        case CMD_M_DSPRIGHT:
        {
            switch (pCmd->m_SCmd)
            {
            case CMD_S_SET:
                OnDevCMDNotify(s, NULL, DEV_RIGHT_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                break;
            case CMD_S_GET:
            {
#define TG_MAX_SIZE 256
                DEV_RIGHT_ARRAY r_array = {0};
                DEV_RIGHT RG[TG_MAX_SIZE] = {0};
                if (0 == GET_ERR(pCmd->m_CFlag))
                {
                    CARRAY* pArray = (CARRAY*)(pCmd + 1);
                    DSPRIGHT* pRG = (DSPRIGHT*)(pArray->m_Block);
                    r_array.m_nCount = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
                    r_array.m_pRtArray = RG;
                    assert(pArray->m_Count <= TG_MAX_SIZE);
                    for (int i = 0; i < r_array.m_nCount; ++i)
                    {
                        RG[i].m_RightID = pRG[i].m_RightID;
                        memcpy(RG[i].m_TimeGID, pRG[i].m_TimeGID, sizeof(pRG[i].m_TimeGID));
                        RG[i].m_bHolidyValid = (BOOL)(pRG[i].m_Mode & CHECK_HOLIDY);
                        RG[i].m_bActionLock = (BOOL)(pRG[i].m_Mode & ACTION_LOCK);
                        RG[i].m_bActionOutPut = (BOOL)(pRG[i].m_Mode & ACTION_OUTPUT);
                    }
                }
                OnDevCMDNotify(s, NULL, DEV_RIGHT_OPER, DEV_OPER_DOWNLOAD, &r_array, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            }
            break;
            case CMD_S_DEL:
                OnDevCMDNotify(s, NULL, DEV_RIGHT_OPER, DEV_OPER_DELETE, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                break;
            }
        }
        break;
        case CMD_M_MCAST:
        {
            switch (pCmd->m_SCmd)
            {
            case CMD_S_SET:
                OnDevCMDNotify(s, NULL, DEV_REGION_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                break;
            case CMD_S_GET:
                break;
            default:
                break;
            }
        }
        break;
        case CMD_M_TGROUP:
        {
            switch (pCmd->m_SCmd)
            {
            case CMD_S_SET:
                OnDevCMDNotify(s, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                break;
            case CMD_S_GET:
            {
#define TG_MAX_SIZE 256
                if (OPER_SUCCESS == GET_ERR(pCmd->m_CFlag))
                {
                    CARRAY* pArray = (CARRAY*)(pCmd + 1);
                    assert(pArray->m_Count <= TG_MAX_SIZE);
                    DSPTIMEG* pTG = (DSPTIMEG*)(pArray->m_Block);
                    DEV_TIMEGROUP_ARRAY t_array = {0};
                    DEV_TIMEGROUP TG[TG_MAX_SIZE] = {0};
                    t_array.m_nCount = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
                    t_array.m_pTGArray = TG;
                    for (int i = 0; i < t_array.m_nCount; ++i)
                    {
                        TG[i].m_TGID = pTG[i].m_TGID;
                        memcpy(&(TG[i].m_Start), &(pTG[i].m_Start), sizeof(DATETIME));
                        memcpy(&(TG[i].m_End), &(pTG[i].m_End), sizeof(DATETIME));
                        TG[i].m_TGType = DEV_TIMEGROUP_TYPE(pTG[i].m_TGType);
                        TG[i].m_CheckFlag = pTG[i].m_CheckFlag;
                        TG[i].m_WeekFlag = pTG[i].m_WeekFlag;
                    }
                    OnDevCMDNotify(s, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DOWNLOAD, &t_array, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                }
                else
                {
                    OnDevCMDNotify(s, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DOWNLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                }
            }
            break;
            case CMD_S_DEL:
                OnDevCMDNotify(s, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DELETE, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                break;
            }
        }
        break;
        case CMD_M_VIDEO_OUT:
        {
            switch (pCmd->m_SCmd)
            {
            case CMD_S_SET:
                OnDevCMDNotify(s, NULL, DEV_VIDEO_TRANSFER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                break;
            case CMD_S_GET:
                OnDevCMDNotify(s, NULL, DEV_VIDEO_TRANSFER, DEV_OPER_DOWNLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                break;
            default:
                break;
            }
        }
        break;
        case CMD_M_UGROUP:
        {
            switch (pCmd->m_SCmd)
            {
            case CMD_S_SET:
                OnDevCMDNotify(s, NULL, DEV_USERGROUP_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                break;
            case CMD_S_GET:
            {
#define TG_MAX_SIZE 256
                DEV_USERGROUP_ARRAY u_array = {0};
                DEV_USERGROUP UG[TG_MAX_SIZE] = {0};
                if (OPER_SUCCESS == GET_ERR(pCmd->m_CFlag))
                {
                    CARRAY* pArray = (CARRAY*)(pCmd + 1);
                    assert(pArray->m_Count <= TG_MAX_SIZE);
                    USERGROUP* pUG = (USERGROUP*)(pArray->m_Block);
                    u_array.m_nCount = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
                    u_array.m_pUGArray = UG;
                    for (int i = 0; i < u_array.m_nCount; ++i)
                    {
                        UG[i].m_GroupID = pUG[i].m_GroupID;
                        UG[i].m_bGroupOrder = pUG[i].m_GroupFlag ;
                        UG[i].m_NormalValid = pUG[i].m_NormalValid;
                        UG[i].m_ForceValid = pUG[i].m_ForceValid;
                        memcpy(UG[i].m_NormalUsers, pUG[i].m_NormalUsers, sizeof(pUG[i].m_NormalUsers));
                        memcpy(UG[i].m_ForceUsers, pUG[i].m_ForceUsers, sizeof(pUG[i].m_ForceUsers));
                    }
                }
                OnDevCMDNotify(s, NULL, DEV_USERGROUP_OPER, DEV_OPER_DOWNLOAD, &u_array, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            }
            break;
            case CMD_S_DEL:
                OnDevCMDNotify(s, NULL, DEV_RIGHT_OPER, DEV_OPER_DELETE, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                break;
            }
        }
        break;
        case CMD_M_VERIFY:
        {
            assert(pCmd);
            DEV_RECORD Tmp;
            memset(&Tmp, 0, sizeof(Tmp));
            if (0 == GET_ERR(pCmd->m_CFlag))
            {
                DspRec2UspRec(*(RECORD*)(pCmd + 1), Tmp, pCmd->m_SCmd == CMD_S_REALTIME_SEND);
            }
            else if (OPER_ERR_BUSY != GET_ERR(pCmd->m_CFlag))
            {
                memcpy(Tmp.m_SerialID.m_ID, pCmd + 1, sizeof(CID));//??
            }
            OnDevCMDNotify(s, NULL, DEV_RECORD_OPER, DEV_OPER_DOWNLOAD, &Tmp, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
        }
        break;
        case CMD_M_ALARM:
        {
            DEV_RECORD Tmp ;
            memset(&Tmp, 0x0, sizeof(Tmp));
            if (0 == GET_ERR(pCmd->m_CFlag))
            {
                DspRec2UspRec(*(RECORD*)(pCmd + 1), Tmp, pCmd->m_SCmd == CMD_S_REALTIME_SEND);
            }
            else
            {
                memcpy(Tmp.m_SerialID.m_ID, pCmd + 1, sizeof(CID));
            }
            OnDevCMDNotify(s, NULL, DEV_ALARM_OPER, DEV_OPER_DOWNLOAD, &Tmp, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
        }
        break;
        case CMD_M_RECORD_EX:
        {
#define BATCH_MAX 200
            if (SUC_EXECUT == GET_ERR(pCmd->m_CFlag))
            {
                CARRAY* pArray = (CARRAY*)(pCmd + 1);
                switch (pCmd->m_SCmd)
                {
                case CMD_M_VERIFY:
                case CMD_M_ALARM:
                {
                    RECORD* pData = (RECORD*)(pArray->m_Block);
                    DEV_BATCH_RECORD records = {0};
                    DEV_RECORD record[BATCH_MAX];
                    records.m_nCount = pArray->m_Count;
                    records.m_pRecordArray = record;
                    for (int i = 0; i < pArray->m_Count; ++i)
                    {
                        DspRec2UspRec(*pData, record[i], false);
                        pData = (RECORD*)((char*)pData + RECORD_STEP(*pData));
                    }
                    if (pCmd->m_CFlag & 0x00000800)
                    {
                        pCmd->m_CFlag = OPER_BATCH_DL_COMPLETE;
                    }
                    OnDevCMDNotify(s, NULL, DEV_RECORD_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                }
                break;
                case CMD_M_OPTLOG:
                {
                    DSPLOG* pRecord = (DSPLOG*)(pArray->m_Block);
                    DEV_BATCH_LOG records = {0};
                    DEV_LOG record[BATCH_MAX];
                    records.m_nCount = pArray->m_Count;
                    records.m_pLogArray = record;
                    for (int i = 0; i < pArray->m_Count; ++i)
                    {
                        DEV_LOG& Log = record[i];
                        Log.m_LogType = DEV_LOG_TYPE(pRecord->m_LogType);
                        Log.m_bRTLog = FALSE;
                        memcpy(Log.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
                        memcpy(Log.m_BeOptUser.m_ID, pRecord->m_BeOptUser.m_ID, sizeof(CID));
                        memcpy(Log.m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
                        memcpy(&(Log.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
                        pRecord = (DSPLOG*)((char*)pRecord + DSPLOG_STEP(*pRecord));
                    }
                    if (pCmd->m_CFlag & 0x00000800)
                    {
                        pCmd->m_CFlag = OPER_BATCH_DL_COMPLETE;
                    }
                    OnDevCMDNotify(s, NULL, DEV_LOG_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                }
                break;
                case CMD_M_CIMAGE:
                {
                    CHANGEIMAGE* pRecord = (CHANGEIMAGE*)(pArray->m_Block);
                    DEV_BATCH_CIMAGE records = {0};
                    DEV_CHANGEIMAGE record[BATCH_MAX];
                    records.m_nCount = pArray->m_Count;
                    records.m_pCImageArray = record;
                    for (int i = 0; i < pArray->m_Count; ++i)
                    {
                        DEV_CHANGEIMAGE& Image = record[i];
                        Image.m_PicBytes = pRecord->m_PicBytes;
                        Image.m_PicData = pRecord->m_Block;
                        Image.m_bRTChangeImage = (pCmd->m_SCmd == CMD_S_REALTIME_SEND) ? TRUE : FALSE;
                        memcpy(&(Image.m_UserID), &(pRecord->m_UserID), sizeof(CID));
                        memcpy(&(Image.m_SerialID), &(pRecord->m_SerialID), sizeof(CID));
                        memcpy(Image.m_AppendID.m_ID, pRecord->m_AppendID.m_ID, sizeof(CID));
                        memcpy(Image.m_UserName.m_ID, pRecord->m_UserName.m_ID, sizeof(CID));
                        memcpy(Image.m_Department.m_ID, pRecord->m_Department.m_ID, sizeof(CID));
                        memcpy(Image.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
                        memcpy(&(Image.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
                        pRecord = (CHANGEIMAGE*)((char*)pRecord + CGIMG_STEP(*pRecord));
                    }
                    if (pCmd->m_CFlag & 0x00000800)
                    {
                        pCmd->m_CFlag = OPER_BATCH_DL_COMPLETE;
                    }
                    OnDevCMDNotify(s, NULL, DEV_CIMAGE_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                }
                break;
                default:
                    break;
                }
            }
            else
            {
                switch (pCmd->m_SCmd)
                {
                case CMD_M_VERIFY:
                case CMD_M_ALARM:
                    OnDevCMDNotify(s, NULL, DEV_RECORD_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                    break;
                case CMD_M_OPTLOG:
                    OnDevCMDNotify(s, NULL, DEV_LOG_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                    break;
                case CMD_M_CIMAGE:
                    OnDevCMDNotify(s, NULL, DEV_CIMAGE_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                    break;
                default:
                    break;
                }
            }
        }
        break;
        case CMD_M_USER:
        {
            switch (pCmd->m_SCmd)
            {
            case CMD_S_SET:
            {
                DEV_USER user = {0};
                memcpy(user.m_UserID.m_ID, pCmd + 1, sizeof(CID));
                OnDevCMDNotify(s, NULL, DEV_USER_OPER, DEV_OPER_UPLOAD, &user, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            }
            break;
            case CMD_S_DEL:
            {
                DEV_USER user = {0};
                memcpy(user.m_UserID.m_ID, pCmd + 1, sizeof(CID));
                OnDevCMDNotify(s, NULL, DEV_USER_OPER, DEV_OPER_DELETE, &user, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            }
            break;
            case CMD_S_REALTIME_LOAD:
            {
                DEV_USER_REAL_LOAD mode = {0};
                if (pCmd->m_CFlag & F_DL_PIC)
                {
                    mode.m_LoadFlag |= DL_DEV_PIC;
                }
                if (pCmd->m_CFlag & F_DL_FEAT)
                {
                    mode.m_LoadFlag |= DL_DEV_USER_FEAT;
                }
                mode.m_ReqSource = pCmd->m_Param1;
                mode.m_bApeendID = (pCmd->m_CFlag & F_DL_APPENDID) ? TRUE : FALSE;
                memcpy(mode.m_ID.m_ID, pCmd + 1, sizeof(CID));
                OnDevCMDNotify(s, NULL, DEV_USER_RT_OPER, DEV_OPER_UPLOAD, &mode, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            }
            break;
            case CMD_S_GET:
            case CMD_S_REALTIME_SEND:
            case CMD_S_UPDATE_SEND:
            {
                DEV_USER user = {0};
                if (0 == GET_ERR(pCmd->m_CFlag))
                {
                    USER* pUser = (USER*)(pCmd + 1);
                    memcpy(&user, pUser, OFF_SET(USER, m_UserType));
                    user.m_UserType = (DEV_USER_TYPE)pUser->m_UserType;
                    user.m_FeatData =                 pUser->m_Block;     //特征数据
                    user.m_PicData = pUser->m_Block + pUser->m_FeatBytes; //照片数据
                    switch (pCmd->m_SCmd)
                    {
                    case CMD_S_REALTIME_SEND:
                        user.m_bRTUser = 1;
                        break;
                    case CMD_S_UPDATE_SEND:
                        user.m_bRTUser = 2;
                        break;
                    }
                }
                else if (OPER_ERR_BUSY != GET_ERR(pCmd->m_CFlag))
                {
                    memcpy(user.m_UserID.m_ID, pCmd + 1, sizeof(CID));
                }
                OnDevCMDNotify(s, NULL, DEV_USER_OPER, DEV_OPER_DOWNLOAD, &user, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            }
            break;
            case CMD_S_REALTIME_CAP:
            {
                DEV_USER user = {0};
                if (0 == pCmd->m_Param1)
                {
                    if (0 == GET_ERR(pCmd->m_CFlag))
                    {
                        USER* pUser = (USER*)(pCmd + 1);
                        memcpy(&user, pUser, OFF_SET(USER, m_UserType));
                        user.m_UserType = (DEV_USER_TYPE)pUser->m_UserType;
                        user.m_FeatData =                 pUser->m_Block;     //特征数据
                        user.m_PicData = pUser->m_Block + pUser->m_FeatBytes; //照片数据
                        user.m_bRTUser = 1;
                    }
                }
                OnDevCMDNotify(s, NULL, DEV_USER_REMOTE_CAP, DEV_OPER_UPLOAD, &user, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            }
            break;
            case CMD_S_REALTIME_CAP_INFO:
            {
                DEV_RCAPINFO* pInfo = (DEV_RCAPINFO*)(pCmd + 1);
                OnDevCMDNotify(s, NULL, DEV_USER_REMOTE_RECAP, DEV_OPER_UPLOAD, pInfo, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
            }
            break;
            default:
                break;
            }
        }
        break;
        case CMD_M_USER_EX:
        {
            switch (pCmd->m_SCmd)
            {
            case CMD_S_SET:
            {
                DEV_BATCH_USER users = {0};
                if (OPER_ERR_BUSY != GET_ERR(pCmd->m_CFlag))
                {
                    CARRAY* pArray = (CARRAY*)(pCmd + 1);
                    users.m_nCount = pArray->m_Count;
                    if (users.m_nCount)
                    {
                        users.m_pUserArray = (DEV_USER*)malloc(ALIGN4(sizeof(DEV_USER) * users.m_nCount));
                        CID* pID = (CID*)(pArray->m_Block);
                        DEV_USER* pUser = users.m_pUserArray;
                        for (int i = 0; i < pArray->m_Count; ++i)
                        {
                            memset(pUser , 0, sizeof(DEV_USER));
                            strncpy(pUser->m_UserID.m_ID , pID->m_ID , sizeof(CID) - 1);
                            ++pUser;
                            ++pID;
                        }
                    }
                }
                OnDevCMDNotify(s, NULL, DEV_USER_EX_OPER, DEV_OPER_UPLOAD, &users, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                free((char*)users.m_pUserArray);
            }
            break;
            case CMD_S_DEL:
            {
                DEV_BATCH_USER users;
                if (OPER_ERR_BUSY != GET_ERR(pCmd->m_CFlag))
                {
                    CARRAY* pArray = (CARRAY*)(pCmd + 1);
                    users.m_nCount = pArray->m_Count;
                    if (users.m_nCount)
                    {
                        users.m_pUserArray = (DEV_USER*)malloc(ALIGN4(sizeof(DEV_USER) * users.m_nCount));
                        CID* pID = (CID*)(pArray->m_Block);
                        DEV_USER* pUser = users.m_pUserArray;
                        for (int i = 0; i < pArray->m_Count; ++i)
                        {
                            memset(pUser , 0, sizeof(DEV_USER));
                            strncpy(pUser->m_UserID.m_ID , pID->m_ID , sizeof(CID) - 1);
                            ++pUser;
                            ++pID;
                        }
                    }
                }
                OnDevCMDNotify(s, NULL, DEV_USER_EX_OPER, DEV_OPER_DELETE, &users, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                free((char*)users.m_pUserArray);
            }
            break;
            case CMD_S_GET:
            {
                DEV_BATCH_USER users = {0};
                CARRAY* pArray = (CARRAY*)(pCmd + 1);
                if (OPER_SUCCESS == GET_ERR(pCmd->m_CFlag))
                {
                    users.m_nCount = pArray->m_Count;
                    users.m_pUserArray = (DEV_USER*)malloc(ALIGN4(sizeof(DEV_USER) * pArray->m_Count));
                    if (users.m_pUserArray)
                    {
                        if (pCmd->m_CFlag & USER_TEXT)
                        {
                            USER* pData = (USER*)(pArray->m_Block);
                            DEV_USER* pCurUser = users.m_pUserArray;
                            for (int i = 0; i < pArray->m_Count; ++i)
                            {
                                memcpy(pCurUser, pData, OFF_SET(USER , m_UserType));
                                pCurUser->m_UserType = (DEV_USER_TYPE)pData->m_UserType;
                                pCurUser->m_FeatData =                 pData->m_Block;     //特征数据
                                pCurUser->m_PicData = pData->m_Block + pData->m_FeatBytes; //照片数据
                                pData = (USER*)((char*)pData + USER_STEP(*pData));
                                ++pCurUser;
                            }
                        }
                        else
                        {
                            assert(false);
                        }
                    }
                    if (pCmd->m_CFlag & BATCH_END)
                    {
                        pCmd->m_CFlag = OPER_BATCH_DL_COMPLETE;
                    }
                }
                OnDevCMDNotify(s, NULL, DEV_USER_EX_OPER, DEV_OPER_DOWNLOAD, &users, pCmd->m_Param1, GET_ERR(pCmd->m_CFlag));
                if (users.m_pUserArray)
                {
                    free((char*)users.m_pUserArray);
                }
            }
            break;
            }
        }
        break;
        default:
            break;
        }
    }
}
BOOL DevUserGroup_set(const devsock_t* s, const DEV_USERGROUP_ARRAY& cArray, DEV_OPER_FLAG eFlag, int nSeq)
{
    int ret, nLen = sizeof(CARRAY) + sizeof(USERGROUP) * cArray.m_nCount;
    char* pbBuf = (char*)malloc(nLen);
    assert(pbBuf);
    int m_SCmd = 0;
    switch (eFlag)
    {
    case DEV_OPER_UPLOAD:
        m_SCmd = CMD_S_SET;
        break;
    case DEV_OPER_DOWNLOAD:
        m_SCmd = CMD_S_GET;
        break;
    case DEV_OPER_DELETE:
        m_SCmd = CMD_S_DEL;
        break;
    }
    CARRAY* pArray = (CARRAY*)(pbBuf);
    pArray->m_Count = cArray.m_nCount;
    USERGROUP* pUG = (USERGROUP*)(pArray->m_Block);
    DEV_USERGROUP* pTmp = cArray.m_pUGArray;
    for (int i = 0; i < pArray->m_Count; ++i)
    {
        pUG->m_GroupID = pTmp->m_GroupID;
        pUG->m_GroupFlag = pTmp->m_bGroupOrder ;
        pUG->m_NormalValid = pTmp->m_NormalValid;
        pUG->m_ForceValid = pTmp->m_ForceValid;
        memcpy(pUG->m_NormalUsers, pTmp->m_NormalUsers, sizeof(pUG->m_NormalUsers));
        memcpy(pUG->m_ForceUsers, pTmp->m_ForceUsers, sizeof(pUG->m_ForceUsers));
        pUG++;
        pTmp++;
    }
    ret = sendNCMD(s, CMD_M_UGROUP, m_SCmd, 0, nSeq, pbBuf, nLen);
    free(pbBuf);
    return ret;
}
static BOOL DevTimeGroup_set(const devsock_t* s, const DEV_TIMEGROUP_ARRAY& cArray, int nType, DEV_OPER_FLAG eFlag, int nSeq)
{
    int ret, nLen = sizeof(CARRAY) + sizeof(DSPTIMEG) * (cArray.m_nCount);
    char* pbBuf = (char*)malloc(nLen);
    int m_SCmd = 0, m_CFlag = 0;
    switch (eFlag)
    {
    case DEV_OPER_UPLOAD:
        m_SCmd = CMD_S_SET;
        break;
    case DEV_OPER_DOWNLOAD:
        m_SCmd = CMD_S_GET;
        break;
    case DEV_OPER_DELETE:
        m_SCmd = CMD_S_DEL;
        break;
    }
    switch (nType)
    {
    case DEV_NORMAL:
        m_CFlag = F_DL_NORMAL_TG;
        break;
    case DEV_HOLIDY:
        m_CFlag = F_DL_HOLIDAY_TG;
        break;
    case DEV_ANY_TIME:
        m_CFlag = F_DL_NORMAL_TG | F_DL_HOLIDAY_TG;
        break;
    default:
        break;
    }
    CARRAY* pArray = (CARRAY*)(pbBuf);
    pArray->m_Count = cArray.m_nCount;
    DSPTIMEG* pTG = (DSPTIMEG*)(pArray->m_Block);
    for (int i = 0; i < pArray->m_Count; ++i)
    {
        pTG[i].m_TGID = (cArray.m_pTGArray)[i].m_TGID;
        memcpy(&(pTG[i].m_Start), &((cArray.m_pTGArray)[i].m_Start), sizeof(DATETIME));
        memcpy(&(pTG[i].m_End), &((cArray.m_pTGArray)[i].m_End), sizeof(DATETIME));
        pTG[i].m_TGType = (cArray.m_pTGArray)[i].m_TGType;
        pTG[i].m_CheckFlag = (cArray.m_pTGArray)[i].m_CheckFlag;
        pTG[i].m_WeekFlag = (cArray.m_pTGArray)[i].m_WeekFlag;
    }
    ret = sendNCMD(s, CMD_M_TGROUP, m_SCmd, m_CFlag, nSeq, pbBuf, nLen);
    free(pbBuf);
    return ret;
}
static BOOL DevRight_set(const devsock_t* s, const DEV_RIGHT_ARRAY& cArray, DEV_OPER_FLAG eFlag, int nSeq)
{
    int ret = 0, nLen = sizeof(CARRAY) + sizeof(DSPRIGHT) * (cArray.m_nCount);
    char* pbBuf = (char*)malloc(nLen);
    assert(pbBuf);
    int m_SCmd = 0;
    switch (eFlag)
    {
    case DEV_OPER_UPLOAD:
        m_SCmd = CMD_S_SET;
        break;
    case DEV_OPER_DOWNLOAD:
        m_SCmd = CMD_S_GET;
        break;
    case DEV_OPER_DELETE:
        m_SCmd = CMD_S_DEL;
        break;
    }
    CARRAY* pArray = (CARRAY*)(pbBuf);
    DSPRIGHT* pRG = (DSPRIGHT*)(pArray->m_Block);
    pArray->m_Count = cArray.m_nCount;
    for (int i = 0; i < pArray->m_Count; ++i)
    {
        pRG[i].m_RightID = (cArray.m_pRtArray)[i].m_RightID;
        memcpy(pRG[i].m_TimeGID, (cArray.m_pRtArray)[i].m_TimeGID, sizeof(pRG[i].m_TimeGID));
        pRG[i].m_Mode = cArray.m_pRtArray[i].m_bHolidyValid ? CHECK_HOLIDY : 0;
        pRG[i].m_Mode |= cArray.m_pRtArray[i].m_bActionLock ? ACTION_LOCK : 0;
        pRG[i].m_Mode |= cArray.m_pRtArray[i].m_bActionOutPut ? ACTION_OUTPUT : 0;
    }
    ret = sendNCMD(s, CMD_M_DSPRIGHT, m_SCmd, 0, nSeq, pbBuf, nLen);
    free(pbBuf);
    return ret;
}
static BOOL DevID_set(const devsock_t* s, int nClass, int nSeq) //下载所有
{
    return sendNCMD(s, nClass, CMD_S_GET, F_DL_ID | F_DL_ID_ALL, nSeq, 0, 0);
}
static BOOL DevID_set(const devsock_t* s, int nClass, const DEV_TIMESEG& cTimeSeg, int nSeq) //时间段下载
{
    return sendNCMD(s, nClass, CMD_S_GET, F_DL_ID | F_ID_TIME_RANGE, nSeq, &cTimeSeg, sizeof(TIMESE));
}
static BOOL DevID_set(const devsock_t* s, int nClass, int nFlag, const DEV_DL_RECORD_RANGE& cRange, int nSeq) //区间段下载
{
    int m_CFlag = (nFlag & 0x80000000) ? 0 : (F_DL_ID | F_ID_RANGE);
    if (nFlag & DL_DEV_PIC)
    {
        m_CFlag |= F_DL_PIC;
    }
    return sendNCMD(s, CMD_M_RANGE, CMD_S_GET, m_CFlag, nSeq, &cRange, sizeof(RECORDRANGE));
}
static BOOL CopyDevUser2DspUser(USER* dspUser , const DEV_USER* devUser)
{
    memset(dspUser , 0, sizeof(dspUser));
    memcpy(dspUser, devUser, OFF_SET(USER, m_UserType));
    dspUser->m_UserType = devUser->m_UserType;
    if (devUser->m_FeatLen)
    {
        if ((devUser->m_UserFlag & DEV_USER_FEAT_BASE64) && devUser->m_FeatData[3])
        {
            dspUser->m_FeatBytes = CBase64Convert_deBase64(devUser->m_FeatData , devUser->m_FeatLen , dspUser->m_Block);
        }
        else
        {
            memcpy(dspUser->m_Block, devUser->m_FeatData, devUser->m_FeatLen);
        }
    }
    if (devUser->m_PicLen)
    {
        if (devUser->m_UserFlag & DEV_USER_IMAGE_BASE64)
        {
            dspUser->m_PicBytes = CBase64Convert_deBase64(devUser->m_PicData , devUser->m_PicLen , dspUser->m_Block + dspUser->m_FeatBytes);
        }
        else
        {
            memcpy(dspUser->m_Block + dspUser->m_FeatBytes , devUser->m_PicData, devUser->m_PicLen);
        }
    }
    return 0;
}
static BOOL DevUser_set(const devsock_t* s, const CID& cID, int nFlag, int nSeq) //获取
{
    int m_CFlag = 0;
    if (nFlag & DL_DEV_PIC)
    {
        m_CFlag |= F_DL_PIC;
    }
    if (nFlag & DL_DEV_USER_FEAT)
    {
        m_CFlag |= F_DL_FEAT;
    }
    return sendNCMD(s, CMD_M_USER, CMD_S_GET, m_CFlag, nSeq, &cID, sizeof(cID));
}
static BOOL DevUser_set(const devsock_t* s, const CID& cID, int nSeq) //用户删除操作
{
    return sendNCMD(s, CMD_M_USER, CMD_S_DEL, 0, nSeq, &cID, sizeof(cID));
}
static BOOL DevUser_set(const devsock_t* s, const DEV_USER& cUser, BOOL bAddFlag, int nSeq) //用户添加操作
{
    int ret = 0, nLen = sizeof(USER) + (cUser).m_FeatLen + (cUser).m_PicLen + 20;
    char* pbBuf = (char*)malloc(nLen);
    USER* pUser = (USER*)(pbBuf);
    CopyDevUser2DspUser(pUser , &cUser);
    nLen = USER_STEP(*pUser);
    ret = sendNCMD(s, CMD_M_USER, bAddFlag ? CMD_S_SET : CMD_S_REALTIME_CAP, 0, nSeq, pbBuf, nLen);
    free(pbBuf);
    return ret;
}
static BOOL DevUser_set(const devsock_t* s, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad) //用户实时添加操作
{
    int ret = 0, nLen = sizeof(USER) + (cUser).m_FeatLen + (cUser).m_PicLen + 20;
    char* pbBuf = (char*)malloc(nLen);
    int m_CFlag = 0;
    if (cLoad.m_bApeendID)
    {
        m_CFlag |= F_DL_APPENDID;
    }
    if (cLoad.m_LoadFlag & DL_DEV_PIC)
    {
        m_CFlag |= F_DL_PIC;
    }
    if (cLoad.m_LoadFlag & DL_DEV_USER_FEAT)
    {
        m_CFlag |= F_DL_FEAT;
    }
    USER* pUser = (USER*)(pbBuf);
    CopyDevUser2DspUser(pUser , &cUser);
    nLen = USER_STEP(*pUser);
    ret = sendNCMD(s, CMD_M_USER, CMD_S_REALTIME_LOAD, m_CFlag, cLoad.m_ReqSource, pbBuf, nLen);
    free(pbBuf);
    return ret;
}
// 用户批量操作
static BOOL DevUserEx_UserAdd(const devsock_t* s, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq)
{
    int ret = 0, nLen = 0;
    if (nFlag & DL_DEV_USER_TEXT)
    {
        int binLen = sizeof(CARRAY) , i = 0;
        for (; i < cUsers.m_nCount; ++i)
        {
            binLen += ALIGN4(sizeof(USER) + cUsers.m_pUserArray[i].m_PicLen + cUsers.m_pUserArray[i].m_FeatLen);
        }
        char* pbBuf = NULL;
        pbBuf = (char*)malloc(binLen);
        CARRAY* pArray = (CARRAY*)(pbBuf);
        pArray->m_Count = cUsers.m_nCount;
        USER* pData = (USER*)(pArray->m_Block);
        DEV_USER* pCurUser = cUsers.m_pUserArray;
        for (i = 0; i < cUsers.m_nCount; ++i)
        {
            CopyDevUser2DspUser(pData , &pCurUser[i]);
            nLen += USER_STEP(*pData);
            pData = (USER*)((char*)pData + USER_STEP(*pData));
        }
        ret = sendNCMD(s, CMD_M_USER_EX, CMD_S_SET, USER_TEXT, nSeq, pbBuf, binLen);
        free(pbBuf);
    }
    else
    {
        assert(false);
    }
    return ret;
}
// 用户批量删除操作
static BOOL DevUserEx_UserDel(const devsock_t* s, DEV_CID* pUserIDArray, int nIDCount, int nSeq)
{
    int ret = 0, nLen = sizeof(CARRAY) + sizeof(CID) * nIDCount;
    char* pbBuf = (char*)malloc(nLen);
    CARRAY* pArray = (CARRAY*)(pbBuf);
    CID* pID = (CID*)(pArray->m_Block);
    pArray->m_Count = nIDCount;
    DEV_CID* pIDTmp = pUserIDArray;
    for (int i = 0; i < nIDCount; ++i)
    {
        memset(pID , 0, sizeof(CID));
        strncpy(pID->m_ID , pIDTmp->m_ID , sizeof(CID) - 1);
        ++pID;
        ++pIDTmp;
    }
    ret = sendNCMD(s, CMD_M_USER_EX, CMD_S_DEL, 0, nSeq, pbBuf, nLen);
    free(pbBuf);
    return 0;
}
// 用户批量下载操作
static BOOL DevUserEx_UserDown(const devsock_t* s, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq)
{
    int ret = 0, nLen = sizeof(CARRAY) + sizeof(CID) * nIDCount;
    char* pbBuf = (char*)malloc(nLen);
    int m_CFlag = 0;
    if (nFlag & DL_DEV_USER_TEXT)
    {
        m_CFlag |= USER_TEXT;
    }
    if (nFlag & DL_DEV_USER_FEAT)
    {
        m_CFlag |= DL_FEAT;
    }
    if (nFlag & DL_DEV_PIC)
    {
        m_CFlag |= DL_IMAGE;
    }
    CARRAY* pArray = (CARRAY*)(pbBuf);
    CID* pID = (CID*)(pArray->m_Block);
    pArray->m_Count = nIDCount;
    DEV_CID* pIDTmp = pUserIDArray;
    for (int i = 0; i < nIDCount; ++i)
    {
        ZeroMemory(pID , sizeof(CID));
        strncpy(pID->m_ID , pIDTmp->m_ID , sizeof(CID) - 1);
        ++pID;
        ++pIDTmp;
    }
    ret = sendNCMD(s, CMD_M_USER_EX, CMD_S_GET, m_CFlag, nSeq, pbBuf, nLen);
    free(pbBuf);
    return ret;
}
static BOOL DevCImage_set(const devsock_t* s, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq)
{
    int m_CFlag = 0;
    if (nFlag & DL_DEV_PIC)
    {
        m_CFlag |= F_DL_PIC;
    }
    return sendNCMD(s, CMD_M_CIMAGE, CMD_S_GET, m_CFlag, nSeq, &cRange, sizeof(DEV_DL_RECORD_RANGE));
}
BOOL GetDevUser(devsock_t* s, const DEV_DATA_DL& cDlData, int nSeq)
{
    switch (cDlData.m_DLType)
    {
    case DL_SINGLE :
    {
        return DevUser_set(s, *(CID*)(cDlData.m_DLContent), cDlData.m_DLFlag, nSeq);
    }
    break;
    case DL_ALL:
    {
        return DevID_set(s, CMD_M_USER, nSeq);
    }
    break;
    case DL_SEGTIME:
    {
        return DevID_set(s, CMD_M_USER, *(DEV_TIMESEG*)(cDlData.m_DLContent), nSeq);
    }
    break;
    default:
        break;
    }
    return 0;
}
BOOL GetDevRecord(devsock_t* s, int nClass, const DEV_DATA_DL& cDlData, int nSeq)
{
    switch (cDlData.m_DLType)
    {
    case DL_SINGLE:
        return DevID_set(s, nClass, cDlData.m_DLFlag | 0x80000000, *(DEV_DL_RECORD_RANGE*)(cDlData.m_DLContent), nSeq);
        break;
    case DL_RANGE:
        return DevID_set(s, nClass, 0 , *(DEV_DL_RECORD_RANGE*)(cDlData.m_DLContent), nSeq);
        break;
    case DL_ALL:
        return DevID_set(s, nClass, nSeq);
        break;
    case DL_SEGTIME:
        return DevID_set(s, nClass, *(DEV_TIMESEG*)(cDlData.m_DLContent), nSeq);
        break;
    default:
        break;
    }
    return 0;
}

