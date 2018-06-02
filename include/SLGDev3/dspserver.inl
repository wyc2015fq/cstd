typedef struct
{
    BOOL m_NetInitOK;
    CSelectRecv m_recv[1];
    SOCKET m_hListenSock;
    int m_af;
    SELECT_DATA dataListen[1];
    devsock_t dev[1];
} CNetS;
CNetS svr[1] = {0};
BOOL CNetS_Init(int af)
{
    svr->m_af = af;
    CSelect_Init(svr->m_recv, 1);
    return false;
}
void CNetS_RecvNotify(void* user, const char* pbData, int nLen, int socktype)
{
    devsock_t* s = (devsock_t*)user;
    NCMD* pCmd = (NCMD*)pbData;
    //SOCK_ADDRESS l_Addr = s->m_Addr;
    //s->SetActive();//重置心跳包
    DEBUG_INFO("SOCKET=%d NCMD len=%d M=%s P2=%u", s->m_sSock, nLen, MCmdtoString(pCmd->m_MCmd), pCmd->m_Param2);
    switch (pCmd->m_MCmd)
    {
    case C_HEART:
        //回发心跳
        //dev_send(s, pbData, nLen);
        return;
    case C_VERIFY:
        return;
    case CMD_M_AUTH:
        sendNCMD(s, pCmd->m_MCmd, CMD_S_SET, pCmd->m_CFlag, 0, 0, 0);
        return;
    case CMD_M_APPUPDATE:
    {
        APPUPDATE* app = (APPUPDATE*)(pCmd + 1);
        int ret = 1;
        switch (app->m_Type)
        {
        case F_PSD_CERTIFI:
          {
            void* m_pSsl;
            char pass[sizeof(CID)] = {0};
            int len = CBase64Convert_deBase64(app->m_CtxInfo.m_ID, strlen(app->m_CtxInfo.m_ID), pass);
            pass[len] = 0;
            ret = OpenSsl_InitSrvice(app->m_Block, app->m_Bytes, pass);
            sendNCMD(s, pCmd->m_MCmd, CMD_S_SET, app->m_Type, ret, 0, 0);
            if (ret)
            {
              m_pSsl = OpenSsl_BindSrvice(s->m_sSock);
            }
            ret = m_pSsl != NULL;
            if (m_pSsl)
            {
              s->m_pSsl = m_pSsl;
            }
        }
        break;
        default:
            sendNCMD(s, pCmd->m_MCmd, CMD_S_SET, app->m_Type, ret, 0, 0);
            break;
        }
    }
    return;
    default:
        break;
    }
}
int SLGDEV_CALL CNetS_RecvCB(SOCKET m_SOCK, const addr_in* Addr, void* user, int socktype)
{
    devsock_t* s = (devsock_t*)user;
    return CmdRecv(s, m_SOCK, Addr, user, socktype, CNetS_RecvNotify);
}
int SLGDEV_CALL CNetS_RecvListenCB(SOCKET m_SOCK, const addr_in* Addr, void* user, int socktype)
{
    CNetS* s = (CNetS*)user;
    s->dev->m_sSock = m_SOCK;
    s->dev->addr = *Addr;
    CSelect_StartRecv(s->m_recv, s->dev->data, m_SOCK, 0, CNetS_RecvCB, s->dev);
    return 0;
}
BOOL CNetS_StartListen(BOOL flag)
{
    BOOL ret = FALSE;
    CNetS* s = svr;
    do
    {
        if (flag)
        {
            addr_in Addr = {0};
            net_addr(ANYIP(s->m_af), SERVER_PORT, &Addr);
            s->m_hListenSock = sock_create(s->m_af, SOCK_STREAM);
            int i = 0;
            while (i++ < BINDE_TIMES)
            {
                if (sock_bind(s->m_hListenSock, &Addr))
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
            sock_listen(s->m_hListenSock, 5);
            CSelect_StartRecv(s->m_recv, s->dataListen, s->m_hListenSock, 1, CNetS_RecvListenCB, s);
        }
        else
        {
            ret = true;
            CSelect_StopRecv(s->m_recv, s->dataListen);
            sock_close(s->m_hListenSock);
            s->m_hListenSock = 0;
        }
    }
    while (0);
    return ret;
}

