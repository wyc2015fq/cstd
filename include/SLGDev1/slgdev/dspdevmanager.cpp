
#include "../slgdev_private.h"
#include "devdatestruct.h"
#include "devsock.h"

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
  MCMDDEF( TT, CMD_M_RANGE_EX ) \
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
    snprintf(buf, 32, "%d", mcmd);
#undef MCMDDEF
    return buf;
}

static const char* SCmdtoString(int mcmd)
{
    static char buf[32] = {0};
#define MCMDDEF(TT, XX ) if (mcmd==XX) return #XX ;
    SCMDDEFDEF(MCMDDEF, 0);
    snprintf(buf, 32, "%d", mcmd);
#undef MCMDDEF
    return buf;
}

void SLGDEV_CALL DevRecvFromCallBack(SOCKET hSock, const char* pbData, int nLen, const addr_in* addr, void* pvContext, void* user)
{
    DspDevManager* pNet = (DspDevManager*)pvContext;
    pNet->RecvFromNotify(hSock, pbData, nLen, addr, user);
}

void SLGDEV_CALL DevRecvCallBack(SOCKET hSock, const char* pbData, int nLen, void* pvContext, void* user)
{
    DspDevManager* pNet = (DspDevManager*)pvContext;
    pNet->RecvNotify(hSock, pbData, nLen, user);
}

void DspDevManager::RecvNotify(SOCKET hSock, const char* pbData, int nLen, void* user)
{
    CDevSock* s = (CDevSock*)user;

    if (!s)
    {
        return ;
    }

    SOCK_ADDRESS l_Addr = s->GetAddr();
    s->SetActive();//重置心跳包

    if (pbData == NULL && nLen < (int)sizeof(NCMD))
    {
        NetDCN(s, DEV_CONNECT_CUT);
        return;
    }

    NCMD* pCmd = (NCMD*)pbData;
    DEBUG_INFO("SOCKET=%d NCMD len=%d M=%s P2=%u", hSock, nLen, MCmdtoString(pCmd->m_MCmd), pCmd->m_Param2);

    switch (pCmd->m_MCmd)
    {
    case C_CNDEV:
        if (S_CN == pCmd->m_SCmd)
        {
            if (GET_ERR(pCmd->m_CFlag) == SUC_EXECUT)
            {
                NetCN_Imp(l_Addr.IP_Address);
            }

            else
            {
                this->OnCNNotify(l_Addr.IP_Address, DEV_CONNECT_FAILUE);
            }

        }

        else if (S_DN == pCmd->m_SCmd)
        {
            NetDCN(s, DEV_CONNECT_CUT, false);
        }

        return;

    case C_HEART:
        //回发心跳
        SLG_Send(hSock, pbData, &nLen);
        return;

    case C_VERIFY:
        this->OnDevCMDNotify(s, NULL, CLI_AUTH_OPER, DEV_OPER_UPLOAD, NULL, pCmd->m_Param1, (DEV_OPER_RESULT)GET_ERR(pCmd->m_CFlag));
        return;

    default:
        break;
    }

    this->OnRecvNotify(s, pbData, nLen);
}

void DspDevManager::RecvFromNotify(SOCKET hSock, const char* pbData, int nLen, const addr_in* Addr, void* user)
{
    if (!m_bInit)
    {
        return;
    }

    BOOL bExit = false;
    CDevSock* s = (CDevSock*)user;

    if (NULL == s)
    {
        if (pbData && pbData[0] == SYN_ACK_CONNECT)
        {
            this->OnCNNotify(s->GetAddr().IP_Address, DEV_CONNECT_NONE);
        }

    }

    else
    {
        s->OnRecvFromNotify(Addr, pbData, nLen);
    }

}

BOOL DspDevManager::StartWork(BOOL bFlag)
{
    BOOL ret = false;

    if (bFlag)
    {
        if (!m_bInit && SLG_InitSock(USE_NET_MODEL , MAX_MEMRESERVER_SIZE, MEMRESERVER_UNIT))
        {
            SLG_SetDataRecvFromCallBack(::DevRecvFromCallBack, this);
            SLG_SetDataRecvCallBack(::DevRecvCallBack, this);
            m_bInit = ret = SLG_StartWork(true);
        }

    }

    else
    {
        if (m_bInit)
        {
            m_bInit = false;
            {
                //主动断开所有连接
                AoutLock lock(&m_syn);
                SOCK_MAP::iterator it = m_map.begin();

                for (; it != m_map.end(); ++it)
                {
                    SOCKET sock = it->second->GetHandle();

                    if (VSOCKET != sock)
                    {
                        SLG_ReleaseSock(sock, it->second);
                    }

                    delete it->second;
                }

                m_map.clear();
            }

            SLG_StartWork(false);
            SLG_UnInitSock();
        }

        ret = true;
    }

    return ret;
}

void DspDevManager::CallSrvSB(const char* ip, SOCK_ADDRESS* addr)
{
    assert(TransMode());
    AoutLock lock(&m_SynSrvCb);

    if (m_SrvCb)
    {
        m_SrvCb(ip, addr, m_SrvCtx);
    }

}

CDevSock* DspDevManager::FindTranSrv(const SOCK_ADDRESS& addr)
{
    assert(TransMode());
    AoutLock lock(&m_syn);
    SOCK_MAP::iterator it = m_map.begin();

    for (; it != m_map.end(); ++it)
    {
        if (it->second->GetHandle() != VSOCKET)
        {
            if (strcmp(addr.IP_Address, it->second->GetAddr().IP_Address) == 0)
            {
                return it->second;
            }

        }

    }

    return NULL;
}

BOOL DspDevManager::NetDCN(CDevSock* s, E_CONNECT_STATUS eFlag, BOOL tri)
{
    if (!m_bInit)
    {
        return false;
    }

    BOOL bExit = false;

    if (s)
    {
        AoutLock lock(&m_syn);
        SOCK_MAP::iterator it = m_map.find(s->GetAddr().IP_Address);

        if (it != m_map.end())
        {
            //对方断开连接
            SOCK_ADDRESS l_Addr = s->GetAddr();
            s->StopVideo(0);
            this->OnCNNotify(l_Addr.IP_Address, eFlag);
            m_map.erase(it);
            delete s;
            bExit = true;
        }

    }

    return bExit;
}

int DspDevManager::NetCN_Imp(const char* cszIP)
{
    std::string strip(cszIP);
    CDevSock* s = getSock(cszIP);
    int len = 0;

    if (s)
    {
        DEBUG_INFO("%s", "LINK_ALREADY\n");
        return TRUE;
    }

    BOOL srv = m_hSockSrv != 0;
    int linkCode = DEV_CONNECT_FAILUE;
    addr_in addr[1] = {0};
    SOCKET hSock = 0;
    BOOL ret = 0;
    len = net_addr(cszIP, SERVER_PORT, addr);

    if (len <= 0)
    {
        DEBUG_INFO("net_addr err:%d\n", len);
        linkCode = DEV_CONNECT_FAILUE;
    }

    else
    {
        if (srv)
        {
            CDevSock* s = NULL;
            hSock = m_hSockSrv;
            m_syn.Lock();
            m_map.insert(std::pair<std::string, CDevSock*>(strip, s = new CDevSock(hSock, addr, cszIP, SERVER_PORT, TRUE, srv)));
            m_syn.Unlock();
            ret = SLG_Recv(hSock , s);
            linkCode = ret ? DEV_CONNECT_SUCCESS : DEV_CONNECT_FAILUE;
        }

        else
        {
            hSock = SLG_CreateSock(addr->family, SOCK_STREAM);

            if (hSock != INVALID_SOCKET)
            {
                m_synLink.Lock();
                BOOL bRst = SLG_Connect(hSock, addr);
                m_synLink.Unlock();

                if (!bRst)
                {
                    DEBUG_INFO("SLG_Connect err:%d", bRst);
                    SLG_ReleaseSock(hSock, NULL);
                    hSock = 0;
                }

                else
                {
                    CDevSock* s = NULL;
                    m_syn.Lock();
                    m_map.insert(std::pair<std::string, CDevSock*>(strip, s = new CDevSock(hSock, addr, cszIP, SERVER_PORT, TRUE, srv)));
                    m_syn.Unlock();
                    SLG_SetSendBufSize(hSock, SEND_BUF);
                    SLG_SetRecvBufSize(hSock, RECV_BUF);
                    ret = SLG_Recv(hSock , s);
                    linkCode = ret ? DEV_CONNECT_SUCCESS : DEV_CONNECT_FAILUE;

                }

            }

        }

    }

    this->OnCNNotify(cszIP, (E_CONNECT_STATUS)linkCode);
    return linkCode;
}

BOOL DspDevManager::SerchBegin(const char* cszIP)
{
    addr_in addr[1] = {0};
    net_addr(cszIP, MULTYGROUP_PORT, addr);
    SOCKET hSock = 0;
    std::string l(cszIP);
    {
        m_syn.Lock();
        SOCK_MAP::iterator it = m_map.find(l);

        if (it != m_map.end())
        {
            hSock = it->second->GetHandle();
        }

        m_syn.Unlock();

        if (!hSock)
        {
            hSock = SLG_CreateSock(addr->family, SOCK_DGRAM);

            if (hSock)
            {
                addr_in addrLocal[1] = {0};

                if (SLG_GetLocalIP(addr, 0, addrLocal) && SLG_Bind(hSock, addrLocal))
                {
                    CDevSock* s = NULL;
                    m_syn.Lock();
                    m_map.insert(std::pair<std::string, CDevSock*>(l, s = new CDevSock(hSock, addrLocal, cszIP, MULTYGROUP_PORT, false, 0)));
                    m_syn.Unlock();
                    SLG_Recv(hSock , s);
                }

                else
                {
                    SLG_ReleaseSock(hSock, NULL);
                    hSock = 0;
                }

            }

        }

    }

    if (hSock)
    {
        int szData[ ALIGN(OFFSET_SIZE + 2, 4) >> 2 ] = {0};
        char* tmp = (char*)szData;
        tmp[OFFSET_SIZE] = SYN_REQUES_CONNECT;
        int nLen = 2;
        return SLG_SendTo(hSock, addr, tmp + OFFSET_SIZE, &nLen);
    }

    return false;
}

BOOL DspDevManager::SerchEnd(const char* cszIP)
{
    std::string l(cszIP);
    SOCKET hSock = 0;
    {
        AoutLock lock(&m_syn);
        SOCK_MAP::iterator it = m_map.find(l);

        if (it != m_map.end())
        {
            hSock = it->second->GetHandle();
            delete it->second;
            m_map.erase(it);
        }

    }

    if (hSock)
    {
        SLG_ReleaseSock(hSock, NULL);
    }

    return true;
}

void DspDevManager::TimeOut(int nDelay)
{
    AoutLock lock(&m_syn);
    SOCK_MAP::iterator it = m_map.begin();

    for (; it != m_map.end();)
    {
        CDevSock* s = it->second;
        ++it;

        if (s && !s->IsActive())
        {
            DEBUG_INFO("%s", "TimeOut\n");
            assert(VSOCKET != s->GetHandle());
            SLG_ReleaseSock(s->GetHandle(), s);
        }

    }

}

void DevCMDNotify(const CDevSock* s,
                  const char* cszClientAddr,
                  DEV_OPER_TYPE eType,
                  DEV_OPER_FLAG eFlag,
                  void* pvContent,
                  int nSeq,
                  DEV_OPER_RESULT rt,
                  void* pvClientData
                 )
{
    DspDevManager* pManager = (DspDevManager*)pvClientData;
    pManager->OnDevCMDNotify(s, cszClientAddr, eType, eFlag, pvContent, nSeq, rt);
}

BOOL InternalSend(const CDevSock* s, const char* pbBuf, unsigned int uLen, void* pvClientData)
{
    DspDevManager* pManager = (DspDevManager*)pvClientData;
    return s->Send(pbBuf, &uLen);
}

void DspDevManager::Run()
{
    uutime m_Inval;

    while (m_bRunning)
    {
        Sleep(2 * 1000);

        if (m_Inval.elapsed() > 6)
        {
            TimeOut();
            m_Inval.restart();
        }

    }

}

int DspDevManager::OnCNNotify(const char* sIP, E_CONNECT_STATUS eFlag, BOOL bServer)
{
    //AoutLock lock( &m_syn );
    int type = 0;

    if (m_pfnCN)
    {
        type = (*m_pfnCN)(sIP , eFlag, m_pCNContext);
    }

    return type;
}

void DspDevManager::OnDevCMDNotify(const CDevSock* s,
                                   const char* cszClientAddr,
                                   DEV_OPER_TYPE eType,
                                   DEV_OPER_FLAG eFlag,
                                   void* pvContent,
                                   int nSeq,
                                   DEV_OPER_RESULT eResult
                                  )
{
    //AoutLock lock( &m_syn );
    //DEBUG_INFO4(_T("1 %s , %d , %x , %d") , s->GetAddr().IP_Address , (int)eType , m_pOpNotifyContext , (int)eResult);
    if (m_pfnOpNotify)
    {
        m_pfnOpNotify(s->GetAddr().IP_Address, eType, eFlag, pvContent, nSeq, eResult, m_pOpNotifyContext);
    }

    //DEBUG_INFO4(_T("2 %s , %d , %x , %d") , s->GetAddr().IP_Address , (int)eType , m_pOpNotifyContext , (int)eResult);
}

void DspDevManager::OnRecvNotify(CDevSock* s, const char* cpbBuf, unsigned int uLen, BOOL bServer)
{
    if (uLen < sizeof(NCMD))
    {
        return;
    }

    NCMD* pNCMD = (NCMD*)cpbBuf;
    NCMD* pCMD = (NCMD*)cpbBuf;

    if (pNCMD->m_CFlag & F_DL_ID)
    {
        switch (pCMD->m_MCmd)
        {
        case CMD_M_USER:
        {
            DEV_DL_USER_RANGE Range = {0};

            if (0 == GET_ERR(pCMD->m_CFlag))
            {
                CARRAY* pArray = (CARRAY*)(pCMD + 1);
                Range.m_Count = pArray->m_Count;
                Range.m_pArray = (DEV_VUSER*)(pArray->m_Block);
            }

            OnDevCMDNotify(s, NULL, DEV_USER_RANGE_OPER, DEV_OPER_DOWNLOAD, &Range, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        }

        break;

        case CMD_M_VERIFY:
        case CMD_M_ALARM:
        case CMD_M_OPTLOG:
        case CMD_M_CIMAGE:
        case CMD_M_RANGE:
        {
            DEV_OPER_TYPE type = DEV_RECORD_RANGE_OPER;

            switch (pCMD->m_MCmd)
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

            if (0 == GET_ERR(pCMD->m_CFlag))
            {
                CARRAY* pArray = (CARRAY*)(pCMD + 1);
                temp.m_Count = pArray->m_Count;
                temp.m_pRange = (DEV_DL_RECORD_RANGE*)(pArray->m_Block);
            }

            OnDevCMDNotify(s, NULL, type, DEV_OPER_DOWNLOAD, &temp, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        }

        break;

        default:
            break;
        }

        return ;
    }

    switch (pNCMD->m_MCmd)
    {
    case CMD_M_AUTH:
        assert(pNCMD || CMD_S_SET == pNCMD->m_SCmd);
        OnDevCMDNotify(s, NULL, (pNCMD->m_CFlag & F_AUTH_VERIFY) ? DEV_AUTH_OPER : DEV_AUTH_SET_OPER, DEV_OPER_UPLOAD, NULL, pNCMD->m_Param1, GET_ERR(pNCMD->m_CFlag));
        break;

    case CMD_M_DEVINFO:
        OnDevCMDNotify(s, NULL, DEV_BASEINFO_OPER, DEV_OPER_DOWNLOAD, (DEV_BASEINFO*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        break;

    case CMD_M_REBOOT:
        OnDevCMDNotify(s, NULL, DEV_REBOOT_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        break;

    case CMD_M_RESETSCREEN:
        OnDevCMDNotify(s, NULL, DEV_SCREEN_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        break;

    case CMD_M_NETPRO:
        OnDevCMDNotify(s, NULL, DEV_NETPRO_OPER, DEV_OPER_DOWNLOAD, (DEV_NETPRO*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        break;

    case CMD_M_FORMAT:
        OnDevCMDNotify(s, NULL, DEV_FORMAT_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        break;

    case M_WPA_SET:
        OnDevCMDNotify(s, NULL, DEV_WAPINFO_OPER, pCMD->m_SCmd == CMD_S_GET ? DEV_OPER_DOWNLOAD : DEV_OPER_UPLOAD, pCMD->m_SCmd == CMD_S_GET ? (WAPINFO*)(pCMD + 1) : NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        break;

    case CMD_M_HEART:
    {
        s->sendNCMD(CMD_M_HEART, CMD_S_SET, 0, 0, 0, 0);
    }

    break;

    case CMD_M_OPTLOG:
    {
        DEV_LOG Log ;
        assert(pCMD);
        memset(&Log, 0x0, sizeof(DEV_LOG));

        if (0 == GET_ERR(pCMD->m_CFlag))
        {
            DSPLOG* pRecord = (DSPLOG*)(pCMD + 1);
            Log.m_LogType = DEV_LOG_TYPE(pRecord->m_LogType);
            Log.m_bRTLog = (pCMD->m_SCmd == CMD_S_REALTIME_SEND) ? TRUE : FALSE;
            memcpy(Log.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
            memcpy(Log.m_BeOptUser.m_ID, pRecord->m_BeOptUser.m_ID, sizeof(CID));
            memcpy(Log.m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
            memcpy(&(Log.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
        }

        else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag))
        {
            memcpy(Log.m_SerialID.m_ID, pCMD + 1, sizeof(CID));
        }

        OnDevCMDNotify(s, NULL, DEV_LOG_OPER, DEV_OPER_DOWNLOAD, &Log, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
    }

    break;

    case CMD_M_DOOR_STATE:
    {
        if (pCMD->m_SCmd == CMD_S_SET)
        {
            OnDevCMDNotify(s, NULL, DEV_DOOR_STATE_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        }

        else if (pCMD->m_SCmd == CMD_S_GET)
        {
            DEV_DOOR_STATE nStatus = DEV_DOOR_NOMAL;

            if (pCMD->m_CFlag & DEV_DOOR_OPEN)
            {
                nStatus = DEV_DOOR_OPEN;
            }

            else if (pCMD->m_CFlag & DEV_DOOR_CLOSE)
            {
                nStatus = DEV_DOOR_CLOSE;
            }

            else
            {
                nStatus = DEV_DOOR_NOMAL;
            }

            OnDevCMDNotify(s, NULL, DEV_DOOR_STATE_OPER, DEV_OPER_DOWNLOAD, &nStatus, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        }

    }

    break;

    case CMD_M_REMOTE_OPEN:
        OnDevCMDNotify(s, NULL, DEV_REMOTE_OPEN_DOOR, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        break;

    case CMD_M_NOPEN:
        OnDevCMDNotify(s, NULL, DEV_NOPEN_OPER, DEV_OPER_DOWNLOAD, (DEV_NOPEN*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        break;

    case CMD_M_IOCTRL:
        OnDevCMDNotify(s, NULL, DEV_IOCTRL_OPER, DEV_OPER_DOWNLOAD, (DEV_IOCTRL*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        break;

    case CMD_M_APPUPDATE:
        switch (pCMD->m_CFlag)
        {
        case F_PSD_CERTIFI:
            if (pCMD->m_Param1)
            {
                int pRs = 0;
                SSL* sl = COpenSsl::Obj().Bind(s->m_sSock , 3 , pRs);
                s->m_pSsl = sl;//COpenSsl _BindClient(s->m_sSock);
            }

            break;

        default:
            break;
        }

        OnDevCMDNotify(s, NULL, DEV_UPDATE_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        break;

    case CMD_M_STATIS:
        OnDevCMDNotify(s, NULL, DEV_STATISINFO_OPER, DEV_OPER_DOWNLOAD, (DEV_STATIS*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        break;

    case CMD_M_CIMAGE:
    {
        DEV_CHANGEIMAGE Image = {0};

        if (SUC_EXECUT == GET_ERR(pCMD->m_CFlag))
        {
            CHANGEIMAGE* pRecord = (CHANGEIMAGE*)(pCMD + 1);
            Image.m_PicBytes = pRecord->m_PicBytes;
            Image.m_bRTChangeImage = (pCMD->m_SCmd == CMD_S_REALTIME_SEND) ? TRUE : FALSE;
            memcpy(Image.m_UserID.m_ID, pRecord->m_UserID.m_ID, sizeof(CID));
            memcpy(Image.m_SerialID.m_ID, pRecord->m_SerialID.m_ID, sizeof(CID));
            memcpy(Image.m_AppendID.m_ID, pRecord->m_AppendID.m_ID, sizeof(CID));
            memcpy(Image.m_UserName.m_ID, pRecord->m_UserName.m_ID, sizeof(CID));
            memcpy(Image.m_Department.m_ID, pRecord->m_Department.m_ID, sizeof(CID));
            memcpy(Image.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
            memcpy(&(Image.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
            Image.m_PicData = pRecord->m_Block;
        }

        else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag))
        {
            memcpy(&(Image.m_SerialID), pCMD + 1, sizeof(CID));
        }

        OnDevCMDNotify(s, NULL, DEV_CHANGEIMAGE_OPER, DEV_OPER_DOWNLOAD, &Image, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
    }

    break;

    case CMD_M_DEVSET:
    {
        switch (pCMD->m_SCmd)
        {
        case CMD_S_SET:
            OnDevCMDNotify(s, NULL, DEV_WORKATT_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            break;

        case CMD_S_GET:
        {
            DEV_WORKATT att = {0};

            if (0 == GET_ERR(pCMD->m_CFlag))
            {
                DEVSET* pSet = (DEVSET*)(pCMD + 1);
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

            OnDevCMDNotify(s, NULL, DEV_WORKATT_OPER, DEV_OPER_DOWNLOAD, &att, pCMD->m_Param1, (DEV_OPER_RESULT)pCMD->m_CFlag);
        }

        break;

        default:
            break;
        }

    }

    break;

    case CMD_M_TIME:
    {
        switch (pCMD->m_SCmd)
        {
        case CMD_S_SET:
            OnDevCMDNotify(s, NULL, DEV_SYS_TIME_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            break;

        case CMD_S_GET:
            OnDevCMDNotify(s, NULL, DEV_SYS_TIME_OPER, DEV_OPER_DOWNLOAD, (DEV_DATETIME*)(pCMD + 1), pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            break;

        default:
            break;
        }

    }

    break;

    case CMD_M_DSPRIGHT:
    {
        switch (pCMD->m_SCmd)
        {
        case CMD_S_SET:
            OnDevCMDNotify(s, NULL, DEV_RIGHT_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            break;

        case CMD_S_GET:
        {
#define TG_MAX_SIZE 256
            DEV_RIGHT_ARRAY r_array = {0};
            DEV_RIGHT RG[TG_MAX_SIZE] = {0};

            if (0 == GET_ERR(pCMD->m_CFlag))
            {
                CARRAY* pArray = (CARRAY*)(pCMD + 1);
                DSPRIGHT* pRG = (DSPRIGHT*)(pArray->m_Block);
                r_array.m_nCount = pArray->m_Count > TG_MAX_SIZE ? TG_MAX_SIZE : pArray->m_Count;
                r_array.m_pRtArray = RG;
                assert(pArray->m_Count <= TG_MAX_SIZE);

                for (int i = 0; i < r_array.m_nCount; ++i)
                {
                    RG[i].m_RightID = pRG[i].m_RightID;
                    memcpy(RG[i].m_TimeGID, pRG[i].m_TimeGID, sizeof(pRG[i].m_TimeGID));
                    RG[i].m_bHolidyValid = (BOOL)(pRG[i].m_Mode & DEV_CHECK_HOLIDY);
                    RG[i].m_bActionLock = (BOOL)(pRG[i].m_Mode & DEV_ACTION_LOCK);
                    RG[i].m_bActionOutPut = (BOOL)(pRG[i].m_Mode & DEV_ACTION_OUTPUT);
                }

            }

            OnDevCMDNotify(s, NULL, DEV_RIGHT_OPER, DEV_OPER_DOWNLOAD, &r_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        }

        break;

        case CMD_S_DEL:
            OnDevCMDNotify(s, NULL, DEV_RIGHT_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            break;
        }

    }

    break;

    case CMD_M_MCAST:
    {
        switch (pCMD->m_SCmd)
        {
        case CMD_S_SET:
            OnDevCMDNotify(s, NULL, DEV_REGION_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
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
        switch (pCMD->m_SCmd)
        {
        case CMD_S_SET:
            OnDevCMDNotify(s, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            break;

        case CMD_S_GET:
        {
#define TG_MAX_SIZE 256

            if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag))
            {
                CARRAY* pArray = (CARRAY*)(pCMD + 1);
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

                OnDevCMDNotify(s, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DOWNLOAD, &t_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            }

            else
            {
                OnDevCMDNotify(s, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            }

        }

        break;

        case CMD_S_DEL:
            OnDevCMDNotify(s, NULL, DEV_TIME_GROUP_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            break;
        }

    }

    break;

    case CMD_M_VIDEO_OUT:
    {
        switch (pCMD->m_SCmd)
        {
        case CMD_S_SET:
            OnDevCMDNotify(s, NULL, DEV_VIDEO_TRANSFER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            break;

        case CMD_S_GET:
            OnDevCMDNotify(s, NULL, DEV_VIDEO_TRANSFER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            break;

        default:
            break;
        }

    }

    break;

    case CMD_M_UGROUP:
    {
        switch (pCMD->m_SCmd)
        {
        case CMD_S_SET:
            OnDevCMDNotify(s, NULL, DEV_USERGROUP_OPER, DEV_OPER_UPLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            break;

        case CMD_S_GET:
        {
#define TG_MAX_SIZE 256
            DEV_USERGROUP_ARRAY u_array = {0};
            DEV_USERGROUP UG[TG_MAX_SIZE] = {0};

            if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag))
            {
                CARRAY* pArray = (CARRAY*)(pCMD + 1);
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

            OnDevCMDNotify(s, NULL, DEV_USERGROUP_OPER, DEV_OPER_DOWNLOAD, &u_array, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        }

        break;

        case CMD_S_DEL:
            OnDevCMDNotify(s, NULL, DEV_RIGHT_OPER, DEV_OPER_DELETE, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            break;
        }

    }

    break;

    case CMD_M_VERIFY:
    {
        assert(pCMD);
        DEV_RECORD Tmp;
        memset(&Tmp, 0, sizeof(Tmp));

        if (0 == GET_ERR(pCMD->m_CFlag))
        {
            DspRec2UspRec(*(RECORD*)(pCMD + 1), Tmp, pCMD->m_SCmd == CMD_S_REALTIME_SEND);
        }

        else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag))
        {
            memcpy(Tmp.m_SerialID.m_ID, pCMD + 1, sizeof(CID));//??
        }

        OnDevCMDNotify(s, NULL, DEV_RECORD_OPER, DEV_OPER_DOWNLOAD, &Tmp, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
    }

    break;

    case CMD_M_ALARM:
    {
        DEV_RECORD Tmp ;
        memset(&Tmp, 0x0, sizeof(Tmp));

        if (0 == GET_ERR(pCMD->m_CFlag))
        {
            DspRec2UspRec(*(RECORD*)(pCMD + 1), Tmp, pCMD->m_SCmd == CMD_S_REALTIME_SEND);
        }

        else
        {
            memcpy(Tmp.m_SerialID.m_ID, pCMD + 1, sizeof(CID));
        }

        OnDevCMDNotify(s, NULL, DEV_ALARM_OPER, DEV_OPER_DOWNLOAD, &Tmp, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
    }

    break;

    case CMD_M_RANGE_EX:
    {
#define BATCH_MAX 200

        if (SUC_EXECUT == GET_ERR(pCMD->m_CFlag))
        {
            CARRAY* pArray = (CARRAY*)(pCMD + 1);

            switch (pCMD->m_SCmd)
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

                if (pCMD->m_CFlag & 0x00000800)
                {
                    pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
                }

                OnDevCMDNotify(s, NULL, DEV_RECORD_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
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

                if (pCMD->m_CFlag & 0x00000800)
                {
                    pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
                }

                OnDevCMDNotify(s, NULL, DEV_LOG_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
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
                    Image.m_bRTChangeImage = (pCMD->m_SCmd == CMD_S_REALTIME_SEND) ? TRUE : FALSE;
                    memcpy(&(Image.m_UserID), &(pRecord->m_UserID), sizeof(CID));
                    memcpy(&(Image.m_SerialID), &(pRecord->m_SerialID), sizeof(CID));
                    memcpy(Image.m_AppendID.m_ID, pRecord->m_AppendID.m_ID, sizeof(CID));
                    memcpy(Image.m_UserName.m_ID, pRecord->m_UserName.m_ID, sizeof(CID));
                    memcpy(Image.m_Department.m_ID, pRecord->m_Department.m_ID, sizeof(CID));
                    memcpy(Image.m_Admin.m_ID, pRecord->m_Admin.m_ID, sizeof(CID));
                    memcpy(&(Image.m_RecTime), &(pRecord->m_RecTime), sizeof(DATETIME));
                    pRecord = (CHANGEIMAGE*)((char*)pRecord + CGIMG_STEP(*pRecord));
                }

                if (pCMD->m_CFlag & 0x00000800)
                {
                    pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
                }

                OnDevCMDNotify(s, NULL, DEV_CIMAGE_EX_OPER, DEV_OPER_DOWNLOAD, &records, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            }

            break;

            default:
                break;
            }

        }

        else
        {
            switch (pCMD->m_SCmd)
            {
            case CMD_M_VERIFY:
            case CMD_M_ALARM:
                OnDevCMDNotify(s, NULL, DEV_RECORD_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
                break;

            case CMD_M_OPTLOG:
                OnDevCMDNotify(s, NULL, DEV_LOG_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
                break;

            case CMD_M_CIMAGE:
                OnDevCMDNotify(s, NULL, DEV_CIMAGE_EX_OPER, DEV_OPER_DOWNLOAD, NULL, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
                break;

            default:
                break;
            }

        }

    }

    break;

    case CMD_M_USER:
    {
        switch (pCMD->m_SCmd)
        {
        case CMD_S_SET:
        {
            DEV_USER user = {0};
            memcpy(user.m_UserID.m_ID, pCMD + 1, sizeof(CID));
            OnDevCMDNotify(s, NULL, DEV_USER_OPER, DEV_OPER_UPLOAD, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        }

        break;

        case CMD_S_DEL:
        {
            DEV_USER user = {0};
            memcpy(user.m_UserID.m_ID, pCMD + 1, sizeof(CID));
            OnDevCMDNotify(s, NULL, DEV_USER_OPER, DEV_OPER_DELETE, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        }

        break;

        case CMD_S_REALTIME_LOAD:
        {
            DEV_USER_REAL_LOAD mode = {0};

            if (pCMD->m_CFlag & F_DL_PIC)
            {
                mode.m_LoadFlag |= DL_DEV_PIC;
            }

            if (pCMD->m_CFlag & F_DL_FEAT)
            {
                mode.m_LoadFlag |= DL_DEV_USER_FEAT;
            }

            mode.m_ReqSource = pCMD->m_Param1;
            mode.m_bApeendID = (pCMD->m_CFlag & F_DL_APPENDID) ? TRUE : FALSE;
            memcpy(mode.m_ID.m_ID, pCMD + 1, sizeof(CID));
            OnDevCMDNotify(s, NULL, DEV_USER_RT_OPER, DEV_OPER_UPLOAD, &mode, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        }

        break;

        case CMD_S_GET:
        case CMD_S_REALTIME_SEND:
        case CMD_S_UPDATE_SEND:
        {
            DEV_USER user = {0};

            if (0 == GET_ERR(pCMD->m_CFlag))
            {
                USER* pUser = (USER*)(pCMD + 1);
                memcpy(&user, pUser, OFF_SET(USER, m_UserType));
                user.m_UserType = (DEV_USER_TYPE)pUser->m_UserType;
                user.m_FeatData =                 pUser->m_Block;     //特征数据
                user.m_PicData = pUser->m_Block + pUser->m_FeatBytes; //照片数据

                switch (pCMD->m_SCmd)
                {
                case CMD_S_REALTIME_SEND:
                    user.m_bRTUser = 1;
                    break;

                case CMD_S_UPDATE_SEND:
                    user.m_bRTUser = 2;
                    break;
                }

            }

            else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag))
            {
                memcpy(user.m_UserID.m_ID, pCMD + 1, sizeof(CID));
            }

            OnDevCMDNotify(s, NULL, DEV_USER_OPER, DEV_OPER_DOWNLOAD, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        }

        break;

        case CMD_S_REALTIME_CAP:
        {
            DEV_USER user = {0};

            if (0 == pCMD->m_Param1)
            {
                if (0 == GET_ERR(pCMD->m_CFlag))
                {
                    USER* pUser = (USER*)(pCMD + 1);
                    memcpy(&user, pUser, OFF_SET(USER, m_UserType));
                    user.m_UserType = (DEV_USER_TYPE)pUser->m_UserType;
                    user.m_FeatData =                 pUser->m_Block;     //特征数据
                    user.m_PicData = pUser->m_Block + pUser->m_FeatBytes; //照片数据
                    user.m_bRTUser = 1;
                }

            }

            OnDevCMDNotify(s, NULL, DEV_USER_REMOTE_CAP, DEV_OPER_UPLOAD, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        }

        break;

        case CMD_S_REALTIME_CAP_INFO:
        {
            DEV_RCAPINFO* pInfo = (DEV_RCAPINFO*)(pCMD + 1);
            OnDevCMDNotify(s, NULL, DEV_USER_REMOTE_RECAP, DEV_OPER_UPLOAD, pInfo, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
        }

        break;

        default:
            break;
        }

    }

    break;

    case CMD_M_USER_EX:
    {
        switch (pCMD->m_SCmd)
        {
        case CMD_S_SET:
        {
            DEV_BATCH_USER users = {0};

            if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag))
            {
                CARRAY* pArray = (CARRAY*)(pCMD + 1);
                users.m_nCount = pArray->m_Count;

                if (users.m_nCount)
                {
                    users.m_pUserArray = (DEV_USER*)CMemPool::GetInstance().GetMem(ALIGN4(sizeof(DEV_USER) * users.m_nCount));
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

            OnDevCMDNotify(s, NULL, DEV_USER_EX_OPER, DEV_OPER_UPLOAD, &users, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            CMemPool::GetInstance().FreeMem((char*)users.m_pUserArray);
        }

        break;

        case CMD_S_DEL:
        {
            DEV_BATCH_USER users;

            if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag))
            {
                CARRAY* pArray = (CARRAY*)(pCMD + 1);
                users.m_nCount = pArray->m_Count;

                if (users.m_nCount)
                {
                    users.m_pUserArray = (DEV_USER*)CMemPool::GetInstance().GetMem(ALIGN4(sizeof(DEV_USER) * users.m_nCount));
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

            OnDevCMDNotify(s, NULL, DEV_USER_EX_OPER, DEV_OPER_DELETE, &users, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));
            CMemPool::GetInstance().FreeMem((char*)users.m_pUserArray);
        }

        break;

        case CMD_S_GET:
        {
            DEV_BATCH_USER users = {0};
            CARRAY* pArray = (CARRAY*)(pCMD + 1);

            if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag))
            {
                users.m_nCount = pArray->m_Count;
                users.m_pUserArray = (DEV_USER*)CMemPool::GetInstance().GetMem(ALIGN4(sizeof(DEV_USER) * pArray->m_Count));

                if (users.m_pUserArray)
                {
                    if (pCMD->m_CFlag & USER_TEXT)
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

                if (pCMD->m_CFlag & BATCH_END)
                {
                    pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
                }

            }

            OnDevCMDNotify(s, NULL, DEV_USER_EX_OPER, DEV_OPER_DOWNLOAD, &users, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag));

            if (users.m_pUserArray)
            {
                CMemPool::GetInstance().FreeMem((char*)users.m_pUserArray);
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

BOOL DspDevManager::SetMode(BOOL srvMode, QuerySrvCB pfn, void* pvContext)
{
    AoutLock lock(&m_SynSrvCb);
    m_SrvCb = pfn;
    m_SrvCtx = pvContext;
    m_Trans = srvMode;
    return true;
}

BOOL DspDevManager::InitSys(BOOL bFlag)
{
    BOOL ret = TRUE;

    if (bFlag)
    {
        ret = StartWork(TRUE);

        if (ret)
        {
            CMemPool::GetInstance().CreateMemPool(1024 * 1024 * 2, MEMRESERVER_UNIT);
            CThread::StartThread();
        }

    }

    else
    {
        CThread::StopThread();
        StartWork(FALSE);
    }

    return ret;
}

BOOL DspDevManager::DevSerch(BOOL bFlag, const char* cszIP)
{
    if (!m_bInit)
    {
        return false;
    }

    return bFlag ? SerchBegin(cszIP) : SerchEnd(cszIP);
}

void DspDevManager::DCNDev(const char* cszIP)
{
    CDevSock* s = getSock(cszIP);

    if (s)
    {
        //NetDCN(s, DEV_CONNECT_CUT); //
        SLG_ReleaseSock(s->m_sSock, s);     // 由回调调用NetDCN
    }

}
