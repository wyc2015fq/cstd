
#ifndef _NET_H_
#define _NET_H_
#define MAX_TIME_INTERVAL 30
#define VSOCKET ((SOCKET)0xffffffff)
//enum { LINK_FAIL = 0, LINK_OK, LINK_ALREADY };
class DspDevManager;
#include "devdatestruct.h"

#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1))
// stdafx.cpp : source file that includes just the standard includes
// slgdev.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information
#include <map>
#include <string>
using namespace std;
//状态标记位
#define F_ERR_BUSY ERR_BUSY
#define F_ERR_LIMIT ERR_LIMIT
#define F_ERR_NOFIND ERR_NOFIND
#define F_ERR_SAVE_FAIL ERR_SAVE_FAIL
#define F_ERR_SET_FAIL ERR_SET_FAIL
#define F_ERR_VERIFY_FAIL ERR_VERIFY_FAIL
//其他标记位
#define F_DL_ID         CMD_DOWN_ID       //下载ID集合
#define F_DL_ID_ALL     CMD_ID_ALL        //下载所有数据的ID集合
#define F_ID_TIME_RANGE CMD_ID_TIME_RANGE //下载制定时间段ID集合
#define F_ID_RANGE F_ID_TIME_RANGE<<1
#define F_AUTH_VERIFY   AUTH_VERIFY   //用户身份验证
#define F_DL_PIC        DL_IMAGE      //需要下载照片
#define F_DL_FEAT       DL_FEAT       //需要下载人脸特征
#define F_DL_APPENDID   DL_APPENDID   //远程加在时，m_ID表示卡号
#define F_DL_NORMAL_TG  DL_NORMAL_TG  //下载正常时间组
#define F_DL_HOLIDAY_TG DL_HOLIDAY_TG //下载节假日
#define F_DOOR_CLOSE    DOOR_CLOSE    //紧急关门，否则为紧急开门
#define F_DOOR_OPEN     DOOR_OPEN     //长开或者长关状态取消
#define F_G_ORDER       G_ORDER       //组合开门有序
#define F_DEFAULT_NO_GROUP DEFAULT_NO_GROUP
#define TIMEGROUP_NUMS TGROUP_NUM    //时间组个数
#define USER_COMBINS   MAX_GROUPUSER //用户组合个数
#define USER_JPEG_SIZE (64*1024)
#define REC_JPEG_SIZE (18*1024)
#define FEAT_SIZE (90*1024)
#define OFFSET_SIZE 20
//#define TRANS_SERVER
#ifndef TRANS_SERVER
#define CMD_S_CONNECT_CUT     DEV_CONNECT_CUT     //断开连接
#define CMD_S_CONNECT_SUCCESS DEV_CONNECT_SUCCESS //连接成功
#define CMD_S_CONNECT_FAILUE  DEV_CONNECT_FAILUE  //连接失败
#define CMD_S_CONNECT_NONE    DEV_CONNECT_NONE    //未连接
#define CMD_S_CONNECT_REQ     DEV_CONNECT_NONE+1  //请求连接 
#define CMD_S_CONNECT_REQ_CUT CMD_S_CONNECT_REQ+1 //请求断开连接
#define E_CONNECT_STATUS DEV_CONNECT_STATUS
#else
#define CMD_S_CONNECT_CUT     NET_CONNECT_CUT     //断开连接
#define CMD_S_CONNECT_SUCCESS NET_CONNECT_SUCCESS //连接成功
#define CMD_S_CONNECT_FAILUE  NET_CONNECT_FAILUE  //连接失败
#define CMD_S_CONNECT_NONE    NET_CONNECT_NONE    //未连接
#define CMD_S_CONNECT_REQ     NET_CONNECT_NONE+1  //请求连接 
#define CMD_S_CONNECT_REQ_CUT CMD_S_CONNECT_REQ+1 //请求断开连接
#define E_CONNECT_STATUS NET_CONNECT_STATUS
#endif
#ifndef TRANS_SERVER
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
#endif


class CDevSock : public CSockBase
{
public:
    uutime m_uutime;
    CriticalSection m_syn;
public:
    CDevSock(SOCKET sSOCK, addr_in* paddr, const char* cszIP, USHORT port, BOOL bCalTimeOut, BOOL srv)
    {
        m_sSock = (sSOCK), m_bCalTimeOut = (bCalTimeOut);
        memset(&m_Addr, 0x0, sizeof(m_Addr));
        strncpy(m_Addr.IP_Address, cszIP, sizeof(m_Addr.IP_Address) - 1);
        m_Addr.Port = port;
        addr = *paddr;
        m_Pro = NULL;
        m_srv = srv;
        m_pSsl = NULL;
    }

    ~CDevSock() {}

public:
    SOCKET GetHandle(void) const
    {
        return m_sSock;
    }

    SOCKET GetRsock(void) const
    {
        return m_sSock;
    }

    const SOCK_ADDRESS& GetAddr() const
    {
        return m_Addr;
    }

    BOOL IsActive()
    {
        //return true;
        return m_bCalTimeOut ? (m_uutime.elapsed() < MAX_TIME_INTERVAL) : true ;
    }

    void SetActive()
    {
        m_uutime.restart();
    }

    BOOL NetDCN()
    {
        int temp[ ALIGN4(sizeof(NCMD)) >> 2 ] = { 0 };
        NCMD* pCMD = (NCMD*)((char*)temp + OFFSET_SIZE);
        pCMD->m_MCmd = C_CNDEV;
        pCMD->m_SCmd = S_DN;
        addr_to_addrx(&pCMD->m_Param2);
        int ret = 0, nLen = sizeof(NCMD);
        return SLG_Send(m_sSock, (char*)pCMD, &nLen);
    }

    BOOL NetCN_Imp()
    {
        int temp[ ALIGN4(sizeof(NCMD)) >> 2 ] = { 0 };
        NCMD* pCMD = (NCMD*)((char*)temp + OFFSET_SIZE);
        pCMD->m_MCmd = C_CNDEV;
        pCMD->m_SCmd = S_CN;
        addr_to_addrx(&pCMD->m_Param2);
        int ret = 0, nLen = sizeof(NCMD);
        return SLG_Send(m_sSock, (char*)pCMD, &nLen);
    }

    void addr_to_addrx(unsigned int* pout) const
    {
        if (AF_INET == addr.family)
        {
            struct sockaddr_in* p = (struct sockaddr_in*)&addr;
            //unsigned int aa = 0;
            //DotIpToLong1("192.168.1.150", &aa);
            *pout = p->sin_addr.s_addr;
        }

        else
        {
            *pout = 0xffffffff;
        }

    }

    BOOL sendNCMD(int m_MCmd, int m_SCmd, int m_CFlag, int m_Param1, const void* pbBuf, int uLen) const
    {
        int ret, len = uLen + OFFSET_SIZE + sizeof(NCMD) + sizeof(SOCK_ADDRESS);
        char* buf1 = (char*)malloc(len);
        memset(buf1, 0, len);
        NCMD* pTMP = (NCMD*)(buf1 + OFFSET_SIZE);
        pTMP->m_MCmd = m_MCmd;
        pTMP->m_SCmd = m_SCmd;
        pTMP->m_CFlag = m_CFlag;
        pTMP->m_Param1 = m_Param1;
        addr_to_addrx(&pTMP->m_Param2);

        if (pbBuf)
        {
            memcpy(buf1 + OFFSET_SIZE + sizeof(NCMD), pbBuf, uLen);
        }

        *(SOCK_ADDRESS*)(buf1 + uLen + OFFSET_SIZE + sizeof(NCMD)) = m_Addr;
        uLen = len - OFFSET_SIZE;
        ret = SLG_Send(m_sSock, buf1 + OFFSET_SIZE, &uLen);
        free(buf1);
        return ret;
    }

    BOOL Send(const char* pbBuf, unsigned int* puLen) const
    {
        return SLG_Send(m_sSock, pbBuf, (int*)puLen);
    }

    void OnRecvFromNotify(const addr_in* Addr, const char* cpbBuf, unsigned int uLen)
    {
        if (uLen < sizeof(NCMD))
        {
            return ;
        }

        NCMD* pNCMD = (NCMD*)cpbBuf;

        if (pNCMD->m_MCmd == CMD_M_VIDEO_OUT)
        {
            DEVIMAGE* pImage = (DEVIMAGE*)(pNCMD + 1);
            ShowVideo(pImage->m_Block, pImage->m_W, pImage->m_H, pImage->m_Step, pImage->m_Type);
        }

    }

    void RecvNotify(SOCKET hSock, const char* pbData, int nLen, void* user);
    //验证
    BOOL Auth(const DEV_CID& cName, const DEV_CID& cPws, int nFlag, int nSeq = -1);
    //区域
    BOOL SetDevRgn(const char* cszGroupIP = DEV_REGION_ADDR, int nSeq = -1);
    BOOL GetDevRgn(int nSeq = -1);
    //时间
    BOOL SetDevTime(const DEV_DATETIME& cTime, int nSeq = -1);
    BOOL GetDevTime(int nSeq = -1);
    //硬件基本信息
    BOOL GetDevInfo(int nSeq = -1);
    //统计信息
    BOOL GetDevStatis(int nSeq = -1);
    //工作属性
    BOOL SetDevWorkAtt(const DEV_WORKATT& cAtt, int nSeq = -1);
    BOOL GetDevWorkAtt(int nSeq = -1);
    //格式化设备
    BOOL FormatDev(int nSeq = -1);
    //更新设备程序
    int UpdateApp(const DEV_APPUPDATE& cData, int nSeq = -1);
    //用户
    BOOL AddDevUser(const DEV_USER& cUser, int nSeq = -1);
    BOOL AddDevRealTimeUser(const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad);
    BOOL RealTimeUserCap(const DEV_USER& cUser, int nSeq) ;
    BOOL GetDevUser(const DEV_DATA_DL& cDlData, int nSeq = -1);
    BOOL DelDevUser(const DEV_CID& cID, int nSeq = -1);
    //记录
    BOOL GetDevRecord(int nClass, const DEV_DATA_DL& cDlData, int nSeq = -1);
    //紧急开门
    BOOL SetOpenDoorState(DEV_DOOR_STATE eState, int nSeq = -1);
    BOOL GetOpenDoorState(int nSeq = -1);
    BOOL RemoteOpenDoor(BOOL bOpen, int nSeq = -1);
    BOOL IOCtrl(const DEV_IOCTRL& ioctrl, int nSeq = -1);
    //时间组
    BOOL SetDevTG(const DEV_TIMEGROUP_ARRAY& cArray, int nSeq = -1);
    BOOL GetDevTG(const DEV_TIMEGROUP_ARRAY& cArray, DEV_TIMEGROUP_TYPE eType, int nSeq = -1);
    BOOL DelDevTG(const DEV_TIMEGROUP_ARRAY& cArray, DEV_TIMEGROUP_TYPE eType, int nSeq = -1);
    //权限
    BOOL SetDevRT(const DEV_RIGHT_ARRAY& cArray, int nSeq = -1);
    BOOL GetDevRT(const DEV_RIGHT_ARRAY& cArray, int nSeq = -1);
    BOOL DelDevRT(const DEV_RIGHT_ARRAY& cArray, int nSeq = -1);
    //用户组
    BOOL SetDevUserGroup(const DEV_USERGROUP_ARRAY& cArray, int nSeq);
    BOOL GetDevUserGroup(const DEV_USERGROUP_ARRAY& cArray, int nSeq);
    BOOL DelDevUserGroup(const DEV_USERGROUP_ARRAY& cArray, int nSeq);
    //视频传输
    BOOL StartVideo(void* hwnd, VideoPro_CallBack VideoCB, int nSeq);
    BOOL StopVideo(int nSeq);
    void ShowVideo(const char* cpbImage, int w, int h, int step, int nImageType);
    //用户扩展操作
    BOOL ULUserEx(const DEV_BATCH_USER& cUsers, int nFlag, int nSeq);
    BOOL DLUserEx(DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq);
    BOOL DELUserEx(DEV_CID* pUserIDArray, int nIDCount, int nSeq);
    //记录扩展操作
    BOOL DLRecEx(const DEV_DL_RECORD_RANGE& cRange, int nSeq /*= -1*/);
    //设备重启
    BOOL RebootDev(int nSeq /*= -1*/);
    //获得和设置网络属性
    BOOL DevNetPro(DEV_NETPRO* pNetPro , int nSeq);
    //屏幕校验
    BOOL ResetScreen(int nSeq /*= -1*/);
    //屏幕校验
    // 用于获取和设置802.1x认证相关的三个参数：开启/关闭802.1x认证、用户名、密码
    BOOL SetWapInfo(const WAPINFO* pWapInfo, int nSeq);
    BOOL GetWapInfo(int nSeq);
};
class DspDevManager : public CThread
{
    friend class CNet;
public:
    SOCKET m_hSockSrv;
    DspDevManager() : m_pfnCN(NULL),
        m_pCNContext(NULL),
        m_pfnOpNotify(NULL),
        m_pOpNotifyContext(NULL),
        m_bInit(FALSE),
        m_Trans(FALSE),
        m_SrvCb(NULL),
        m_hSockSrv(0),
        m_SrvCtx(NULL)
    {
    }

    ~DspDevManager()
    {
        StartWork(FALSE);
    }

public:
    void RegDevOPNotify(DevOperResultNotify pfnOpNotify, void* pvContext)   //数据操作结果回调
    {
        // AoutLock lock( &m_syn );
        m_pfnOpNotify = pfnOpNotify;
        m_pOpNotifyContext = pvContext;
    }

    void RegNetCNNotify(DevConnectStatus pfnCN, void* pCNContext)   //注册设备连接状态提醒回调
    {
        // AoutLock lock( &m_syn );
        m_pfnCN = pfnCN;
        m_pCNContext = pCNContext;
    }

public:
    BOOL StartWork(BOOL bFlag);
    BOOL Serch(BOOL bFlag, const char* cszIP);
    void TimeOut(int nDelay = MAX_TIME_INTERVAL);
    BOOL TransMode(void)
    {
        return m_Trans;
    }

    void RecvNotify(SOCKET hSOCK, const char* pbData, int nLen, void* user);
    void RecvFromNotify(SOCKET hSOCK, const char* pbData, int nLen, const addr_in* Addr, void* user);
    BOOL NetDCN(CDevSock* s, E_CONNECT_STATUS eFlag, BOOL tri = FALSE);
    int NetCN_Imp(const char* cszIP);
    BOOL SerchBegin(const char* cszIP);
    BOOL SerchEnd(const char* cszIP);
    void CallSrvSB(const char* ip, SOCK_ADDRESS* addr);
    CDevSock* FindTranSrv(const SOCK_ADDRESS& addr);
public:
    BOOL InitSys(BOOL bFlag);
    BOOL DevSerch(BOOL bFlag, const char* cszGroupIP = DEV_REGION_ADDR);
    //设备工作模式
    BOOL SetMode(BOOL srvMode, QuerySrvCB pfn, void* pvContext);
    //连接断开设备
    BOOL CNDev(const char* cszIP);
    void DCNDev(const char* cszIP);
    BOOL CnSrv(const char* cszIP)
    {
        addr_in addr[1] = {0};
        SOCKET hSock = 0;
        int linkCode = DEV_CONNECT_FAILUE;
        int len = net_addr(cszIP, SERVER_PORT, addr);

        if (len <= 0)
        {
            DEBUG_INFO("net_addr err:%d\n", len);
            linkCode = DEV_CONNECT_FAILUE;
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
                    m_hSockSrv = hSock;
                }

            }

            else
            {
                hSock = 0;
            }

        }

        return hSock != 0;
    }

protected:
    virtual void Run();
public:
    friend BOOL InternalSend(const CDevSock* s, const char* pbBuf, unsigned int uLen, void* pvClientData);
    friend void CNNotify(const CDevSock* s, E_CONNECT_STATUS eFlag, BOOL bServer, void* pvClientData);
public:
    int OnCNNotify(const char* sIP, E_CONNECT_STATUS eFlag, BOOL bServer = TRUE);
    void OnRecvNotify(CDevSock* s, const char* cpbBuf, unsigned int uLen, BOOL bServer = TRUE);
    void OnDevCMDNotify(const CDevSock* s, const char* cszClientAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult);
private:
    DevConnectStatus m_pfnCN;
    void* m_pCNContext;
    DevOperResultNotify m_pfnOpNotify;
    void* m_pOpNotifyContext;
public:
    typedef std::map<std::string, CDevSock*> SOCK_MAP;
    CDevSock* getSock(const char* ip)
    {
        std::string ss(ip);
        SOCK_MAP::iterator it = m_map.find(ss);

        if (it != m_map.end())
        {
            return it->second;
        }

        return NULL;
    }

    SOCK_MAP m_map;
    CriticalSection m_syn;
    CriticalSection m_SynSrvCb;
    CriticalSection m_synLink;
    BOOL m_bInit;
    BOOL m_Trans;
    QuerySrvCB m_SrvCb;
    void* m_SrvCtx;
    // CriticalSection m_syn;
};
//REG_OBJECT(CMD_M_USER_EX, DevUserEx, true);
#define OFF_SET(T, member) (size_t( &(((T*)0)->member)))
void DspRec2UspRec(RECORD& dsp, DEV_RECORD& usp, BOOL realTime);
#if 0
static BOOL DevRecord_set(const CDevSock* s, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq)
{
    assert(pbBuf);
    NCMD* pCMD = (NCMD*)pbBuf;
    ret = s->sendNCMD(CMD_M_VERIFY, CMD_S_GET, 0, nSeq, pbBuf, nLen);

    if (nFlag & DL_DEV_PIC)
    {
        pCMD->m_CFlag |= F_DL_PIC;
    }

    memcpy(pbBuf, &cRange, sizeof(RECORDRANGE));
    nLen = sizeof(RECORDRANGE);
}

static BOOL DevAlarm_set(const CDevSock* s, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq)
{
    assert(pbBuf);
    NCMD* pCMD = (NCMD*)pbBuf;
    ret = s->sendNCMD(CMD_M_ALARM, CMD_S_GET, 0, nSeq, pbBuf, nLen);

    if (nFlag & DL_DEV_PIC)
    {
        pCMD->m_CFlag |= F_DL_PIC;
    }

    memcpy(pbBuf, &cRange, sizeof(RECORDRANGE));
    nLen = sizeof(RECORDRANGE);
}

#endif
BOOL DevUserGroup_set(const CDevSock* s, const DEV_USERGROUP_ARRAY& cArray, DEV_OPER_FLAG eFlag, int nSeq);
BOOL DevUser_set(const CDevSock* s, const CID& cID, int nFlag, int nSeq);
BOOL DevUser_set(const CDevSock* s, const CID& cID, int nSeq);
BOOL DevUser_set(const CDevSock* s, const DEV_USER& cUser, BOOL bAddFlag, int nSeq);
BOOL DevUser_set(const CDevSock* s, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad);

BOOL DevID_set(const CDevSock* s, int nClass, int nSeq);
BOOL DevID_set(const CDevSock* s, int nClass, const DEV_TIMESEG& cTimeSeg, int nSeq);
BOOL DevID_set(const CDevSock* s, int nClass, int nFlag, const DEV_DL_RECORD_RANGE& cRange, int nSeq);
BOOL CopyDevUser2DspUser(USER* dspUser , const DEV_USER* devUser);

BOOL DevUser_set(const CDevSock* s, const CID& cID, int nFlag, int nSeq);

BOOL DevUser_set(const CDevSock* s, const CID& cID, int nSeq);
BOOL DevUser_set(const CDevSock* s, const DEV_USER& cUser, BOOL bAddFlag, int nSeq);
BOOL DevUser_set(const CDevSock* s, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad);
BOOL DevUserEx_UserAdd(const CDevSock* s, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq);
BOOL DevUserEx_UserDel(const CDevSock* s, DEV_CID* pUserIDArray, int nIDCount, int nSeq);
BOOL DevUserEx_UserDown(const CDevSock* s, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq);
BOOL DevCImage_set(const CDevSock* s, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq);
BOOL DevTimeGroup_set(const CDevSock* s, const DEV_TIMEGROUP_ARRAY& cArray, int nType, DEV_OPER_FLAG eFlag, int nSeq);
BOOL DevRight_set(const CDevSock* s, const DEV_RIGHT_ARRAY& cArray, DEV_OPER_FLAG eFlag, int nSeq);

#endif // _NET_H_
