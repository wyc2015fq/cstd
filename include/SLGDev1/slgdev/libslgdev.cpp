#ifndef _LIBSLGDEV_INL_
#define _LIBSLGDEV_INL_

#include "../slgdev_private.h"
#include "devsock.h"


// slgdev.cpp : Defines the entry point for the DLL application.
//

slgdev_config_t* get_config() {
  
  static slgdev_config_t g_cfg[] = {1};
  return g_cfg;
}

DspDevManager* gDspDevManager = NULL;
BOOL SLGDEV_CALL SLG_InitSys(BOOL bFlag)
{
    BOOL ret = FALSE;

    if (bFlag)
    {
        if (NULL == gDspDevManager)
        {
            gDspDevManager = new DspDevManager;
            assert(gDspDevManager);
            ret = gDspDevManager->InitSys(bFlag);
        }

        else
        {
            ret = TRUE;
        }
    }

    else
    {
        ret = TRUE;

        if (gDspDevManager)
        {
            gDspDevManager->InitSys(bFlag);
            delete gDspDevManager;
            gDspDevManager = NULL;
        }

    }

    return ret;
}

void SLGDEV_CALL SLG_RegDevConnectStatusCB(DevConnectStatus cb, void* param)//注册设备连接状态提醒回调
{
    if (! gDspDevManager)
    {
        return ;
    }

    assert(gDspDevManager);
    gDspDevManager->RegNetCNNotify(cb, param);
    return ;
}

void SLGDEV_CALL SLG_RegOperResultNotifyCB(DevOperResultNotify cb, void* param)
{
    if (! gDspDevManager)
    {
        return ;
    }

    assert(gDspDevManager);
    gDspDevManager->RegDevOPNotify(cb, param);
    return ;
}

BOOL SLGDEV_CALL SLG_SetMode(BOOL srvMode, QuerySrvCB pfn, void* pvContext)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    return gDspDevManager->SetMode(srvMode, pfn, pvContext);
}

BOOL SLGDEV_CALL SLG_CnSrv(BOOL bFlag, const char* ip, const char* name, const char* psw)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    return gDspDevManager->CnSrv(ip);
}

BOOL SLGDEV_CALL SLG_StartDevSerch(BOOL flag, const char* dev_region_addr)//启动设备搜索与否
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    return gDspDevManager->DevSerch(flag, dev_region_addr);
}

BOOL SLGDEV_CALL SLG_CNDev(const char* cszDevAddr) //手动连接设备
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    //DEBUG_INFO1("SLG_CNDev IP = %s" , cszDevAddr);
    return gDspDevManager->NetCN_Imp(cszDevAddr);
}

void SLGDEV_CALL SLG_DCNDev(const char* cszDevAddr) //断开设备连接
{
    if (! gDspDevManager)
    {
        return ;
    }

    assert(gDspDevManager);
    gDspDevManager->DCNDev(cszDevAddr);
    return ;
}

#define SLG_APIDEF_IMPL(fun) { CDevSock* s = gDspDevManager->getSock(cszDevAddr); if (s) { return s->fun; } else {return FALSE;} }

#define SLG_APIDEF(ret, nameargs, fun) ret SLGDEV_CALL nameargs {assert(gDspDevManager); SLG_APIDEF_IMPL(fun) }

SLG_APIDEF(BOOL, SLG_ULDevAuth(const char* cszDevAddr, const DEV_CID& user_name, const DEV_CID& pws, int flag, int nSeq) , Auth(user_name, pws, flag, nSeq));
SLG_APIDEF(BOOL, SLG_ULDevRegionAddr(const char* cszDevAddr, const char* dev_region_addr, int nSeq) , SetDevRgn(dev_region_addr, nSeq));
SLG_APIDEF(BOOL, SLG_ULDevTime(const char* cszDevAddr, const DEV_DATETIME& dev_time, int nSeq) , SetDevTime(dev_time,   nSeq));               //设置设备时间
SLG_APIDEF(BOOL, SLG_DLDevTime(const char* cszDevAddr, int nSeq) ,  GetDevTime(nSeq));    //获取设备时间
SLG_APIDEF(BOOL, SLG_DLDevBaseInfo(const char* cszDevAddr, int nSeq) ,   GetDevInfo(nSeq));    //获取设备基本信息
SLG_APIDEF(BOOL, SLG_GetWapInfo(const char* cszDevAddr, int nSeq) , GetWapInfo(nSeq));    //获取设备基本信息
SLG_APIDEF(BOOL, SLG_SetWapInfo(const char* cszDevAddr, const WAPINFO* pWapInfo, int nSeq) , SetWapInfo(pWapInfo,   nSeq));               //获取设备基本信息
SLG_APIDEF(BOOL, SLG_DLDevStatisInfo(const char* cszDevAddr, int nSeq) ,    GetDevStatis(nSeq));  //获取设备统计信息
SLG_APIDEF(BOOL, SLG_DLDevWorkAttInfo(const char* cszDevAddr, int nSeq) ,   GetDevWorkAtt(nSeq)); //获取设备工作属性信息
SLG_APIDEF(BOOL, SLG_ULDevWorkAttInfo(const char* cszDevAddr, const DEV_WORKATT& att, int nSeq), SetDevWorkAtt(att, nSeq));               //设置设备工作属性信息
SLG_APIDEF(BOOL, SLG_ULUser(const char* cszDevAddr, const DEV_USER& user, int nSeq), AddDevUser(user,   nSeq));               //上传用户
SLG_APIDEF(BOOL, SLG_ULRealTimeUser(const char* cszDevAddr, const DEV_USER& user, const DEV_USER_REAL_LOAD& rt_load), AddDevRealTimeUser(user, rt_load));            //
SLG_APIDEF(BOOL, SLG_ULRealTimeUserCap(const char* cszDevAddr, const DEV_USER& cUser, int nSeq), RealTimeUserCap(cUser, nSeq));               //
SLG_APIDEF(BOOL, SLG_DELUser(const char* cszDevAddr, const DEV_CID& user_id, int nSeq), DelDevUser(user_id, nSeq));               //删除某以用户
BOOL SLGDEV_CALL SLG_DLSingleUser(const char* cszDevAddr, const DEV_CID& user_id, int flag, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_CID)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf ;
    dl.m_DLType = DL_SINGLE;
    dl.m_DLFlag = flag;
    memcpy(dl.m_DLContent, &user_id, sizeof(DEV_CID));
    SLG_APIDEF_IMPL(GetDevUser(dl, nSeq));
}

BOOL SLGDEV_CALL SLG_DLAllUser(const char* cszDevAddr, int nSeq)//下载所有用户
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    DEV_DATA_DL dl ;
    memset(&dl, 0x0, sizeof(dl)) ;
    dl.m_DLType = DL_ALL;
    SLG_APIDEF_IMPL(GetDevUser(dl, nSeq));
}

BOOL SLGDEV_CALL SLG_DLSegTimeUser(const char* cszDevAddr, const DEV_DATE& start, const DEV_DATE& end , int nSeq)//下载某一时间段内的注册用户
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_TIMESEG)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLType = DL_SEGTIME;
    DEV_TIMESEG seg = {0};
    seg.m_Start = start;
    seg.m_End = end;
    memcpy(dl.m_DLContent, &seg, sizeof(DEV_TIMESEG));
    SLG_APIDEF_IMPL(GetDevUser(dl, nSeq));
}

BOOL SLGDEV_CALL SLG_DLAllIdentifyRecord(const char* cszDevAddr, int nSeq)//下载所有识别记录
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    DEV_DATA_DL dl;
    memset(&dl, 0x0, sizeof(dl));
    dl.m_DLType = DL_ALL;
    SLG_APIDEF_IMPL(GetDevRecord(CMD_M_VERIFY, dl, nSeq));
}

BOOL SLGDEV_CALL SLG_DLSegTimeIdentifyRecord(const char* cszDevAddr, const DEV_DATE& start, const DEV_DATE& end, int nSeq)//下载指定时间段识别记录
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_TIMESEG)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLType = DL_SEGTIME;
    dl.m_DLFlag = 0;
    DEV_TIMESEG seg = {0};
    seg.m_Start = start;
    seg.m_End = end;
    memcpy(dl.m_DLContent, &seg, sizeof(DEV_TIMESEG));
    SLG_APIDEF_IMPL(GetDevRecord(CMD_M_VERIFY, dl, nSeq));
}

BOOL SLGDEV_CALL SLG_DLRangeRec(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , int nFlag, BOOL bKnowRange , int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLFlag = nFlag;
    dl.m_DLType = bKnowRange ? DL_SINGLE : DL_RANGE;
    memcpy(dl.m_DLContent, &cRange, sizeof(DEV_DL_RECORD_RANGE));
    SLG_APIDEF_IMPL(GetDevRecord(CMD_M_VERIFY, dl, nSeq));
}

BOOL SLGDEV_CALL SLG_DLAllAlarmRecord(const char* cszDevAddr, int nSeq)//下载所有报警记录
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    DEV_DATA_DL dl ;
    memset(&dl, 0x0, sizeof(dl)) ;
    dl.m_DLType = DL_ALL;
    dl.m_DLFlag = 0;
    SLG_APIDEF_IMPL(GetDevRecord(CMD_M_ALARM, dl, nSeq));
}

BOOL SLGDEV_CALL SLG_DLSegTimeAlarmRecord(const char* cszDevAddr, const DEV_DATE& start, const DEV_DATE& end, int nSeq)//下载指定时间段报警记录
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    char szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_TIMESEG)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLType = DL_SEGTIME;
    dl.m_DLFlag = 0;
    DEV_TIMESEG seg = {0};
    seg.m_Start = start;
    seg.m_End = end;
    memcpy(dl.m_DLContent, &seg, sizeof(DEV_TIMESEG));
    SLG_APIDEF_IMPL(GetDevRecord(CMD_M_ALARM, dl, nSeq));
}

BOOL SLGDEV_CALL SLG_DLRangeAlarm(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , int nFlag, BOOL bKnowRange, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    char szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLFlag = nFlag;
    dl.m_DLType = bKnowRange ? DL_SINGLE : DL_RANGE;
    memcpy(dl.m_DLContent, &cRange, sizeof(DEV_DL_RECORD_RANGE));
    SLG_APIDEF_IMPL(GetDevRecord(CMD_M_ALARM, dl, nSeq));
}

BOOL SLGDEV_CALL SLG_DLLog(const char* cszDevAddr, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLType = DL_ALL;
    SLG_APIDEF_IMPL(GetDevRecord(CMD_M_OPTLOG, dl, nSeq));
}

BOOL SLGDEV_CALL SLG_DLRangeLog(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , BOOL bKnowRange, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    int szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLFlag = 0;
    dl.m_DLType = bKnowRange ? DL_SINGLE : DL_RANGE;
    memcpy(dl.m_DLContent, &cRange, sizeof(DEV_DL_RECORD_RANGE));
    SLG_APIDEF_IMPL(GetDevRecord(CMD_M_OPTLOG, dl, nSeq));
}

BOOL SLGDEV_CALL SLG_DLChangeImage(const char* cszDevAddr, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    char szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLType = DL_ALL;
    SLG_APIDEF_IMPL(GetDevRecord(CMD_M_CIMAGE, dl, nSeq));
}

BOOL SLGDEV_CALL SLG_DLRangeCImage(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange , int nFlag, BOOL bKnowRange, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    char szBuf[sizeof(DEV_DATA_DL) + sizeof(DEV_DL_RECORD_RANGE)] = {0};
    DEV_DATA_DL& dl = *(DEV_DATA_DL*)szBuf;
    dl.m_DLFlag = nFlag;
    dl.m_DLType = bKnowRange ? DL_SINGLE : DL_RANGE;
    memcpy(dl.m_DLContent, &cRange, sizeof(DEV_DL_RECORD_RANGE));
    SLG_APIDEF_IMPL(GetDevRecord(CMD_M_CIMAGE, dl, nSeq));
}

BOOL SLGDEV_CALL SLG_ULTimeGroup(const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& tg_array, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(SetDevTG(tg_array, nSeq));
}

BOOL SLGDEV_CALL SLG_DLTimeGroup(const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& tg_array, DEV_TIMEGROUP_TYPE tg_type, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(GetDevTG(tg_array, tg_type, nSeq));
}

BOOL SLGDEV_CALL SLG_DELTimeGroup(const char* cszDevAddr, const DEV_TIMEGROUP_ARRAY& tg_array, DEV_TIMEGROUP_TYPE tg_type, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(DelDevTG(tg_array, tg_type, nSeq));
}

BOOL SLGDEV_CALL SLG_DLRight(const char* cszDevAddr, const DEV_RIGHT_ARRAY& rt_array, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(GetDevRT(rt_array, nSeq));
}

BOOL SLGDEV_CALL SLG_ULRight(const char* cszDevAddr, const DEV_RIGHT_ARRAY& rt_array, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(SetDevRT(rt_array, nSeq));
}

BOOL SLGDEV_CALL SLG_DELRight(const char* cszDevAddr, const DEV_RIGHT_ARRAY& rt_array, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(DelDevRT(rt_array, nSeq));
}

BOOL SLGDEV_CALL SLG_ULUserGroup(const char* cszDevAddr, const DEV_USERGROUP_ARRAY& ug_array, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(SetDevUserGroup(ug_array, nSeq));
}

BOOL SLGDEV_CALL SLG_DLUserGroup(const char* cszDevAddr, const DEV_USERGROUP_ARRAY& ug_array, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(GetDevUserGroup(ug_array, nSeq));
}

BOOL SLGDEV_CALL SLG_DELUserGroup(const char* cszDevAddr, const DEV_USERGROUP_ARRAY& ug_array, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(DelDevUserGroup(ug_array, nSeq));
}

BOOL SLGDEV_CALL SLG_ULOpenDoorState(const char* cszDevAddr, DEV_DOOR_STATE eState, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(SetOpenDoorState(eState, nSeq));
}

BOOL SLGDEV_CALL SLG_DLOpenDoorState(const char* cszDevAddr, int nSeq /*= -1*/)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(GetOpenDoorState(nSeq));
}

BOOL SLGDEV_CALL SLG_ULRemoteOpenDoor(const char* cszDevAddr, BOOL bOpen, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(RemoteOpenDoor(bOpen, nSeq));
}

BOOL SLGDEV_CALL SLG_IOCtrl(const char* cszDevAddr, const DEV_IOCTRL& ioctrl, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(IOCtrl(ioctrl, nSeq));
}

BOOL SLGDEV_CALL SLG_ULFormat(const char* cszDevAddr, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(FormatDev(nSeq));
}

#pragma pack(1)
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
#ifdef WIN32
typedef unsigned __int64 uint64_t;
#else
//typedef unsigned long long uint64_t;
#endif
typedef struct
{
    uint8_t pos;
    uint32_t dict_size;
    uint64_t dst_size;
} lzma_header_t;
#pragma pack()
BOOL SLGDEV_CALL SLG_ULUpdate(const char* cszDevAddr, const DEV_APPUPDATE& data, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(UpdateApp(data, nSeq));
}

BOOL SLGDEV_CALL SLG_StartVideo(const char* cszDevAddr, void* hwnd, VideoPro_CallBack VideoCB, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(StartVideo(hwnd, VideoCB, nSeq));
}

BOOL SLGDEV_CALL SLG_StopVideo(const char* cszDevAddr, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    assert(gDspDevManager);
    SLG_APIDEF_IMPL(StopVideo(nSeq));
}

BOOL SLGDEV_CALL SLG_ULUserEx(const char* cszDevAddr, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    SLG_APIDEF_IMPL(ULUserEx(cUsers, nFlag, nSeq));
}

BOOL SLGDEV_CALL SLG_DLUserEx(const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    SLG_APIDEF_IMPL(DLUserEx(pUserIDArray, nIDCount, nFlag, nSeq));
}

BOOL SLGDEV_CALL SLG_DELUserEx(const char* cszDevAddr, DEV_CID* pUserIDArray, int nIDCount, int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    SLG_APIDEF_IMPL(DELUserEx(pUserIDArray, nIDCount, nSeq));
}

BOOL SLGDEV_CALL SLG_RebootDev(const char* cszDevAddr, int nSeq /*= -1*/)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    SLG_APIDEF_IMPL(RebootDev(nSeq));
}

BOOL SLGDEV_CALL SLG_ResetScreen(const char* cszDevAddr, int nSeq /*= -1*/)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    SLG_APIDEF_IMPL(ResetScreen(nSeq));
}

BOOL SLGDEV_CALL SLG_DLRecEx(const char* cszDevAddr, const DEV_DL_RECORD_RANGE& cRange, int nSeq /*= -1*/)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    SLG_APIDEF_IMPL(DLRecEx(cRange, nSeq));
}

BOOL SLGDEV_CALL SLG_NetPro(const char* cszDevAddr, DEV_NETPRO* pNetPro , int nSeq)
{
    if (! gDspDevManager)
    {
        return FALSE;
    }

    SLG_APIDEF_IMPL(DevNetPro(pNetPro, nSeq));
}

long SLGDEV_CALL SLG_SetLong(long type, long value)
{
  slgdev_config_t* g_cfg = get_config();
    switch (type)
    {
    case SLG_DEBUG:
        return g_cfg->m_debug = value;
        break;

    default:
        break;
    }

    return 0;
}

#endif // _LIBSLGDEV_INL_

