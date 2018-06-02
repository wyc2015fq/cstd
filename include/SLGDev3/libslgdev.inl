#include "slgdev.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma comment( lib, "mswsock.lib")
#ifdef _DEBUG
struct slgdev_config_t
{
    int m_debug;
} g_cfg = {1};
#else
struct slgdev_config_t
{
    int m_debug;
} g_cfg = {0};
#endif
#include <assert.h>
#define DEBUG_INFO printf
#define WIN32_LEAN_AND_MEAN
#include "debugassistant.inl"
#include "sock.inl"
#include "openssl.inl"
#include "dspdatestruct.h"
#include "devsock.inl"
#define GETSOCK1(ip)   devsock_t* s = getSock(ip);
#define GETSOCK(ip)   devsock_t* s = getSock(ip); if (NULL==s) {return FALSE;}
devsock_t* getSock(const char* ip)
{
    devsock_t* dev = g_dev->dev;
    unsigned int hash = myRSHash(ip, strlen(ip));
    while (dev)
    {
        if (hash == dev->hash && 0 == strcmp(ip, dev->m_Addr.IP_Address))
        {
            return dev;
        }
    }
    return NULL;
}
void devsock_close(devsock_t* s)
{
    if (s)
    {
        OnCNNotify(s->m_Addr.IP_Address, DEV_CONNECT_CUT);
        CSelect_StopRecv(g_dev->m_recv, s->data);
        sock_close(s->m_sSock);
        dev_free(s);
    }
}
int SLGDEV_CALL DspRecvCB(SOCKET m_SOCK, const addr_in* Addr, void* user, int socktype)
{
    devsock_t* s = (devsock_t*)user;
    int ret = CmdRecv(s, m_SOCK, Addr, user, socktype, RecvNotify);
    if (!ret)
    {
        devsock_close(s);
    }
    return ret;
}
// CriticalSection m_syn;
BOOL SLGDEV_CALL SLG_InitSys(BOOL bFlag)
{
    if (bFlag)
    {
        memset(g_dev, 0, sizeof(*g_dev));
        g_dev->m_bInit = 1;
        CriticalSection_Init(&g_dev->m_syn);
        CriticalSection_Init(&g_dev->m_SynSrvCb);
        CriticalSection_Init(&g_dev->m_synLink);
    }
    else
    {
        CriticalSection_Free(&g_dev->m_syn);
        CriticalSection_Free(&g_dev->m_SynSrvCb);
        CriticalSection_Free(&g_dev->m_synLink);
        memset(g_dev, 0, sizeof(*g_dev));
    }
    CSelect_Init(g_dev->m_recv, bFlag);
    return TRUE;
}
void SLGDEV_CALL SLG_RegDevConnectStatusCB(DevConnectStatus pfnCNNotify, void* pvContext)
{
    g_dev->m_pfnCN = pfnCNNotify;
    g_dev->m_pCNContext = pvContext;
    return;
}
void SLGDEV_CALL SLG_RegOperResultNotifyCB(DevOperResultNotify pfnOperNotify, void* pvContext)
{
    g_dev->m_pfnOpNotify = pfnOperNotify;
    g_dev->m_pOpNotifyContext = pvContext;
    return;
}
BOOL SLGDEV_CALL SLG_SetMode(BOOL srvMode, QuerySrvCB pfn, void* pvContext)
{
    CriticalSection_Lock(&g_dev->m_SynSrvCb);
    g_dev->m_SrvCb = pfn;
    g_dev->m_SrvCtx = pvContext;
    g_dev->m_Trans = srvMode;
    CriticalSection_Unlock(&g_dev->m_SynSrvCb);
    return TRUE;
}
BOOL SLGDEV_CALL SLG_CnSrv(BOOL bFlag, const char* ip, const char* name, const char* psw)
{
    addr_in addr[1] = {0};
    SOCKET hSock = 0;
    int linkCode = DEV_CONNECT_FAILUE;
    int len = net_addr(ip, SERVER_PORT, addr);
    if (len <= 0)
    {
        DEBUG_INFO("net_addr err:%d\n", len);
        linkCode = DEV_CONNECT_FAILUE;
    }
    else
    {
        hSock = sock_create(addr->family, SOCK_STREAM);
        if (hSock != INVALID_SOCKET)
        {
            CriticalSection_Lock(&g_dev->m_synLink);
            BOOL bRst = sock_connect(hSock, addr);
            CriticalSection_Unlock(&g_dev->m_synLink);
            if (!bRst)
            {
                DEBUG_INFO("SLG_Connect err:%d", bRst);
                sock_close(hSock);
                hSock = 0;
            }
            else
            {
                g_dev->m_hSockSrv = hSock;
            }
        }
        else
        {
            hSock = 0;
        }
    }
    return TRUE;
}
BOOL SLGDEV_CALL SLG_StartDevSerch(BOOL bFlag, const char* cszRgnAddr)
{
    //assert(0);
    return TRUE;
}
BOOL SLGDEV_CALL SLG_CNDev(const char* ip)
{
    GETSOCK1(ip);
    int len = 0;
    if (s)
    {
        DEBUG_INFO("%s", "LINK_ALREADY\n");
        return TRUE;
    }
    BOOL srv = g_dev->m_hSockSrv != 0;
    DEV_CONNECT_STATUS linkCode = DEV_CONNECT_FAILUE;
    addr_in addr[1] = {0};
    SOCKET hSock = 0;
    BOOL ret = 0;
    len = net_addr(ip, SERVER_PORT, addr);
    if (len <= 0)
    {
        DEBUG_INFO("net_addr err:%d\n", len);
        linkCode = DEV_CONNECT_FAILUE;
    }
    else
    {
        if (srv)
        {
            devsock_t* s = NULL;
            hSock = g_dev->m_hSockSrv;
            s = dev_new(hSock, addr, ip, SERVER_PORT, TRUE, srv);
            linkCode = DEV_CONNECT_SUCCESS;
        }
        else
        {
            hSock = sock_create(addr->family, SOCK_STREAM);
            if (hSock != INVALID_SOCKET)
            {
                CriticalSection_Lock(&g_dev->m_synLink);
                BOOL bRst = sock_connect(hSock, addr);
                CriticalSection_Unlock(&g_dev->m_synLink);
                if (!bRst)
                {
                    DEBUG_INFO("SLG_Connect err:%d", bRst);
                    sock_close(hSock);
                    hSock = 0;
                }
                else
                {
                    devsock_t* s = NULL;
                    s = dev_new(hSock, addr, ip, SERVER_PORT, TRUE, srv);
                    sock_set_sendbuf_size(hSock, SEND_BUF);
                    sock_set_recvbuf_size(hSock, RECV_BUF);
                    CSelect_StartRecv(g_dev->m_recv, s->data, hSock, 0, DspRecvCB, s);
                    linkCode = DEV_CONNECT_SUCCESS;
                }
            }
        }
    }
    OnCNNotify(ip, linkCode);
    return TRUE;
}
void SLGDEV_CALL SLG_DCNDev(const char* ip)
{
    GETSOCK1(ip);
    devsock_close(s);
}
BOOL SLGDEV_CALL SLG_ULDevAuth(const char* ip, const DEV_CID& cName, const DEV_CID& cPws, int nFlag, int nSeq)
{
    GETSOCK(ip);
    DEV_AUTH cAuth = {0};
    cAuth.m_UserName = cName;
    cAuth.m_Password = cPws;
    return sendNCMD(s, CMD_M_AUTH, CMD_S_SET, (nFlag == 2) ? 0 : F_AUTH_VERIFY, nSeq, &cAuth, sizeof(DEV_AUTH));
}
BOOL SLGDEV_CALL SLG_ULDevRegionAddr(const char* ip, const char* cszRgnAddr, int nSeq)
{
    GETSOCK(ip);
    CID id[1] = {0};
    memcpy(id->m_ID, cszRgnAddr, strlen(cszRgnAddr));
    return sendNCMD(s, CMD_M_MCAST, CMD_S_SET, 0, nSeq, id, sizeof(CID));
}
BOOL SLGDEV_CALL SLG_ULDevTime(const char* ip, const DEV_DATETIME& cSysTime, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_TIME, CMD_S_SET, 0, nSeq, &cSysTime, sizeof(DATETIME));
}
BOOL SLGDEV_CALL SLG_DLDevTime(const char* ip, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_TIME, CMD_S_GET, 0, nSeq, 0, 0);
}
BOOL SLGDEV_CALL SLG_DLDevBaseInfo(const char* ip, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_DEVINFO, CMD_S_GET, 0, nSeq, 0, 0);
}
BOOL SLGDEV_CALL SLG_DLDevStatisInfo(const char* ip, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_STATIS, CMD_S_GET, 0, nSeq, 0, 0);
}
BOOL SLGDEV_CALL SLG_DLDevWorkAttInfo(const char* ip, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_DEVSET, CMD_S_GET, 0, nSeq, 0, 0);
}
BOOL SLGDEV_CALL SLG_ULDevWorkAttInfo(const char* ip, const DEV_WORKATT& cAtt, int nSeq)
{
    GETSOCK(ip);
    int ret;
    DEVSET pSet[1] = {0};
    memcpy(pSet->m_DevCtx, cAtt.m_TimeGID, sizeof(cAtt.m_TimeGID));
    pSet->m_DefaultRight = cAtt.m_DefaultRight;
    pSet->m_BaseSet = cAtt.m_BaseSet;
    memcpy(pSet->m_SuperPassword.m_ID, cAtt.m_szSuperPWD.m_ID, sizeof(CID));
    pSet->m_DoorMangetTime = cAtt.m_DoorMangetTime;
    pSet->m_LockTime = cAtt.m_LockTime;
    pSet->m_VerifyMode = cAtt.m_VerifyMode;
    //pSet->m_OutputWG = cAtt.m_bWGOutPut ? OUTPUT_WG_YES : 0x00;
    //pSet->m_OutputWG |= cAtt.m_nWGType ? OUTPUT_WG_34 : 0x00;
    if (cAtt.m_bWGOutPut)
    {
        pSet->m_OutputWG |= (pSet->m_OutputWG & 0x0f) | ((cAtt.m_nWGType & 0x0f) << 4);
    }
    else
    {
        pSet->m_OutputWG |= (pSet->m_OutputWG & 0x0f);
    }
    pSet->m_OutputWG |= cAtt.m_nWGOutType ? OUTPUT_WG_CTX : 0x00;
    pSet->m_OutputWG |= cAtt.m_nWGOutIDType ? OUTPUT_WG_UID : 0x00;
    pSet->m_OutputCtx = cAtt.m_nWGOutContent;
    ret = sendNCMD(s, CMD_M_DEVSET, CMD_S_SET, 0, nSeq, pSet, sizeof(DEVSET));
    return ret;
}
BOOL SLGDEV_CALL SLG_ULUser(const char* ip, const DEV_USER& cUser, int nSeq)
{
    GETSOCK(ip);
    return DevUser_set(s, cUser, TRUE, nSeq);
}
BOOL SLGDEV_CALL SLG_ULRealTimeUser(const char* ip, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad)
{
    GETSOCK(ip);
    return DevUser_set(s, cUser, cLoad);
}
BOOL SLGDEV_CALL SLG_ULRealTimeUserCap(const char* ip, const DEV_USER& cUser, int nSeq)
{
    GETSOCK(ip);
    return DevUser_set(s, cUser, FALSE, nSeq);
}
BOOL SLGDEV_CALL SLG_DELUser(const char* ip, const DEV_CID& cID, int nSeq)
{
    GETSOCK(ip);
    return DevUser_set(s, (CID&)cID, nSeq);
}
BOOL SLGDEV_CALL SLG_DLSingleUser(const char* ip, const DEV_CID& cID, int nFlag, int nSeq)
{
    GETSOCK(ip);
    int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_CID)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLType = DL_SINGLE;
    dl.m_DLFlag = nFlag;
    memcpy(dl.m_DLContent, &cID, sizeof(DEV_CID));
    return GetDevUser(s, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_DLAllUser(const char* ip, int nSeq)
{
    GETSOCK(ip);
    DEV_DATA_DL dl;
    memset(&dl, 0x0, sizeof(dl));
    dl.m_DLType = DL_ALL;
    return GetDevUser(s, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_DLSegTimeUser(const char* ip, const DEV_DATE& cStart, const DEV_DATE& cEnd, int nSeq)
{
    GETSOCK(ip);
    int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_TIMESEG)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLType = DL_SEGTIME;
    DEV_TIMESEG seg = {0};
    seg.m_Start = cStart;
    seg.m_End = cEnd;
    memcpy(dl.m_DLContent, &seg, sizeof(DEV_TIMESEG));
    return GetDevUser(s, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_DLAllIdentifyRecord(const char* ip, int nSeq)
{
    GETSOCK(ip);
    DEV_DATA_DL dl;
    memset(&dl, 0x0, sizeof(dl));
    dl.m_DLType = DL_ALL;
    return GetDevRecord(s, CMD_M_VERIFY, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_DLSegTimeIdentifyRecord(const char* ip, const DEV_DATE& cStart, const DEV_DATE& cEnd, int nSeq)
{
    GETSOCK(ip);
    int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_TIMESEG)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLType = DL_SEGTIME;
    dl.m_DLFlag = 0;
    DEV_TIMESEG seg = {0};
    seg.m_Start = cStart;
    seg.m_End = cEnd;
    memcpy(dl.m_DLContent, &seg, sizeof(DEV_TIMESEG));
    return GetDevRecord(s, CMD_M_VERIFY, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_DLRangeRec(const char* ip, const DEV_DL_RECORD_RANGE& cRange, int nFlag, BOOL bKnowRange, int nSeq)
{
    GETSOCK(ip);
    int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLFlag = nFlag;
    dl.m_DLType = bKnowRange ? DL_SINGLE : DL_RANGE;
    memcpy(dl.m_DLContent, &cRange, sizeof(DEV_DL_RECORD_RANGE));
    return GetDevRecord(s, CMD_M_VERIFY, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_DLAllAlarmRecord(const char* ip, int nSeq)
{
    GETSOCK(ip);
    DEV_DATA_DL dl ;
    memset(&dl, 0x0, sizeof(dl)) ;
    dl.m_DLType = DL_ALL;
    dl.m_DLFlag = 0;
    return GetDevRecord(s, CMD_M_ALARM, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_DLSegTimeAlarmRecord(const char* ip, const DEV_DATE& cStart, const DEV_DATE& cEnd, int nSeq)
{
    GETSOCK(ip);
    char szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_TIMESEG)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLType = DL_SEGTIME;
    dl.m_DLFlag = 0;
    DEV_TIMESEG seg = {0};
    seg.m_Start = cStart;
    seg.m_End = cEnd;
    memcpy(dl.m_DLContent, &seg, sizeof(DEV_TIMESEG));
    return GetDevRecord(s, CMD_M_ALARM, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_DLRangeAlarm(const char* ip, const DEV_DL_RECORD_RANGE& cRange, int nFlag, BOOL bKnowRange, int nSeq)
{
    GETSOCK(ip);
    char szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLFlag = nFlag;
    dl.m_DLType = bKnowRange ? DL_SINGLE : DL_RANGE;
    memcpy(dl.m_DLContent, &cRange, sizeof(DEV_DL_RECORD_RANGE));
    return GetDevRecord(s, CMD_M_ALARM, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_DLLog(const char* ip, int nSeq)
{
    GETSOCK(ip);
    int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLType = DL_ALL;
    return GetDevRecord(s, CMD_M_OPTLOG, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_DLRangeLog(const char* ip, const DEV_DL_RECORD_RANGE& cRange, BOOL bKnowRange, int nSeq)
{
    GETSOCK(ip);
    int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLFlag = 0;
    dl.m_DLType = bKnowRange ? DL_SINGLE : DL_RANGE;
    memcpy(dl.m_DLContent, &cRange, sizeof(DEV_DL_RECORD_RANGE));
    return GetDevRecord(s, CMD_M_OPTLOG, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_DLChangeImage(const char* ip, int nSeq)
{
    GETSOCK(ip);
    char szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLType = DL_ALL;
    return GetDevRecord(s, CMD_M_CIMAGE, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_DLRangeCImage(const char* ip, const DEV_DL_RECORD_RANGE& cRange, int nFlag, BOOL bKnowRange, int nSeq)
{
    GETSOCK(ip);
    char szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLFlag = nFlag;
    dl.m_DLType = bKnowRange ? DL_SINGLE : DL_RANGE;
    memcpy(dl.m_DLContent, &cRange, sizeof(DEV_DL_RECORD_RANGE));
    return GetDevRecord(s, CMD_M_CIMAGE, dl, nSeq);
}
BOOL SLGDEV_CALL SLG_ULTimeGroup(const char* ip, const DEV_TIMEGROUP_ARRAY& cTGArray, int nSeq)
{
    GETSOCK(ip);
    return DevTimeGroup_set(s, cTGArray, DEV_ANY_TIME, DEV_OPER_UPLOAD, nSeq);
}
BOOL SLGDEV_CALL SLG_DLTimeGroup(const char* ip, const DEV_TIMEGROUP_ARRAY& cTGArray, DEV_TIMEGROUP_TYPE eType, int nSeq)
{
    GETSOCK(ip);
    return DevTimeGroup_set(s, cTGArray, eType, DEV_OPER_DOWNLOAD, nSeq);
}
BOOL SLGDEV_CALL SLG_DELTimeGroup(const char* ip, const DEV_TIMEGROUP_ARRAY& cTGArray, DEV_TIMEGROUP_TYPE eType, int nSeq)
{
    GETSOCK(ip);
    return DevTimeGroup_set(s, cTGArray, eType, DEV_OPER_DELETE, nSeq);
}
BOOL SLGDEV_CALL SLG_ULRight(const char* ip, const DEV_RIGHT_ARRAY& cRTArray, int nSeq)
{
    GETSOCK(ip);
    return DevRight_set(s, cRTArray, DEV_OPER_UPLOAD, nSeq);
}
BOOL SLGDEV_CALL SLG_DLRight(const char* ip, const DEV_RIGHT_ARRAY& cRTArray, int nSeq)
{
    GETSOCK(ip);
    return DevRight_set(s, cRTArray, DEV_OPER_DOWNLOAD, nSeq);
}
BOOL SLGDEV_CALL SLG_DELRight(const char* ip, const DEV_RIGHT_ARRAY& cRTArray, int nSeq)
{
    GETSOCK(ip);
    return DevRight_set(s, cRTArray, DEV_OPER_DELETE, nSeq);
}
BOOL SLGDEV_CALL SLG_ULUserGroup(const char* ip, const DEV_USERGROUP_ARRAY& cUGrray, int nSeq)
{
    GETSOCK(ip);
    return DevUserGroup_set(s, cUGrray, DEV_OPER_UPLOAD, nSeq);
}
BOOL SLGDEV_CALL SLG_DLUserGroup(const char* ip, const DEV_USERGROUP_ARRAY& cUGrray, int nSeq)
{
    GETSOCK(ip);
    return DevUserGroup_set(s, cUGrray, DEV_OPER_DOWNLOAD, nSeq);
}
BOOL SLGDEV_CALL SLG_DELUserGroup(const char* ip, const DEV_USERGROUP_ARRAY& cUGrray, int nSeq)
{
    GETSOCK(ip);
    return DevUserGroup_set(s, cUGrray, DEV_OPER_DELETE, nSeq);
}
BOOL SLGDEV_CALL SLG_ULOpenDoorState(const char* ip, DEV_DOOR_STATE eState, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_DOOR_STATE, CMD_S_SET, eState, nSeq, 0, 0);
}
BOOL SLGDEV_CALL SLG_DLOpenDoorState(const char* ip, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_DOOR_STATE, CMD_S_GET, 0, nSeq, 0, 0);
}
BOOL SLGDEV_CALL SLG_ULRemoteOpenDoor(const char* ip, BOOL bOpen, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_REMOTE_OPEN, CMD_S_SET, bOpen ? F_DOOR_OPEN : F_DOOR_CLOSE, nSeq, 0, 0);
}
BOOL SLGDEV_CALL SLG_IOCtrl(const char* ip, const DEV_IOCTRL& ioctrl, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_IOCTRL, CMD_S_SET, 0, nSeq, &ioctrl, sizeof(DEV_IOCTRL));
}
BOOL SLGDEV_CALL SLG_ULFormat(const char* ip, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_FORMAT, CMD_S_SET, 0, nSeq, 0, 0);
}
BOOL SLGDEV_CALL SLG_ULUpdate(const char* ip, const DEV_APPUPDATE& cData, int nSeq)
{
    GETSOCK(ip);
    int ret = 1, nLen = sizeof(APPUPDATE) + cData.m_Bytes;
    if (F_PSD_CERTIFI == cData.m_Type)
    {
        ret = pk12_VerifyCert(cData.m_Buf, cData.m_Bytes, cData.m_CtxInfo.m_ID);
    }
    if (ret)
    {
        char* pbBuf = (char*)malloc(nLen);
        memset(pbBuf, 0x0, OFFSET_SIZE + sizeof(NCMD) + sizeof(APPUPDATE));
        //DevProUpdate update(this, cData, nSeq, pbBuf + OFFSET_SIZE, nLen);
        APPUPDATE* pData = (APPUPDATE*)(pbBuf);
        pData->m_Bytes = cData.m_Bytes;
        pData->m_Type = cData.m_Type;
        if (F_PSD_CERTIFI==cData.m_Type) {
          CBase64Convert_enBase64(cData.m_CtxInfo.m_ID, strlen(cData.m_CtxInfo.m_ID), pData->m_CtxInfo.m_ID, sizeof(CID));
        } else {
          memcpy(&pData->m_CtxInfo , &cData.m_CtxInfo, sizeof(CID));
        }
        memcpy(&pData->m_FileName , &cData.m_FileName, sizeof(CID));
        //memcpy(pData, &cData, sizeof(APPUPDATE));
        memcpy(pData->m_Block, cData.m_Buf, cData.m_Bytes);
        ret = sendNCMD(s, CMD_M_APPUPDATE, CMD_S_SET, 0, nSeq, pbBuf, nLen);
        free(pbBuf);
    }
    return TRUE;
}
BOOL SLGDEV_CALL SLG_StartVideo(const char* ip, void* hwnd, VideoPro_CallBack VideoCB, int nSeq)
{
    GETSOCK(ip);
    addr_in addrLocal = {0};
    if (NULL == s->m_Pro)
    {
        if (!sock_getlocalip(&s->addr, 0, &addrLocal))
        {
            return false;
        }
        SOCKET hVideo = sock_create(addrLocal.family, SOCK_DGRAM);
        if (!sock_bind(hVideo, &addrLocal))
        {
            sock_close(hVideo);
            return false;
        }
        sock_name(hVideo, &addrLocal);
        s->vsock = (devsock_t*)malloc(sizeof(devsock_t));
        memset(s->vsock, 0, sizeof(devsock_t));
        CSelect_StartRecv(g_dev->m_recv, s->vsock->data, hVideo , 0, DspRecvFromCB, s->vsock);
        VideoPro* pro = (VideoPro*)malloc(sizeof(VideoPro));
        memset(pro, 0, sizeof(VideoPro));
        s->vsock->m_sSock = pro->m_hVideoHandle = hVideo;
        pro->m_Addr = addrLocal;
        pro->m_pRGBTMP = NULL;
        pro->m_pRGBTRANS = NULL;
        pro->m_hWnd = hwnd;
        pro->VideoCB = VideoCB;
        s->m_Pro = s->vsock->m_Pro = pro;
        //DEBUG_INFO3("StartVideo HWND = %d , W = %d , H = %d" , (int)hwnd , pro->m_Rect.right - pro->m_Rect.left , pro->m_Rect.bottom - pro->m_Rect.top);
    }
    else
    {
        s->m_Pro->m_hWnd = hwnd;
        addrLocal = s->m_Pro->m_Addr;
    }
    SOCK_ADDRESS Addr[1] = {0};
    Addr->Port = net_ntop(&addrLocal, Addr->IP_Address, IP_LEN);
    //DevVideo Video(this, Addr->IP_Address, Addr->Port, nSeq, buf + OFFSET_SIZE, nLen);
    //SOCK_ADDRESS Addr = {0};
    //strncpy(Addr.IP_Address, Addr->IP_Address, sizeof(Addr.IP_Address) - 1);
    //Addr.Port = Addr->Port;
    return sendNCMD(s, CMD_M_VIDEO_OUT, CMD_S_SET, 0, nSeq, Addr, sizeof(SOCK_ADDRESS));
}
BOOL SLGDEV_CALL SLG_StopVideo(const char* ip, int nSeq)
{
    GETSOCK(ip);
    int ret = sendNCMD(s, CMD_M_VIDEO_OUT, CMD_S_GET, 0, nSeq, 0, 0);
    CriticalSection_Lock(&g_dev->m_syn);
    if (s->m_Pro)
    {
        sock_close(s->m_Pro->m_hVideoHandle);
        CSelect_StopRecv(g_dev->m_recv, s->vsock->data);
        free(s->m_Pro->m_pRGBTMP);
        s->m_Pro->m_pRGBTMP = NULL;
        free(s->m_Pro->m_pRGBTRANS);
        s->m_Pro->m_pRGBTRANS = NULL;
        //::ReleaseDC( it->second->m_hWnd, it->second->m_hDC );
        free(s->m_Pro);
        free(s->vsock);
        s->m_Pro = NULL;
        s->vsock = NULL;
    }
    CriticalSection_Unlock(&g_dev->m_syn);
    return ret;
}
BOOL SLGDEV_CALL SLG_ULUserEx(const char* ip, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq)
{
    GETSOCK(ip);
    return DevUserEx_UserAdd(s, cUsers, nFlag, nSeq);
}
BOOL SLGDEV_CALL SLG_DLUserEx(const char* ip, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq)
{
    GETSOCK(ip);
    return DevUserEx_UserDown(s, pUserIDArray, nIDCount, nFlag, nSeq);
}
BOOL SLGDEV_CALL SLG_DELUserEx(const char* ip, DEV_CID* pUserIDArray, int nIDCount, int nSeq)
{
    GETSOCK(ip);
    return DevUserEx_UserDel(s, pUserIDArray, nIDCount, nSeq);
}
BOOL SLGDEV_CALL SLG_DLRecEx(const char* ip, const DEV_DL_RECORD_RANGE& cRange, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_RECORD_EX, CMD_S_GET, F_DL_PIC, nSeq, &cRange, sizeof(RECORDRANGE));
}
BOOL SLGDEV_CALL SLG_RebootDev(const char* ip, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_REBOOT, CMD_S_SET, 0, nSeq, 0, 0);
}
BOOL SLGDEV_CALL SLG_NetPro(const char* ip, DEV_NETPRO* pNetPro, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_NETPRO, pNetPro ? CMD_S_SET : CMD_S_GET, 0, nSeq, pNetPro, pNetPro ? sizeof(DEV_NETPRO) : 0);
}
BOOL SLGDEV_CALL SLG_ResetScreen(const char* ip, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, CMD_M_RESETSCREEN, CMD_S_SET, 0, nSeq, 0, 0);
}
long SLGDEV_CALL SLG_SetLong(long type, long value)
{
    switch (type)
    {
    case SLG_DEBUG:
        return g_cfg.m_debug = value;
        break;
    default:
        break;
    }
    return TRUE;
}
BOOL SLGDEV_CALL SLG_SetWapInfo(const char* ip, const WAPINFO* pWapInfo, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, M_WPA_SET, CMD_S_SET, 0, nSeq, pWapInfo, sizeof(WAPINFO));
}
BOOL SLGDEV_CALL SLG_GetWapInfo(const char* ip, int nSeq)
{
    GETSOCK(ip);
    return sendNCMD(s, M_WPA_SET, CMD_S_GET, 0, nSeq, 0, 0);
}

