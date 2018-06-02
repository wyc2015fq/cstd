struct CSockInfo : public CSockBase
{
    SOCKET sock;
    BOOL iscli;
    SOCK_ADDRESS addr;
    typedef std::map<SOCKET, int> SOCK_MAP;
    SOCK_MAP m_map;
    int insert(SOCKET s, int aa)
    {
        SOCK_MAP::iterator it = m_map.find(s);
        if (m_map.end() == it)
        {
            m_map.insert(std::pair<SOCKET, int>(s, aa));
        }
    }
};
struct TranSvr : public CSockBase
{
    BOOL m_bInit;
    CriticalSection m_syn;
    TranSvr()
    {
        m_bInit = 0;
    }
    typedef std::map<std::string, CSockInfo*> SOCK_MAP;
    SOCK_MAP m_map;
    // bFlag 1=开始 0=停止
    BOOL StartWork(BOOL bFlag)
    {
        BOOL ret = false;
        if (bFlag)
        {
            if (!m_bInit && SLG_InitSock(USE_NET_MODEL , MAX_MEMRESERVER_SIZE, MEMRESERVER_UNIT))
            {
                SLG_SetDataRecvFromCallBack(TranSvr::DevRecvFromCallBack, this);
                SLG_SetDataRecvCallBack(TranSvr::DevRecvCallBack, this);
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
                        SOCKET sock = it->second->sock;
                        if (VSOCKET != sock)
                        {
                            SLG_ReleaseSock(sock, NULL);
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
    SOCKET m_hListenSock;
    void ListenCB(SOCKET hSock, const addr_in* pAddr, CSockBase* user)
    {
        if (!m_bInit)
        {
            SLG_ReleaseSock(hSock, user);
            return ;
        }
        SLG_SetSendBufSize(hSock, SEND_BUF);
        SLG_SetRecvBufSize(hSock, RECV_BUF);
        int type = 0;
        CSockInfo* si = new CSockInfo();
        memset(si, 0, sizeof(CSockInfo));
        si->sock = hSock;
        si->iscli = 1;
        SLG_Recv(hSock , si);
        char buf[256] = {0};
        net_ntop(pAddr, buf, 256);
        DEBUG_INFO("ListenCB pAddr = %s", buf);
        strcpy(si->addr.IP_Address, buf);
        m_map.insert(std::pair<std::string, CSockInfo*>(buf, si));
    }
    static void SLGDEV_CALL ListenCB(SOCKET hSock, const addr_in* pAddr, void* param, CSockBase* user)
    {
        TranSvr* pManager = (TranSvr*)param;
        pManager->ListenCB(hSock, pAddr, user);
    }
    CSockInfo* getDevSock(const char* ip)
    {
        std::string ss(ip);
        SOCK_MAP::iterator it = m_map.find(ss);
        if (it != m_map.end())
        {
            return it->second;
        }
        return 0;
    }
    CriticalSection m_synLink;
    void RecvNotify(SOCKET hSock, const char* pbData, int nLen, void* user)
    {
        //DEBUG_INFO("RecvNotify len = %d", nLen);
        AoutLock lock(&m_syn);
        CSockInfo* si = (CSockInfo*)user;
        NCMD* pCmd = (NCMD*)pbData;
        if (pbData && nLen >= sizeof(NCMD))
        {
            SOCK_ADDRESS dstaddr[1] = {0};
            if (si->iscli)
            {
                if (nLen >= (sizeof(NCMD) + sizeof(SOCK_ADDRESS)))
                {
                    *dstaddr = *(SOCK_ADDRESS*)(pbData + nLen - sizeof(SOCK_ADDRESS));
                }
                pCmd->m_Param2 = hSock;
                SOCKET hDevSock = 0;
                const char* ip = dstaddr->IP_Address;
                CSockInfo* si = getDevSock(ip);
                int ret = 0;
                if (NULL == si)
                {
                    addr_in addr[1] = {0};
                    net_addr(ip, SERVER_PORT, addr);
                    hDevSock = SLG_CreateSock(addr->family, SOCK_STREAM);
                    if (hDevSock != INVALID_SOCKET)
                    {
                        m_synLink.Lock();
                        BOOL bRst = SLG_Connect(hDevSock, addr);
                        m_synLink.Unlock();
                        if (!bRst)
                        {
                            DEBUG_INFO("SLG_Connect err:%d", bRst);
                            SLG_ReleaseSock(hDevSock, NULL);
                            hDevSock = 0;
                        }
                        else
                        {
                            std::string ss(ip);
                            si = new CSockInfo;
                            si->iscli = 0;
                            si->sock = hDevSock;
                            strcpy(si->addr.IP_Address, ip);
                            SLG_Recv(hDevSock , si);
                            m_syn.Lock();
                            m_map.insert(std::pair<std::string, CSockInfo*>(ss, si));
                            m_syn.Unlock();
                        }
                    }
                }
                if (si)
                {
                    hDevSock = si->sock;
                    ret = SLG_Send(si->sock, pbData, &nLen);
                }
                DEBUG_INFO("dev(%s)[%d], NCMD len=%d M=%s S=%s P2=%u", dstaddr->IP_Address, hDevSock, nLen, MCmdtoString(pCmd->m_MCmd), SCmdtoString(pCmd->m_SCmd), pCmd->m_Param2);
            }
            else
            {
                SOCKET hSockcli = pCmd->m_Param2;
                if (hSockcli)
                {
                    SOCK_MAP::iterator it = m_map.begin();
                    for (; it != m_map.end(); ++it)
                    {
                        if (hSockcli == it->second->sock)
                        {
                            *dstaddr = it->second->addr;
                        }
                    }
                }
                DEBUG_INFO("cli(%s)[%d], NCMD len=%d M=%s S=%s P2=%u", dstaddr->IP_Address, hSockcli, nLen, MCmdtoString(pCmd->m_MCmd), SCmdtoString(pCmd->m_SCmd), pCmd->m_Param2);
                if (CMD_M_HEART == pCmd->m_MCmd)
                {
                    pCmd->m_SCmd = CMD_S_SET;
                    SOCK_MAP::iterator it = m_map.begin();
                    for (; it != m_map.end(); ++it)
                    {
                        if (it->second->iscli)
                        {
                            SOCKET sock = it->second->sock;
                            SLG_Send(sock, pbData, &nLen);
                        }
                    }
                    //SLG_Send(hSock, pbData, &nLen);
                }
                else
                {
                    hSockcli = hSockcli ? hSockcli : hSock;
                    SLG_Send(hSockcli, pbData, &nLen);
                }
            }
        }
        else
        {
            SLG_ReleaseSock(si->sock, 0);
            SOCK_MAP::iterator it = m_map.find(si->addr.IP_Address);
            DEBUG_INFO("del %s", si->addr.IP_Address);
            if (it != m_map.end())
            {
                m_map.erase(it);
                delete si;
            }
            else
            {
                ASSERT(0);
            }
        }
        return;
    }
    void RecvFromNotify(SOCKET hSock, const char* pbData, int nLen, const addr_in* Addr, void* user)
    {
        DEBUG_INFO("RecvFromNotify len = %d", nLen);
        return ;
    }
    static void SLGDEV_CALL DevRecvFromCallBack(SOCKET hSock, const char* pbData, int nLen, const addr_in* addr, void* pvContext, void* user)
    {
        TranSvr* pNet = (TranSvr*)pvContext;
        pNet->RecvFromNotify(hSock, pbData, nLen, addr, user);
    }
    static void SLGDEV_CALL DevRecvCallBack(SOCKET hSock, const char* pbData, int nLen, void* pvContext, void* user)
    {
        TranSvr* pNet = (TranSvr*)pvContext;
        pNet->RecvNotify(hSock, pbData, nLen, user);
    }
    BOOL StartListen(BOOL flag)
    {
        if (!m_bInit)
        {
            return FALSE;
        }
        BOOL ret = FALSE;
        do
        {
            if (flag)
            {
                addr_in Addr = {0};
                int m_af = AF_INET;
                net_addr(ANYIP(m_af), SERVER_PORT, &Addr);
                m_hListenSock = SLG_CreateSock(m_af, SOCK_STREAM);
                int i = 0;
                while (i++ < BINDE_TIMES)
                {
                    if (SLG_Bind(m_hListenSock, &Addr))
                    {
                        ret = true;
                        break;
                    }
                    SLEEP(MILLISEC * 1000 * 10);
                }
                if (!ret)
                {
                    break;
                }
                SLG_SetListenCallBack(TranSvr::ListenCB, this);
                SLG_Listen(m_hListenSock, this);
            }
            else
            {
                ret = true;
                SLG_ReleaseSock(m_hListenSock, NULL);
                m_hListenSock = 0;
            }
        }
        while (0);
        return ret;
    }
};

